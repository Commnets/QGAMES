/** \ingroup OGRE */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: ogrescreen.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 15/11/2015 \n
 *	Description: Defines ogre screen. \n
 *	Versions: 1.0 Initial
 */

#include <Common/screen.hpp>
#include <CommonOGRE/ogregame.hpp>
#include <CommonOGRE/ogreform.hpp>
#include <OgreHeaderPrefix.h>
#include <OgreRenderWindow.h>
#include <OgreCamera.h>
#include <OgreHeaderSuffix.h>
#include <SDL.h>

#ifndef __QGAMES_OGRESCREEN__
#define __QGAMES_OGRESCREEN__

/** This class rerpesents a window in OGRE System.
	Under that engine, there is no way to draw things by themselves.
	All of them have to be attached to a scene node. 
	So no methods to draw specific forms are needed at all. All of them are implemented as black.
	Visual transactions are also managed by the engine, so the method related
	with opening, closing or updating transactions are implemented as blanks also. 
	In OGRE scale is related with viewports and not with the screen. 
	Viewports is the way a content manager is seen through a camera.
	So all methods retaled with scale are also declared as standard. */
class OGREScreen : public QGAMES::Screen
{
	public:
	/** Buffer in oree makes no sense. It controls everything. */
	class OGREBuffer : public QGAMES::Screen::Buffer
	{
		public:
		OGREBuffer (QGAMES::Screen* s, int w = -1, int h = -1)
			: QGAMES::Screen::Buffer (s, w, h)
							{ }

		/** @see parent, but in OGRE those methods make no sense,
			because the engine manages everything. */
		virtual void open ()
							{ }
		virtual void close ()
							{ }
		virtual void moveToScreen (int x = 0, int y = 0)
							{ }
		virtual void moveToBuffer (Buffer* b,int x = 0, int y = 0)
							{ }
	};

	OGREScreen (Ogre::Root* r, 
		const std::string& t, const QGAMES::Position& p, int w, int h, 
		int wPX, int wPY, float sX = 1.0f, float sY = 1.0f);
	~OGREScreen ();

	/** @see parent. */
	virtual void setIcon (QGAMES::Form* f);
	virtual void windowAtPosition (int x, int y);
	virtual void windowAtCenter ();

	Ogre::Root* root ()
							{ return (_root); }
	const Ogre::Root* root () const
							{ return (_root); }
	Ogre::RenderWindow* window () 
							{ return (_window); }
	const Ogre::RenderWindow* window () const
							{ return (_window); }

	// The screeen in ogree system is open using SDL primitives...
	SDL_Window* SDLWindow () 
							{ return (_SDLWindow); }
	const SDL_Window* SDLWindow () const
							{ return (_SDLWindow); }
	SDL_Renderer* SDLRenderer ()
							{ return (_SDLRenderer); }
	const SDL_Renderer* SDLRenderer () const
							{ return (_SDLRenderer); }

	protected:
	/** The transactions in OGRE are quite important.
		They are needed to manage how to draw when the QGAMES code orders it.
		All drawings are drawn on top of the 3D draw. */
	class OGRETransaction : public QGAMES::Screen::Transaction, public Ogre::RenderQueueListener
	{
		public:
		OGRETransaction (QGAMES::Screen* s)
			: QGAMES::Screen::Transaction (s),
			  _sceneManagers (), 
			  _vertexList (), _elementsToDraw (),
			  _renderOps (),
			  _vertexData (NULL),
			  _vertexBuffer (), _pixelBuffer (),
			  _renderOp ()
							{ }

		/** To keep a point. */
		void keepPoint (int x, int y, const QGAMES::Color& c);
		/** To keep a line. */
		void keepLine (int x1, int y1, int x2, int y2, const QGAMES::Color& c);
		/** To keep a a triangle. 
			The last parameter notice whether the triangle is fullfilled or not. */
		void keepTriangle (int x1, int y1, int x2, int y2, int x3, int y3, 
			const QGAMES::Color& c);
		/** To keep an image. */
		void keepGraphic (OGRE2DForm* frm, int x, int y, int px, int py, int w, int h, int fW, int fH, int a);

		/** @see parent. 
			To pour the content over the screen graphics card. */
		virtual void renderQueueEnded (Ogre::uint8 queueGroupId, const Ogre::String& invocation, 
			bool& repeatThisInvocation);

