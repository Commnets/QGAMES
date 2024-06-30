/** \ingroup OGRE */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: ogregame.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 27/08/2017 \n
 *	Description: Defines the way a game based on OGRE is implemented. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_OGREGAME__
#define __QGAMES_OGREGAME__

#include <Common/element.hpp>
#include <Common/widgetsystem.hpp>
#include <Common/gameimplementation.hpp>
#include <Common/definitions.hpp>
#include <Common/screen.hpp>
#include <Arcade/world.hpp>
#include <Arcade/scene.hpp>
#include <CommonOGRE/ogremovementcontroller.hpp>
#include <CommonOGRE/ogreengine.hpp>
#include <OgreHeaderPrefix.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreRenderQueueListener.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlay.h>
#include <OgreCamera.h>
#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
#include <OgreMaterialManager.h>
#include <RTShaderSystem/OgreRTShaderSystem.h>
#endif 
#include <OgreHeaderSuffix.h>
#include <map>

/** To represent the a game based on OGRE. */
class OGREGame : public QGAMES::GameImplementation
{
	public:
	typedef std::map <std::string, Ogre::SceneManager*> SceneManagers;
	typedef std::map <std::string, QGAMES::Screen*> Screens;
	
	OGREGame ()
		: QGAMES::GameImplementation (),
		  _root (NULL),
		  _sceneManagers (),
		  _overlaySystem (NULL),
		  _overlay (NULL),
		  _cameraManagers (),
		  _screens (),
		  _renderQueueListener (NULL)
#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
		  ,_shaderGenerator (NULL), 
		  _materialMgrListener (NULL), 
		  _RTShaderLibPath (__NULL_STRING__)
#endif
							{ }
	virtual ~OGREGame ();

	/** @see parent. */
	virtual QGAMES::_ExEngine* createExternalEngine ()
							{ return (new OGREExternalEngine (this)); }

	/** To get ogree main reference,
		that is used in other important parts of the code like ogrescreen.hpp */
	Ogre::Root* root ()
							{ return (_root); }
	
	// Managing scene managers...
	SceneManagers sceneManagers ()
							{ return (_sceneManagers); }
	/** To get a scene manager. */
	bool existSceneManager (const std::string& id);
	Ogre::SceneManager* sceneManager (const std::string& id);
	/** Try to get the one called __OGREMAINSCENEMANAGERID__, 
		otherwise get the first in the list. */
	Ogre::SceneManager* mainSceneManager ();
	/** To get the overlay system and the overlay used. */
	Ogre::OverlaySystem* overlaySystem ()
							{ return (_overlaySystem); }
	Ogre::Overlay* overlay ()
							{ return (_overlay); }

	// To manage the cameras...
	QGAMES::MapOfCameras cameraManagers ()
							{ return (_cameraManagers); }
	bool existCameraManager (const std::string& id);
	/** To get a camera manager. */
	QGAMES::Camera* cameraManager (const std::string& id);
	/** To get the one called __OGRE_MAINCAMERAID__,
		otherwise the first in the list */
	QGAMES::Camera* mainCameraManager ();

	/** @see parent. */
	/**
	 *	This method is important as well for QGAMES/OGRE system stability
	 *	First load plugins files. 
	 *	The name of the file with the plugins' name is stored in a variable of the QGAMES' 
	 *	configuration file called "PLUGINSFILE". 
	 *	If none is defined then the default value defined in the defaultParamer method at Game class level is used.
	 *	And if there is nothing there the system will crash, because there will be nothing to load!
	 *	Then load the resources used by the engine. 
	 *	The name of the file with the resources' definitions is stored in a variable of the QGAMES'
	 *	configuration file called "RESOURCESFILE".
	 *	If none is defined then the default value defined in the defaultParamer method at Game class level is used.
	 *	And if there is nothing there the system will crash, because there will be nothing to load!
	 *	Then the system is initialized using the definition kept in the 
	 *	file ogre.cfg. No window to select the render system is opened!. 
	 *	If no default configuration is defined the system will crash then.
	 */
	virtual bool initialize ();

	/** @see parent. */
	virtual QGAMES::FormBuilder* createFormBuilder ();
	virtual	QGAMES::SoundBuilder* createSoundBuilder ();

	/** @see parent. */
	virtual QGAMES::InputHandler* createInputHandler (QGAMES::InputHandler::Behaviour* bhv);
	virtual QGAMES::Screen* createScreen (const std::string& t, 
		const QGAMES::Position& pos, int w, int h, int pX, int pY);

	/** 
	  *	This method is really important in OGRE System.
	  *	Can be changed, but it is really dangerous for QGAMES/OGRE stability.
	  *	Take into account how it works if any:
	  * Load the resources,
	  * Initialize the Shader System (if any)
	  *	Create the scene managers,
	  *	Then the camera managers,
	  *	finally the view ports.
	  */
	virtual void initializeScreens ();
	/** The equivalent but finishing the implementation. */
	virtual void finalizeScreens ();

	/** @ see parent. */
	virtual void inEveryLoop ();
	virtual void handleEvents ()
							{ } // Makes no sense in OGRE (everything will be faced by the input handler...

