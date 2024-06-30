#include <FMOD/fmodsoundbuilder.hpp>
#include <FMOD/fmodsound.hpp>

// ---
FMODSoundBuilder::FMODSoundBuilder (const std::string& def, int nC)
	: QGAMES::SoundBuilder (def),
	  _fmodSystem (NULL),
	  _numberChannels (nC)
{ 
	FMOD_RESULT error;

	// Create the main system object.
	error = FMOD::System_Create (&_fmodSystem);
	if (!_fmodSystem)
		return;

	error = _fmodSystem -> init (_numberChannels, FMOD_INIT_NORMAL, NULL);    
	error = _fmodSystem -> set3DSettings (1.0f, 1.0f, 0.5f); // Doppler effect...
}

// ---
FMODSoundBuilder::~FMODSoundBuilder ()
{
	FMOD_RESULT error;
	if (_fmodSystem)
		error = _fmodSystem -> release ();
}

//---
QGAMES::Sound* FMODSoundBuilder::createSound 
	(const QGAMES::SoundBuilder::SoundDefinition& def)
{
	FMOD_RESULT error;
	FMOD::Sound* fmodSound = NULL;
	switch (def._type)
	{
		case FMODSound::Type::_2D:
			error = _fmodSystem -> createStream (def._fileDefinition.c_str (), 
				(def._loop) ? (FMOD_LOOP_NORMAL | FMOD_DEFAULT) : FMOD_DEFAULT, NULL, &fmodSound);
			break;
		case FMODSound::Type::_3D:
			error = _fmodSystem -> createStream (def._fileDefinition.c_str (), 
				(def._loop) ? (FMOD_LOOP_NORMAL | FMOD_3D) : FMOD_3D, NULL, &fmodSound);
			break;
		default:
			break;
	}

	assert (error == FMOD_OK && fmodSound != NULL);
	return (new FMODSound (def._id, _fmodSystem, fmodSound, 
		QGAMES::Sound::sound_type::SOUND_MUSIC, def._fileDefinition, def._loop));
}

// ---
void FMODSoundBuilder::createSoundSystem ()
{
	FMODSoundSystem* nS = new FMODSoundSystem (_fmodSystem, _numberChannels);
}

