/**\ingroup AdArcadeGame */
/**\ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: adscenes.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 18/11/2018 \n
 *	Description: Defines complex scenes.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ADSCENES__
#define __QGAMES_ADSCENES__

#include <AdvancedArcade/addefinitions.hpp>
#include <AdvancedArcade/admaps.hpp>
#include <Arcade/arcadeinclude.hpp>

namespace QGAMES
{
	/** A scene with background.
		The scene with background can have a background map or not, and many others too. */
	class SceneWithBackground : public Scene
	{
		public:
		SceneWithBackground (int id, const Maps& m, const Scene::Connections& cn,
			const SceneProperties& p = SceneProperties (), const EntitiesPerLayer& ePL = EntitiesPerLayer ())
				: Scene (id, m, cn, p, ePL),
				  _backgroundMap (NULL) // Nothing by default...
							{ }

		// Managing the background
		/** To get the background map associated with this scene. */
		const BackgroundMap* backgroundMap () const
								{ return (_backgroundMap); }
		BackgroundMap* backgroundMap ()
								{ return (_backgroundMap); }
		/** To set the background map.
			if the map selected is not a background map, then no background map will be set. 
			If __MININT__ is passed as parameter no background map will be set as well. */
		void setBackgroundMap (int nM)
								{ _backgroundMap = (nM == __MININT__) ? NULL : dynamic_cast <BackgroundMap*> (map (nM)); }

		/** @see parent. */
		virtual void initialize ();
		virtual void inEveryLoop ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		virtual void finalize ();

		protected:
		BackgroundMap* _backgroundMap;
	};
}

#endif
  
// End of the file
/*@}*/