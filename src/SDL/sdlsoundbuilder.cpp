#include <SDL/sdlsoundbuilder.hpp>
#include <SDL/sdlsound.hpp>
#include <SDL_mixer.h>
#include <Common/resourcesreader.hpp>
#include <iostream>

// ---
SDLSoundBuilder::SDLSoundBuilder (const std::string& dF)
	: QGAMES::SoundBuilder (dF)
{
	int e = Mix_OpenAudio (MIX_DEFAULT_FREQUENCY, AUDIO_S16, 2, 4096);
	#ifndef NDEBUG
	if (e != 0)
		std::cout << Mix_GetError () << std::endl;
	#endif
}

// ---
SDLSoundBuilder::~SDLSoundBuilder ()
{
	Mix_CloseAudio ();
}

//---
QGAMES::Sound* SDLSoundBuilder::createSound 
	(const QGAMES::SoundBuilder::SoundDefinition& def)
{
	QGAMES::Sound* result = NULL;

	switch (def._type)
	{
		case QGAMES::Sound::SOUND_MUSIC:
			{
				Mix_Music* dt = Mix_LoadMUS (__RL (def._fileDefinition).c_str ());
				if (dt != NULL)
					result = new SDLSound (def._id, QGAMES::Sound::SOUND_MUSIC, def._fileDefinition,
						def._loop, dt);
				else
					std::cout << SDL_GetError () << std::endl 
							  << Mix_GetError () << std::endl;
			}
			break;

		case QGAMES::Sound::SOUND_SFX:
			{
				Mix_Chunk* dt = Mix_LoadWAV (__RL (def._fileDefinition).c_str ());
				if (dt != NULL)
					result = new SDLSound (def._id, QGAMES::Sound::SOUND_SFX, def._fileDefinition,
						def._loop, dt);
				else
					std::cout << SDL_GetError () << std::endl 
							  << Mix_GetError () << std::endl;
			}
			break;

		default:
			break;
	}

	return (result);
}

// ---
void SDLSoundBuilder::createSoundSystem ()
{
	QGAMES::SoundSystem* nS = new SDLSoundSystem (); // THis a static object, so it is not needed to return it...
}

