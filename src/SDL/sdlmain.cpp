#include <Common/game.hpp>
#include <Common/resourcesreader.hpp>

int QGAMESMain (int argc, char *argv []);

#ifndef _CONSOLE
#include <SDL.h>
#ifdef __cplusplus
#define C_LINKAGE "C"
#else
#define C_LINKAGE
#endif /* __cplusplus */
extern C_LINKAGE int main (int argc, char *argv [])
#else
int _tmain (int argc, char *argv [])
#endif /* _CONSOLE */
{
#ifdef NDEBUG
	// In the release version, the resources are in a zip file
	// This instruction reads them and store in temporaly files when needed!
	// The resource reader is deleted at the end, and all the temporal file with them!
	QGAMES::InZipResourceReader rR 
		(std::string (".") + __PATH_SEPARATOR__ + std::string ("temp"));
#endif

	// Calls the basic Main function every QGAMES game has to have defined...
	return (QGAMESMain (argc, argv));
}