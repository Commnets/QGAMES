/** \ingroup Example */
/*@{*/

/**	
 *	@file	
 *	File: main.cpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/01/2018 \n
 *	Description: A pure QGAMES game based in OGRE behind.
 *	Versions: 1.0 Initial
 */

#include "TestCommonOGRE.hpp"
#include <Windows.h>

using namespace TestOGRE;

INT WINAPI WinMain (HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
{
#ifdef NDEBUG
	// In the release version, the resources are in a zip file
	// This instruction reads them and store in temporaly files when needed!
	// The resource reader is deleted at the end, and all the temporal file with them!
	QGAMES::InZipResourceReader rR 
		(QGAMES::ConfigurationFile (std::string (__GAME_CONFIGURATIONFILE__)).
			property (std::string (__GAME_DATADIRPROPERTYNAME__)));
#endif

	Game game;
	game.setLinkDrawToFrameRate (true); // The normal thing is to draw at the same pace than the frame rate...
	game.setPreLoad (true); // The normal thing is to preload all componenets...
	game.exec ();
	return (0);
}
  
// End of the file
/*@}*/
