/**\ingroup AdArcadeGame */
/**\ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: admap.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 18/11/2018 \n
 *	Description: Defines complex maps.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ADMAPS__
#define __QGAMES_ADMAPS__

#include <AdvancedArcade/addefinitions.hpp>
#include <AdvancedArcade/adlayers.hpp>
#include <Arcade/arcadeinclude.hpp>

namespace QGAMES
{
	/** A map as background.
		This map is made of only one layer and that layer has to be a background layer. */
	class BackgroundMap : public ObjectMap
	{
		public:
		BackgroundMap (int id, const Layers& l, int w, int h, const MapProperties& p = MapProperties ());

		// Managing the direction of the movement
		/** To set the direction of the movement of the layer background. */
		void setMove (const Vector& d)
							{ background () -> setMove (d); }
		/** Changes the direction of the movement. */
		void changeMovDirection ()
							{ background () -> changeMovDirection (); }

		// Moving the background
		/** Just move the background. */
		void move ()
							{ background () -> move (); }
		/** Stop the automatic movement of the background. */
		void stop ()
							{ background () -> stop (); }
		/** Starts the automatic movement of the background. */
		void automaticMove ()
							{ background () -> automaticMove (); }
		/** To move the background like a block. */
		void solidMove (const Vector& d)
							{ background () -> solidMove (d); } 

		private:
		// Implementation
		const BackgroundLayer* background () const
							{ return ((const BackgroundLayer*) (*_layers.begin ())); }
		BackgroundLayer* background ()
							{ return ((BackgroundLayer*) (*_layers.begin ())); }
	};
}

#endif
  
// End of the file
/*@}*/