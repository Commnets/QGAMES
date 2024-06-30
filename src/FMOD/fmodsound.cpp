#include <FMOD/fmodsound.hpp>
#include <assert.h>

//---
int FMODSound::play (int ch)
{
	FMOD_RESULT error;

	// Gets the channel...
	if ((error = _fmodSystem -> getChannel (ch, &_fmodChannel)) != FMOD_OK)
		return (-1); // With error, not to continue...

	// Gets the real channel index...
	error = _fmodChannel -> getIndex (&_realChannel);  // Already playing this sound...(or error)

	// Verifies whether it is playing...
	bool isPlaying = false;
	error = _fmodChannel -> isPlaying (&isPlaying);
	if (isPlaying)
		return (_realChannel); // Then returns...

	// Play the sound, and if everything goes ok, then fix the volumen!
	if ((error = _fmodSystem -> playSound (_fmodSound, NULL, false, &_fmodChannel)) == FMOD_OK)
		_fmodChannel -> setVolume (1.0);
	return (_realChannel); // ...and returns the real channel id...
}

// ---
void FMODSound::pause ()
{
	FMOD_RESULT error;
	error = _fmodChannel -> setPaused (true);
}

// ---
void FMODSound::resume ()
{
	FMOD_RESULT error;
	error = _fmodChannel -> setPaused (false);
}

// ---
void FMODSound::stop ()
{
	FMOD_RESULT error;
	error = _fmodChannel -> stop ();
}

// ---
bool FMODSound::isPlaying ()
{
	FMOD_RESULT error;
	bool isPlaying = false;
	error = _fmodChannel -> isPlaying (&isPlaying);
	return (isPlaying);
}

// ---
void FMODSoundSystem::pause ()
{
	FMOD_RESULT error;
	FMOD::Channel* channel;
	for (int i = 0; i < _numberChannels; i++)
		if ((error = _fmodSystem -> getChannel (i, &channel)) == FMOD_OK)
			error = channel -> setPaused (true);
}

// ---
void FMODSoundSystem::pauseChannel (int ch)
{
	FMOD_RESULT error;
	FMOD::Channel* channel;
	if ((error = _fmodSystem -> getChannel (ch, &channel)) == FMOD_OK)
		error = channel -> setPaused (true);
}

// ---
void FMODSoundSystem::resume ()
{
	FMOD_RESULT error;
	FMOD::Channel* channel;
	for (int i = 0; i < _numberChannels; i++)
		if ((error = _fmodSystem -> getChannel (i, &channel)) == FMOD_OK)
			error = channel -> setPaused (false);
}

// ---
void FMODSoundSystem::resumeChannel (int ch)
{
	FMOD_RESULT error;
	FMOD::Channel* channel;
	if ((error = _fmodSystem -> getChannel (ch, &channel)) == FMOD_OK)
		error = channel -> setPaused (false);
}

// ---
int FMODSoundSystem::channelsPlaying ()
{
	FMOD_RESULT error;

	int result = 0;
	FMOD::Channel* channel;
	for (int i = 0; i < _numberChannels; i++)
	{
		bool isPlaying = false;
		if ((error = _fmodSystem -> getChannel (i, &channel)) == FMOD_OK &&
			(error = channel -> isPlaying (&isPlaying)) == FMOD_OK)
				result++;
	}

	return (result);
}

// ---
bool FMODSoundSystem::isChannelPlaying (int c)
{
	FMOD_RESULT error;
	FMOD::Channel* channel;
	bool isPlaying = false;
	if ((error = _fmodSystem -> getChannel (c, &channel)) == FMOD_OK)
		error = channel -> isPlaying (&isPlaying);
	return (isPlaying);
}
