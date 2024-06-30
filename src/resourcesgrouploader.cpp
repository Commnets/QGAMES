#include <Common/resourcesgrouploader.hpp>
#include <Common/definitions.hpp>
#include <Common/genalgorithms.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/formbuilder.hpp>
#include <Common/soundbuilder.hpp>
#include <Common/movementbuilder.hpp>
#include <Common/entitybuilder.hpp>
#include <Common/objectbuilder.hpp>
#include <Common/game.hpp>
#include <assert.h>

// ---
QGAMES::ResourcesGroupLoader::TypeFormLoader::TypeFormLoader (const std::string& type, QGAMES::FormBuilder* fBld)
	: QGAMES::ResourcesGroupLoader::TypeLoader (type),
	  _formBuilder (fBld)
{
	assert (_formBuilder);
	// Just check. It can't be NULL at all.
}

// ---
void QGAMES::ResourcesGroupLoader::TypeFormLoader::load (const std::vector <int>& rscs)
{
	for (int i = 0; i < (int) rscs.size (); i++)
		_formBuilder -> form (rscs [i]); // Just invoke the method to load it...
}

// ---
QGAMES::ResourcesGroupLoader::StdTypeFormLoader::StdTypeFormLoader ()
	: QGAMES::ResourcesGroupLoader::TypeFormLoader 
		(__QGAMES_STDTYPEFORMSLOADER__, QGAMES::Game::game () -> formBuilder ())
{
	// Nothing else to do...
}

// ---
QGAMES::ResourcesGroupLoader::TypeSoundLoader::TypeSoundLoader (const std::string& type, QGAMES::SoundBuilder* sBld)
	: QGAMES::ResourcesGroupLoader::TypeLoader (type),
	  _soundBuilder (sBld)
{
	assert (_soundBuilder);
	// Just check. It can't be NULL at all.
}

// ---
void QGAMES::ResourcesGroupLoader::TypeSoundLoader::load (const std::vector <int>& rscs)
{
	for (int i = 0; i < (int) rscs.size (); i++)
		_soundBuilder -> sound (rscs [i]); // Just invoke the method to load it...
}

// ---
QGAMES::ResourcesGroupLoader::StdTypeSoundLoader::StdTypeSoundLoader ()
	: QGAMES::ResourcesGroupLoader::TypeSoundLoader 
		(__QGAMES_STDTYPESOUNDSLOADER__, QGAMES::Game::game () -> soundBuilder ())
{
	// Nothing else to do...
}

// ---
QGAMES::ResourcesGroupLoader::TypeMovementLoader::TypeMovementLoader (const std::string& type, QGAMES::MovementBuilder* mBld)
	: QGAMES::ResourcesGroupLoader::TypeLoader (type),
	  _movementBuilder (mBld)
{
	assert (_movementBuilder);
	// Just check. It can't be NULL at all.
}

// ---
void QGAMES::ResourcesGroupLoader::TypeMovementLoader::load (const std::vector <int>& rscs)
{
	for (int i = 0; i < (int) rscs.size (); i++)
		_movementBuilder -> movement (rscs [i]); // Just invoke the method to load it...
}

// ---
QGAMES::ResourcesGroupLoader::StdTypeMovementLoader::StdTypeMovementLoader ()
	: QGAMES::ResourcesGroupLoader::TypeMovementLoader 
		(__QGAMES_STDTYPEMOVEMENTSLOADER__, QGAMES::Game::game () -> movementBuilder ())
{
	// Nothing else to do...
}

// ---
QGAMES::ResourcesGroupLoader::TypeEntityLoader::TypeEntityLoader (const std::string& type, QGAMES::EntityBuilder* eBld)
	: QGAMES::ResourcesGroupLoader::TypeLoader (type),
	  _entityBuilder (eBld)
{
	assert (_entityBuilder);
	// Just check. It can't be NULL at all.
}

// ---
void QGAMES::ResourcesGroupLoader::TypeEntityLoader::load (const std::vector <int>& rscs)
{
	for (int i = 0; i < (int) rscs.size (); i++)
		_entityBuilder -> entity (rscs [i]); // Just invoke the method to load it...
}

// ---
QGAMES::ResourcesGroupLoader::StdTypeEntityLoader::StdTypeEntityLoader ()
	: QGAMES::ResourcesGroupLoader::TypeEntityLoader 
		(__QGAMES_STDTYPEENTITIESLOADER__, QGAMES::Game::game () -> entityBuilder ())
{
	// Nothing else to do...
}

// ---
QGAMES::ResourcesGroupLoader::TypeObjectLoader::TypeObjectLoader (const std::string& type, QGAMES::ObjectBuilder* oBld)
	: QGAMES::ResourcesGroupLoader::TypeLoader (type),
	  _objectBuilder (oBld)
{
	assert (_objectBuilder);
	// Just check. It can't be NULL at all.
}

// ---
void QGAMES::ResourcesGroupLoader::TypeObjectLoader::load (const std::vector <int>& rscs)
{
	for (int i = 0; i < (int) rscs.size (); i++)
		_objectBuilder -> object (rscs [i]); // Just invoke the method to load it...
}

// ---
QGAMES::ResourcesGroupLoader::StdTypeObjectLoader::StdTypeObjectLoader ()
	: QGAMES::ResourcesGroupLoader::TypeObjectLoader 
		(__QGAMES_STDTYPEOBJECTSLOADER__, QGAMES::Game::game () -> objectBuilder ())
{
	// Nothing else to do...
}

