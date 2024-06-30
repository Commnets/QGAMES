#include <Common/sprite2dbuilder.hpp>
#include <Common/definitions.hpp>
#include <Common/genalgorithms.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/game.hpp>

// ---
QGAMES::Sprite2DBuilder::Sprite2DBuilder (const std::string& fDef, QGAMES::Sprite2DFrameDataFactory* fF)
	: QGAMES::FormBuilder (fDef),
	  _frameDataFactory (fF)
{
	assert (_frameDataFactory != NULL);

	readFile (__RL (_definitionFile)); 
	// It could be recursevly...using include directive...
	// At this point the definition file variable has been set and the definition list is clear!
}

// ---
QGAMES::Form* QGAMES::Sprite2DBuilder::formDefined 
	(int id, const std::string& n, 
	 int nf, const std::string& s, int w, int h, int sFW, int sFH, int spc, int mrg, int aC, int pT, int aT, 
	 const QGAMES::ListOfSprite2DFrameData& fD, const std::map <std::string, std::string>& prps)
{
	QGAMES::Forms::const_iterator i = _forms.find (id);
	if (i != _forms.end ())
		return ((*i).second);

	QGAMES::Sprite2DBuilder::SpriteDefinition* def = 
		new QGAMES::Sprite2DBuilder::SpriteDefinition ();
	
	def -> _id = id;
	def -> _name = n;
	def -> _numberFrames = nf;
	def -> _fileDefinition = s;
	def -> _width = w;
	def -> _height = h;
	def -> _standardFrameWidth = sFW;
	def -> _standardFrameHeight = sFH;
	def -> _spacing = spc;
	def -> _margin = mrg;
	def -> _alphaColor= aC;
	def -> _projectionType = pT; 
	def -> _adjustType = aT;
	def -> _properties = prps;

	if (!fD.empty ())
	{
		assert ((int) fD.size () == def -> _numberFrames); // They must both match...

		def -> _frameDefinitions.resize (def -> _numberFrames);
		for (int i = 0; i < def -> _numberFrames; i++)
			def -> _frameDefinitions [i] = QGAMES::Sprite2DBuilder::SpriteDefinition::FrameDefinition
				(i, fD [i] -> width (), fD [i] -> height (), fD [i] -> xPosition (), fD [i] -> yPosition (), 
				 fD [i] -> definitionWidth (), fD [i] -> definitionHeight (), fD [i] -> xOffset (), fD [i] -> yOffset ());
	}

	_definitions [def -> _id] = def;

	return (form (def -> _id));
}

// ---
void QGAMES::Sprite2DBuilder::readFile (const std::string& fDef)
{
	__WLOGPLUS (std::string ("Reading forms definition file: ") + fDef);

	TiXmlDocument doc (fDef.c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the root element doesn't call "Forms" the code will work too!
	// This is done to speed up the loading...

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
			QGAMES::FormBuilder::FormDefinition* def = 
				readFormDefinition (defElement);
			assert (def && (def -> _id != -1)); // Is it a valid form?
			_definitions.insert (QGAMES::FormBuilder::Definitions::value_type (def -> _id, def));
		}
	}
}

// ---
QGAMES::Sprite2DBuilder::SpriteDefinition* QGAMES::Sprite2DBuilder::readFormDefinition (TiXmlElement* e)
{
	assert (e);

	QGAMES::Sprite2DBuilder::SpriteDefinition* result = new QGAMES::Sprite2DBuilder::SpriteDefinition ();

	e -> Attribute (__QGAMES_FORMATTRID__, &result -> _id);
	result -> _name = e -> Attribute (__QGAMES_FORMATTRNAME__);
	result -> _fileDefinition = _basePath + std::string (__PATH_SEPARATOR__) + 
		e -> Attribute (__QGAMES_FORMATTRFILE__);
	e -> Attribute (__QGAMES_FORMATTRWIDTH__, &result -> _width);
	e -> Attribute (__QGAMES_FORMATTRHEIGHT__, &result -> _height);
	e -> Attribute (__QGAMES_FORMATTRFRAMESPACING__, &result -> _spacing);
	e -> Attribute (__QGAMES_FORMATTRFRAMEMARGIN__, &result -> _margin);
	e -> Attribute (__QGAMES_FORMATTRFRAMEWIDTH__, &result -> _standardFrameWidth);
	e -> Attribute (__QGAMES_FORMATTRFRAMEHEIGHT__, &result -> _standardFrameHeight);
	e -> Attribute (__QGAMES_FORMATTRALPHACOLOR__, &result -> _alphaColor);
	e -> Attribute (__QGAMES_FORMATATTRPRJTYPE__, &result -> _projectionType); // If not defined = 0 (orthogonal)
	e -> Attribute (__QGAMES_FORMATATTRADJUSTTYPE__, &result -> _adjustType); // If not defined = 1 (usual)
	e -> Attribute (__QGAMES_FORMATTRNUMBERFRAMES__, &result -> _numberFrames);

	// Reads potential special widths, and heights...
	// Those one would be children of the main node already read...
	for (TiXmlElement* sE = e -> FirstChildElement ();
		sE != NULL; sE = sE -> NextSiblingElement ())
	{
		// If the element doesn't call "Frame" the code will work too!
		// This is done to speed up the loading...
		int nF = -1; 
		int sW = -1; int sH = -1;
		int sPX = -1; int sPY = -1;
		int sDW = -1; int sDH = -1;
		int sOX = -1; int sOY = -1;
		sE -> Attribute (__QGAMES_FORMATTRSPECIALFRAMEID__, &nF);
		sE -> Attribute (__QGAMES_FORMATTRSPECIALFRAMEWIDTH__, &sW); // width, used for graphical propouses
		sE -> Attribute (__QGAMES_FORMATTRSPECIALFRAMEHEIGHT__, &sH); // height, used for graphical propouses
		sE -> Attribute (__QGAMES_FORMATTRSPECIALFRAMEPOSX__, &sPX);
		sE -> Attribute (__QGAMES_FORMATTRSPECIALFRAMEPOSY__, &sPY);
		sE -> Attribute (__QGAMES_FORMATTRSPECIALFRAMEDFWIDTH__, &sDW); // fWidth, used for paint purpouses
		sE -> Attribute (__QGAMES_FORMATTRSPECIALFRAMEDFHEIGHT__, &sDH); // fHeight, used for painting purpouses
		sE -> Attribute (__QGAMES_FORMATTRSPECIALFRAMEXOFF__, &sOX);
		sE -> Attribute (__QGAMES_FORMATTRSPECIALFRAMEYOFF__, &sOY);

		assert (nF != -1); // At least the number of the frame has to be defined...
		if (sPX == -1 || sPY == -1) // It is not a mistake, but the frames generated couldn't be as expected...
			__WLOGPLUS (std::string ("Form: ") + std::to_string (result -> _id) + 
				std::string (". No frame: ") + std::to_string (nF) + std::string (" positions not defined"));
		if (sOX == -1 || sOY == -1) // It is not a mistake either, but the frames generated couldn't be as expected...
			__WLOGPLUS (std::string ("Form: ") + std::to_string (result -> _id) + 
				std::string (". No frame: ") + std::to_string (nF) + std::string (" offsets not defined or negative"));

		result -> _frameDefinitions.push_back (QGAMES::Sprite2DBuilder::SpriteDefinition::FrameDefinition 
			(nF, sW, sH, sPX, sPY, sDW, sDH, sOX, sOY)); // Insert a new frame definition...
	}

	// The list of frames has to be empty or if it is not the number of elements has to match the size...
	assert (result -> _frameDefinitions.empty () ||
			(!result -> _frameDefinitions.empty () && result -> _numberFrames == (int) result -> _frameDefinitions.size ()));

	// Returns the final definition read...
	return (result);
}

