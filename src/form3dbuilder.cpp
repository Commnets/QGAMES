#include <Common/form3dbuilder.hpp>
#include <Common/form3d.hpp>
#include <Common/definitions.hpp>
#include <Common/genalgorithms.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/game.hpp>

// ---
QGAMES::Form3DBuilder::Form3DBuilder (const std::string& fDef, QGAMES::Sprite2DBuilder* sB)
	: QGAMES::FormBuilder (fDef),
	  _sprite2DBuilder (sB)
{
	assert (_sprite2DBuilder);

	readFile (__RL (_definitionFile)); 
	// It could be recursively...using include directive...
	// At this point the definition file variable has been set and the definition list is clear!
}

// ---
QGAMES::Form* QGAMES::Form3DBuilder::createForm (QGAMES::FormBuilder::FormDefinition* def)
{
	QGAMES::Form3DBuilder::Form3DDefinition* defF =
		dynamic_cast <QGAMES::Form3DBuilder::Form3DDefinition*> (def);
	assert (defF);

	return (new QGAMES::Form3D 
		(defF -> _id, defF -> _name, defF -> _numberFrames, createForm3DDdataFromForm3DDefinition (defF)));
}

// ---
void QGAMES::Form3DBuilder::readFile (const std::string& fDef)
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
			QGAMES::Form3DBuilder::Form3DDefinition* def = readFormDefinition (defElement);
			_definitions.insert (QGAMES::FormBuilder::Definitions::value_type (def -> _id, def));
		}
		else
			assert (0); // Bad defined...
	}
}

// ---
QGAMES::Form3DBuilder::Form3DDefinition* QGAMES::Form3DBuilder::readFormDefinition (TiXmlElement* e)
{
	assert (e);

	assert (e -> Attribute (__QGAMES_FORMATTRID__) && 
			e -> Attribute (__QGAMES_FORMATTRNAME__) && 
			e -> Attribute (__QGAMES_FORMATTRFILE__) &&
			e -> Attribute (__QGAMES_FORMATTRNUMBERFRAMES__));

	QGAMES::Form3DBuilder::Form3DDefinition* result = new QGAMES::Form3DBuilder::Form3DDefinition;

	e -> Attribute (__QGAMES_FORMATTRID__, &result -> _id);
	result -> _name = e -> Attribute (__QGAMES_FORMATTRNAME__);
	result -> _fileDefinition = _basePath + std::string (__PATH_SEPARATOR__) + e -> Attribute (__QGAMES_FORMATTRFILE__);
	e -> Attribute (__QGAMES_FORMATTRNUMBERFRAMES__, &result -> _numberFrames);

	// Right conditions?
	assert (result -> _id != -1 &&
			result -> _name != std::string (__NULL_STRING__) &&
			result -> _fileDefinition != std::string (__NULL_STRING__) &&
			result -> _numberFrames > 0);

	// Articulations and frames info read...
	for (TiXmlElement* sE = e -> FirstChildElement ();
		sE != NULL; sE = sE -> NextSiblingElement ())
	{
		if (strcmp (sE -> Value (), __QGAMES_FORM3DARTICULATIONTAGNAME__) == 0) // The definition of an articulation...
		{
			QGAMES::Form3DBuilder::Form3DDefinition::ArticulationDefinition aD = readArticulationDefinition (sE);
			result -> _articulations [aD._id] = aD;
		}
		else
		if (strcmp (sE -> Value (), __QGAMES_FORM3DFRAMETAGNAME__) == 0) // The definition of a frame...
			result -> _frames.push_back (readFrameDefinition (sE));
		else
			assert (0); // Not well defined...
	}

	// Minimum one frame and one articulation...
	assert (!result -> _articulations.empty () && !result -> _frames.empty ());
	// The number of frames read has to fit the ones defined at the beginning
	assert ((int) result -> _frames.size () == result -> _numberFrames);

	// At this point it doesn't mean the information has coherence!
	// It could mean e.g a frame information referes to an articulation that doesn't exist at all...

	// Returns the final definition read...
	return (result);
}

