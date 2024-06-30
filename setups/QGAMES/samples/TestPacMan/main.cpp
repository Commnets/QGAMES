/**	
 *	@file	
 *	File: main.cpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 21/08/2021 \n
 *	Description: Main programm for PacManII game
 *	Versions: 1.0 Initial
 */

#include "stdafx.h"

#include <PacManLike/pcinclude.hpp>

using namespace PACMAN;

#ifndef _CONSOLE
#include <SDL.h>
#ifdef __cplusplus
#define C_LINKAGE "C"
#else
#define C_LINKAGE
#endif /* __cplusplus */
#if _MSC_VER >= 1900
extern C_LINKAGE FILE __iob_func[3] = { *stdin,*stdout,*stderr };
#endif
extern C_LINKAGE int main(int argc, char *argv[])
#else
int _tmain (int argc, char *argv [])
#endif /* _CONSOLE */
{
#ifdef NDEBUG
	// In the release version, the resources are in a zip file
	// This instruction reads them and store in temporaly files when needed!
	// The resource reader is deleted at the end, and all the temporal file with them!
	QGAMES::InZipResourceReader rR 
		(QGAMES::ConfigurationFile (std::string (__GAME_CONFIGURATIONFILE__)).
			property (std::string (__GAME_DATADIRPROPERTYNAME__)));
#endif

	// The more difficult the game is more number of levels it has...
	Game game ({ 
				 DataGame::classicDataGame (25), //Easy
				 DataGame::classicDataGame (30), // Difficult
				 DataGame::classicDataGame (35)  // Hard 
			   }); 
	game.setLinkDrawToFrameRate (true); 
	game.setPreLoad (false); 
	game.exec ();

	return (0);
}
