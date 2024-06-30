#include <Common/characterstepsbuilder.hpp>
#include <Common/definitions.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/genalgorithms.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/game.hpp>
#include <tinyxml.h>

// ---
QGAMES::CharacterControlStepsMonitorBuilder::CharacterControlStepsMonitorBuilder (const std::string& fDef)
	: _definitions (),
	  _definitionFile (fDef),
	  _basePath (QGAMES::onlyPath (fDef))
{
	if (_basePath == std::string (__NULL_STRING__)) 
		_basePath = std::string (".");

	readFile (__RL (_definitionFile));
}

QGAMES::CharacterControlStepsMonitor* QGAMES::CharacterControlStepsMonitorBuilder::monitorFor 
	(int id, QGAMES::Character* chr)
{
	__WLOGPLUS (std::string ("Loading steps monitor: ") + std::to_string (id));

	QGAMES::CharacterControlStepsMonitorBuilder::Definitions::const_iterator i = _definitions.find (id);
	assert (i != _definitions.end ());

	QGAMES::CharacterControlSteps steps;
	QGAMES::CharacterControlStepsMonitorBuilder::MonitorDefinition mDef = (*i).second;
	for (int j = 0; j < (int) mDef._steps.size (); j++)
		steps.push_back (createStep (mDef._steps [j]));
	return (createMonitor ((*i).second, steps, chr));
}

// ---
QGAMES::CharacterControlStepsMonitor* QGAMES::CharacterControlStepsMonitorBuilder::createMonitor 
	(const QGAMES::CharacterControlStepsMonitorBuilder::MonitorDefinition& def, 
	 const QGAMES::CharacterControlSteps& stps, Character* chr)
{
	return (new QGAMES::CharacterControlStepsMonitor (def._id, stps, chr, def._cyclic));
}

