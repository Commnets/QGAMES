#include <Platform/ptworlds.hpp>
#include <Platform/ptscenes.hpp>

// ---
QGAMES::PlatformWorld::PlatformWorld (int c, const QGAMES::Scenes& s, const QGAMES::WorldProperties& p)
	: QGAMES::World (c, s, p)
{
	for (QGAMES::Scenes::const_iterator i = _scenes.begin (); i != _scenes.end (); i++)
		assert (dynamic_cast <QGAMES::PlatformScene*> ((*i).second)); // All of the same type...
}
