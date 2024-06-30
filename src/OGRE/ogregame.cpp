#include <CommonOGRE/ogregame.hpp>
#include <CommonOGRE/ogredefinitions.hpp>
#include <CommonOGRE/ogrescreen.hpp>
#include <CommonOGRE/ogreinputhandler.hpp>
#include <CommonOGRE/ogreformbuilder.hpp>
#include <FMOD/fmodsoundbuilder.hpp>  // OGRE library uses FMOD sound librray to produce sounds...
#include <CommonOGRE/ogreform.hpp>
#include <Common/game.hpp>
#include <Common/genalgorithms.hpp>
#include <OgreHeaderPrefix.h>
#include <OgreString.h>
#include <OgreConfigFile.h>
#include <OgreResourceManager.h>
#include <OgreOverlayManager.h>
#include <OgrePlugin.h>
#include <RTShaderSystem/OgreRTShaderSystem.h>
#include <OgreHeaderSuffix.h>
#include <assert.h>

// ---
OGREGame::~OGREGame ()
{
	// The camera managers have not to be deleted as they are owned by screen...

	// Extract the render listener from any scene manager...
	for (OGREGame::SceneManagers::const_iterator i = _sceneManagers.begin ();
			i != _sceneManagers.end (); i++)
		(*i).second -> removeRenderQueueListener (_renderQueueListener);
	// Deletes the render queue listener for all scene managers...
	delete (_renderQueueListener);
	_renderQueueListener = NULL;

	// Clear the list of scenes managers (they are not destroyed but the system keeps cleaned)
	_sceneManagers.clear ();
	// ...and then destroy the overlay system...
	delete (_overlaySystem);
	_overlaySystem = NULL;

	// When the root is deleted the scene managers created behind are also deleted too...
	delete (_root);
	_root = NULL;
}

// ---
bool OGREGame::existSceneManager (const std::string& id)
{
	return (_sceneManagers.find (id) != _sceneManagers.end ());
}

// ---
Ogre::SceneManager* OGREGame::sceneManager (const std::string& id)
{
	OGREGame::SceneManagers::const_iterator i = _sceneManagers.find (id);
	assert (i != _sceneManagers.end ());
	return ((*i).second);
}

// ---
Ogre::SceneManager* OGREGame::mainSceneManager ()
{
	if (existSceneManager (std::string (__OGREMAINSCENEMANAGERID__)))
		return (sceneManager (std::string (__OGREMAINSCENEMANAGERID__)));

	assert (_sceneManagers.size () > (int) 0);

	return ((*_sceneManagers.begin ()).second);
}

// ---
bool OGREGame::existCameraManager (const std::string& id)
{
	return (_cameraManagers.find (id) != _cameraManagers.end ());
}

// ---
QGAMES::Camera* OGREGame::cameraManager (const std::string& id)
{
	QGAMES::MapOfCameras::const_iterator i = _cameraManagers.find (id);
	assert (i != _cameraManagers.end ());
	return ((*i).second);
}

// ---
QGAMES::Camera* OGREGame::mainCameraManager ()
{
	std::string cmN = std::string (__OGREMAINSCENEMANAGERID__) + std::string (":") + 
			std::string (__OGREMAINCAMERAID__); // the main camera for the main scene manager...
	if (existCameraManager (cmN))
		return (cameraManager (cmN));

	assert (_cameraManagers.size () > (int) 0);

	return ((*_cameraManagers.begin ()).second);
}

// ---
bool OGREGame::initialize ()
{
	// Inits ogre...	
	// The configuration file name is stored in a variable of the conf file.
	// Init also the plugins. They are needed for many things within ogre 3D engine!
	Ogre::String pluginsCfgFile = 
		game () -> parameter (std::string (__OGREPLUGINSFILEVARIABLE__));
	_root = new Ogre::Root (pluginsCfgFile);

	// ...and gets the configuration defined in the ogre configuration file called ogre.cfg
	// ..and stored where the runtime file is.
	// Be sure the information exists there!
	if (!_root -> restoreConfig ())
		return (false);
	// In this case, there is no any configuration dialog to show
	// The configuration data file has to exist previously
	// So the instalation of an ogre game is critical!

	// Initialize the root to avoid the autocreattion of a window...
	_root -> initialise (false);

	return (true);
}