// ---
QGAMES::Form3DBuilder::Form3DDefinition::ArticulationDefinition 
	QGAMES::Form3DBuilder::readArticulationDefinition (TiXmlElement* e)
{
	assert (e);

	assert (e -> Attribute (__QGAMES_FORM3DARTIDATTRNAME__) &&
			e -> Attribute (__QGAMES_FORM3DARTPOSATTRNAME__) &&
			e -> Attribute (__QGAMES_FORM3DARTORATTRNAME__));

	QGAMES::Form3DBuilder::Form3DDefinition::ArticulationDefinition result;

	e -> Attribute (__QGAMES_FORM3DARTIDATTRNAME__, &result._id);
	result._position = QGAMES::Position (std::string (e -> Attribute (__QGAMES_FORM3DARTPOSATTRNAME__)));
	result._orientation = QGAMES::Position (std::string (e -> Attribute (__QGAMES_FORM3DARTORATTRNAME__)));

	// Riht conditions?
	assert (result._id != -1 &&
			result._position != QGAMES::Position::_noPoint &&
			result._orientation != QGAMES::Vector::_noPoint);

	// Articulations nested and member info read...
	for (TiXmlElement* sE = e -> FirstChildElement ();
		sE != NULL; sE = sE -> NextSiblingElement ())
	{
		if (strcmp (sE -> Value (), __QGAMES_FORM3DARTICULATIONTAGNAME__) == 0) // Nested articulation
		{
			QGAMES::Form3DBuilder::Form3DDefinition::ArticulationDefinition aD = readArticulationDefinition (sE); // Recursive...
			result._articulations [aD._id] = aD;
		}
		else
		if (strcmp (sE -> Value (), __QGAMES_FORM3DMEMBERTAGNAME__) == 0) // Member info
			result._members.push_back (readMemberDefinition (sE));
		else
			assert (0); // Not well defined
	}

	// Minimum one member...
	assert (!result._members.empty ());

	return (result);
}

// ---
QGAMES::Form3DBuilder::Form3DDefinition::MemberDefinition 
	QGAMES::Form3DBuilder::readMemberDefinition (TiXmlElement* e)
{
	assert (e);

	assert (e -> Attribute (__QGAMES_FORM3DMEMBERIDATTRNAME__) && 
			e -> Attribute (__QGAMES_FORM3DMEMBERPOSATTRNAME__));

	QGAMES::Form3DBuilder::Form3DDefinition::MemberDefinition result;

	e -> Attribute (__QGAMES_FORM3DMEMBERIDATTRNAME__, &result._id);
	result._relativePosition = QGAMES::Position (std::string (e -> Attribute (__QGAMES_FORM3DMEMBERPOSATTRNAME__)));

	// Right conditions?
	assert (result._id != -1 &&
			result._relativePosition != QGAMES::Position::_noPoint);

	// Faces are read...
	for (TiXmlElement* sE = e -> FirstChildElement ();
		sE != NULL; sE = sE -> NextSiblingElement ())
	{
		if (strcmp (sE -> Value (), __QGAMES_FORM3DFACETAGNAME__) == 0) // Face
			result._faces.push_back (readFaceDefinition (sE));
		else
			assert (0); // Not well defined
	}

	// Minimum one face per member...
	assert (!result._faces.empty ());
	
	return (result);
}

// ---
QGAMES::Form3DBuilder::Form3DDefinition::FaceDefinition 
	QGAMES::Form3DBuilder::readFaceDefinition (TiXmlElement* e)
{
	assert (e);

	assert (e -> Attribute (__QGAMES_FORM3DFACEIDATTRNAME__) && 
			e -> Attribute (__QGAMES_FORM3DFACECOLORATTRNAME__));

	QGAMES::Form3DBuilder::Form3DDefinition::FaceDefinition result;

	e -> Attribute (__QGAMES_FORM3DFACEIDATTRNAME__, &result._id);
	result._color = QGAMES::Color (std::string (e -> Attribute (__QGAMES_FORM3DFACECOLORATTRNAME__)));

	// Right conditions?
	assert (result._id != -1);

	// Faces are read...
	for (TiXmlElement* sE = e -> FirstChildElement ();
		sE != NULL; sE = sE -> NextSiblingElement ())
	{
		if (strcmp (sE -> Value (), __QGAMES_FORM3DPOSITIONTAGNAME__) == 0) // Face
			result._relativePositions.push_back 
				(QGAMES::Position (std::string (e -> Attribute (__QGAMES_FORM3DPOSVALUEATTRNAME__))));
		else
			assert (0); // Not well defined
	}

	// The number of positions defined for the face have to be three the minimum...
	assert ((int) result._relativePositions.size () >= 3);
	
	return (result);
}

// ---
QGAMES::Form3DBuilder::Form3DDefinition::FrameDefinition QGAMES::Form3DBuilder::readFrameDefinition (TiXmlElement* e)
{
	assert (e);

	assert (e -> Attribute (__QGAMES_FORM3DFRAMEIDATTRNAME__));

	QGAMES::Form3DBuilder::Form3DDefinition::FrameDefinition result;

	e -> Attribute (__QGAMES_FORM3DFRAMEIDATTRNAME__, &result._id);

	// Right conditions?
	assert (result._id != -1);

	// Turns are read...
	for (TiXmlElement* sE = e -> FirstChildElement ();
		sE != NULL; sE = sE -> NextSiblingElement ())
	{
		if (strcmp (sE -> Value (), __QGAMES_FORM3DTURNTAGNAME__) == 0) // Turn
			result._turns.push_back (readArticulationTurnDefinition (e));
		else
			assert (0); // Not well defined...
	}

	// The number of turns can be 0 for a frame
	// It makes sense only when there is only one frame!

	return (result);
}

