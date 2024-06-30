#include <CommonOGRE/ogrescreen.hpp>
#include <CommonOGRE/ogredefinitions.hpp>
#include <Common/game.hpp>
#include <Common/resourcesreader.hpp>
#include <OgreViewport.h>
#include <OgreRoot.h>
#include <OgreCamera.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <Windows.h> // To move the screen to the middle of the screen...
#endif
#include <SDL_image.h>
#include <SDL_syswm.h> // To get native Window info to be passed to OGRE...
#include <iostream>

int OGREScreen::_ONEWINDOWCREATED = 0;

// ---
OGREScreen::OGREScreen (Ogre::Root* r, 
		const std::string& t, const QGAMES::Position& p, int w, int h, int wPX, int wPY, float sX, float sY)
	: QGAMES::Screen (t, w, h),
	  _root (r),
	  _window (NULL),
	  _SDLWindow (NULL),
	  _SDLRenderer (NULL),
	  _windowPositionX (wPX), _windowPositionY (wPY),
	  _scaleX (sX), _scaleY (sY),
	  _whiteMaterial (), /** Null */ _formMaterials () /** Empty. */
{
	assert (_root);

	// Create the window associated to a SDL one...
	Ogre::NameValuePairList miscParams;
	Ogre::ConfigOptionMap ropts = _root -> getRenderSystem () -> getConfigOptions ();
	miscParams ["FSAA"] = ropts ["FSAA"].currentValue;
	miscParams ["vsync"] = ropts ["VSync"].currentValue;
	miscParams ["gamma"] = ropts ["sRGB Gamma Conversion"].currentValue;
	if (_ONEWINDOWCREATED != 0)
		miscParams ["currentGLContext"] = "true"; // The global context was already created...

	if (!SDL_WasInit (SDL_INIT_VIDEO))
	{
		int e = SDL_InitSubSystem (SDL_INIT_VIDEO);
#ifndef NDEBUG
		if (e != 0)
			std::cout << SDL_GetError () << std::endl; // Just in case...
#endif

		Uint32 flags = (ropts ["Full Screen"].currentValue == "Yes") 
			? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE; // Full screen?
		_SDLWindow = SDL_CreateWindow (t.c_str(),
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags); // A pointer to the SDL Window...
		assert (_SDLWindow); // Has to be created...
		_SDLRenderer = SDL_CreateRenderer (_SDLWindow, -1, 
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
		assert (_SDLRenderer); // Too. It is neccessary to draw later in 2D...

		SDL_SysWMinfo wmInfo;
		SDL_VERSION (&wmInfo.version);
		SDL_GetWindowWMInfo (_SDLWindow, &wmInfo); // Shich is the Id of the new window?

		miscParams ["externalWindowHandle"] = Ogre::StringConverter::toString (size_t (wmInfo.info.win.window));
	}

	_window = _root -> createRenderWindow (t, w, h, false, &miscParams);
	_window -> resize (w, h);
	_window -> reposition (wPX, wPY);

	_ONEWINDOWCREATED++;

	// Sets the camera, because none has been created before by default...
	QGAMES::Camera* cm = new QGAMES::Camera (
			std::string (__QGAMES_MAINCAMERAID__), // Main camera...
			QGAMES::Camera::ProjectionType::_ORTHOGONAL, 
			new QGAMES::ZoommerOnScreen (__BD 1 /** no zoom. */, w, h, QGAMES::ZoommerOnScreen::_TOUPLEFT), 
			w, h, p);
	addCamera (cm); // Probably this camera will be deleted later as it is not an  "OGRE" camara!
	setCurrentCamera (cm); // By default the camera is the current one...
}

// ---
OGREScreen::~OGREScreen ()
{
	_root -> destroyRenderTarget (_window);

	SDL_DestroyWindow (_SDLWindow);
	if (--_ONEWINDOWCREATED == 0)
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

// ---
void OGREScreen::setIcon (QGAMES::Form* f)
{
	QGAMES::Sprite2D* sp = static_cast <QGAMES::Sprite2D*> (f);
	if (!sp)
		return; // It has to be something in 2D

	// The form used has to be an sprite 2d...
	// Otherwise the icon will not be drawn!
	std::string s = sp -> source (); 
	SDL_Surface* sfc = IMG_Load ((__RL (s)).c_str ());
	SDL_SetWindowIcon (_SDLWindow, sfc);
	SDL_FreeSurface (sfc);
}

// ---
void OGREScreen::windowAtPosition (int x, int y)
{
	if (_window -> isFullScreen ())
		return; // Full screen window can't be move to any position!

	_window -> reposition (x, y);
	_windowPositionX = x; _windowPositionY = y;
}

// ---
void OGREScreen::windowAtCenter ()
{
	if (_window -> isFullScreen ()) 
		return; // It is just in the middle!

	SDL_DisplayMode d;
	SDL_GetCurrentDisplayMode (0, &d);
	windowAtPosition (((int) ((float) d.w / _scaleX) - _visualWidth) >> 1, 
		(int) ((int) ((float) d.h / _scaleY) - _visualHeight) >> 1);
}

// ---
void OGREScreen::OGRETransaction::keepPoint (int x, int y, const QGAMES::Color& c)
{
	keepElementToDraw (OGREScreen::OGRETransaction::ElementToDraw 
		(OGREScreen::OGRETransaction::ElementToDraw::_POINT, numberVertexs (), 1));

	keepVertex (OGREScreen::OGRETransaction::Vertex 
		(__BD x / __BD (screen () -> visualWidth () >> 1), 
		 __BD y / __BD (screen () -> visualHeight () >> 1), 
		 __BD 0, 
		 c));

	addRenderPointOperation ();
}

// ---
void OGREScreen::OGRETransaction::keepLine (int x1, int y1, int x2, int y2, const QGAMES::Color& c)
{
	int sW = screen () -> visualWidth () >> 1;
	int sH = screen () -> visualHeight () >> 1;

	keepElementToDraw (OGREScreen::OGRETransaction::ElementToDraw 
		(OGREScreen::OGRETransaction::ElementToDraw::_LINE, numberVertexs (), 2));

	keepVertex (OGREScreen::OGRETransaction::Vertex 
		(__BD x1 / __BD sW, __BD y1 / __BD sH, __BD 0, c));
	keepVertex (OGREScreen::OGRETransaction::Vertex 
		(__BD x2 / __BD sW, __BD y2 / sH, __BD 0, c));

	addRenderLineOperation ();
}

// ---
void OGREScreen::OGRETransaction::keepTriangle (int x1, int y1, int x2, int y2, int x3, int y3, 
	const QGAMES::Color& c)
{
	int sW = screen () -> visualWidth () >> 1;
	int sH = screen () -> visualHeight () >> 1;

	keepElementToDraw (OGREScreen::OGRETransaction::ElementToDraw 
		(OGREScreen::OGRETransaction::ElementToDraw::_TRIANGLE, numberVertexs (), 3));

	keepVertex (OGREScreen::OGRETransaction::Vertex 
		(__BD x1 / __BD sW, __BD y1 / __BD sH, __BD 0, c));
	keepVertex (OGREScreen::OGRETransaction::Vertex 
		(__BD x2 / __BD sW, __BD y2 / __BD sH, __BD 0, c));
	keepVertex (OGREScreen::OGRETransaction::Vertex 
		(__BD x3 / __BD sW, __BD y3 / __BD sH, __BD 0, c));

	addRenderTriangleOperation ();
}

// ---
void OGREScreen::OGRETransaction::keepGraphic 
	(OGRE2DForm* frm, int x, int y, int px, int py, int w, int h, int fW, int fH, int a)
{
	assert (frm);

	int sW = screen () -> visualWidth () >> 1;
	int sH = screen () -> visualHeight () >> 1;

	OGREScreen* scr = dynamic_cast <OGREScreen*> (_screen);
	assert (scr); // Just in case...

	Ogre::MaterialPtr mat = scr -> formMaterial (frm);
	keepElementToDraw (OGREScreen::OGRETransaction::ElementToDraw 
		(OGREScreen::OGRETransaction::ElementToDraw::_SPRITE, numberVertexs (), 6, mat, px, py, w, h, fW, fH));

	QGAMES::Color bkColor (255, 255, 255, a); // It will be multiplied...

	// The 0,0 position of the texture is the bottom left corner
	// So to get the right position to draw it is necessary to substract the position from the height received!
	// Important to notice that the coordinates in OGRE (Y axis) goes up instead of down
	// That is the reference in QGAMES. Take this into account when you read how py and h is managed...
	keepVertex (OGREScreen::OGRETransaction::Vertex 
		(__BD x / __BD sW, __BD y / __BD sH, __BD 0, bkColor, 
			Ogre::Vector2 ((Ogre::Real) px / (Ogre::Real) fW, (Ogre::Real) (fH - py) / (Ogre::Real) fH)));
	keepVertex (OGREScreen::OGRETransaction::Vertex 
		(__BD (x + w) / __BD sW, __BD y / __BD sH, __BD 0, bkColor, 
			Ogre::Vector2 ((Ogre::Real) (px + w) / (Ogre::Real) fW, (Ogre::Real) (fH - py) / (Ogre::Real) fH)));
	keepVertex (OGREScreen::OGRETransaction::Vertex 
		(__BD (x + w) / __BD sW, __BD (y - h) / __BD sH, __BD 0, bkColor, 
			Ogre::Vector2 ((Ogre::Real) (px + w) / (Ogre::Real) fW, (Ogre::Real) (fH - (py - h)) / (Ogre::Real) fH)));

	keepVertex (OGREScreen::OGRETransaction::Vertex 
		(__BD x / __BD sW, __BD y / __BD sH, __BD 0, bkColor, 
			Ogre::Vector2 ((Ogre::Real) px / (Ogre::Real) fW, (Ogre::Real) (fH - py) / (Ogre::Real) fH)));
	keepVertex (OGREScreen::OGRETransaction::Vertex 
		(__BD (x + w) / __BD sW, __BD (y - h) / __BD sH, __BD 0, bkColor, 
			Ogre::Vector2 ((Ogre::Real) (px + w) / (Ogre::Real) fW, (Ogre::Real) (fH - (py - h)) / (Ogre::Real) fH)));
	keepVertex (OGREScreen::OGRETransaction::Vertex 
		(__BD x / __BD sW, __BD (y - h) / __BD sH, __BD 0, bkColor, 
			Ogre::Vector2 ((Ogre::Real) px / (Ogre::Real) fW, (Ogre::Real) (fH - (py - h)) / (Ogre::Real) fH)));

	addRenderGraphicOperation (mat);
}

// ---
void OGREScreen::OGRETransaction::renderQueueEnded 
	(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
{
	// If the renderer group is not the last, then go to next...
	if (queueGroupId != Ogre::RENDER_QUEUE_OVERLAY)
		return;

	// If there is nothing to render, then return...
	if (_vertexList.empty ())
		return;

	// Resize the buffer...
	_vertexBuffer = Ogre::HardwareBufferManager::getSingletonPtr () ->
		createVertexBuffer (_renderOp.vertexData -> vertexDeclaration -> getVertexSize (0),
							_vertexList.size (),
							Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
							false);
	_renderOp.vertexData -> vertexStart = 0;
	_renderOp.vertexData -> vertexBufferBinding -> setBinding (0, _vertexBuffer);

	// Pour out the data in the buffers...
	OGREScreen::OGRETransaction::Vertex* writeIterator = 
		(OGREScreen::OGRETransaction::Vertex*) _vertexBuffer -> lock (Ogre::HardwareBuffer::HBL_DISCARD);
	size_t i = 0;
	for (OGREScreen::OGRETransaction::VertexList::const_iterator i = _vertexList.begin ();
			i != _vertexList.end (); i++)
		*writeIterator++ = (*i);
	_vertexBuffer -> unlock ();

	// Prepare the render system...
	Ogre::RenderSystem* sys = Ogre::Root::getSingletonPtr () -> getRenderSystem ();
	sys -> _setWorldMatrix (Ogre::Matrix4::IDENTITY);
	sys -> _setProjectionMatrix (Ogre::Matrix4::IDENTITY);
	sys -> _setViewMatrix (Ogre::Matrix4::IDENTITY);
	sys -> _setTextureMatrix (0, Ogre::Matrix4::IDENTITY);
	sys -> _setCullingMode (Ogre::CULL_NONE);
	sys -> _setPolygonMode (Ogre::PolygonMode::PM_SOLID); // It should be obvious, but just in case...

	// Do the right drawing operations...
	for (OGREScreen::OGRETransaction::RenderOpList::const_iterator i = _renderOps.begin (); 
			i != _renderOps.end (); i++)
	{
		Ogre::SceneManager* sM = 
			((OGREScreen*) screen ()) -> window () -> getViewport (0) -> getCamera () -> getSceneManager ();
		assert (sM); // Just in case...
		Ogre::MaterialPtr wM = ((OGREScreen*) screen ()) -> whiteMaterial ();

		_renderOp.operationType = (*i)._type;

		// The possible types are point, list and triangle...
		if (_renderOp.operationType == Ogre::RenderOperation::OT_POINT_LIST ||
			_renderOp.operationType == Ogre::RenderOperation::OT_LINE_LIST)
			 sM -> _setPass (wM -> getTechnique (0) -> getPass (0));
		else
		{
			Ogre::MaterialPtr mat = _elementsToDraw [(*i)._initialElement]._material;
			sM -> _setPass (!mat ? wM -> getTechnique (0) -> getPass (0) : mat -> getTechnique (0) -> getPass (0));
			// At list the graphics are brawn using the white color...
		}

		_renderOp.vertexData -> vertexStart = (*i)._initialVertex;
		_renderOp.vertexData -> vertexCount = (*i)._numberVertexs;

		sys -> _render (_renderOp); // Render the op...
	}
}

// ---
void OGREScreen::OGRETransaction::openFirst ()
{
	// Add this objet as queue listener of the render queues
	// Just to treat with the 2D draws...
	OGREScreen* scr = dynamic_cast <OGREScreen*> (screen ());
	assert (scr); // Just in case...

	// Load the white texture is if wasn't loaded before...
	scr -> whiteMaterial (); // To be able to draw things with colors later...
							 // The result is not used. It is invoked here just to speed up the uses later...

	for (int i = 0; i < scr -> window () -> getNumViewports (); i++)
	{
		Ogre::SceneManager* scnMng = 
			scr -> window () -> getViewport (i) -> getCamera () -> getSceneManager ();
		scnMng -> addRenderQueueListener (this);
		_sceneManagers.push_back (scnMng);
	}

	// Initialize all the internal variables...
	_vertexList = OGREScreen::OGRETransaction::VertexList ();
	_elementsToDraw = OGREScreen::OGRETransaction::ElementToDrawList ();
	_renderOps = OGREScreen::OGRETransaction::RenderOpList ();
	_renderOp = Ogre::RenderOperation ();

	// Creates the vertext data op...
	_vertexData = OGRE_NEW Ogre::VertexData ();
	// ...and links it to the render operation...
	_renderOp.vertexData = _vertexData;

	// These two parameters will change along the operation...
	_renderOp.vertexData -> vertexStart = 0;
	_renderOp.operationType = Ogre::RenderOperation::OT_POINT_LIST;
	_renderOp.useIndexes = false; // No indexes...

	// Stablish the declaration...
	Ogre::VertexDeclaration* vertexDecl = _renderOp.vertexData -> vertexDeclaration;
	size_t offset = 0;
	// Position...
	vertexDecl -> addElement (0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	offset += Ogre::VertexElement::getTypeSize (Ogre::VET_FLOAT3);
	// Colour
	vertexDecl -> addElement (0, offset, Ogre::VET_FLOAT4, Ogre::VES_DIFFUSE);
	offset += Ogre::VertexElement::getTypeSize (Ogre::VET_FLOAT4);
	// Texture coordinates
	vertexDecl -> addElement (0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);

	// Now it is the time of the buffer...
	_vertexBuffer = Ogre::HardwareBufferManager::getSingletonPtr ()
		-> createVertexBuffer (vertexDecl -> getVertexSize (0),
							   1, // This size will be modified before drawing...
							   Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
							   false);
	// ..and links it to the data...
	_renderOp.vertexData -> vertexBufferBinding -> setBinding (0, _vertexBuffer);
}

// ---
void OGREScreen::OGRETransaction::closeLast ()
{
	// When the last transaction is closed, 
	// it has also to be removed from the list of queue listeners...
	for (int i = 0; i < (int) _sceneManagers.size (); i++)
		_sceneManagers [i] -> removeRenderQueueListener (this);
	_sceneManagers = std::vector <Ogre::SceneManager*> (); // Keep it clear...

	OGRE_DELETE (_vertexData);
	_vertexData = NULL;
}

// ---
void OGREScreen::OGRETransaction::addRenderOperation (int nE, int nV, 
	Ogre::RenderOperation::OperationType t, const Ogre::MaterialPtr& m)
{
	if ((int) _renderOps.size () == 0)
		_renderOps.push_back (OGREScreen::OGRETransaction::RenderOp (0, nE, 0, nV, t)); // A new one...
	else
	{
		OGREScreen::OGRETransaction::RenderOp lO = _renderOps [(int) _renderOps.size () - 1];
		OGREScreen::OGRETransaction::ElementToDraw lE = _elementsToDraw [lO._initialElement + lO._numberElements - 1];
		if (lO._type == t && lE._material == m)
		// The material and the size of the has to be also the same to reuse the last transaction...
		{
			_renderOps [(int) _renderOps.size () - 1]._numberElements += nE;
			_renderOps [(int) _renderOps.size () - 1]._numberVertexs += nV;
		}
		else
			_renderOps.push_back (OGREScreen::OGRETransaction::RenderOp 
				(lO._initialElement + lO._numberElements, nE, lO._initialVertex + lO._numberVertexs, nV, t)); 
		// A new one...
	}
}

// ---
void OGREScreen::drawForm (int x, int y, QGAMES::Form* f, int px, int py, int w, int h, int a)
{
	OGRE2DForm* oF = dynamic_cast <OGRE2DForm*> (f);
	if (!oF) 
		return; // Only OGRE2DForms can go beyond here...

	OGREScreen::OGRETransaction* trst = dynamic_cast <OGREScreen::OGRETransaction*> (currentTransaction ());
	assert (trst); // It shouldn't be null, but just in case...

	trst -> keepGraphic (oF, 
		x - (visualWidth () >> 1), (visualHeight () >> 1) - y, px, py, w, h, oF -> width (), oF -> height (), a);
}

// ---
void OGREScreen::drawPoint (int x, int y, const QGAMES::Color& c)
{ 
	OGREScreen::OGRETransaction* trst = dynamic_cast <OGREScreen::OGRETransaction*> (currentTransaction ());
	assert (trst); // It shouldn't be null, but just in case...

	trst -> keepPoint (x - (visualWidth () >> 1), (visualHeight () >> 1) - y, c); 
}

// ---
void OGREScreen::drawLine (int x1, int y1, int x2, int y2, const QGAMES::Color& c, int t)
{ 
	int sW = visualWidth () >> 1;
	int sH = visualHeight () >> 1;

	OGREScreen::OGRETransaction* trst = dynamic_cast <OGREScreen::OGRETransaction*> (currentTransaction ());
	assert (trst); // It shouldn't be null, but just in case...

	trst -> keepLine (x1 - sW, sH - y1, x2 - sW, sH - y2, c); 
}

// ---
void OGREScreen::drawRectangle (int x1, int y1, int x2, int y2, const QGAMES::Color& c, bool f)
{
	int sW = visualWidth () >> 1;
	int sH = visualHeight () >> 1;
	
	OGREScreen::OGRETransaction* trst = dynamic_cast <OGREScreen::OGRETransaction*> (currentTransaction ());
	assert (trst); // It shouldn't be null, but just in case...

	if (f)
	{
		trst -> keepTriangle (x1 - sW, sH - y1, x2 - sW, sH - y1, x2 - sW, sH - y2, c);
		trst -> keepTriangle (x2 - sW, sH - y2, x1 - sW, sH - y2, x1 - sW, sH - y1, c);
	}
	else
	{
		trst -> keepLine (x1 - sW, sH - y1, x2 - sW, sH - y1, c);
		trst -> keepLine (x2 - sW, sH - y1, x2 - sW, sH - y2, c);
		trst -> keepLine (x2 - sW, sH - y2, x1 - sW, sH - y2, c);
		trst -> keepLine (x1 - sW, sH - y2, x1 - sW, sH - y1, c);
	}
}

// ---
void OGREScreen::drawPolygon (int* x, int* y, int np, const QGAMES::Color& c, bool f)
{
	assert (np > 0);

	// It is really a point...
	if (np == 1)
	{
		drawPoint (x [0], y [0], c);
		return;
	}

	// It is really a line (the thickness is always 1)...
	if (np == 2)
	{
		drawLine (x [0], y[0], x [1], y [1], c, 1); // Thickness only 1...
		return;
	}

	int sW = visualWidth () >> 1;
	int sH = visualHeight () >> 1;

	OGREScreen::OGRETransaction* trst = dynamic_cast <OGREScreen::OGRETransaction*> (currentTransaction ());
	assert (trst); // It shouldn't be null, but just in case...

	if (f)
	{
		// When np == 3 the functions works like drawing a traingle...
		for (int i = 0; i < (np - 2); i++)
			trst -> keepTriangle (x [0] - sW, sH - y [0],
								  x [i + 1] - sW, sH - y [i + 1],
								  x [i + 2] - sW, sH - y [i + 2], c);
	}
	else
	{
		for (int i = 0; i < (np - 1); i++) // Just one point less...
			trst -> keepLine (x [i] - sW, sH - y [i], x [i + 1] - sW, sH - y [i + 1], c);
		trst -> keepLine (x [np - 1] - sW, sH - y [np - 1], x [0] - sW, sH - y [0], c); // Close the line...
	}
}

// ---
void OGREScreen::drawTriangle (int x1, int y1, int x2, int y2, int x3, int y3, const QGAMES::Color& c, bool f)
{
	int sW = visualWidth () >> 1;
	int sH = visualHeight () >> 1;

	OGREScreen::OGRETransaction* trst = dynamic_cast <OGREScreen::OGRETransaction*> (currentTransaction ());
	assert (trst); // It shouldn't be null, but just in case...

	if (f) 
	{ // A fullfil rectangle is made up of two rectangles
		trst -> keepTriangle (x1 - sW, sH - y1, x2 - sW, sH - y2, x3 - sW, sH - y3, c);
	}
	else
	{
		trst -> keepLine (x1 - sW, sH - y1, x2 - sW, sH - y2, c);
		trst -> keepLine (x2 - sW, sH - y2, x3 - sW, sH - y3, c);
		trst -> keepLine (x3 - sW, sH - y3, x1 - sW, sH - y1, c);
	}
}

// ---
void OGREScreen::drawCircle (int x, int y, int a1, int a2, const QGAMES::Color& c, bool f)
{
	// A ellipse is made up of many lines
	// The number of them will depend on the size of the ellipse itself
	// The bigger the ellipse is the more number of lines to define it!
	int nP = (std::max (a1, a2)) / 2;
	float inc = (float) a1 / (float) nP;
	float a12 = (float) a1 * (float) a1;
	float xE = (float) (x - a1); float oxE = xE; // From left to right...
	float SRTY = sqrt (1 - (((xE - x) * (xE - x)) / a12));
	float yE1 = y + (a2 * SRTY); float oyE1 = yE1;
	float yE2 = y - (a2 * SRTY); float oyE2 = yE2;
	int x0 = (int) xE; int y10 = (int) yE1; int y20 = (int) yE2; // The initial position never changes...

	for (int i = 0; i < (2 * nP); i++)
	{
		xE += inc;
		SRTY = sqrt (1 - (((xE - x) * (xE - x)) / a12));
		yE1 = y + (a2 * SRTY);
		yE2 = y - (a2 * SRTY);

		if (f)
		{
			drawTriangle (x0, y10, (int) oxE, (int) oyE1, (int) xE, (int) yE1, c, f);
			drawTriangle (x0, y20, (int) oxE, (int) oyE2, (int) xE, (int) yE2, c, f);
		}
		else
		{
			drawLine ((int) oxE, (int) oyE1, (int) xE, (int) yE1, c, f);
			drawLine ((int) oxE, (int) oyE2, (int) xE, (int) yE2, c, f);
		}

		oxE = xE; oyE1 = yE1; oyE2 = yE2;
	}
}

// ---
void OGREScreen::drawOnYourOwn ()
{
	// Just to do it with one screen...let's say the main one...
	if (this != QGAMES::Game::game () -> mainScreen ()) return;
	if (!root () -> renderOneFrame ()) // If something is wrong the game exists!
		QGAMES::Game::game () -> exitGame (); 
}

// ---
Ogre::MaterialPtr OGREScreen::whiteMaterial ()
{
	if (_whiteMaterial)
		return (_whiteMaterial); // Just once per screen...

	_whiteMaterial = Ogre::MaterialManager::getSingletonPtr () -> getByName ("Commnets/CommtyWhite");
	assert (_whiteMaterial); // It has to exist...
	Ogre::Pass* pass = _whiteMaterial -> getTechnique (0) -> getPass (0);
	if (pass -> hasVertexProgram ())
		pass -> getVertexProgram () -> load ();
	if (pass -> hasFragmentProgram ())
		pass -> getFragmentProgram () -> load ();
	assert (!pass -> getVertexProgram () -> hasCompileError () &&
			!pass -> getFragmentProgram () -> hasCompileError ()); 
	// The vertex and fragment programs has to be well defined...

	// This is a very important method, otherwise the material will not be drawn in surfaces
	_whiteMaterial -> setCullingMode (Ogre::CULL_NONE);

	return (_whiteMaterial);
}

// ---
Ogre::MaterialPtr OGREScreen::formMaterial (OGRE2DForm* frm)
{
	assert (frm);

	std::map <int, Ogre::MaterialPtr>::const_iterator i = _formMaterials.find (frm -> id ());
	if (i != _formMaterials.end ())
		return ((*i).second); // It has already been created...

	std::string fIdS = std::to_string (frm -> id ());

	Ogre::MaterialPtr mat = 
		Ogre::MaterialManager::getSingletonPtr () -> getByName ("Commnets/FormsBasic") -> 
			clone (std::string ("MAT") + fIdS);
	assert (mat);
	mat -> setCullingMode (Ogre::CULL_NONE);

	Ogre::Pass* pass = mat -> getTechnique (0) -> getPass (0);
	if (pass -> hasVertexProgram ())
		pass -> getVertexProgram () -> load ();
	if (pass -> hasFragmentProgram ())
		pass -> getFragmentProgram () -> load ();
	assert (!pass -> getVertexProgram () -> hasCompileError () &&
			!pass -> getFragmentProgram () -> hasCompileError ()); 
	// The vertex and fragment programs has to be well defined...

	// Changes the texture of the basic image loaded...
	pass -> getTextureUnitStates ()[0] -> 
		setTexture (Ogre::TextureManager::getSingletonPtr () -> 
			loadImage (std::string ("RTXT") + fIdS, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
				(*frm -> image ())));

	_formMaterials [frm -> id ()] = mat;

	return (mat);
}
