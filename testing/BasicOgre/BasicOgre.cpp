/*-------------------------------------------------------------------------
This source file is a part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/


Copyright (c) 2000-2013 Torus Knot Software Ltd
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
-------------------------------------------------------------------------*/

#include <OgreHeaderPrefix.h>
#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <RTShaderSystem/OgreRTShaderSystem.h>
#include <OgreApplicationContext.h>
#include <OgreHeaderSuffix.h>
#include <iostream>
#include <Windows.h>

using namespace Ogre;
using namespace OgreBites;

class BasicTutorial1
		: public ApplicationContext
		, public InputListener
{
	public:
	BasicTutorial1 ();
	virtual ~BasicTutorial1 ()
							{ }

	void setup ();
	bool keyPressed (const KeyboardEvent& evt);
};

BasicTutorial1::BasicTutorial1 ()
	: ApplicationContext ("OgreTutorialApp")
{
}

void BasicTutorial1::setup ()
{
	// do not forget to call the base first
	ApplicationContext::setup ();
	addInputListener (this);

	// get a pointer to the already created root
	Root* root = getRoot ();
	SceneManager* scnMgr = root -> createSceneManager ();

	// register our scene with the RTSS
	RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr ();
	shadergen->addSceneManager (scnMgr);

	// The light
	scnMgr -> setAmbientLight (ColourValue (0.5, 0.5, 0.5));
	Light* light = scnMgr -> createLight ("MainLight");
	SceneNode* lightNode = scnMgr -> getRootSceneNode () -> createChildSceneNode ();
	lightNode -> attachObject (light);
	lightNode->setPosition (20, 80, 50);

	// The cam
	SceneNode* camNode = scnMgr -> getRootSceneNode () -> createChildSceneNode ();
	Camera* cam = scnMgr -> createCamera ("myCam");
	cam -> setNearClipDistance (5); // specific to this sample
	cam -> setAutoAspectRatio (true);
	camNode -> attachObject (cam);
	camNode -> setPosition (0, 47, 222);
	getRenderWindow () -> addViewport (cam);

	// 1
	Entity* ogreEntity = scnMgr -> createEntity ("robot.mesh");
	SceneNode* ogreNode = scnMgr -> getRootSceneNode () -> createChildSceneNode ();
	ogreNode -> attachObject (ogreEntity);

	// 2
	Entity* ogreEntity2 = scnMgr -> createEntity("ninja.mesh");
	SceneNode* ogreNode2 = scnMgr -> getRootSceneNode () -> createChildSceneNode (Vector3 (84, 48, 0));
	ogreNode2 -> attachObject (ogreEntity2);

	// 3
	Entity* ogreEntity3 = scnMgr -> createEntity ("ogrehead.mesh");
	SceneNode* ogreNode3 = scnMgr -> getRootSceneNode () -> createChildSceneNode ();
	ogreNode3 -> setPosition (0, 104, 0);
	ogreNode3 -> setScale ((Ogre::Real) 2, (Ogre::Real) 1.2, (Ogre::Real) 1);
	ogreNode3 -> attachObject (ogreEntity3);

	// 4
	Entity* ogreEntity4 = scnMgr -> createEntity ("facial.mesh");
	SceneNode* ogreNode4 = scnMgr -> getRootSceneNode () -> createChildSceneNode ();
	ogreNode4 -> setPosition (-84, 48, 0);
	ogreNode4 -> roll (Degree (-90));
	ogreNode4 -> attachObject (ogreEntity4);
}

bool BasicTutorial1::keyPressed (const KeyboardEvent& evt)
{
	if (evt.keysym.sym == SDLK_ESCAPE)
	{
		getRoot () -> queueEndRendering ();
	}

	return true;
}

INT WINAPI WinMain (HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
{
	try
	{
		BasicTutorial1 app;
		app.initApp();
		app.getRoot()->startRendering();
		app.closeApp();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error occurred during execution: " << e.what() << '\n';
		return 1;
	}

	return 0;
}