	/** Actualize the system when something happens to the gui system. */
	virtual void whenCreateGUISystem (QGAMES::GUISystem* sys) { }
	virtual void whenDeleteGUISystem (QGAMES::GUISystem* sys) { }
	virtual void whenInitialize (QGAMES::GUISystem* sys) { }
	virtual void whenInEveryLoop (QGAMES::GUISystem* sys) { }
	virtual void whenDrawOn (QGAMES::GUISystem* sys, QGAMES::Screen* s, const QGAMES::Position& p) { }

	/** Actualize the system when something happens in the elements. 
		By default nothing happens except updatePositions that the position and others is actualized. */
	virtual void whenCreateElement (QGAMES::Element* elmnt); 
	// The _ogreSceneManager of the additional data is set here by defauly, take care!
	// And the first sceneManager created (@see ogregame.cpp) is used by default
	// So, all elements will be drawn with the same level of detail and quality (if any)
	virtual void whenDeleteElement (QGAMES::Element* elmnt) { }
	virtual void whenInitialize (QGAMES::Element* elmnt) { }
	virtual void whenInEveryLoop (QGAMES::Element* elmnt);
	virtual void whenUpdatePositions (QGAMES::Element* elmnt);
	virtual void whenDrawOn (QGAMES::Element* elmnt, QGAMES::Screen* s, const QGAMES::Position& p) { }
	virtual void whenFinalize (QGAMES::Element* elmnt) { }

	protected:
	/** 
	 *	This part of the code manages in configuration all OGRE possibilities.
	 *
	 *	SCENE MANAGERS:
	 *	---------------
	 *	There can be many Scene Managers. Every scene manager renders an scene.
	 *	Every scene element (or part of) has to he hung from a scene manager later, just to be rendered by this!.
	 *	Every scene manager created in the game, is added to a map with all scene managers.
	 *	createSceneManager default behaviour creates just one scene manager
	 *	with the default name __OGREMAINSCENEMANAGERID__.
	 *	Every scene manager has a render queue listener associated to listen 
	 *	potential events generated when a scene queue managed by the scene manager is rendered.
	 *	createRenderWueueListener's default behaviour creates nothing.
	 *
	 *	CAMERA MANAGERS:
	 *  ----------------
	 *	Every scene manager has a set of camera managers associated, 
	 *	and every camera manager has camera behind.
	 *	The camera describes where the position of a pair of "eyes" are
	 *	and in which direction "the eyes" are looking at the scene manager.
	 *	The method createCamaneraManagersFor creates the a set of camera managers per scene manager.
	 *	It receives the name and a reference to the scene manager just created above.
	 *	The default behaviour creates a single camera manager per scene manager.
	 *  OGRE's internal camera name is the same that the Scene Manager.
	 *	The QGAME's camera name is made up of Scene Manager name and the word __OGREMAINCAMERAID__.
	 *	The camera is initially located at the origin and looking towards the z+ axis.
	 *	The clipping distance is also set to 5. 
	 *	Both parameters can be changed later.
	 *
	 *	VIEWPORTS:
	 *	----------
	 *	Now it is time to manage viewports.
	 *	A viewport is a way to draw the scenes observed by a camera in a screen.
	 *	There can be many cameras represented in the same screen.
	 *	So a method to know which cameras are projected in an screen is needed.
	 *	The method cameraManagersForScreen does it. The default behaviour is to returned all.
	 *	Then a viewport or viewports are created per camera in every screen, 
	 *	using the method createViewportForCameraInWindow.
	 *	The default behaviour is to create an simple viewport for the screen 
	 *	received and link it to the scene manager also received.
	 *	The background of the viewport is set to BLANK and the relation between width and hight 
	 *	will be the same than the window parameter.
	 */
	// The methods
	/** Creates the scene managers.
		By default just one is created, but ogre allows to create more than one.
		Minimum one is needed. The default one is created as ST_GENERIC, 
		but overloading the class any number and different types can be created. 
		The scene manager type created is probably not the more powerful to render big sceenarios i.e
		It is useful to render closer scenearios as rooms. */
	virtual SceneManagers createSceneManagers ();
	/** To create a set of camera managers associated to an specific scene manager.
		The default implementation is to create a single camera manager per scene manager. 
		As the default implementation it also to create a single scene manager, there will be
		a single camera manager, and its name would be: __OGREMAINCAMERAID__. */
	virtual QGAMES::MapOfCameras createCameraManagersFor (const std::string& name, 
		Ogre::SceneManager* scnMng);
	/** Creates the render listener.
		When all render events are processed, then this listner will be invoked.
		By default no queue listener is defined. */
	virtual Ogre::RenderQueueListener* createRenderQueueListener () 
							{ return (NULL); }
	/** To get the camera manager per an specific screen. 
		By default this method returns all camenra managers created. */
	virtual QGAMES::MapOfCameras cameraManagersForScreen (QGAMES::Screen* scr)
							{ return (_cameraManagers); }
	/** To add a viwport to the camera manager. 
		The default method creates a viewport for the camera with the same dimensaions
		of the screen it project. It can also be changed inheriting the class. 
		The proyection ratio of the camera is then also modified to adopt the one of the viewport */
	virtual Ogre::Viewport* createViewportForCameraInWindow (const std::string& n, 
		Ogre::Camera* cm, Ogre::RenderWindow* rd);
	/** To load the resources. */
	void loadResources ();
	/** To install the resources. 
		They have to be loaded previously. */
	void installResources ();