// ---
QGAMES::FormBuilder* OGREGame::createFormBuilder ()
{
	std::string fN = game () -> parameter (__GAME_PROPERTYFORMSFILE__);
	std::string fN2D = 
		QGAMES::fileWithoutExtension (fN) + 
		std::string ("2D.") + // This is the default extension for 2D...
		QGAMES::fileExtension (fN);
	return (new OGREFormBuilder (fN, fN2D));
}

// ---
QGAMES::SoundBuilder* OGREGame::createSoundBuilder ()
{
	return (new FMODSoundBuilder (game () -> parameter (__GAME_PROPERTYSOUNDSFILE__)));
}

// ---
QGAMES::InputHandler* OGREGame::createInputHandler (QGAMES::InputHandler::Behaviour* bhv)
{
	return (new OGREInputHandler (_root, _game, bhv));
}

// ---
QGAMES::Screen* OGREGame::createScreen (const std::string& t, const QGAMES::Position& pos, 
		int w, int h, int pX, int pY)
{
	// There can't be any other screen already created with similar title
	// probably it is a limitation of the system...Who knows!!
	assert (_screens.find (t) == _screens.end ());
	// This is just only because a bug in SDL version
	// They say they will solve it in further versions...we will see!!
	// This happens when the version of the sdl linked to is less than 2.0.20!
	SDL_version v; SDL_GetVersion (&v);
	bool scl = SDL_VERSIONNUM (v.major, v.minor, v.patch) < SDL_VERSIONNUM (2, 0, 20);
	float sX = scl ? (float) std::atof (game () -> parameter (__GAME_XSCALE__).c_str ()) : 1.0f;
	float sY = scl ? (float) std::atof (game () -> parameter (__GAME_YSCALE__).c_str ()) : 1.0f;

	// Every new screen has to be created after the root of the system
	// and the camera managers. Otherwise the system will crash!
	// The root is created when the game is initialized!
	QGAMES::Screen* scr = new OGREScreen (root (), t, pos, w, h, pX, pY, sX, sY);
	_screens [t] = scr;

	// The screen is returned!
	return (scr);
}

// ---
void OGREGame::initializeScreens ()
{
	loadResources ();

	// Create the shadder system...if any
#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
	if (Ogre::RTShader::ShaderGenerator::initialize ())
	{
		_shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr ();
		// Create and register the material manager listener if it doesn't exist yet.
		if (!_materialMgrListener) 
			Ogre::MaterialManager::getSingleton ().addListener 
				(_materialMgrListener = createMaterialMngListener ());
	}
#endif

	// Creates the scene manager for the game...
	// Every scene manager has an id.
	// The default method creates the single scene manager with the name __OGREMAINSCENEMANAGERID__!
	_sceneManagers = createSceneManagers (); // Normal thing is having just one!

	// Creates the overlay system, and adds it to all the scene managers...
	// If there was different things to be painted per scene manager, 
	// it would have to be know at render queue listener level...
	_overlaySystem = new Ogre::OverlaySystem ();
	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton ();
	_overlay = overlayManager.create (__OGREOVERLAYLAYENAME__); // The compatibility with QGAMES:..
	for (OGREGame::SceneManagers::const_iterator i = _sceneManagers.begin ();
			i != _sceneManagers.end (); i++)
		(*i).second -> addRenderQueueListener (_overlaySystem);

	// The render queue listener is the point where OGRE reports events
	// after finishing the render process per queue!
	// The same situation that the Overlay System:
	// It is, there is only one queue listener in the system, so
	// If there were different things to do per scene manager, it would have to be defined at 
	// render queue listener behaviour level!
	// By default there is no additional queue listener than Overlay added to the system!
	_renderQueueListener = createRenderQueueListener ();
	// The QGAMES system uses the same render queue listener
	// to track all scene renders...
	if (_renderQueueListener != NULL)
		for (OGREGame::SceneManagers::const_iterator i = _sceneManagers.begin ();
				i != _sceneManagers.end (); i++)
			(*i).second -> addRenderQueueListener (_renderQueueListener);

	// Events from the input devices are listen at frame level
	// When the frame has been rendered and before moving the information to the graphics card
	// The events will be processed!!

	// When the transaction is set up first time, another queue listener is created
	// This is to treat the 2D drawings only. @see OGRE::Screen::Transaction for further details.

	// Every scene manager has to have cameras to track different elements.
	// The deafult behaviour only creates a camera per scene.
	for (OGREGame::SceneManagers::const_iterator i = _sceneManagers.begin ();
			i != _sceneManagers.end (); i++)
	{
		QGAMES::MapOfCameras cms = 
			createCameraManagersFor ((*i).first, (*i).second);
		_cameraManagers.insert (cms.begin (), cms.end ());
	}

	// Now it is time to determinate what the cameras are seeing is going
	// to be projected in the different screens of the game...
	// Per screen the cameras affected are selected (the cameras are connected to and scene manager). 
	// Then per camera a view port has to be created
	// and added to the screen that is going to visulized its content...
	// The size of the view determinates where in the screen the content of the came is projected.
	// In the default behaviour as there is only one content and one camera, there will
	// be just one viewport to visualized everything and filling the full screen.
	for (OGREGame::Screens::const_iterator i = _screens.begin ();
			i != _screens.end (); i++)
	{
		OGREScreen* scr = dynamic_cast <OGREScreen*> ((*i).second);
		// Is it valid for next steps?
		// Otherwise step over it, and goes to next!
		// It is not normal to create an type of screen not valid for ogre but the
		// code does it possible, so it has to be taken into consideration!
		if (scr == NULL)
			continue; 

		QGAMES::MapOfCameras cms = cameraManagersForScreen (scr);
		for (QGAMES::MapOfCameras::const_iterator j = cms.begin ();
				j != cms.end (); j++)
		{
			OGRECameraWrapper* cm = 
				dynamic_cast <OGRECameraWrapper*> ((*j).second -> externalLink ()); 
			if (!cm)
				continue; // Not valid...the wrapper set either is null or not an ogre wrapper...

			Ogre::Viewport* vp = 
				createViewportForCameraInWindow ((*j).first, cm -> getCamera (), scr -> window ());
			// Adds the camara controller to the screen...
			scr -> addCamera ((*j).second);
			// and that one is set as the default one...
			scr -> setCurrentCamera ((*j).second);
			// Take into account that the constructor of the OGREScreen has built a default camera
			// This camera should be used for 2D purposes only...
		}
	}

	installResources ();
}

