/** \ingroup ArcadeGame */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: layer.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class representing a layer. \n
 *				 Every map can have several layers. \n
 *				 There are several kind of layers, depending on the content the have. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_LAYER__
#define __QGAMES_LAYER__

#include <Common/rectangle.hpp>
#include <Common/element.hpp>
#include <Common/object.hpp>
#include <Arcade/tile.hpp>
#include <Arcade/definitions.hpp>
#include <string>
#include <map>
#include <list>

namespace QGAMES
{
	class Map;
	class Layer;
	typedef std::list <Layer*> Layers;
	class TileLayer;
	typedef std::map <int, TileLayer*> TileLayers;
	class ObjectLayer;
	typedef std::map <int, ObjectLayer*> ObjectLayers;
	class ImageLayer;
	typedef std::map <int, ImageLayer*> ImageLayers;
	typedef std::map <std::string, std::string> LayerProperties;
	class Entity;

	/** There can be many different types of layers.
		The most important layer is the TileLayer which is made of tiles. 
		The layer can moves using a behaviour movement. */
	class Layer : public Element
	{
		public:
		/** 
		  *	To define an effect over the layer.
		  *	Imagine e.g. the layer is something representing a base where the main character is standing.
		  *	An effect could be little bubbles comming from the base.
		  *	This kind of effects could be done using this class. 
		  *	The effect can be built using the properties of the layer. So it is possible
		  *	to define the effet in the external file defining a layer too. 
		  */
		class VisualEffect
		{
			public:
			VisualEffect (Layer* ly, const QGAMES::LayerProperties& pts)
				: _layer (ly),
				  _layerProperties (pts)
							{ assert (ly); }
			virtual ~VisualEffect () 
							{ }

			/** The usual funtions used in almost every element in the game. */
			virtual void initialize () = 0; // To initiale whatever is needed to initialize... 
			virtual void updatePositions () = 0; // Same for updating the positions...
			virtual void drawOn (Screen* s, const Position& pos = Position::_cero) = 0; // To draw the visual effect...
			virtual void finalize () // To finalize the effect, any time it is changed. By default it does nothing...
							{ } 

			protected:
			Layer* _layer;
			QGAMES::LayerProperties _layerProperties;
		};

		typedef std::vector <VisualEffect*> VisualEffects;

		/** A class effect made up of others. */
		class ComplexVisualEffect : public VisualEffect
		{
			public:
			ComplexVisualEffect (Layer* ly, 
				const VisualEffects& effs, const LayerProperties& prps)
				: VisualEffect (ly, prps),
				  _effects (effs)
							{ }
			~ComplexVisualEffect ();

			/** @see parent. 
				Execute the instructions for every visual effect inside. */
			virtual void initialize (); 
			virtual void updatePositions ();
			virtual void drawOn (Screen* s, const Position& pos = Position::_cero);
			virtual void finalize ();

			protected:
			VisualEffects _effects;
		};

		/** To define a potential movement. */
		class MovementBehaviour
		{
			public:
			MovementBehaviour (unsigned int s)
				: _speed (s),
				  _originalPosition (Position::_noPoint),
				  _counterSpeed (0),
				  _vectorMoved (Position::_cero),
				  _averageVectorMoved (Position::_cero),
				  _counterSteps (0)
							{ }
			virtual ~MovementBehaviour ()
							{ }

			/** To get a clone. \n
				A new object is created. Who creates it must assume to destroy it later. \n 
				The method has to be defined by any class inheriting this one. */
			virtual MovementBehaviour* clone () const = 0;
			/** To get the opposite movement. \n
				A new object is created. Who creates it must assume to destroy it later. \n 
				The method has to be defined by any class inheriting this one. */
			virtual MovementBehaviour* opposite () = 0;

