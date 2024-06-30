/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: _exengine.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2017 \n
 *	Description: Defines all elements necessary to wrap an external game engine.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_EXENGINE__
#define __QGAMES_EXENGINE__

#include <Common/definitions.hpp>

namespace QGAMES
{
	class Element;
	class GUISystem;
	class Screen;
	class Position;

	/** A class to represent a wrapper of a external engine used to manage any element of the game. \n
		Some times, specially some graphical interfaces need to manage the elements of the game (OGRE3D e.g.) \n
		So, any action done with them has to beinformed to the external engine. \n
		This external engine will be invoked any time some happens in the Elements of the Game (if defined). */
	class _ExEngine
	{
		public:
		// No constructor is needed by default
		// The destructor is defined just in case later something is needed 
		// to be destroyed in futher classes
		virtual ~_ExEngine () 
							{ }

		// For the GUI Systems
		virtual void whenCreateGUISystem (GUISystem* sys) = 0;
		virtual void whenDeleteGUISystem (GUISystem* elmnt) = 0;
		virtual void whenInitialize (GUISystem* elmnt) = 0;
		virtual void whenInEveryLoop (GUISystem* elmnt) = 0;
		virtual void whenDrawOn (GUISystem* elmnt, Screen* s, const Position& p) = 0;

		// For elements...
		virtual void whenCreateElement (Element* elmnt) = 0; 
		virtual void whenDeleteElement (Element* elmnt) = 0;
		virtual void whenInitialize (Element* elmnt) = 0;
		virtual void whenInEveryLoop (Element* elmnt) = 0;
		virtual void whenUpdatePositions (Element* elmnt) = 0;
		virtual void whenDrawOn (Element* elmnt, Screen* s, const Position& p) = 0;
		virtual void whenFinalize (Element* elmnt) = 0;
	};
}

#endif
  
// End of the file
/*@}*/