// ---
QGAMES::Sprite2DFormData* QGAMES::Sprite2DBuilder::createSprite2DFormDataFromSpriteDefinition 
	(QGAMES::Sprite2DBuilder::SpriteDefinition* defS)
{
	assert (defS); 

	QGAMES::Sprite2DFormData* data = NULL;
	if (defS -> _frameDefinitions.empty ()) // No specific information for the frames...
		data = new QGAMES::Sprite2DFormData 
			(defS -> _numberFrames, defS -> _projectionType, defS -> _spacing, defS -> _margin, 
			 defS -> _width, defS -> _height, defS -> _standardFrameWidth, defS -> _standardFrameHeight, 
			 defS -> _alphaColor, defS -> _adjustType,
			 frameDataFactory ()); 
	else
	{
		QGAMES::ListOfSprite2DFrameData frameData;

		// Standard number of elements per row... (just if needed)
		int wS = (defS -> _width - (defS -> _margin << 1) + defS -> _spacing) / 
			(defS -> _standardFrameWidth + defS -> _spacing);
		if (wS == 0) // It can be 0 if it is going to be used...
			__WLOGPLUS (std::string ("Form: ") + std::to_string (defS -> _id) + 
				std::string (" standard frame size not well defined if needed"));

		frameData.resize (defS -> _numberFrames);
		for (int i = 0; i < defS -> _numberFrames; i++)
		{
			int sPosX = (wS == 0) 
				? 0
				: (i % wS) * (defS -> _standardFrameWidth + defS -> _spacing) + defS -> _margin; 
			int sPosY = (wS == 0)
				? 0
				: (i / wS) * (defS -> _standardFrameHeight + defS -> _spacing) + defS -> _margin; 

			frameData [i] = frameDataFactory () -> createFrameDataForProjection // Taking into account the type of projection...
				(defS -> _projectionType,
				 defS -> _frameDefinitions [i]._width == -1 
					? defS -> _standardFrameWidth : defS -> _frameDefinitions [i]._width, 
				 defS -> _frameDefinitions [i]._height == -1 
					? defS -> _standardFrameHeight : defS -> _frameDefinitions [i]._height, 
				 defS -> _frameDefinitions [i]._xPosition == -1 
					? sPosX : defS -> _frameDefinitions [i]._xPosition, 
				 defS -> _frameDefinitions [i]._xPosition == -1 
					? sPosY : defS -> _frameDefinitions [i]._yPosition, 
				 defS -> _frameDefinitions [i]._definitionWidth == -1 
					? defS -> _standardFrameWidth : defS -> _frameDefinitions [i]._definitionWidth, 
				 defS -> _frameDefinitions [i]._definitionHeight == -1 
					? defS -> _standardFrameHeight : defS -> _frameDefinitions [i]._definitionHeight, 
				 defS -> _frameDefinitions [i]._xOffset == -1 
					? 0 : defS -> _frameDefinitions [i]._xOffset, 
				 defS -> _frameDefinitions [i]._yOffset == -1 
					? 0 : defS -> _frameDefinitions [i]._yOffset, 
				 defS -> _adjustType);
		}

		data = new QGAMES::Sprite2DFormData 
			(defS -> _numberFrames, defS -> _projectionType, defS -> _spacing, defS -> _margin, 
			 defS -> _width, defS -> _height, defS -> _standardFrameWidth, defS -> _standardFrameHeight, 
			 defS -> _alphaColor, defS -> _adjustType,
			 frameData);
	}

	return (data);
}