	/** A set of methods to define what to do which every specifc type of element. 
		Can be overload to include how to interpretate new specific attributes. */
	virtual void whenCreateEntity (OGREElementAdditionalData* eD, QGAMES::Entity* ety);
	// Only a default implementation is provided for entities...
	// For the rest, nothing is implemented! (take care)
	// A new scene node is always created, 
	// and it is linked to the rootNode of the sceneManager except
	// when it is a scoreObject that then it is attached to the overlay3D layer.
	virtual void whenCreateScene (OGREElementAdditionalData* eD, QGAMES::Scene* scn) { }
	virtual void whenCreateWorld (OGREElementAdditionalData* eD, QGAMES::World* wrld) { }
	virtual void whenCreateMap (OGREElementAdditionalData* eD, QGAMES::Map* mp) { }

	/** A set of methods to define what to do which every specifc type of element. 
		Can be overload to include how to interpretate new specific attributes. */
	virtual void whenInEveryLoopForEntity (OGREElementAdditionalData* eD, QGAMES::Entity* ety);
	// Only a default implementation is provided for entities...
	// For the rest, nothing is implemented! (take care)
	virtual void whenInEveryLoopForScene (OGREElementAdditionalData* eD, QGAMES::Scene* scn) { }
	virtual void whenInEveryLoopForWorld (OGREElementAdditionalData* eD, QGAMES::World* wrld) { }
	virtual void whenInEveryLoopForMap (OGREElementAdditionalData* eD, QGAMES::Map* mp) { }

	/** A set of methods to define what to do which every specifc type of element. 
		Can be overload to include how to interpretate new specific attributes. */
	virtual void whenUpdatePositionsForEntity (OGREElementAdditionalData* eD, QGAMES::Entity* ety);
	virtual void whenUpdatePositionsForScene (OGREElementAdditionalData* eD, QGAMES::Scene* scn);
	// Only a default implementation is provided for entities and scenes
	// For the rest, nothing is implemented! (take care)
	// For an entity, the position of the node is copied to the node's position
	// and if there is mesh behind (OGREForm), the frame is converted into a skeleton movement
	// For an scene, what is passed (by default only) is the background as an sky object...
	virtual void whenUpdatePositionsForWorld (OGREElementAdditionalData* eD, QGAMES::World* wrld) { }
	virtual void whenUpdatePositionsForMap (OGREElementAdditionalData* eD, QGAMES::Map* mp) { }

#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
	/** Can be overloaded, if any. */
	virtual Ogre::MaterialManager::Listener* createMaterialMngListener ()
							{ return (new STDTechniqueResolverListener (_shaderGenerator)); }
#endif

	protected:
	/** The basic of anything in Ogree. */
	Ogre::Root* _root;
	/** The scene manager any game needs. 
		A game could have many scxene managers but one is minimum. */
	SceneManagers _sceneManagers;
	/** The overlay system.
		It is used to put 2d images over the 3d world. 
		The elements to introduce there have to be score objects. */
	Ogre::OverlaySystem* _overlaySystem;
	/** The overlay system need an specific overlay. */
	Ogre::Overlay* _overlay;
	/** A set of camera managers used to control de camera associated to an scene manager. */
	QGAMES::MapOfCameras _cameraManagers;
	/** The windows created. */
	Screens _screens;
	/** The render queue listener used by the system.
		Event if there are more than one scene manager the render queue listener will be unique. */
	Ogre::RenderQueueListener* _renderQueueListener;

#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
	/** To manage when the shadder can't be managed. */
	class STDTechniqueResolverListener : public Ogre::MaterialManager::Listener 
	{
		public:
		STDTechniqueResolverListener (Ogre::RTShader::ShaderGenerator* sG)
			: _shaderGenerator (sG)
							{ }

		/** @see parent. */
		virtual Ogre::Technique* handleSchemeNotFound (unsigned short schemeIndex,
			const Ogre::String& schemeName, Ogre::Material* originalMaterial, unsigned short lodIndex,
			const Ogre::Renderable* rend);
		virtual bool afterIlluminationPassesCreated (Ogre::Technique* tech);
		virtual bool beforeIlluminationPassesCleared (Ogre::Technique* tech);

		protected:
		Ogre::RTShader::ShaderGenerator* _shaderGenerator; 
	};

	/** An reference to the instance generator used. */
	Ogre::RTShader::ShaderGenerator* _shaderGenerator;
	/** Shadder generator material manager listener. */
	Ogre::MaterialManager::Listener* _materialMgrListener;
	/** Where the shadders are defined. */
	Ogre::String _RTShaderLibPath;
#endif // INCLUDE_RTSHADER_SYSTEM

};

#endif

// End of the file
/*@}*/