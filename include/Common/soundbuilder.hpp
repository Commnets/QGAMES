/** \ingroup Game */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: soundbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class to build up sounds. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_SOUNDBUILDER__
#define __QGAMES_SOUNDBUILDER__

#include <Common/sound.hpp>
#include <Common/definitions.hpp>
#include <string>
#include <vector>

namespace QGAMES
{
	/** To create sounds. The builder owns the sound it creates. */
	class SoundBuilder
	{
		public:
		/** 
		  *	The sound defition file should have an structure as follows: \n
		  *	The file attribute is something relative to the path where 
		  *	the exe file is located. \n
		  *	Take care an include line can appear! \n
		  *	<?xml version="1.0"?> \n
		  *	<Sounds> \n
		  *		<Include file="CCC"/> \n
		  *		<Sound id="X" file="XXX"/> \n
		  *		... \n
		  *	</Sounds> 
		  */
		SoundBuilder (const std::string& fDef);
		/** The builder owns all sounds created with it.
			When the builder is destroyed all sounds created are also destroyed. */
		virtual ~SoundBuilder ();

		/** To get the number of definitions included in the definition file. */
		int numberResources () const
							{ return ((int) _definitions.size ()); }
		/** To get the id of every resource to be loaded. */
		std::vector <int> resourcesId () const;

		/** Once the definitions of the sounds have been created, 
			the sounds can be loaded into memoty usually before the game starts. \n
			Doing so, the overall performance will be better. */
		void preLoad ();
		/** To get a sound. If the sound has not been loaded before, 
			it is loaded from the information that is in the defintion created at construction time. \n
			First time a sound is requested the sound system is created (@see createSounSystem method). */
		Sound* sound (int sId);

		protected:
		struct SoundDefinition
		{
			SoundDefinition ()
				: _id (-1),
				  _type (Sound::SOUND_MUSIC),
				  _fileDefinition (__NULL_STRING__),
				  _loop (0)
							{ }

			int _id;
			Sound::sound_type _type;
			std::string _fileDefinition;
			int _loop;
		};

		virtual Sound* createSound (const SoundDefinition& def) = 0;
		virtual void createSoundSystem () = 0; // Used in preload and the first time a sound is created if any...

		private:
		void readFile (const std::string& fDef);

		private:
		typedef std::map <int, SoundDefinition> Definitions;

		Definitions _definitions;
		Sounds _sounds;

		std::string _definitionFile;
		std::string _basePath;
	};
}


#endif

// End of the file
/*@}*/
