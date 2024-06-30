#include <Common/maskbuilder.hpp>
#include <Common/definitions.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/game.hpp>
#include <tinyxml.h>

// ---
QGAMES::MaskBuilder::MaskBuilder (const std::string& eDef) 
	: _definitions (),
	  _masks (),
	  _definitionFile (__RL (eDef)),
	  _basePath (QGAMES::onlyPath (__RL (eDef)))
{
	if (_basePath == std::string (__NULL_STRING__)) 
		_basePath = std::string (".");

	readFile (__RL (_definitionFile));
	// Because there can be include line, this method can be called recursevely...
	// At this point the definition file, has the file to read,
	// and the list of definitions is empty...
}

// ---
QGAMES::MaskBuilder::~MaskBuilder ()
{
	QGAMES::Masks::const_iterator i;
	for (i = _masks.begin (); i != _masks.end (); i++)
		delete ((*i).second);
	_masks.clear ();
}

// ---
QGAMES::Mask* QGAMES::MaskBuilder::mask (int nm)
{
	QGAMES::Masks::const_iterator i = _masks.find (nm);
	if (i != _masks.end ())
		return ((*i).second);

	__WLOGPLUS (std::string ("Loading mask: ") + std::to_string (nm));

	QGAMES::Mask* result = NULL;
	QGAMES::MaskBuilder::Definitions::const_iterator j = _definitions.find (nm);
	assert (j != _definitions.end ());
	result = createMask ((*j).second);
	assert (result);

	// Adds the mask to the list of those...
	_masks.insert (QGAMES::Masks::value_type (nm, result));

	return (result);
}

// ---
QGAMES::Mask* QGAMES::MaskBuilder::createMask (const QGAMES::MaskBuilder::MaskDefinition& mDef)
{
	QGAMES::Mask* result = NULL;

	if (mDef._type == __QGAMES_MASKTYPEINCIRCLE__)
		result = new QGAMES::INMiddleCircleMask (mDef._id, mDef._blendingType,
			QGAMES::INMiddleCircleMask::Properties (mDef._parameters), mDef._width, mDef._height);
	else if (mDef._type == __QGAMES_MASKTYPEOUTCIRCLE__)
		result = new QGAMES::OUTMiddleCircleMask (mDef._id, mDef._blendingType,
			QGAMES::OUTMiddleCircleMask::Properties (mDef._parameters), mDef._width, mDef._height);
	else if (mDef._type == __QGAMES_MASKTYPELEFTZONE__)
		result = new QGAMES::LeftSideMask (mDef._id, mDef._blendingType,
			QGAMES::LeftSideMask::Properties (mDef._parameters), mDef._width, mDef._height);
	else if (mDef._type == __QGAMES_MASKTYPERIGHTZONE__)
		result = new QGAMES::RightSideMask (mDef._id, mDef._blendingType,
			QGAMES::RightSideMask::Properties (mDef._parameters), mDef._width, mDef._height);
	else if (mDef._type == __QGAMES_MASKTYPEUPZONE__)
		result = new QGAMES::UpSideMask (mDef._id, mDef._blendingType,
			QGAMES::UpSideMask::Properties (mDef._parameters), mDef._width, mDef._height);
	else if (mDef._type == __QGAMES_MASKTYPEDOWNZONE__)
		result = new QGAMES::DownSideMask (mDef._id, mDef._blendingType,
			QGAMES::DownSideMask::Properties (mDef._parameters), mDef._width, mDef._height);
	else if (mDef._type == __QGAMES_MASKTYPEINCIRCLEGEN__)
		result = new QGAMES::INCircleMask (mDef._id, mDef._blendingType,
			QGAMES::INCircleMask::Properties (mDef._parameters), mDef._width, mDef._height);
	else if (mDef._type == __QGAMES_MASKTYPEOUTCIRCLEGEN__)
		result = new QGAMES::OUTCircleMask (mDef._id, mDef._blendingType,
			QGAMES::OUTCircleMask::Properties (mDef._parameters), mDef._width, mDef._height);

	assert (result);

	return (result);
}

// ---
void QGAMES::MaskBuilder::readFile (const std::string& fDef)
{
	__WLOGPLUS (std::string ("Reading masks definition file: ") + fDef);

	TiXmlDocument doc (fDef.c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the root element doesn't call "Masks" the code will work too!
	// This is done to speed up the loading...

	for (TiXmlElement* defElement = rootElement -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		// The line can be an include line...
		// That line can be at any place in the mask definition file...
		if (strcmp (defElement -> Value (), __QGAMES_INCLUDENAME__) == 0)
		{
			assert (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)); 
			readFile (__RL (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)));
		}
		else
		// The other type of line could be a simple mask line...
		if (strcmp (defElement -> Value (), __QGAMES_MASKTAGNAME__) == 0)
		{
			QGAMES::MaskBuilder::MaskDefinition def = 
				readMaskDefinition (defElement);
			assert (def._id != -1 && def._type != -1); // Is it a valid mask?
			_definitions.insert (QGAMES::MaskBuilder::Definitions::value_type (def._id, def));
		}
	}
}

// ---
QGAMES::MaskBuilder::MaskDefinition QGAMES::MaskBuilder::readMaskDefinition (TiXmlElement* e)
{
	assert (e);

	QGAMES::MaskBuilder::MaskDefinition result;
	e -> Attribute (__QGAMES_MASKATTRID__, &result._id);
	e -> Attribute (__QGAMES_MASKATTRTYPE__, &result._type);
	e -> Attribute (__QGAMES_MASKATTRBLENDINGTYPE__, &result._blendingType);
	e -> Attribute (__QGAMES_MASKATTRWIDTH__, &result._width);
	e -> Attribute (__QGAMES_MASKATTRHEIGHT__, &result._height);
	// If any...
	for (TiXmlElement* subElement = e -> FirstChildElement ();
		subElement != NULL; subElement = subElement -> NextSiblingElement ())
	{
		if (strcmp (subElement -> Value (), __QGAMES_MASKPARAMETERTAGNAME__) == 0) // Is it a parameter?
		{
			std::string id, value;
			id = subElement -> Attribute (__QGAMES_MASKPARAMATTRID__);
			value = subElement -> Attribute (__QGAMES_MASKPARAMATTRVALUE__);
			assert (id != std::string (__NULL_STRING__)); // Is it a valid parameter definition?...
			result._parameters.insert (std::map <std::string, std::string>::value_type (id, value));
		}
	}

	return (result);
}