//
QGAMES::Form3DBuilder::Form3DDefinition::ArticulationTurnDefinition 
	QGAMES::Form3DBuilder::readArticulationTurnDefinition (TiXmlElement* e)
{
	assert (e != NULL);

	QGAMES::Form3DBuilder::Form3DDefinition::ArticulationTurnDefinition result;

	result._pathId = std::string (e -> Attribute (__QGAMES_FORM3DTURNIDATTRNAME__));
	double ang; e -> Attribute (__QGAMES_FORM3DTURNANGLEATTRNAME__, &ang); result._angle = __BD ang;

	// Right deefinition?
	assert (result._pathId != std::string (__NULL_STRING__));
	// The angle can be 0, if the attribute defined in the file wasn't right!

	return (result);
}

// ---
QGAMES::Form3DData* QGAMES::Form3DBuilder::createForm3DDdataFromForm3DDefinition 
	(QGAMES::Form3DBuilder::Form3DDefinition* d)
{
	assert (d);

	// Lambda to create a face
	auto createFace = [](const QGAMES::Form3DBuilder::Form3DDefinition::FaceDefinition& fD) 
			-> QGAMES::Form3DData::Face
		{ return (QGAMES::Form3DData::Face (fD._id, fD._relativePositions, fD ._color)); };

	// lambda to create a mamber using the previous one
	auto createMamber = [&createFace](const QGAMES::Form3DBuilder::Form3DDefinition::MemberDefinition& mD) 
			-> QGAMES::Form3DData::Member
		{ QGAMES::Form3DData::Faces fC;
		  for (auto i : mD._faces)
			  fC.push_back (createFace (i));
		  return (QGAMES::Form3DData::Member (mD._id, fC, mD._relativePosition)); };

	// Lambda (recursive) to create an articulation, using the two ones defined before
	const std::function <QGAMES::Form3DData::Articulation 
			(const QGAMES::Form3DBuilder::Form3DDefinition::ArticulationDefinition&)> // To allow recursion...
		createArticulation = [&createArticulation, &createMamber]
				(const QGAMES::Form3DBuilder::Form3DDefinition::ArticulationDefinition& aD) -> QGAMES::Form3DData::Articulation
		{ QGAMES::Form3DData::Articulations arts;
		  for (auto i : aD._articulations)
			{ QGAMES::Form3DData::Articulation sArt = createArticulation (i.second); 
			  arts.insert (QGAMES::Form3DData::Articulations::value_type (sArt.id (),sArt)); }
		  QGAMES::Form3DData::Members mbs;
		  for (auto i : aD._members)
			  mbs.push_back (createMamber (i));
		  return (QGAMES::Form3DData::Articulation (aD._id, aD._position, aD._orientation, arts, mbs)); };

	// To create the articulations
	QGAMES::Form3DData::Articulations arts;
	for (auto i : d -> _articulations)
		{ QGAMES::Form3DData::Articulation art = createArticulation (i.second); 
		  arts.insert (QGAMES::Form3DData::Articulations::value_type (art.id (), art)); }

	// Lambda to create the information to turn an articulation
	auto createArticulationTurnInfo = [](const QGAMES::Form3DBuilder::Form3DDefinition::ArticulationTurnDefinition& tD)
			-> QGAMES::Form3DData::ArticulationTurnInfo
		{ return (QGAMES::Form3DData::ArticulationTurnInfo (tD._pathId, tD._angle)); };

	// Lambda to create a list of turns for articulations
	auto createArticulationsTurnInfo = 
			[&createArticulationTurnInfo](const QGAMES::Form3DBuilder::Form3DDefinition::ArticulationTurnDefinitions& tD)
				-> QGAMES::Form3DData::ArticulationTurnInfoList
		{ QGAMES::Form3DData::ArticulationTurnInfoList r;
		  for (auto i : tD) r.push_back (createArticulationTurnInfo (i)); 
		  return (r); };

	// Lamda to create the frame info
	auto createFrameInfo = 
			[&createArticulationsTurnInfo] (const QGAMES::Form3DBuilder::Form3DDefinition::FrameDefinition& fD)
				-> QGAMES::Form3DData::FrameInfo
		{ return (QGAMES::Form3DData::FrameInfo (fD._id, createArticulationsTurnInfo (fD._turns))); };

	// To create the frame info.
	QGAMES::Form3DData::FramesInfo fInfo;
	for (auto i : d -> _frames)
		fInfo.push_back (createFrameInfo (i));

	// ...and now to create the form data
	// What a beatiful code using lambdas, even one of them recursive!
	return (new QGAMES::Form3DData (arts, fInfo));
}