// ---
void OGREGame::finalizeScreens ()
{
	const Ogre::GpuProgramManager& gpuMgr = Ogre::GpuProgramManager::getSingleton ();
	if (gpuMgr.getSaveMicrocodesToCache () && gpuMgr.isCacheDirty ())
	{
		Ogre::String binFile = 
			game () -> parameter (std::string (__OGRECACHEFILEVARIABLE__));
		std::fstream outFile (binFile.c_str (), std::ios::out | std::ios::binary);
		if (outFile.is_open ())
			gpuMgr.saveMicrocodeCache (Ogre::DataStreamPtr 
				(new Ogre::FileStreamDataStream (binFile, &outFile, false)));
	}

#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
	Ogre::MaterialManager::getSingleton ().setActiveScheme (Ogre::MaterialManager::DEFAULT_SCHEME_NAME);
	if (_materialMgrListener != NULL)
	{
		Ogre::MaterialManager::getSingleton ().removeListener (_materialMgrListener);
		delete _materialMgrListener; _materialMgrListener = NULL;
	}

	// Destroy RTShader system.
	if (_shaderGenerator != NULL)
	{
		Ogre::RTShader::ShaderGenerator::destroy ();
		_shaderGenerator = NULL;
	}
#endif
}

// ---
void OGREGame::inEveryLoop ()
{
	// Manage th visibility of the score objects...
	// If no score object is visible, then the score layer (Overlay) will not be visible too.
	bool someVisible = false;
	const QGAMES::Entities& sObjs = game () -> scoreObjects ();
	for (QGAMES::Entities::const_iterator i = sObjs.begin (); i != sObjs.end () && !someVisible; i++)
		if ((*i).second -> isVisible ())
			someVisible = true;
	if (!someVisible)
		_overlay -> hide (); // The overlay layer is not visible...

	QGAMES::GameImplementation::inEveryLoop ();
}

