/** \ingroup SDL */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: sdlsoundbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a sdl implememtation to build up sounds. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_SDLSOUND__
#define __QGAMES_SDLSOUND__

#include <Common/sound.hpp>
#include <SDL_mixer.h>

/** To play a sound using SDL libraries. */
class SDLSound : public QGAMES::Sound
{
	public:
	SDLSound (int id, sound_type t, const std::string& s, int l, Mix_Music* m)
		: QGAMES::Sound (id, t, s, l),
		  _music (m),
		  _sound (NULL)
							{ }
	SDLSound (int id, sound_type t, const std::string& s, int l, Mix_Chunk* m)
		: QGAMES::Sound (id, t, s, l),
		  _music (NULL),
		  _sound (m)
							{ }
	virtual ~SDLSound ();

	/** @see parent. */
	virtual int play (int ch);
	virtual void pause ();
	virtual void resume ();
	virtual void stop ();
	virtual bool isPlaying ();

	protected:
	Mix_Music* _music;
	Mix_Chunk* _sound;
};

/** Represents the sound system using SDL libraries. */
class SDLSoundSystem : public QGAMES::SoundSystem
{
	public:
	SDLSoundSystem ()
		: QGAMES::SoundSystem ()
							{ }

	/** @see parent. */
	virtual void pause ();
	virtual void pauseChannel (int ch);
	virtual void resume ();
	virtual void resumeChannel (int ch);
	virtual int channelsPlaying ();
	virtual bool isChannelPlaying (int c);
};

#endif
  
// End of the file
/*@}*/
