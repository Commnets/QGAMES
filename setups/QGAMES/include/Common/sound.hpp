/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: sound.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a sound. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_SOUND__
#define __QGAMES_SOUND__

#include <string>
#include <map>

namespace QGAMES
{
	class Sound;
	typedef std::map <int, Sound*> Sounds;

	/** A sound can be long (MUSIC) or short (SOUND)
		Usually the method to play both are not the same. */
	class Sound
	{
		public:
		enum sound_type
		{
			SOUND_MUSIC = 0,
			SOUND_SFX = 1
		};

		Sound (int id, sound_type t, const std::string& s, int l)
			: _id (id),
			  _type (t),
			  _source (s),
			  _loop (l),
			  _realChannel (0)
							{ }
		virtual ~Sound ()
							{ }

		int id () const
							{ return (_id); }
		sound_type type () const
							{ return (_type); }
		const std::string& source () const
							{ return (_source); }
		int loop () const
							{ return (_loop); }
		int channel () const
							{ return (_realChannel); }

		virtual int play (int ch) = 0;
		virtual void pause () = 0;
		virtual void resume () = 0;
		virtual void stop () = 0;
		virtual bool isPlaying () = 0;

		protected:
		int _id;
		sound_type _type;
		std::string _source;
		int _loop;
		int _realChannel;
	};

	/** The sound system represents who controls all sounds. 
		Sound system can be only one alive in the game. */
	class SoundSystem
	{
		public:
		SoundSystem ()
							{ if (_system != NULL) exit (1); _system = this; }
		virtual ~SoundSystem ()
							{ }

		static SoundSystem* system ();
		static bool existSystem () 
							{ return (_system != NULL); }
		static void finishSystem ()
							{ delete (_system); _system = NULL; }

		virtual void pause () = 0;
		virtual void pauseChannel (int ch) = 0;
		virtual void resume () = 0;
		virtual void resumeChannel (int ch) = 0;
		virtual int channelsPlaying () = 0;
		virtual bool isChannelPlaying (int c) = 0;

		private:
		static SoundSystem* _system;
	};
}

#endif

// End of the file
/*@}*/
