#include <Common/commsbuilder.hpp>
#include <Common/game.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/game.hpp>
#include <Common/definitions.hpp>
#include <tinyxml.h>

// ---
bool QGAMES::CommunicationChannelBuilder::StandardAddsOn::canCreateType (const std::string& t)
{
	return (t == std::string (__QGAMES_COMMSCHANNELNOTHING__) || 
			t == std::string (__QGAMES_COMMSCHANNELCONSOLELOG__) ||
			t == std::string (__QGAMES_COMMSCHANNELFILELOG__));
}

// ---
QGAMES::CommunicationChannel* QGAMES::CommunicationChannelBuilder::StandardAddsOn::createChannelType 
	(const std::string& t, const std::map <std::string, std::string>& p)
{
	QGAMES::CommunicationChannel* result = NULL;

	if (t == std::string (__QGAMES_COMMSCHANNELNOTHING__))
		result = new QGAMES::CommunicationChannel (new QGAMES::NothingChannelImplementation);
	else if (t == std::string (__QGAMES_COMMSCHANNELCONSOLELOG__))
		result = new QGAMES::CommunicationChannel (new ConsoleLogChannelImplementation);
	else if (t == std::string (__QGAMES_COMMSCHANNELFILELOG__))
	{
		std::string fN (__QGAMES_COMMSCHANNELDEFFILELOGNAME__); // Default file log name...
		const std::map <std::string, std::string>::const_iterator i = 
			p.find (std::string (__QGAMES_COMMSCHANNELDEFFILELOGNAME__)); // Unless other has been defined...
		if (i != p.end ()) fN = (*i).second;
		result = new QGAMES::CommunicationChannel (new FileLogChannelImplementation (fN));
	}
	else
		assert (0); // Not allowed...

	return (result);
}

// ---
QGAMES::CommunicationChannelBuilder::CommunicationChannelBuilder (const std::string& fDef)
	: _definitionFile (fDef),
	  _basePath (QGAMES::onlyPath (fDef))
{
	// The basic and the standard addon is added...
	addAddsOn (new QGAMES::CommunicationChannelBuilder::StandardAddsOn); 

	if (_basePath == std::string (__NULL_STRING__)) 
		_basePath = std::string (".");

	readFile (__RL (_definitionFile));
	// Because there can be "include" lines, this method can be called recursevely...
	// At this point the definitionFile variable is storing the name of file to read,
	// and the list of definitions is empty...
}

// ---
QGAMES::CommunicationChannelBuilder::~CommunicationChannelBuilder ()
{
	for (int i = 0; i < (int) _addsOn.size (); delete (_addsOn [i++]));
	_addsOn.clear ();
}

// ---
void QGAMES::CommunicationChannelBuilder::addAddsOn (QGAMES::CommunicationChannelBuilder::AddsOn* aO)
{
	assert (aO); // Can't be null...

	_addsOn.push_back (aO);
}

// ---
QGAMES::CommunicationChannel* QGAMES::CommunicationChannelBuilder::createCommChannel 
	(const std::string& t, const std::map <std::string, std::string>& p)
{
	QGAMES::CommunicationChannel* result = NULL;

	// Find the add on able to create the type requested...
	for (int i = 0; i < (int) _addsOn.size () && !result; i++)
		if (_addsOn [i] -> canCreateType (t))
			result = _addsOn [i] -> createChannelType (t, p); // ...and invoke it if it is found...

	assert (result); // The result can't be null, otherwise something will crash later...

	return (result);
}

// ---
void QGAMES::CommunicationChannelBuilder::readFile (const std::string& fDef)
{
	// It is not recommeneded to log anything
	// Only because this is the class in charge of loading the log system
	// and invoking here the log could arise either an error or a recursive loop...

	TiXmlDocument doc (fDef.c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the root element doesn't call "Communications" the code will work too!
	// This is done to speed up the loading...

	for (TiXmlElement* defElement = rootElement -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		// The line can be an include line...
		// That line can be at any place in the entity definition file...
		if (strcmp (defElement -> Value (), __QGAMES_INCLUDENAME__) == 0)
		{
			assert (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)); 
			readFile (__RL (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)));
		}
		else
		// The other type of line could be a simple communcations line...
		if (strcmp (defElement -> Value (), __QGAMES_COMMSTAGNAME__) == 0)
		{
			QGAMES::CommunicationChannelBuilder::CommsChannelDefinition def = readCommsDefinition (defElement);
			assert (def._id != -1); // Is it a valid entity?
			_definitions.insert (QGAMES::CommunicationChannelBuilder::Definitions::value_type (def._id, def));
		}
	}
}

// ---
QGAMES::CommunicationChannelBuilder::CommsChannelDefinition 
	QGAMES::CommunicationChannelBuilder::readCommsDefinition (TiXmlElement* elmnt)
{
	assert (elmnt);

	QGAMES::CommunicationChannelBuilder::CommsChannelDefinition result;
	elmnt -> Attribute (__QGAMES_COMMSATTRID__, &result._id);
	std::string tAttr (__NULL_STRING__); // Take care with the type...
	tAttr = elmnt -> Attribute (__QGAMES_COMMSATTRTYPE__);

	for (TiXmlElement* groupElement = elmnt -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __QGAMES_COMMSATTRSBLOCK__) == 0)
		result._parameters = readAttributesGroup (groupElement);
	}

	return (result);
}

// ---
std::map <std::string, std::string> QGAMES::CommunicationChannelBuilder::readAttributesGroup (TiXmlElement* g)
{
	assert (g);

	std::map <std::string, std::string> result;
	for (TiXmlElement* aElement = g -> FirstChildElement ();
		aElement != NULL; aElement = aElement -> NextSiblingElement ())
	{
		// If the element doesn't call "Attribute" the code will work too!
		// This is done to speed up the loading...
		
		std::string id, value;
		id = aElement -> Attribute (__QGAMES_COMMSATTRATTRID__);
		value = aElement -> Attribute (__QGAMES_COMMSATTRATTRVALUE__);
		assert (id != std::string (__NULL_STRING__)); // Is it a valid variable definition?...
		result.insert (std::map <std::string, std::string>::value_type (id, value));
	}

	return (result);
}
