#include <SDL/sdlsound.hpp>

//---
SDLSound::~SDLSound ()
{
	// The sound can't be deleted unless the audio is open...
	// Then it is needed to test it before!
	int f, ch; Uint16 ft;
	if (Mix_QuerySpec (&f, &ft, &ch))
	{
		delete (_sound);
		_sound = NULL;
	}
	// When the sounds are deleted from the builder, the destructors will never be exectued
	// Because closing the audio channel would have been executed previously!
}

//---
int SDLSound::play (int ch)
{
	_realChannel = ch;
	switch (_type)
	{
		case SOUND_MUSIC:
			_realChannel = Mix_PlayMusic (_music, _loop);
			break;
		
		case SOUND_SFX:
			_realChannel = Mix_PlayChannel (ch, _sound, _loop);
			break;

		default:
			break;
	};

	return (_realChannel);
}

// ---
void SDLSound::pause ()
{
	switch (_type)
	{
		case SOUND_MUSIC:
			break;
		
		case SOUND_SFX:
			Mix_Pause (_realChannel);
			break;

		default:
			break;
	};
}

// ---
void SDLSound::resume ()
{
	switch (_type)
	{
		case SOUND_MUSIC:
			break;
		
		case SOUND_SFX:
			Mix_Resume (_realChannel);
			break;

		default:
			break;
	};
}

// ---
void SDLSound::stop ()
{
	switch (_type)
	{
		case SOUND_MUSIC:
			break;
		
		case SOUND_SFX:
			Mix_HaltChannel (_realChannel);
			break;

		default:
			break;
	};
}

// ---
bool SDLSound::isPlaying ()
{
	return (Mix_Playing (_realChannel) != 0);
}

// ---
void SDLSoundSystem::pause ()
{
	Mix_Pause (-1);
}

// ---
void SDLSoundSystem::pauseChannel (int ch)
{
	Mix_Pause (ch);
}

// ---
void SDLSoundSystem::resume ()
{
	Mix_Resume (-1);
}

// ---
void SDLSoundSystem::resumeChannel (int ch)
{
	Mix_Resume (ch);
}

// ---
int SDLSoundSystem::channelsPlaying ()
{
	return (Mix_Playing (-1));
}

// ---
bool SDLSoundSystem::isChannelPlaying (int c)
{
	return (Mix_Playing (c) == 1 ? true : false);
}