			/** To get a list of open values describing the movement.
				By default, an empty list is returned. */
			virtual SetOfOpenValues runtimeValues () const
							{ return (SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESMOVTYPE__))); }
			/** To initialize the movement from a set of configuration values. 
				By default, nothing is done. */
			virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg)
							{ assert (cfg.name () == std::string (__QGAMES_RUNTIMEVALUESMOVTYPE__)); }

			// Managing the speed
			/** Request the current speed. */
			unsigned int speed () const
							{ return (_speed); }
			/** Sets the speed. */
			void setSpeed (unsigned int s)
							{ _speed = s; _counterSpeed = 0; }

			/** Knowing the last vector moved. */
			const Vector& vectorMoved () const
							{ return (_vectorMoved); }
			/** Same but in average. */
			const Vector& averageVectorMoved () const
							{ return (_averageVectorMoved); }
			/** To know wehether the movement has finished. */
			virtual bool hasFinished () const = 0;

			virtual void initialize ();
			virtual void finalize () // By default it does nothing...
							{ }

			/** To calculate what is the next position. \n
				Taking into account the speed. */
			virtual Position newPosition (const Position& p);

			protected:
			virtual Position calculateNewPosition (const Position& p) = 0;

			protected:
			unsigned int _speed;

			protected:
			// Implementation
			Position _originalPosition;
			unsigned int _counterSpeed;
			Vector _vectorMoved;
			Vector _averageVectorMoved;
			int _counterSteps;
		};

		/** No movement is a type of movement too. */
		class NoMovementBehaviour : public MovementBehaviour
		{
			public:
			NoMovementBehaviour ()
				: MovementBehaviour (0)
							{ }

			/** @see parent. */
			virtual MovementBehaviour* clone () const 
							{ return (new NoMovementBehaviour ()); }
			virtual MovementBehaviour* opposite ()
							{ return (new NoMovementBehaviour ()); }

			/** @see parent. */
			virtual bool hasFinished () const
							{ return (true); }
			virtual void initialize ()
							{ }

			protected:
			/** @see parent. */
			virtual Position calculateNewPosition (const Position& p)
							{ return (p); }
		};

		typedef std::vector <MovementBehaviour*> MovementBehaviours;

		/** To define a set of movements to execute them one after other. */
		class SetOfMovementsBehaviour : public MovementBehaviour
		{
			public:
			SetOfMovementsBehaviour (MovementBehaviours movs, bool c = true)
				: MovementBehaviour (0), 
				  // This parameter makes no sense in this class as it is never used!
				  _ciclic (c),
				  _movements (movs), 
				  _currentMovement (0)
							{ }
			virtual ~SetOfMovementsBehaviour ();

			int numberMovements () const
							{ return ((int) _movements.size ()); }

			/** @see parent. */
			virtual MovementBehaviour* clone () const;
			virtual MovementBehaviour* opposite ();

			/** @see parent. \n
				The information added is: \n
				The number of the current movement, (openValue)
				and the runtimeValues per each movement managed by this one (setOfOpenValue). */
			virtual SetOfOpenValues runtimeValues () const;
			virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

			bool hasCurrentMovementFinished () const;

			/** @see parent. */
			virtual bool hasFinished () const;

			/** @see parent. */
			virtual void initialize ();
			virtual void finalize ();

			/** @see parent. */
			virtual Position newPosition (const Position& p);

			protected:
			/** @see parent. \n
				This method is never used, as the method always invoked belongs
				to the active movement behaviour in the set of those. */
			virtual Position calculateNewPosition (const Position& p)
							{ return (Position::_cero); }

			protected:
			/** The list of trhe movements to manage. */
			MovementBehaviours _movements;
			/** Is the set ciclic?. */
			bool _ciclic;
			
			// Implementation
			/** Which is the current movement. */
			int _currentMovement;
		};

		/** To define a linear movement. */
		class LinearMovementBehaviour: public MovementBehaviour
		{
			public:
			/** The second parameter is the number of steps to advance,
				whilst the third one is the direction of the movement. */
			LinearMovementBehaviour (unsigned int s, unsigned int st, const Vector& d)
				: MovementBehaviour (s),
				  _steps (st),
				  _direction (d),
				  _currentStep (0)
							{ }

			/** @see parent. */
			virtual MovementBehaviour* clone () const;
			virtual MovementBehaviour* opposite ();

			/** @see parent. \n
				The information added is the current step (openValue). */
			virtual SetOfOpenValues runtimeValues () const;
			virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

			// Managing the parameters
			// Take care changing these attributes, specially when the movement already started.
			/** Ask for the number of steps to advance. */
			unsigned int steps () const
							{ return (_steps); }
			void setSteps (unsigned int st)
							{ _steps = st; }
			/** To know the direction of the movement. */
			const Vector& direction () const
							{ return (_direction); }
			/** To set a new direction. 
				Then evrything is restarted. */
			void setDirection (const Vector& d);

			/** @see parent. */
			virtual bool hasFinished () const;

			/** @see parent. */
			virtual void initialize ();

			protected:
			/** @see parent. */
			virtual Position calculateNewPosition (const Position& p);

			private:
			/** The number of steps to advance. */
			unsigned int _steps;
			/** The direction of the movement. */
			Vector _direction;

			// Implementation
			unsigned int _currentStep;
		};

		/** To define a circular movement. */
		class CircularMovementBehaviour : public MovementBehaviour
		{
			public:
			/** 
			  * The second parameter in the initial angle to start to rotate,
			  *	The third parameter is the radix of the cinrcunference. \n
			  *	The fourth parameter is the max number of radiand to rotate. 
			  *	and the fifth the angle rotated in any iteration. \n
			  *	The last parameter indicates the rotation direction, Clockwise or not? 
			  */
			CircularMovementBehaviour (unsigned int s, unsigned int r, bdata iA = __BD 0,
					bdata mA = __BD (2 * __PI), bdata a = __BD (__PI / 18), bool cW = false)
				: MovementBehaviour (s),
				  _radix (r), _initialAngle (iA), _maxAngle (mA), _angle (a),
				  _clockwise (cW),
				  _rotationCenter (Position (__BD 0, __BD r, __BD 0)), 
				  _currentAngle (0)
							{ } 

			/** @see parent. */
			virtual MovementBehaviour* clone () const;
			virtual MovementBehaviour* opposite ();

			/** @see parent. \n
				The information added is the current angle (openValue). */
			virtual SetOfOpenValues runtimeValues () const;
			virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

			// Managing the parameters
			// They can't be changed once intialized
			/** Ask fot the radix applied. */
			unsigned int radix () const
							{ return (_radix); }
			void setRadix (unsigned int r); 
			// The rotation center has to be rebuilt
			// ..take care because after that the consequences can be uncontrolled
			// if the platform was already moving!
			/** Ask for the initial angle to rotate. */
			bdata initialAngle () const
							{ return (_initialAngle); }
			void setInitialAngle (bdata iA)
							{ _initialAngle = iA; } // Makes no sense after starting to move!
			/** Ask for the max angle to rotate. */
			bdata maxAngle () const
							{ return (_maxAngle); }
			void setMaxAngle (bdata mA)
							{ _maxAngle = mA; }
			/** Ask for the angle applied. */
			bdata angleStep () const
							{ return (_angle); }
			void setAngleStep (bdata a)
							{ _angle = a; }
			/** Ask for the direction of the rotation. */
			bool clockwise () const
							{ return (_clockwise); }
			void setClockwise (bool cW)
							{ _clockwise = cW; }

			/** @see parent. */
			virtual bool hasFinished () const;

			/** @see parent. */
			virtual void initialize ();

			protected:
			/** @see parent. */
			virtual Position calculateNewPosition (const Position& p);

			protected:
			unsigned int _radix;
			bdata _initialAngle; // In radians.
			bdata _maxAngle; // In radians.
			bdata _angle; // In radians.
			bool _clockwise; // True if clockwise, false opposite.

			// Implementation
			Position _rotationCenter;
			bdata _currentAngle;
		};

		/** Define a movement that when reaches the limit goes to the opposite in a ciclic way. */
		class BounceMovementBehaviour : public SetOfMovementsBehaviour
		{
			public:
			BounceMovementBehaviour (MovementBehaviour* mov);
		};

		/** Defines a movement that when reaches the limit goes to the opposite n times. */
		class BounceNTimesMovementBehaviour : public SetOfMovementsBehaviour
		{
			public:
			BounceNTimesMovementBehaviour (int n, MovementBehaviour* mov);
		};

		// Constructors & Destructors
		Layer (int c, const std::string& n, Map* m = NULL, 
				const LayerProperties& p = LayerProperties ());
		~Layer ();

		/** @see parent. \n
			The information added is the current position of the layer (openValue),
			and the one comming from the current movement (setOfOpenValues. Empty if there is no movement). */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		const std::string& name () const
							{ return (_name); }
		Map* map ()
							{ return (_map); }
		virtual void setMap (Map* m) 
							{ _map = m; }
		virtual int type () const = 0;

		// To manage the porperties
		const LayerProperties& properties () const
							{ return (_properties); }
		/** To know whether the property exists or not. */
		bool existsProperty (const std::string& p) const;
		/** NULL if the property doesn't exist! */
		std::string layerProperty (const std::string& p) const;

		/** The position of an specific layer of the map can be changed.
			This feature can be used i.e to move objects in the screen. */
		const Position& position () const
							{ return (_position); }
		void setPosition (const Position& p)
							{ _position = p; }
		/** The layer has also orientation.
			The orientation of the layer can also be changed. */
		const Vector& orientation () const
							{ return (_orientation); }
		void setOrientation (const Vector& o)
							{ _orientation = o; }

		// To control the movement of a layer, if any...
		/** To know whether the layer is or not moving now. */
		void setMove (bool m)
							{ _moves = m; }
		bool moves () const
							{ return (_movementBehaviour != NULL && _moves); }
		/** Setting a NULL movement the layer stops. */
		void setMovement (MovementBehaviour* mov);
		MovementBehaviour* movement ()
							{ return (_movementBehaviour); }
		const MovementBehaviour* movement () const
							{ return (_movementBehaviour); }

		// To control the visual effects...
		bool hasVisualEffect () const
							{ return (_visualEffect != NULL); }
		void setVisualEffect (VisualEffect* eff);
		VisualEffect* visualEffect ()
							{ return (_visualEffect); }
		const VisualEffect* visualEffect () const
							{ return (_visualEffect); }
		virtual void setFade (int fd)  // Changes the fade...The way it is implemented depends on the type of layer...
							{ }		   

		/** To know whether a position or a box within the layer is or not free. \n
			By default, both members return true.
			It is not the case in tiled maps usually */
		virtual bool emptyPositionAt (const Position& p)
							{ return (true); }
		virtual bool emptyBoxAt (const Position& p, const Position& s)
							{ return (true); }

		/** To initialize the layer, basically the internal movement if any. */
		virtual void initialize ();
		/** The can be composed by many different other elements. \n
			In every loop actualization, their positions or appearance might actualized. \n
			This is the right method to do so. By default, no actualization is needed. */
		virtual void updatePositions ();
		/** To draw when there is an effect in place. */
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		/** A way to draw but taking into consideration a list of elements. 
			As the elements are taken into consideration, they must be deleted from the parameter list. 
			By default all of them asre taken into consideration. */
		virtual void drawOnWithElements (Screen* s, const Position& p, Elements& elmts);
		
		protected:
		std::string _name;
		Map* _map;
		/** The layer owns the movement behaviour. */
		MovementBehaviour* _movementBehaviour;
		/** To indicate whether the layer moves or not. */
		bool _moves;
		/** The layer own the visual effect. */
		VisualEffect* _visualEffect;
		LayerProperties _properties;
		Position _position;
		Vector _orientation;
	};

	/**	
	  * A tile layer is made up of tiles. \n 
	  *	In a tile layer it is possible to test which tiles are around 
	  *	an specific entity. \n
	  *	The Tile layer can be drawn isometric, orthogonal, conic or cavalier.
	  * Remeber without changing neccessary the camera!.
	  *	The orientation has to be passed as parameter at construction time. 
	  *	Tile Layer has not the position into account. 
	  */ 
	class TileLayer : public Layer
	{
		public:
		enum VisualOrientation 
		{
			_ISOMETRIC = 0,
			_ORTHOGONAL = 1,
			_CAVALIER = 2,
			_CONIC = 3,
			_EXTERANLLYDEFINED = 4
		};

		/** The aspect of the tiles can be updated if the right control has been defined. 
			The tile updater is own by the layer. */
		class TileUpdater
		{
			public:
			TileUpdater ()
							{ }
			virtual ~TileUpdater ()
							{ }

			/** To define the direction of the update, if it makes sense. 
				The direction can be diferent attending to a certain tile. */
			virtual Vector updateDirection (const Tile& tl) const
							{ return (Vector::_noPoint); }
			/** To define how many pixels were updated from the last time. 
				Same. It can be different attending to a certain position. */
			virtual bdata pixelsUpdated (const Tile& tl) const
							{ return (0); }

			/** To init the tile updater. 
				By default they don't do anything. */
			virtual void initialize () 
							{ }

			/** What to do has to be defined here.
				No default behaviour has been defined. */
			virtual void updateTiles (const Tiles& t) = 0;

			/** What to do (if any) whe the updater is not longer used.
				e.g. It could be to set the previous situation back. 
				By default nothing is done. */
			virtual void finalize ()
							{ }
		};

		/** Defines a tile updater that generates some kind of wave using a set of tiles. */
		class WaveTileUpdater : public TileLayer::TileUpdater
		{
			public:
			struct Properties
			{
				Properties ()
					: _loopsToUpdate (60),
						_tilesToWave (),
						_acceleration ((bdata) 0),
						_maximumSpeed ((bdata) 0)
								{ }

				/** 
				  *	The properties are: \n
				  *	@param lU	The number of loops before updating the wave. \n
				  *				It could be similar to the speed of the movement.
				  *	@param tW	The tiles used to generate the wave. \n
				  *				The updater takes from the very first of the list to the last but all together.
				  *	@param a	The acceleration of the movement. The bigger it is, the faster the movement is.
				  *	@param mS	The maximum speed of the wave. The bigger it is, the wider the wave is.
				  */
				Properties (int lU, const std::vector <int>& tW, bdata a, bdata mS)
					: _loopsToUpdate (lU),
					  _tilesToWave (tW),
					  _acceleration (a),
					  _maximumSpeed (mS)
								{ assert (lU > 0 && !tW.empty () && a > __BD 0); }

				int _loopsToUpdate;
				std::vector <int> _tilesToWave;
				bdata _acceleration;
				bdata _maximumSpeed;
			};

			WaveTileUpdater (const Properties& prps = Properties ())
				: TileLayer::TileUpdater (),
				  _properties (prps),
				  _tileState (),
				  _counter (0),
				  _cycles (0)
							{ }

			/** To know the number of cycles moved from last. */
			int cycles () const
							{ int result = _cycles; _cycles = 0; return (result); }

			/** @see parent. */
			virtual Vector updateDirection (const Tile& tl) const;
			virtual bdata pixelsUpdated (const Tile& tl) const;

			/** @see parent. */
			virtual void initialize ()
							{ _counter = 0; _cycles = 0; _tileState = TileStateMap (); }
			virtual void updateTiles (const Tiles& tiles);
			virtual void finalize ();

			protected:
			Properties _properties;

			private:
			// Implementation
			struct TileState 
			{
				TileState ()
					: _tileRef (NULL), _originalFrame (0), _down (true), _speed (0.0), _lastSpeed (0.0)
							{ }

				TileState (Tile* tRef, int oF, bool u, double s)
					: _tileRef (tRef), _originalFrame (oF), _down (u), _speed (s), _lastSpeed (s)
							{ }

				Tile* _tileRef;
				int _originalFrame; // The initial frame to rebuilt it back...
				bool _down; 
				double _speed;
				double _lastSpeed;
			};

			typedef std::map <int, TileState> TileStateMap;

			TileStateMap _tileState;
			int _counter;
			mutable int _cycles;
		};

		/** Defines an updater that generates a special flow using a set of tiles. */
		class FlowTileUpdater : public TileLayer::TileUpdater
		{
			public:
			struct Properties
			{
				public:
				Properties ()
					: _loopsToUpdate (60),
					  _tilesToFlow ()
								{ }
			
				/**
				  *	The parameters are: \n
				  *	@param lU	The number of loops to update the content.
				  *	@param tS	The list of tiles (frames) used to generate the flow effect.
				  */
				Properties (int lU, const std::vector <int> tS)
					: _loopsToUpdate (lU),
					  _tilesToFlow (tS)
								{ assert (lU > 0 && !tS.empty ()); }

				int _loopsToUpdate;
				std::vector <int> _tilesToFlow;
			};

			FlowTileUpdater (const Properties& prps = Properties ())
				: TileLayer::TileUpdater (),
				  _properties (prps),
				  _tileState (),
				  _firstUpdate (true),
				  _counter (0),
				  _cycles (0)
							{ }

			/** To know the number of cycles moved from last. */
			int cycles () const
							{ int result = _cycles; _cycles = 0; return (result); }

			/** @see parent. */
			virtual void initialize ()
							{ _firstUpdate = true; _counter = 0; _cycles = 0; _tileState = TileStateMap (); }
			virtual void updateTiles (const Tiles& tiles);
			virtual void finalize ();

			protected:
			Properties _properties;

			private:
			struct TileState
			{
				TileState ()
					: _tileRef (NULL), _originalFrame (0)
							{ }

				TileState (Tile* tRef, int oF)
					: _tileRef (tRef), _originalFrame (oF)
							{ }

				Tile* _tileRef;
				int _originalFrame; // The initial frame to rebuilt it back...
			};

			typedef std::map <int, TileState> TileStateMap;

			// Implementation
			TileStateMap _tileState;
			bool _firstUpdate;
			int _counter;
			mutable int _cycles;
		};

		/** 
		  *	The TileLayer is made of tiles. \n
		  *	There is a parameter at construction time to indicate whether
		  *	the tiles have or not to be observed. \n
		  *	To observe all tiles could be too much time consuming 
		  *	and can delay the game too much as a consequence.
		  *	By default, thet are not. 
		  */
		TileLayer (int c, const std::string& n, const Tiles& t, Map* m = NULL, 
				VisualOrientation o = _ISOMETRIC,
				const LayerProperties& p = LayerProperties (),
				bool oT = false);
		/** A tile layer owns its tiles. \n
			When the tile layer is destroyed the tiles are also destroyed. */
		~TileLayer ();

		/** To get the visual orientation of the tile layer. */
		VisualOrientation visualOrientation () const
							{ return (_visualOrientation); }

		virtual int type () const
							{ return (__QGAMES_TILELAYER__); }

		/** To get and set the map associated to the layer.
			It can be received at construction time, but there is also a specific method to change it. */
		Map* map () 
							{ return (_map); }
		const Map* map () const
							{ return (_map); }
		virtual void setMap (Map* m);

		/** @see parent. */
		virtual void setFade (int fd);  

		// Managing tiles...
		const Tiles& tiles () const
							{ return (_tiles); }
		virtual Tile* tileAt (const Position& p);
		virtual Tiles tilesAt (const Position& p, int n);
		virtual Tiles tilesAt (const std::vector <Position>& p);
		/** To get the first position a tile is.	
			If the tile doesn't exist in the layer a no position is returned. */
		virtual Position tilePosition (const Tile* t) const;
		/** 
		  *	@see artist definition. \n
		  *	To know the tiles rounding an entity. \n 
		  *	This a very time consuming method. So, any time it is invoked
		  *	the result is store in an internal variable. If the method is invoked again later
		  *	and the entity hasn't move its position, it is supossed the tile will be the same
		  *	so the internal variable value will be returned instead of calculating everything again. 
		  */
		virtual Tiles tilesAround (Entity* e, bdata m = (bdata) 0.5, 
			const Vector& pv = Vector::_cero);
		/** To know the list of tile contained in a 3D box. */
		virtual Tiles tilesIn (int pX, int pY, int pZ, int w, int h, int d);
		/** To check whether a position has o no tile. */
		virtual bool emptyPositionAt (const Position& p);
		/** Same than previous but within a box. */
		virtual bool emptyBoxAt (const Position& p, const Position& s);
		/** Look for a tile of an specific type...
			It is possible to specify the number of frame of the tile.
			It is optional. -1 means any. 
			If none is found _noPoint is returned. */
		virtual Position positionFirstTile (int t, int f = -1);

		// To manage the tile updater
		bool setUpdateTiles (bool u)
							{ _updateTiles = u; }
		bool updateTiles () const
							{ return (_tilesUpdater != NULL && _updateTiles); }
		/** It can be null, what means no update will be done. */
		void setTileUpdater (TileUpdater* tU);
		TileUpdater* tileUpdater ()
							{ return (_tilesUpdater); }
		const TileUpdater* tileUpdater () const
							{ return (_tilesUpdater); }

		/** @see parent. */
		virtual void updatePositions ();

		/** @see parent. */
		// Only when drawing the orientation is used...
		virtual void drawOnWithElements (Screen* s, const Position& p, Elements& elmts);

		protected:
		/** To calculate what are the tile positions ocuppied by an entity.
			If the entity is an artist in a tiled map, the information could be reused.
			Otherwise it couldn't. */
		virtual void calculateTilesPositionesFor (Entity* e,
			int& pX, int& pY, int& pZ, int& w, int& h, int& d,
			bdata m, const Vector& pV);

		// Implementation
		// Very internal method
		/** Related with the variable _tilesToDraw. */
		void calculateTilesToDraw ();

		// To define how to draw on an specific projection...
		void drawOnOrthogonal (Screen* s, const Position& p, Elements& elmts);
		void drawOnIsometric (Screen* s, const Position& p, Elements& elmts);
		void drawOnCavalier (Screen* s, const Position& p, Elements& elmts);
		void drawOnConic (Screen* s, const Position& p, Elements& elmts);

		protected:
		Tiles _tiles;
		VisualOrientation _visualOrientation;
		TileUpdater* _tilesUpdater;
		bool _updateTiles;

		// Implementation
		/** Very internal structure to avoid to reapeat floating calculus
			when the tiles around an sprite in a layer have to be determinated. */
		struct TilesAroundCalculus
		{
			TilesAroundCalculus ()
				: _position (Position::_noPoint),
				  _id (-1),
				  _pX (0), _pY (0), _pZ (0), _w (0), _h (0), _d (0),
				  _m ((bdata) 0), _pV (Vector::_noPoint),
				  _tiles ()
							{ }
			TilesAroundCalculus (const Position& p, 
				int id, int pX, int pY, int pZ, int w, int h, int d,
				bdata m, const Vector& pv)
				: _position (p), 
				  _id (id),
				  _pX (pX), _pY (pY), _pZ (pZ), _w (w), _h (h), _d (d),
				  _m (m), _pV (pv),
				  _tiles ()
							{ }
			TilesAroundCalculus (TileLayer* l, Entity* e, bdata m, const Vector& pv);

			bool operator == (const TilesAroundCalculus& t) const
							{ return (_position == t._position && 
									  _id == t._id &&
									  _pX == t._pX && _pY == t._pY && _pZ == t._pZ &&
									  _w == t._w && _h == t._h && _d == t._d &&
									  _m == t._m && _pV == t._pV); }
			bool operator != (const TilesAroundCalculus& t) const
							{ return (_position != t._position ||
									  _id != t._id ||
									  _pX != t._pX || _pY != t._pY || _pZ != t._pZ ||
									  _w != t._w || _h != t._h || _d != t._d ||
									  _m != t._m || _pV != t._pV); }

			const Tiles& result () const
							{ return (_tiles); }
			void actualize (TileLayer* l,
				int id, 
				int pX, int pY, int pZ, int w, int h, int d,
				bdata m, const Vector& pv,
				const Tiles& t);

			/** From the layer */
			Position _position;
			/** From the entity */
			int _id;
			int _pX, _pY, _pZ;
			int _w, _h, _d;
			/** From the calculus */
			bdata _m;
			Vector _pV;

			// The very result of the calculus,
			// If none of the other three group of values changed!
			Tiles _tiles;
		};

		typedef std::map <int, TilesAroundCalculus> TilesAroundCalculusList;
		TilesAroundCalculusList _calculus;

		// Implementation
		// Very internal variable
		/** Just used in drawing method. \n
			A tiled layer could have just a couple of tiles. \n
			It is not usefull to walk throught all tiles checking whether
			each has to be drawn or not. This structure (created first time draw method is invoke)
			keeps just only those ones not null. */
		struct TileToDraw
		{
			TileToDraw (Tile* t, int x, int y, int z)
				: _tile (t), _x (x), _y (y), _z (z)
							{ }

			Tile* _tile;
			int _x, _y, _z;
		};

		// The typedef to simplify the definition of the tiles to draw...
		typedef std::vector <TileToDraw> TilesToDraw;
		TilesToDraw _tilesToDraw;
	};

	/** An object layer is made up of objects. \n
		Remember an object is an element to be drawn, but it is not 
		possible to interate with. */
	class ObjectLayer : public Layer
	{
		public:
		ObjectLayer (int c, const std::string& n, const Objects& o, Map* m = NULL,
				const LayerProperties& p = LayerProperties ())
			: Layer (c, n, m, p),
			  _objects (o)
							{ }
		/** The object layer own its objects.
			When the layer is destroyed the objets are destroyed with it. */
		~ObjectLayer ();

		virtual int type () const
							{ return (__QGAMES_OBJECTLAYER__); }

		const Objects& objects () const
							{ return (_objects); }
		Objects& objects ()
							{ return (_objects); }

		/** @see parent. */
		virtual void setFade (int fd);  

		/** @see parent. */
		virtual void initialize ();
		virtual void inEveryLoop ();
		virtual void drawOnWithElements (Screen* s, const Position& p, Elements& elmts);
		virtual void finalize ();

		protected:
		Objects _objects;
	};

	/** An image layer has a single image usually. */
	class ImageLayer : public Layer
	{
		public:
		ImageLayer (int c, const std::string& n, Image* i, Map* m = NULL,
				const LayerProperties& p = LayerProperties ())
			: Layer (c, n, m, p),
			  _image (i)
							{ }
		/** The image layer own its image.
			When the layer is destroyed the image is destroyed with it. */
		~ImageLayer ();

		virtual int type () const
							{ return (__QGAMES_IMAGELAYER__); }

		/** @see parent. */
		virtual void setFade (int fd);  

		/** @see parent. */
		virtual void drawOnWithElements (Screen* s, const Position& p, Elements& elmts);

		protected:
		Image* _image;
	};
}

#endif
  
// End of the file
/*@}*/