// ---
QGAMES::ResourcesGroupLoader::ResourcesGroupLoader (const QGAMES::ResourcesGroupLoader::TypeLoaders& ldrs,
		const std::string& dF)
	: _loaders (ldrs),
	  _definitionFile (dF),
	  _basePath (QGAMES::onlyPath (dF))
{

	if (_basePath == std::string (__NULL_STRING__)) 
		_basePath = std::string (".");

	readFile (__RL (_definitionFile));
	// Because there can be include line, this method can be called recursevely...
	// At this point the definition file, has the file to read,
	// and the list of definitions is empty...
}

// ---
QGAMES::ResourcesGroupLoader::~ResourcesGroupLoader ()
{
	for (QGAMES::ResourcesGroupLoader::TypeLoaders::const_iterator i = _loaders.begin ();
			i != _loaders.end (); i++)
		delete ((*i).second);
	_loaders.clear ();
}

// ---
void QGAMES::ResourcesGroupLoader::addLoader (QGAMES::ResourcesGroupLoader::TypeLoader* ld)
{
	// It has to exist...
	assert (ld);

	if (_loaders.find (ld -> type ()) != _loaders.end ())
		return;
	_loaders [ld -> type ()] = ld;
}

// ---
void QGAMES::ResourcesGroupLoader::load (int rgrp)
{
	QGAMES::ResourcesGroupLoader::Definitions::const_iterator i = _definitions.find (rgrp);
	assert (i != _definitions.end ());
	QGAMES::ResourcesGroupLoader::ResourceGroupDefinition def = (*i).second;

	for (int i = 0; i < (int) def._references.size (); i++)
		load (def._references [i]); // Recursive...take care...
	for (int i = 0; i < (int) def._resources.size (); i++)
	{
		assert (existLoader (def._resources [i]._type)); // It has to exist before...
		loader (def._resources [i]._type) -> load (def._resources [i]._resources);
	}

	// That's all folks...
}

// ---
QGAMES::ResourcesGroupLoader::TypeLoader* QGAMES::ResourcesGroupLoader::loader (const std::string& tp)
{
	QGAMES::ResourcesGroupLoader::TypeLoaders::const_iterator i = _loaders.find (tp);
	assert (i != _loaders.end ());
	return ((*i).second);
}

// ---
void QGAMES::ResourcesGroupLoader::readFile (const std::string& dF)
{
	TiXmlDocument doc (dF.c_str ());
	int e = doc.LoadFile ();
	assert (e);

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the root element doesn't call "Resources" the code will work too!
	// This is done to speed up the loading...

	for (TiXmlElement* defElement = rootElement -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		// The line can be an include line...
		// That line can be at any place in the entity definition file...
		if (strcmp (defElement -> Value (), __QGAMES_INCLUDENAME__) == 0)
		{
			std::string nF = 
				defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__);
			readFile (__RL (nF));
		}
		else
		// The other type of line could be a simple resource group definition...
		if (strcmp (defElement -> Value (), __QGAMES_RESOURCEGROUPTAGNAME__) == 0)
		{
			QGAMES::ResourcesGroupLoader::ResourceGroupDefinition def = 
				readResourceGroupDefinition (defElement);
			assert (def._id != -1); // Is it a valid group?
			_definitions.insert (QGAMES::ResourcesGroupLoader::Definitions::value_type (def._id, def));
		}
	}
}

// ---
QGAMES::ResourcesGroupLoader::ResourceGroupDefinition 
	QGAMES::ResourcesGroupLoader::readResourceGroupDefinition (TiXmlElement* e)
{
	assert (e);

	QGAMES::ResourcesGroupLoader::ResourceGroupDefinition result;
	e -> Attribute (__QGAMES_RESOURCEGROUPATTRID__, &result._id);
	for (TiXmlElement* groupElement = e -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __QGAMES_RESOURCEGROUPTAGNAME__) == 0)
		{
			int ref;
			groupElement -> Attribute (__QGAMES_RESOURCEGROUPATTRID__, &ref);
			result._references.push_back (ref);
		}
		else
		{
			QGAMES::ResourcesGroupLoader::ResourceListDefinition rLst;
			rLst._type = groupElement -> Value ();
			std::string txt = groupElement -> GetText ();
			rLst._resources = QGAMES::getElementsFromAsInt (txt, ' '); // As many as possible...
			result._resources.push_back (rLst);
		}
	}

	return (result);
}

// ---
QGAMES::CommonLibResourceGroupLoader::CommonLibResourceGroupLoader (const std::string& dF)
	: QGAMES::ResourcesGroupLoader (QGAMES::ResourcesGroupLoader::TypeLoaders (), dF)
{
	addLoader (new QGAMES::ResourcesGroupLoader::StdTypeFormLoader ());
	addLoader (new QGAMES::ResourcesGroupLoader::StdTypeSoundLoader ());
	addLoader (new QGAMES::ResourcesGroupLoader::StdTypeMovementLoader ());
	addLoader (new QGAMES::ResourcesGroupLoader::StdTypeEntityLoader ());
	addLoader (new QGAMES::ResourcesGroupLoader::StdTypeObjectLoader ());
}
