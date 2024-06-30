/** \ingroup FMOD */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: fmodsoundbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 25/12/2017 \n
 *	Description: Defines a fmod implememtation to build up sounds. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_FMODSOUNDBUILDER__
#define __QGAMES_FMODSOUNDBUILDER__

#include <Common/soundbuilder.hpp>
#include <fmod.hpp>

class FMODSoundSystem;

/** To create sounds to be used in a OGRE system. */
class FMODSoundBuilder : public QGAMES::SoundBuilder
{
	public:
	FMODSoundBuilder (const std::string& def, int nC = 16);
	virtual ~FMODSoundBuilder ();

	protected:
	/** @see parent. */
	virtual QGAMES::Sound* createSound (const QGAMES::SoundBuilder::SoundDefinition& def);
	virtual void createSoundSystem ();

	protected:
	/** A reference to the fmod system used internally. */
	FMOD::System* _fmodSystem;
	/** The maximum number of channels. */
	int _numberChannels;
};

#endif
  
// End of the file
/*@}*/