// ---
void OGREGame::whenCreateElement (QGAMES::Element* elmnt)
{
	assert (elmnt);

	// This is common for any type of element...(real 3D element) in OGRE
	OGREElementAdditionalData* eD = new OGREElementAdditionalData ();
	eD -> _ogreSceneManager = (*_sceneManagers.begin ()).second;
	// Take into account that all elements are by default attached to the same 
	// sceneManager that is, in the other one, the one created (hopefully it has to exist!!)
	// That means that all elements will be rendered with the same quality.
	// This behaviour obviously has to be change in order to improve felixbility of the final outcome.
	// If no sceneManager had been created, the previous instruction would issue an error in debug mode!

	if (dynamic_cast <QGAMES::Entity*> (elmnt) != NULL)
		whenCreateEntity (eD, (QGAMES::Entity*) elmnt);
	else if (dynamic_cast <QGAMES::World*> (elmnt) != NULL)
		whenCreateWorld (eD, (QGAMES::World*) elmnt);
	else if (dynamic_cast <QGAMES::Scene*> (elmnt) != NULL)
		whenCreateScene (eD, (QGAMES::Scene*) elmnt);
	else if (dynamic_cast <QGAMES::Map*> (elmnt) != NULL)
		whenCreateMap (eD, (QGAMES::Map*) elmnt);
	
	elmnt -> setExternalData (eD);
}

// ---
void OGREGame::whenInEveryLoop (QGAMES::Element* elmnt)
{
	OGREElementAdditionalData* eD = 
		dynamic_cast <OGREElementAdditionalData*> (elmnt -> externalData ());
	if (!eD)
		return; // It shouln't happen, but just in case...

	if (dynamic_cast <QGAMES::Entity*> (elmnt) != NULL)
		whenInEveryLoopForEntity (eD, (QGAMES::Entity*) elmnt);
	else if (dynamic_cast <QGAMES::World*> (elmnt) != NULL)
		whenInEveryLoopForWorld (eD, (QGAMES::World*) elmnt);
	else if (dynamic_cast <QGAMES::Scene*> (elmnt) != NULL)
		whenInEveryLoopForScene (eD, (QGAMES::Scene*) elmnt);
	else if (dynamic_cast <QGAMES::Map*> (elmnt) != NULL)
		whenInEveryLoopForMap (eD, (QGAMES::Map*) elmnt);
}

// ---
void OGREGame::whenUpdatePositions (QGAMES::Element* elmnt)
{
	OGREElementAdditionalData* eD = 
		dynamic_cast <OGREElementAdditionalData*> (elmnt -> externalData ());
	if (!eD)
		return; // Can't happen, but just in case...

	if (dynamic_cast <QGAMES::Entity*> (elmnt) != NULL)
		whenUpdatePositionsForEntity (eD, (QGAMES::Entity*) elmnt);
	else if (dynamic_cast <QGAMES::World*> (elmnt) != NULL)
		whenUpdatePositionsForWorld (eD, (QGAMES::World*) elmnt);
	else if (dynamic_cast <QGAMES::Scene*> (elmnt) != NULL)
		whenUpdatePositionsForScene (eD, (QGAMES::Scene*) elmnt);
	if (dynamic_cast <QGAMES::Map*> (elmnt) != NULL)
		whenUpdatePositionsForMap (eD, (QGAMES::Map*) elmnt);
}

// ---
OGREGame::SceneManagers OGREGame::createSceneManagers ()
{
	// Just only one by default...
	// The type of scene created is generic
	// The plugin used is by default: Plugin_BSPSceneManager (So, it should be loaded when init: @see plugin.cfg)
	OGREGame::SceneManagers scnMng;
	scnMng [__OGREMAINSCENEMANAGERID__] = 
		_root -> createSceneManager 
			(Ogre::DefaultSceneManagerFactory::FACTORY_TYPE_NAME, __OGREMAINSCENEMANAGERID__);
	return (scnMng);
}

// ---
QGAMES::MapOfCameras OGREGame::createCameraManagersFor (const std::string& name, 
	Ogre::SceneManager* scnMng)
{
	QGAMES::MapOfCameras result;

	// Creates the camera
	// The camera has the same name of the scene manager that it follows...
	Ogre::Camera* cm = scnMng -> createCamera (name);

	// The initial position of the camera is 0
	cm -> setPosition (Ogre::Vector3 (0, 0, 0));
	// Look back along -Z (as default)
	cm -> lookAt (Ogre::Vector3 (0, 0, 100));
	// Closer than 5 pixels doesn't draw anything...
	cm -> setNearClipDistance (5);

	// Adds the camera just created...
	// The name is something composed by the name of the scene manager 
	// and the name of the camera itself, separated by colon.
	std::string nCM = name + std::string (":") + std::string (__OGREMAINCAMERAID__);
	result [nCM] = new OGREFreeMovementCamera (nCM, cm);
	result [nCM] -> setExternalLink (new OGRECameraWrapper (cm)); // Links the external camera to be actualized...

	// ..and returns the result...
	return (result);   
}

