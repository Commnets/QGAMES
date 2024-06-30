/**\ingroup AdArcadeGame */
/**\ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: adlayers.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 07/04/2018 \n
 *	Description: Defines complex layers.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ADLAYERS__
#define __QGAMES_ADLAYERS__

#include <AdvancedArcade/addefinitions.hpp>
#include <Arcade/arcadeinclude.hpp>

namespace QGAMES
{
	class AdvancedTileLayer;
	typedef std::map <int, AdvancedTileLayer*> AdvancedTileLayers;

	/** A layer with only a background.
		The background can be formed by many other "layers" that can move or not. */
	class BackgroundLayer : public ObjectLayer
	{
		public:
		BackgroundLayer (int id, const std::string& n, const Objects& o, Map* m = NULL, 
				const LayerProperties& p = LayerProperties ());

		// Managing the direction of the movement...
		/** To set the direction of the movement. */
		void setMove (const Vector& d)
							{ background () -> setMove (background () -> autoMovement (), d, true); }
		/** Changes the direction of the movement */
		void changeMovDirection ()
							{ background () -> setMove (background () -> 
								autoMovement (), -background () -> movementDirection (), true); }

		// Moving the background...
		/** Just to move a set of pixels in the direction selected. */
		void move ()
							{ background () -> move (); }
		/** Stops the movement of the background object. */
		void stop ()
							{ background () -> setMove (false, background () -> movementDirection ()); }
		/** Starts the automatic movement of the background object. */
		void automaticMove ()
							{ background () -> setMove (true, background () -> movementDirection ()); }
		/** To move the background like a block. */
		void solidMove (const Vector& d)
							{ background () -> solidMove (d); }

		private:
		// Implementation
		const Background* background () const
							{ return ((const Background*) 
								(((const ObjectProxy*) (*_objects.begin ())) -> ptr ())); }
		Background* background ()
							{ return ((Background*) 
								(((ObjectProxy*) (*_objects.begin ())) -> ptr ())); }
	};

	/** To draw an effect over the tiles of a tiled layer.
		e.g. A set of bubbles ovre a layer of water to indicate poisson.
		The properties sent as parameter can have the following parameters: 
		FORM: XX Number of the form representing the visual effect.
		ASPECTS: YY Number of aspects involved in the effect.
		ZONE: x1, y1, z1, x2, y2, x2: Positions where the form changes.
			  The coordinates of the zone are in "tiles" not pixels.
		*/
	class FormBasedTileLayerEffect : public Layer::VisualEffect
	{
		public:
		/** The constructor receives also the number of loops between every change,
			and the probability a form to appear in another tile. 1 will mean 
			full probability to appear, so all tiles will be executing the change of the form
			from the early beginning. */
		FormBasedTileLayerEffect (Layer* ly, const LayerProperties& pts, 
			int spd = 6, bdata prb = 0.4);

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s, const Position& pos = Position::_cero);

		protected:
		class FormStatus
		{
			public:
			FormStatus ()
				: _changing (false), _aspect (0)
								{ }
		
			bool _changing;
			int _aspect;
		};

		typedef std::vector <FormStatus> FormsStatus;

		FormsStatus _formsStatus;
		Form* _form;
		int _numberAspects;
		int _bbX1, _bbY1, _bbZ1, _bbX2, _bbY2, _bbZ2;
		int _numberStatus;
		int _changeSpeed;
		bdata _probabilityToAppear;

		// Implementation
		int _counter;
		int _sqrWidth, _sqrHeight, _sqrDepth;
		int _tileWidth, _tileHeight, _tileDepth;
	};

	/** The advanced tile layer can appear and disappear and
		it is possible to stop the movement, once it has been defined. */
	class AdvancedTileLayer : public TileLayer
	{
		public:
		AdvancedTileLayer (int c, const std::string& n, const Tiles& t, Map* m = NULL, 
				VisualOrientation o = _ISOMETRIC,
				const LayerProperties& p = LayerProperties (),
				bool oT = false);

		/** @see parent. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		// Appearing & dissapearing...
		/** Disappearing. 
			ml is the number of miliseconds among every change in the disappearing status. */
		void disappear (int ml = -1);
		bool isDisappearing ();
		/** To appear back. 
			ml is the number of miliseconds among any change in the appearing status. */
		void appear (int ml = -1);
		bool isAppearing ();
		/** To know whether the layer is or not transparent. 
			The layer is considered transparent when the fade is 75% the minimum by default. */
		bool transparent (float p = 0.75);
		/** The opposite. */
		bool solid (float p = 0.75);

		/** How many pixels the layers moved during last updating, if any
			at a specific tile. Takes into account movement and updater simultaneously! */
		Vector vectorMoved (const Tile& tl) const;

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();

		protected:
		/** Factory method to create the movement if it's been defined in the properties. 
			A default behaviour has been created, but it can be overloaded later. */
		virtual MovementBehaviour* createMovement ();
		/** Factory method to create the tile updater if it's been defined in the properties.
			A default behaviour has been created, but it can be defined later. */
		virtual TileUpdater* createTileUpdater ();
		/** Factory method to crate a visual effect over */
		virtual VisualEffect* createVisualEffect ();

		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		private:
		static const int _fadeValues [];

		static const int _COUNTERTOFADEVALUES = 0;
		static const int _SWITCHAPPEARON = 0;
		static const int _SWITCHDISAPPEARON = 1;

		// Implementation
		Position _originalPosition;
		bool _originalPositionSet;
		int _fadeGrade;
	};
}

#endif
  
// End of the file
/*@}*/