// ---
QGAMES::CharacterControlStep* QGAMES::CharacterControlStepsMonitorBuilder::createStep 
	(const QGAMES::CharacterControlStepsMonitorBuilder::StepDefinition& def)
{
	QGAMES::CharacterControlStep* result = NULL;

	std::map <std::string, std::string>::const_iterator i;
	switch (def._type)
	{
		case __QGAMES_CHRSTEPNONE__:
			return (new CharacterControlStepNone);
			break;

		case __QGAMES_CHRSTEPTIME__:
		case __QGAMES_CHRSTEPRANDOMTIME__:
			{
				QGAMES::bdata nS = 
					((i = def._values.find (__QGAMES_CHRSTEPTIMEATTRTIME__)) != def._values.end ()) 
						? __BD std::atof ((*i).second.c_str ()) : __QGAMES_CHRSTEPTIMEDEFAULTTIME__;
				if (def._type == __QGAMES_CHRSTEPTIME__)
					result = new QGAMES::CharacterControlTimeStep (nS);
				else
					result = new QGAMES::CharacterControlRandomTimeStep ((int) nS);
			}

			break;

		case __QGAMES_CHRSTEPWAITFORSTATE__:
			{
				int sT = 
					((i = def._values.find (__QGAMES_CHRSTEPWAITFORSTATEATTRSTATE__)) != def._values.end ()) 
						? std::atoi ((*i).second.c_str ()) : __QGAMES_CHRSTEPWAITFORSTATEDEFSTATE__;
				result = new QGAMES::CharacterControlWaitForStateStep (sT);
			}

			break;

		case __QGAMES_CHRSTEPCHANGESTATE__:
			{
				int sT = 
					((i = def._values.find (__QGAMES_CHRSTEPCHANGESTATEATTRSTATE__)) != def._values.end ()) 
						? std::atoi ((*i).second.c_str ())	: __QGAMES_CHRSTEPCHANGESTATEDEFSTATE__;
				result = new QGAMES::CharacterControlChangeStateStep (sT);
			}

			break;

		case __QGAMES_CHRSTEPJUSTMOVESTATE__:
		case __QGAMES_CHRSTEPMOVESTATE__:
			{
				std::string dr = 
					((i = def._values.find (__QGAMES_CHRSTEPMOVSTATEATTRDIR__)) != def._values.end ()) 
						? (*i).second.c_str ()	: __QGAMES_CHRSTEPMOVSTATEDEFDIR__;
				std::vector <QGAMES::bdata> drB = QGAMES::getElementsFromAsBDATA (dr, ',', 3);
				std::string accl = 
					((i = def._values.find (__QGAMES_CHRSTEPMOVSTATEATTRACCL__)) != def._values.end ()) 
						? (*i).second.c_str ()	: __QGAMES_CHRSTEPMOVDEFACCL__;
				std::vector <QGAMES::bdata> acclB = QGAMES::getElementsFromAsBDATA (accl, ',', 3);
				if (def._type == __QGAMES_CHRSTEPJUSTMOVESTATE__)
					result = new QGAMES::CharacterControlJustMoveStep 
						(QGAMES::Vector (drB [0], drB [1], drB [2]), QGAMES::Vector (acclB [0], acclB [1], acclB [2]));
				else
					result = new QGAMES::CharacterControlMoveStep 
						(QGAMES::Vector (drB [0], drB [1], drB [2]), QGAMES::Vector (acclB [0], acclB [1], acclB [2]));
			}

			break;

		case __QGAMES_CHRSTEPDISTANCE__:
			{
				QGAMES::bdata dT = 
					((i = def._values.find (__QGAMES_CHRSTEPDISTANCEATTRDISTANCE__)) != def._values.end ()) 
						? __BD std::atof ((*i).second.c_str ()) : __QGAMES_CHRSTEPDISTANCEDEFDISTANCE__;
				result = new QGAMES::CharacterControlDistanceStep (dT);
			}

			break;

		case __QGAMES_CHRSTEPNEARDISTANCE__:
			{
				std::string dr = 
					((i = def._values.find (__QGAMES_CHRSTEPNEARDISTANCEATTRPOINT__)) != def._values.end ()) 
						? (*i).second.c_str ()	: __QGAMES_CHRSTEPNEARDISTANCEDEFPOINT__;
				std::vector <QGAMES::bdata> drB = QGAMES::getElementsFromAsBDATA (dr, ',', 3);
				QGAMES::bdata dT = 
					((i = def._values.find (__QGAMES_CHRSTEPNEARDISTANCEATTRERROR__)) != def._values.end ()) 
						? __BD std::atof ((*i).second.c_str ()) : __QGAMES_CHRSTEPNEARDISTANCEDEFERROR__;
				result = new QGAMES::CharacterControlMoveNearStep (
					QGAMES::Position (drB [0] == __BD -1 ? __MINBDATA__ : drB [0], 
									  drB [1] == __BD -1 ? __MINBDATA__ : drB [1], 
									  drB [2] == __BD -1 ? __MINBDATA__ : drB [2]), dT);
			}

			break;

		case __QGAMES_CHRSTEPLIMITS__:
			{
				std::string crns = 
					((i = def._values.find (__QGAMES_CHRSTEPLIMITSATTRLIMITS__)) != def._values.end ()) 
						? (*i).second.c_str ()	: __QGAMES_CHRSTEPLIMITSDEFLIMITS1__;
				std::vector <QGAMES::bdata> crnsB = QGAMES::getElementsFromAsBDATA (crns, ',', 9);
				QGAMES::bdata dT = 
					((i = def._values.find (__QGAMES_CHRSTEPLIMITSATTRDISTANCE__)) != def._values.end ()) 
						? __BD std::atof ((*i).second.c_str ()) : __QGAMES_CHRSTEPLIMITSDEFDISTANCE__;
				result = new QGAMES::CharacterControlMoveInLimitsStep (
					QGAMES::Rectangle (QGAMES::Position (crnsB [0], crnsB [1], crnsB [2]),
									   QGAMES::Position (crnsB [3], crnsB [4], crnsB [5]),
									   QGAMES::Vector   (crnsB [6], crnsB [7], crnsB [8])), dT);
			}

			break;

		case __QGAMES_CHRCOMPLEXSTEPSEQUENTIAL__:
			return (new QGAMES::SequentialComplexCharacterControlStep (createSteps (def._innerSteps)));
			break;

		case __QGAMES_CHRCOMPLEXSTEPRANDOM__:
			return (new QGAMES::RandomComplexCharacterControlStep (createSteps (def._innerSteps)));
			break;

		case __QGAMES_CHRCOMPLEXSTEPCYCLIC__:
			return (new QGAMES::CyclicComplexCharacterControlStep (createSteps (def._innerSteps)));
			break;

		default:
			assert (0); // To force an error in debug mode...
			break;
	}

	return (result);
}

// ---
void QGAMES::CharacterControlStepsMonitorBuilder::readFile (const std::string& fDef)
{
	__WLOGPLUS (std::string ("Reading steps monitor definition file: ") + fDef);

	TiXmlDocument doc (fDef.c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the root element doesn't call "Monitors" the code will work too!
	// This is done to speed up the loading...

	for (TiXmlElement* defElement = rootElement -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		// The line can be an include line...
		// That line can be at any place in the movement definition file...
		if (strcmp (defElement -> Value (), __QGAMES_INCLUDENAME__) == 0)
		{
			assert (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)); 
			readFile (__RL (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)));
		}
		else
		// The other type of line can be a simple monitor line...
		if (strcmp (defElement -> Value (), __QGAMES_MONITORTAGNAME__) == 0)
		{
			QGAMES::CharacterControlStepsMonitorBuilder::MonitorDefinition def = 
				readMonitorDefinition (defElement);
			assert (def._id != -1); // Is it a valid monitor?
			_definitions.insert (QGAMES::CharacterControlStepsMonitorBuilder::Definitions::value_type (def._id, def));
		}
	}
}