// ---
Ogre::Viewport* OGREGame::createViewportForCameraInWindow (const std::string& n, 
	Ogre::Camera* cm, Ogre::RenderWindow* rw)
{
	Ogre::Viewport* vp = rw -> addViewport (cm);
	// The default background color is black!
	vp -> setBackgroundColour (__OGREDEFAULTVIEWPORTBACKGROUND__);
	// The viewport size is ful screen size...
	// Alter the camera aspect ratio to match the viewport (This is the QGAMES behaviour)
	cm -> setAspectRatio 
		(Ogre::Real (vp -> getActualWidth ()) / Ogre::Real (vp -> getActualHeight ()));

	return (vp);
}

// ---
void OGREGame::loadResources ()
{
	// Gets the configuration filenames for resources used by ogree 3d engine
	// The name of the resource file is stored in a variable of the conf file.
	Ogre::String resourcesCfgFile = 
		game () -> parameter (std::string (__OGRERESOURCESFILEVARIABLE__));
	Ogre::ConfigFile cfg; cfg.load (resourcesCfgFile);

	// Now go through all sections & settings in the file
	// adding the resources into the resource manager...
	Ogre::ConfigFile::SettingsBySection_ sbs = cfg.getSettingsBySection ();
	for (Ogre::ConfigFile::SettingsBySection_::const_iterator i = sbs.begin (); i != sbs.end (); i++)
	{
		Ogre::String secName = (*i).first;
		Ogre::ConfigFile::SettingsMultiMap settings = (*i).second;
		Ogre::ConfigFile::SettingsMultiMap::iterator j;
		Ogre::String typeName, archName;
		for (j = settings.begin (); j != settings.end (); j++)
		{
			typeName = (*j).first;
			archName = (*j).second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
			// OS X does not set the working directory relative to the app.
			// In order to make things portable on OS X we need to provide
			// the loading with it's own bundle path location.
			if (!Ogre::StringUtil::startsWith (archName, "/", false)) // only adjust relative directories
				archName = Ogre::String (Ogre::macBundlePath () + "/" + archName);
#endif
			Ogre::ResourceGroupManager::getSingleton ().
				addResourceLocation (archName, typeName, secName);
			// Store them in the resoures manager...
		}
	}

	// Adds a couple of things in the GENERAL block...
	Ogre::String secName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	const Ogre::ResourceGroupManager::LocationList genLocs = 
		Ogre::ResourceGroupManager::getSingleton ().getResourceLocationList (secName);
	assert (!genLocs.empty ()); // It has to be...
	Ogre::String archName = genLocs.front ().archive -> getName ();
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	arch = Ogre::FileSystemLayer::resolveBundlePath("Contents/Resources/Media");
#else
	archName = Ogre::StringUtil::replaceAll(archName, "Media/../../Tests/Media", "");
	archName = Ogre::StringUtil::replaceAll(archName, "media/../../Tests/Media", "");
#endif
	Ogre::String typeName = genLocs.front ().archive -> getType ();

	// Special use for CG Plugin?
	bool hasCgPlugin = false;
	const Ogre::Root::PluginInstanceList& plugins = _root -> getInstalledPlugins ();
	for (size_t i = 0; i < plugins.size () && !hasCgPlugin; i++)
		if (plugins [i] -> getName () == "Cg Program Manager")
			hasCgPlugin = true;
	bool use_HLSL_Cg_shared = hasCgPlugin || Ogre::GpuProgramManager::getSingleton ().isSyntaxSupported ("hlsl");

	// Locations for shadder languages...
	if (Ogre::GpuProgramManager::getSingleton ().isSyntaxSupported ("glsles"))
		Ogre::ResourceGroupManager::getSingleton ().addResourceLocation 
			(archName + "/materials/programs/GLSLES", typeName, secName);
	else 
	if (Ogre::GpuProgramManager::getSingleton ().isSyntaxSupported ("glsl"))
	{
		Ogre::ResourceGroupManager::getSingleton ().addResourceLocation 
			(archName + "/materials/programs/GLSL120", typeName, secName);
		if (Ogre::GpuProgramManager::getSingleton ().isSyntaxSupported ("glsl150"))
			Ogre::ResourceGroupManager::getSingleton ().addResourceLocation 
				(archName + "/materials/programs/GLSL150", typeName, secName);
		else
		if (Ogre::GpuProgramManager::getSingleton ().isSyntaxSupported ("glsl400"))
			Ogre::ResourceGroupManager::getSingleton ().addResourceLocation 
				(archName + "/materials/programs/GLSL400", typeName, secName);
		else
			Ogre::ResourceGroupManager::getSingleton ().addResourceLocation 
				(archName + "/materials/programs/GLSL", typeName, secName);
	}
	else 
	if (Ogre::GpuProgramManager::getSingleton ().isSyntaxSupported ("hlsl"))
		Ogre::ResourceGroupManager::getSingleton ().addResourceLocation
			(archName + "/materials/programs/HLSL", typeName, secName);

	if (hasCgPlugin)
		Ogre::ResourceGroupManager::getSingleton ().addResourceLocation
			(archName + "/materials/programs/Cg", typeName, secName);
	if (use_HLSL_Cg_shared)
		Ogre::ResourceGroupManager::getSingleton ().addResourceLocation 
			(archName + "/materials/programs/HLSL_Cg", typeName, secName);

#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
	_RTShaderLibPath = archName + "/RTShaderLib";
	Ogre::ResourceGroupManager::getSingleton ().addResourceLocation 
		(_RTShaderLibPath + "/materials", typeName, secName);

	if (Ogre::GpuProgramManager::getSingleton ().isSyntaxSupported ("glsles"))
	{
		Ogre::ResourceGroupManager::getSingleton ().addResourceLocation 
			(_RTShaderLibPath + "/GLSL", typeName, secName);
		Ogre::ResourceGroupManager::getSingleton ().addResourceLocation 
			(_RTShaderLibPath + "/GLSLES", typeName, secName);
	}
	else
	if(Ogre::GpuProgramManager::getSingleton ().isSyntaxSupported ("glsl"))
		Ogre::ResourceGroupManager::getSingleton ().addResourceLocation 
			(_RTShaderLibPath + "/GLSL", typeName, secName);
	else 
	if (Ogre::GpuProgramManager::getSingleton ().isSyntaxSupported ("hlsl"))
		Ogre::ResourceGroupManager::getSingleton ().addResourceLocation 
			(_RTShaderLibPath + "/HLSL", typeName, secName);

	if (hasCgPlugin)
		Ogre::ResourceGroupManager::getSingleton ().addResourceLocation 
			(_RTShaderLibPath + "/Cg", typeName, secName);
	if (use_HLSL_Cg_shared)
		Ogre::ResourceGroupManager::getSingleton ().addResourceLocation
			(_RTShaderLibPath + "/HLSL_Cg", typeName, secName);
#endif
}

