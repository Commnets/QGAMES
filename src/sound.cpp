#include <Common/sound.hpp>
#include <assert.h>

QGAMES::SoundSystem* QGAMES::SoundSystem::_system = NULL;

QGAMES::SoundSystem* QGAMES::SoundSystem::system ()
{
	assert (_system);

	return (_system); 
}
