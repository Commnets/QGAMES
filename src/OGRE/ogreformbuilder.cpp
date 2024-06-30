#include <CommonOGRE/ogreformbuilder.hpp>
#include <CommonOGRE/ogreform.hpp>
#include <CommonOGRE/ogredefinitions.hpp>
#include <CommonOGRE/ogrescreen.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/game.hpp>
#include <OgreHeaderPrefix.h>
#include <Ogre.h>
#include <OgreHeaderSuffix.h>

// ---
QGAMES::Form* OGRE2DFormBuilder::createForm (QGAMES::FormBuilder::FormDefinition* def)
{
	Ogre::Image* image = new Ogre::Image ();
	image -> load (QGAMES::onlyFileName (def -> _fileDefinition), __OGREDEFAULTRESOURCESGROUP__);
	// Take care because the definition file has also to be included into the resource.cfg
	// Otherwise OGRE's standard resource loaders won't find it! 

	QGAMES::Sprite2DBuilder::SpriteDefinition* defS =
		dynamic_cast <QGAMES::Sprite2DBuilder::SpriteDefinition*> (def);
	assert (defS); // Just in case...

	QGAMES::Sprite2DFormData* data = createSprite2DFormDataFromSpriteDefinition (defS);
	
	return (new OGRE2DForm (def -> _id, def -> _name, def -> _numberFrames, def -> _fileDefinition,
		 defS -> _width, defS -> _height, data, image));
}

// ---
OGREFormBuilder::OGREFormBuilder (const std::string& fDef, const std::string& fDef2D)
	: QGAMES::FormBuilder (fDef),
	  _2DFormBuilder (NULL)
{ 
	readFile (__RL (_definitionFile)); 
	// It could be recursevly...using include directive...
	// At this point the definition file variable has been set and the definition list is clear!

	// If 2D forms are gonna be used, then it is neccesary a SDL 2D form loader...
	if (fDef2D != std::string (__NULL_STRING__))
		_2DFormBuilder = new OGRE2DFormBuilder (fDef2D);
}

// ---
OGREFormBuilder::~OGREFormBuilder ()
{
	delete (_2DFormBuilder);
	_2DFormBuilder = NULL;
}

// ---
void OGREFormBuilder::preLoad ()
{
	QGAMES::FormBuilder::preLoad ();
	if (_2DFormBuilder != NULL)
		_2DFormBuilder -> preLoad ();
}

// ---
QGAMES::Form* OGREFormBuilder::form (int fId)
{
	QGAMES::Form* result = NULL;

	// First looks for the form in the 3D definition...
	if (_definitions.find (fId) != _definitions.end ())
		result = QGAMES::FormBuilder::form (fId);
	// Then it exists, looks in the 2D space...
	else if (_2DFormBuilder != NULL)
		result = _2DFormBuilder -> form (fId);

	// The result can't be null...
	assert (result);
	return (result);
}

// ---
QGAMES::Form* OGREFormBuilder::createForm (QGAMES::FormBuilder::FormDefinition* def)
{
	Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton ().load (def -> _fileDefinition,
		Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME); // Autodetect the group...
	assert (mesh);
	return (new OGREForm (def -> _id, def -> _name, mesh));
}

// ---
void OGREFormBuilder::readFile (const std::string& fDef)
{
	TiXmlDocument doc (fDef.c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the main element doesn't call "Forms" the method will continue!

	for (TiXmlElement* defElement = rootElement -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		// The line can be an include line...
		// That line can be at any place in the form definition file...
		if (strcmp (defElement -> Value (), __QGAMES_INCLUDENAME__) == 0)
		{
			assert (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)); 
			readFile (__RL (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)));
		}
		else
		// The other type of line is a simple form line...
		if (strcmp (defElement -> Value (), __QGAMES_FORMTAGNAME__) == 0)
		{
			OGREFormBuilder::OGREMeshDefinition* def = 
				dynamic_cast <OGREFormBuilder::OGREMeshDefinition*> (readFormDefinition (defElement));
			assert (def && def -> _id != -1); // Is it a valid form?
			_definitions.insert (QGAMES::FormBuilder::Definitions::value_type (def -> _id, def));
		}
	}
}

// ---
OGREFormBuilder::OGREMeshDefinition* OGREFormBuilder::readFormDefinition (TiXmlElement* e)
{
	OGREFormBuilder::OGREMeshDefinition* result = 
		new OGREFormBuilder::OGREMeshDefinition ();
	e -> Attribute (__QGAMES_FORMATTRID__, &result -> _id);
	result -> _name = e -> Attribute (__QGAMES_FORMATTRNAME__);
	result -> _fileDefinition = e -> Attribute (__QGAMES_FORMATTRFILE__);
	result -> _numberFrames = 1; // Just one frame. No attributes to identify it...
	return (result);
}