// ---
void OGREGame::installResources()
{
	Ogre::TextureManager::getSingleton ().setDefaultNumMipmaps (5);
	Ogre::ResourceGroupManager::getSingleton ().initialiseAllResourceGroups ();
}

// ---
void OGREGame::whenCreateEntity (OGREElementAdditionalData* eD, QGAMES::Entity* ety)
{
	// Is the new object and score object...
	// This is special because it has to be added to the overlay layer later...
	if (dynamic_cast <QGAMES::ScoreObject*> (ety) != NULL)
	{
		eD -> _ogreSceneNode = eD -> _ogreSceneManager -> createSceneNode 
			(std::string (__OGRESCOREOBJNODEPREFIX__) + std::to_string (ety -> id ())); // Not from the root...
		// The name of the node has something to do with the name of scoreobject (the id really)
		_overlay -> add3D (eD -> _ogreSceneNode); // ...and directly added to the overlay...
	}
	else
		eD -> _ogreSceneNode = eD -> _ogreSceneManager -> 
			getRootSceneNode () -> createChildSceneNode 
				(std::string (__OGREENTITYNODEPREFIX__) + std::to_string (ety -> id ())); 
			// The node the entity is attached to. It has a name relatie to the entity inside...

	// A 2D elemenent has nothing to do over here...

	// Notice that the ogreEntity attribute is not set
	// It will be set first time the main loop iterates if any...
}