// ---
QGAMES::CharacterControlStepsMonitorBuilder::MonitorDefinition
	QGAMES::CharacterControlStepsMonitorBuilder::readMonitorDefinition (TiXmlElement* m)
{
	QGAMES::CharacterControlStepsMonitorBuilder::MonitorDefinition result;
	
	m -> Attribute (__QGAMES_MONITOATTRID__, &result._id);
	std::string cStr (__NO_STRING__);
	cStr = m -> Attribute (__QGAMES_MONITORATTRCYCLIC__);
	result._cyclic = (QGAMES::toUpper (cStr) == std::string (__YES_STRING__));

	// Add the steps if any...(included complex ones)
	for (TiXmlElement* dataElement = m -> FirstChildElement ();
		dataElement != NULL; dataElement = dataElement -> NextSiblingElement ())
	{
		QGAMES::CharacterControlStepsMonitorBuilder::StepDefinition stp;
		if (strcmp (dataElement -> Value (), __QGAMES_MONITORSTEPTAGNAME__) == 0)
			stp = readStepDefinition (dataElement);
		else
		if (strcmp (dataElement -> Value (), __QGAMES_MONITORCOMPLEXSTEPTAGNAME__) == 0)
			stp = readComplexStepDefinition (dataElement);

		assert (stp._type != -1); // Well defined?

		result._steps.push_back (stp);
	}

	return (result);
}

// ---
QGAMES::CharacterControlStepsMonitorBuilder::StepDefinition 
	QGAMES::CharacterControlStepsMonitorBuilder::readStepDefinition (TiXmlElement* m)
{
	QGAMES::CharacterControlStepsMonitorBuilder::StepDefinition result;

	m -> Attribute (__QGAMES_MONITORSTEPATTRTYPE__, &result._type);
	
	// Add the values if any...
	for (TiXmlElement* dataElement = m -> FirstChildElement ();
		dataElement != NULL; dataElement = dataElement -> NextSiblingElement ())
	{
		// If the root element doesn't call "Variable" the code will work too!
		// This is done to speed up the loading...
		std::string id (__NULL_STRING__); 
		std::string value (__NULL_STRING__);
		id = dataElement -> Attribute (__QGAMES_MONITORVARIABLEATTRID__);
		value = dataElement -> Attribute (__QGAMES_MONITORVARIABLEATTRVALUE__);
		if (id != std::string (__NULL_STRING__))
			result._values.insert (std::map <std::string, std::string>::value_type (id, value));
	}

	return (result);
}

// ---
QGAMES::CharacterControlStepsMonitorBuilder::StepDefinition 
	QGAMES::CharacterControlStepsMonitorBuilder::readComplexStepDefinition (TiXmlElement* m)
{
	QGAMES::CharacterControlStepsMonitorBuilder::StepDefinition result;

	m -> Attribute (__QGAMES_MONITORSTEPATTRTYPE__, &result._type);
	
	// Add the inner steps...
	for (TiXmlElement* dataElement = m -> FirstChildElement ();
		dataElement != NULL; dataElement = dataElement -> NextSiblingElement ())
	{
		QGAMES::CharacterControlStepsMonitorBuilder::StepDefinition stp;
		if (strcmp (dataElement -> Value (), __QGAMES_MONITORSTEPTAGNAME__) == 0)
			stp = readStepDefinition (dataElement);
		else
		if (strcmp (dataElement -> Value (), __QGAMES_MONITORCOMPLEXSTEPTAGNAME__) == 0)
			stp = readComplexStepDefinition (dataElement);

		assert (stp._type != -1); // Well defined?

		result._innerSteps.push_back (stp);
	}

	return (result);
}

// ---
QGAMES::CharacterControlSteps QGAMES::CharacterControlStepsMonitorBuilder::createSteps 
	(const QGAMES::CharacterControlStepsMonitorBuilder::StepDefinitions& stps)
{
	QGAMES::CharacterControlSteps result;
	for (int i = 0; i < (int) stps.size (); 
			result.push_back (createStep (stps [i++]))); // Recursive, take care...
	return (result);
}
