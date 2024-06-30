#include <Common/soundbuilder.hpp>
#include <Common/definitions.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/genalgorithms.hpp>
#include <Common/game.hpp>
#include <tinyxml.h>
#include <iostream>

// ---
QGAMES::SoundBuilder::SoundBuilder (const std::string& fDef)
	: _definitions (),
	  _sounds (),
	  _definitionFile (fDef),
	  _basePath (QGAMES::onlyPath (fDef))
{
	if (_basePath == std::string (__NULL_STRING__)) 
		_basePath = std::string (".");

	readFile (__RL (_definitionFile));
	// At this point the definition file has the name of the file to load...
	// ...and the list of definitions is empty!
}

// ---
QGAMES::SoundBuilder::~SoundBuilder ()
{
	QGAMES::Sounds::const_iterator i;
	for (i = _sounds.begin (); i != _sounds.end (); i++)
		delete ((*i).second);
	_sounds.clear ();

	QGAMES::SoundSystem::finishSystem ();
}

// ---
std::vector <int> QGAMES::SoundBuilder::resourcesId () const
{
	std::vector <int> result;
	for (QGAMES::SoundBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end (); i++)
		result.push_back ((*i).first);
	return (result);
}

// ---
void QGAMES::SoundBuilder::preLoad ()
{
	for (QGAMES::SoundBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end (); i++)
		if (_sounds.find ((*i).second._id) == _sounds.end ())
			sound ((*i).second._id);
}

// ---
QGAMES::Sound* QGAMES::SoundBuilder::sound (int sId)
{
	// No system, then it is created...
	if (!QGAMES::SoundSystem::existSystem ())
		createSoundSystem ();

	QGAMES::Sounds::const_iterator i = _sounds.find (sId);
	if (i != _sounds.end ())
		return ((*i).second);

	__WLOGPLUS (std::string ("Loading sound: ") + std::to_string (sId));
	
	QGAMES::SoundBuilder::Definitions::const_iterator j = _definitions.find (sId);
	assert (j != _definitions.end ()); // It doesn't exist
	
	QGAMES::Sound* sound = createSound ((*j).second);
	assert (sound); // createSound is an user's method...
	_sounds.insert (QGAMES::Sounds::value_type (sId, sound));
	return (sound);
}

// ---
void QGAMES::SoundBuilder::readFile (const std::string& fDef)
{
	__WLOGPLUS (std::string ("Reading sounds definition file: ") + fDef);

	TiXmlDocument doc (fDef.c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the root element doesn't call "Sounds" the system will work too...
	// Just to speed up theloading!

	for (TiXmlElement* defElement = rootElement -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		// The line can be an include line...
		// That line can be at any place in the sound definition file...
		if (strcmp (defElement -> Value (), __QGAMES_INCLUDENAME__) == 0)
		{
			std::string nF = defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__);
			readFile (__RL (nF));
		}
		else
		// The other type of line is a simple sound line...
		if (strcmp (defElement -> Value (), __QGAMES_SOUNDTAGNAME__) == 0)
		{
			int type;
			QGAMES::SoundBuilder::SoundDefinition def;
			defElement -> Attribute (__QGAMES_SOUNDATTRID__, &def._id);
			defElement -> Attribute (__QGAMES_SOUNDATTRTYPE__, &type);
			def._type = (type == 0) ? QGAMES::Sound::SOUND_MUSIC : QGAMES::Sound::SOUND_SFX;
			defElement -> Attribute (__QGAMES_SOUNDATTRLOOP__, &def._loop);
			def._fileDefinition = _basePath + std::string (__PATH_SEPARATOR__) + 
				defElement -> Attribute (__QGAMES_SOUNDATTRFILE__);
			assert (def._id != -1); // Is it a valid sound?
			_definitions.insert (QGAMES::SoundBuilder::Definitions::value_type (def._id, def));
		}
	}
}
