#include <Common/conffile.hpp>
#include <tinyxml.h>

// ---
QGAMES::ConfigurationFile::ConfigurationFile (const std::string& fName)
	: _properties ()
{
	TiXmlDocument doc (fName.c_str ());
	int e = doc.LoadFile ();
	if (!e) // Has errors... 
		return;
	// The configuration file is not mandatory...

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the root element doesn't call "Properties" the code will work too!
	// This is done to speed up the loading...

	for (TiXmlElement* defElement = rootElement -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		// If the element doesn't call "Property" the code will work too!
		// This is done to speed up the loading...

		std::string attrId = defElement -> Attribute (__GAME_PROPERTYATTRID__);
		std::string attrValue = 
			defElement -> Attribute (__GAME_PROPERTYATTRVALUE__);
		if (attrId != std::string (__NULL_STRING__) &&
			attrValue != std::string (__NULL_STRING__))
			_properties.insert (std::map <std::string, std::string>::value_type (attrId, attrValue));
	}
}