		protected:
		virtual void openFirst ();
		virtual void closeLast ();
		virtual void openOne ()	  { } // Nothing special to do...
		virtual void closeOne ()  { }

		private:
		/** Used in the triangle and line drawing operations. */
		struct Vertex
		{
			Vertex ()
				: _position (Ogre::Vector3 ((Ogre::Real) 0.0f, (Ogre::Real) 0.0f, (Ogre::Real) 0.0f)),
				  _color (Ogre::ColourValue::White),
				  _vectorTexture (Ogre::Vector2 ((Ogre::Real) 0, (Ogre::Real) 0))
							{ }
			Vertex (const Ogre::Vector3& p, const Ogre::ColourValue& c, 
					const Ogre::Vector2& vt = Ogre::Vector2 ((Ogre::Real) 0, (Ogre::Real) 0))
				: _position (p), 
				  _color (c),
				  _vectorTexture (vt)
							{ }
			Vertex (QGAMES::bdata x, QGAMES::bdata y, QGAMES::bdata z, const QGAMES::Color& c,
					const Ogre::Vector2& vt = Ogre::Vector2 ((Ogre::Real) 0, (Ogre::Real) 0))
				: _position ((Ogre::Real) x, (Ogre::Real) y, (Ogre::Real) z),
				  _color ((Ogre::Real) (__BD c.red () / __BD 255), (Ogre::Real) (__BD c.green () / __BD 255), 
						  (Ogre::Real) (__BD c.blue () / __BD 255), (Ogre::Real) (__BD c.alpha () / __BD 255)),
				  _vectorTexture (vt)
							{ }

			Ogre::Vector3 _position; // The position of the vertex...
			Ogre::ColourValue _color; // What color this has...
			Ogre::Vector2 _vectorTexture; // Where the initial point of the texture to draw starts...
		};

		typedef std::vector <Vertex> VertexList;

		struct ElementToDraw
		{
			typedef enum { _POINT, _LINE, _TRIANGLE, _SPRITE } Type; // Triangle means filled...

			ElementToDraw () // Don't use it, it is dangerous...
				: _type (_POINT), 
				  _initialVertex (0), _numberOfVextex (1), 
				  _material (), 
				  _x1 (0), _y1 (0), _x2 (0), _y2 (0), _fW (1), _fH (1) // When it is a sprite...
							{ }

			ElementToDraw (Type t, int iV, int nV, 
					const Ogre::MaterialPtr& m = Ogre::MaterialPtr (), 
					int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0, int fW = 1, int fH = 1)
				: _type (t), 
				  _initialVertex (iV), _numberOfVextex (nV),
				  _material (m), 
				  _x1 (x1), _y1 (y1), _x2 (x2), _y2 (y2), _fW (fW), _fH (fH) // When it is a sprite...
							{ assert (_fW > 0 && fH > 0); }

			Type _type;
			int _initialVertex;
			int _numberOfVextex;
			Ogre::MaterialPtr _material; // Just in case it is a sprite...
			int _x1, _y1, _x2, _y2; // The limits of the sprite to draw...
			int _fW, _fH; // The size of the form containing the sprite to draw whe it is...
		};

		typedef std::vector <ElementToDraw> ElementToDrawList;

		/* To know what render operation is applicable. */
		struct RenderOp
		{
			RenderOp () // Take care using it. it is dangerous...
				: _initialElement (0), _numberElements (0),
				  _initialVertex (0), _numberVertexs (0), 
				  _type (Ogre::RenderOperation::OT_POINT_LIST)
							{ }
			RenderOp (int iE, int nE, int iV, int nV, Ogre::RenderOperation::OperationType t)
				: _initialElement (iE), _numberElements (nE),
				  _initialVertex (iV), _numberVertexs (nV), 
				  _type (t)
							{ }

			int _initialElement;
			int _numberElements;
			int _initialVertex; 
			int _numberVertexs; 
			Ogre::RenderOperation::OperationType _type; 
			// many...but the important ones are: point, line and triangle...
			// If the triangle were filled up by an image, the details are contained in the operation.
		};

		typedef std::vector <RenderOp> RenderOpList;

