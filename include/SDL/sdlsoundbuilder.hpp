/** \ingroup SDL */
/** \ingroup Structure */
/** \ingroup Builders */
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

#ifndef __QGAMES_SDLSOUNDBUILDER__
#define __QGAMES_SDLSOUNDBUILDER__

#include <Common/soundbuilder.hpp>

class SDLSoundSystem;

/** To instanciate SDL sounds. */
class SDLSoundBuilder : public QGAMES::SoundBuilder
{
	public:
	SDLSoundBuilder (const std::string& dF);
	virtual ~SDLSoundBuilder ();

	protected:
	/** @see parent. */
	virtual QGAMES::Sound* createSound (const QGAMES::SoundBuilder::SoundDefinition& def);
	virtual void createSoundSystem ();
};

#endif
  
// End of the file
/*@}*/