// ---
void OGREGame::whenInEveryLoopForEntity (OGREElementAdditionalData* eD, QGAMES::Entity* ety)
{
	// Moves the animation state forward...if any
	if (eD -> _ogreAnimationState != NULL)
	{
		Ogre::Real tF = ((OGREInputHandler*) game () -> inputHandler ()) -> 
			handler () -> lastFrameEvent () -> timeSinceLastFrame;
		float spd = eD -> _lastAnimation -> 
			parameterAsFloat (std::string (__OGREANIMATIONLOOPSPEEDVARIABLE__));
		if (spd == 0.0f) spd = 1.0f; // It can't be 0...
		eD -> _ogreAnimationState -> addTime (tF * spd);
	}
}

// ---
void OGREGame::whenUpdatePositionsForEntity (OGREElementAdditionalData* eD, QGAMES::Entity* ety)
{
	// If the entity has changed his form from the last time..
	if (ety -> currentForm () != eD -> _lastForm)
	{
		eD -> _lastForm = ety -> currentForm ();
		// ..first detach and delete the all one...
		if (eD -> _ogreEntity != NULL)
		{
			eD -> _ogreSceneNode -> detachObject (eD -> _ogreEntity);
			delete (eD -> _ogreEntity);
			eD -> _ogreEntity = NULL;
		}
				
		// Then sets a new one, if the new form is 3D...
		// It could be 2D but then it can't be associated to any node...
		// But remember that the node exists...
		OGREForm* oF = dynamic_cast <OGREForm*> (eD -> _lastForm);
		if (oF != NULL)
		{
			eD -> _ogreEntity = eD -> _ogreSceneManager -> createEntity (oF -> mesh ());
			eD -> _ogreSceneNode -> attachObject (eD -> _ogreEntity);
		}
	}
		
	// If the entity's animation has changed since last time...
	// (Take into account that the OGRE entity could have disappeared)
	if (ety -> currentAnimation () != eD -> _lastAnimation)
	{
		if (eD -> _ogreEntity != NULL) // It could be null...for a 2D entity...
		{
			if (ety -> currentAnimation () != NULL)
			{
				std::string aName = ety -> currentAnimation () -> 
					parameter (std::string (__OGREANIMATIONNAMEVARIABLE__));
				if (eD -> _ogreEntity -> hasAnimationState (aName)) // Just in case there is an error...
				{
					eD -> _ogreAnimationState = eD -> _ogreEntity -> getAnimationState (aName); // Important...
					eD -> _ogreAnimationState -> setLoop (ety -> currentAnimation () -> 
						parameterAsBool (std::string (__OGREANIMATIONLOOPVARIABLE__)));
					eD -> _ogreAnimationState -> setEnabled (true); // Starts...
				}
			}
			else
			{
				if (eD -> _lastAnimation != NULL)
				{
					eD -> _ogreAnimationState -> setEnabled (false);
					eD -> _ogreAnimationState = NULL;
				}
			}
		}

		eD -> _lastAnimation = ety -> currentAnimation ();
	}

	// Now it is time to set the scene node...
	// This node can't be null as it has been created when the entity was created (OGREEntityBuilder)
	eD -> _ogreSceneNode -> setPosition 
		(Ogre::Vector3 (Ogre::Real (ety -> position ().posX ()), 
						Ogre::Real (ety -> position ().posY ()), 
						Ogre::Real (ety -> position ().posZ ())));
	// Finally, if the entity exists (because his form is not null), its visibility is managed!
	if (eD -> _ogreEntity != NULL)
		eD -> _ogreEntity -> setVisible (ety -> isVisible ());

	// If the object is a score object, it has to be shown additionally...
	if (dynamic_cast <QGAMES::ScoreObject*> (ety) && ety -> isVisible ())
		_overlay -> show ();  // There was a visible object minimum...
}