		int numberVertexs () const
							{ return ((int) _vertexList.size ()); }
		void keepVertex (const Vertex& p)
							{ _vertexList.push_back (p); }
		int numberElementsToDraw () const
							{ return ((int) _elementsToDraw.size ()); }
		void keepElementToDraw (const ElementToDraw& eD)
							{ _elementsToDraw.push_back (eD); }
		void addRenderPointOperation ()
							{ addRenderOperation (1 /** Element */, 1 /** Vertexs */, 
								Ogre::RenderOperation::OT_POINT_LIST); }
		void addRenderLineOperation ()
							{ addRenderOperation (1, 2, Ogre::RenderOperation::OT_LINE_LIST); }
		void addRenderTriangleOperation ()
							{ addRenderOperation (1, 3, Ogre::RenderOperation::OT_TRIANGLE_LIST); }
		void addRenderGraphicOperation (const Ogre::MaterialPtr& m)
							{ addRenderOperation (1, 6, Ogre::RenderOperation::OT_TRIANGLE_LIST, m); }
		void addRenderOperation (int nE, int nV, 
			Ogre::RenderOperation::OperationType t, const Ogre::MaterialPtr& m = Ogre::MaterialPtr ());

		private:
		// Implementation
		/** The list of the scene managers this transaction work for. */
		std::vector <Ogre::SceneManager*> _sceneManagers;

		/** Where the vertex to draw lines are kept */
		VertexList _vertexList; 
		/** The list of the elements to draw. 
			They refer to the list of vertex always, but they keep more info. */
		ElementToDrawList _elementsToDraw;
		/** The list of render operations. */
		RenderOpList _renderOps;
		/** Used as an index of the data to render things */
		Ogre::VertexData* _vertexData;
		/** The buffer for vertex */
		Ogre::HardwareVertexBufferSharedPtr _vertexBuffer;
		/** The buffer for images. */
		Ogre::HardwarePixelBufferSharedPtr _pixelBuffer;
		/** The render operation */
		Ogre::RenderOperation _renderOp; 
	};

	friend OGRETransaction;

	protected:
	virtual void drawForm (int x, int y, QGAMES::Form* f, int px, int py, int w, int h, int a);
	virtual void drawPoint (int x, int y, const QGAMES::Color& c);
	virtual void drawLine (int x1, int y1, int x2, int y2, const QGAMES::Color& c, int t);
	virtual void drawRectangle (int x1, int y1, int x2, int y2, const QGAMES::Color& c, bool f);
	virtual void drawPolygon (int* x, int* y, int np, const QGAMES::Color& c, bool f);
	virtual void drawTriangle (int x1, int y1, int x2, int y2, int x3, int y3, const QGAMES::Color& c, bool f);
	virtual void drawImage (int x, int y, const std::string& i, int a) { }
	virtual void drawCircle (int x, int y, int a1, int a2, const QGAMES::Color& c, bool f);
	virtual void drawText (int x, int y, const std::string& s,
		const std::string& f, int w, int h, const QGAMES::Color& c, int, int ot) { }
	virtual void activeScale () { } // The scale is something that depends on the viewport!
	virtual void desactiveScale () { }
	virtual void drawOnYourOwn (); // This method in this case just call OGRE to render a frame!
	virtual Transaction* createTransaction () 
							{ return (new OGRETransaction (this)); }
	virtual Buffer* createBuffer (int tp, int w, int h)
							{ return (new OGREBuffer (this, w, h)); }

	/** When transaction is createad this method is also invoked. \n
		Doing so a very simple material is created. This material is made up of only one white pixel
		This material is later used to draw (@see Commy2D.hlsl as an example with more detail) 
		if no other has been defined. 
		The material is passed through the GPU programs to change the color. */
	Ogre::MaterialPtr whiteMaterial ();
	/** And a similar method to load the texture for a Image. */
	Ogre::MaterialPtr formMaterial (OGRE2DForm* frm);

	private:
	/** A reference to the root basic element of OGRE system.
		This reference is to owned by this class. */
	Ogre::Root* _root;
	/** Every scren is the reflects of a window. */
	Ogre::RenderWindow* _window;
	SDL_Window* _SDLWindow;
	SDL_Renderer* _SDLRenderer;
	/** Which were the last positions of the window. */
	int _windowPositionX, _windowPositionY;
	/** The sclate of the window. */
	float _scaleX, _scaleY;
	/** The texture used to paint 2D. */
	Ogre::MaterialPtr _whiteMaterial;
	/** A set with the textures used to paint */
	std::map <int, Ogre::MaterialPtr> _formMaterials; // The id of the form

	/* Has a window ever been created? How many? */
	static int _ONEWINDOWCREATED;
};

#endif
  
// End of the file
/*@}*/