/** \ingroup FMOD */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: fmodsoundbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 25/12/2017 \n
 *	Description: Defines a FMOD implememtation to build up OGRE sounds. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_FMODSOUND__
#define __QGAMES_FMODSOUND__

#include <Common/sound.hpp>
#include <fmod.hpp>

/** To represent a sound in a FMOD context,
	using the libraries from FMOD. */
class FMODSound : public QGAMES::Sound
{
	public:
	typedef enum
	{
		_2D = 0,
		_3D,
	} Type;

	FMODSound (int id, FMOD::System* sys, FMOD::Sound* sd, 
			QGAMES::Sound::sound_type t, const std::string& s, int l)
		: QGAMES::Sound (id, t, s, l),
		  _fmodSoundType (_2D),
		  _fmodSystem (sys),
		  _fmodSound (sd),
		  _fmodChannel (NULL)
							{ }

	/** @see parent. */
	virtual int play (int ch);
	virtual void pause ();
	virtual void resume ();
	virtual void stop ();
	virtual bool isPlaying ();

	protected:
	Type _fmodSoundType;
	FMOD::System* _fmodSystem;
	FMOD::Sound* _fmodSound;

	// Implementation
	FMOD::Channel* _fmodChannel;
};

/** To initiate the sound system using FMOD libraries. 
	and to manage sounds in general. */
class FMODSoundSystem : public QGAMES::SoundSystem
{
	public:
	FMODSoundSystem (FMOD::System* sys, int nC)
		: _fmodSystem (sys), _numberChannels (nC)
							{ }
	// No destructor is needed...

	/** @see parent. */
	virtual void pause ();
	virtual void pauseChannel (int ch);
	virtual void resume ();
	virtual void resumeChannel (int ch);
	virtual int channelsPlaying ();
	virtual bool isChannelPlaying (int c);

	protected:
	/** A reference to the fmod system used internally. */
	FMOD::System* _fmodSystem;
	/** The number of channels used by the system. */
	int _numberChannels;
};

#endif
  
// End of the file
/*@}*/