// ---
void OGREGame::whenUpdatePositionsForScene (OGREElementAdditionalData* eD, QGAMES::Scene* scn)
{
	// Sets the ambient light...if the attribute exists...
	std::string prp1;
	if (scn -> existsProperty (std::string (__OGREAMBIENTLIGHTVARIABLE__)) &&
		eD -> storeInCache (std::string (__OGREAMBIENTLIGHTVARIABLE__), 
			(prp1 = scn -> property (std::string (__OGREAMBIENTLIGHTVARIABLE__)))))  
	// Does the variable exist and has it changed?
	{
		std::vector <int> alV = QGAMES::getElementsFromAsInt (prp1, ',', 3);
		eD -> _ogreSceneManager -> // RGB values in ogre from 0 to 1...
			setAmbientLight (Ogre::ColourValue (alV [0] / 255.0f, alV [1] / 255.0f, alV [2] / 255.0f));
	}

	// Sets a light if it has been defined...and not set before...
	std::string prp2;
	if (scn -> existsProperty (std::string (__OGRELIGHTVARIABLE__)) &&
		eD -> storeInCache (std::string (__OGRELIGHTVARIABLE__), 
			(prp2 = scn -> property (std::string (__OGRELIGHTVARIABLE__)))))  
	// Does the variable exist and has it changed?
	{
		std::vector <std::string> lDV = QGAMES::getElementsFrom (prp2, ' ', 2);
		if (eD -> _ogreSceneManager -> hasLight (lDV [0])) // Just created?...
			eD -> _ogreSceneManager -> destroyLight (lDV [0]);
		std::vector <int> lCV = QGAMES::getElementsFromAsInt (lDV [1], ',', 3);
		Ogre::Light* light = eD -> _ogreSceneManager -> createLight (lDV [0]);
		Ogre::SceneNode* lN = eD -> _ogreSceneManager -> createSceneNode ();
		lN -> attachObject (light);
		lN -> setPosition (Ogre::Real (lCV [0]), Ogre::Real (lCV [1]), Ogre::Real (lCV [2]));
	}

	// Sets the sky dome...if any
	std::string prp3;
	if (scn -> existsProperty (std::string (__OGRESKYDOMEVARIABLE__)) &&
		eD -> storeInCache (std::string (__OGRESKYDOMEVARIABLE__), 
			(prp3 = scn -> property (std::string (__OGRESKYDOMEVARIABLE__))))) 
	// Does the variable exist and has it changed?
		eD -> _ogreSceneManager -> setSkyDome (true, prp3);
}

// ---
Ogre::Technique* OGREGame::STDTechniqueResolverListener::handleSchemeNotFound
	(unsigned short schemeIndex,
		const Ogre::String& schemeName, Ogre::Material* originalMaterial, unsigned short lodIndex,
		const Ogre::Renderable* rend)
{
	if (schemeName != Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME)
		return (NULL); // Nothing to do, this is already the default one, so OGRE's wrong installation... 

	if (!_shaderGenerator -> createShaderBasedTechnique
		(*originalMaterial, Ogre::MaterialManager::DEFAULT_SCHEME_NAME, schemeName))
		return (NULL); // The basic installation has been done...

	// Force creating the shaders for the generated technique.
	_shaderGenerator -> validateMaterial
		(schemeName, originalMaterial -> getName (), originalMaterial -> getGroup ());
	// Grab the generated technique.
	Ogre::Technique* result = NULL;
	Ogre::Material::Techniques::const_iterator i;
	for (i = originalMaterial -> getTechniques ().begin (); 
			i != originalMaterial -> getTechniques ().end () && !result; i++)
		if ((*i) -> getSchemeName () == schemeName)
			result = (*i);
	return (result);
}

// ---
bool OGREGame::STDTechniqueResolverListener::afterIlluminationPassesCreated (Ogre::Technique* tech)
{
	bool result = false;
	if (tech->getSchemeName() == Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME)
	{
		Ogre::Material* mat = tech -> getParent ();
		_shaderGenerator -> validateMaterialIlluminationPasses (tech -> getSchemeName (),
			mat -> getName (), mat -> getGroup ());
		result = true;
	}

	return (result);
}

// ---
bool OGREGame::STDTechniqueResolverListener::beforeIlluminationPassesCleared (Ogre::Technique* tech)
{
	bool result = false;
	if (tech-> getSchemeName () == Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME)
	{
		Ogre::Material* mat = tech -> getParent ();
		_shaderGenerator -> invalidateMaterialIlluminationPasses (tech -> getSchemeName (),
			mat -> getName (), mat -> getGroup ());
		result = true;
	}

	return (result);
}
