#include <Platform/ptgame.hpp>
#include <Platform/ptscenes.hpp>
#include <Platform/ptbuilders.hpp>

// ---
QGAMES::SetOfOpenValues QGAMES::PlatformGame::PocketElement::likeSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result;

	result.addOpenValue (__QGAMES_POCKETATTRIBUTEID__, QGAMES::OpenValue (_id));
	result.addOpenValue (__QGAMES_POCKETATTRIBUTETYPE__, QGAMES::OpenValue (_type));
	result.addSetOfOpenValues (0, _values);// Additional parameters...

	return (result);
}

// ---
std::ostream& QGAMES::operator << (std::ostream& oS, const QGAMES::PlatformGame::PocketElement& tE)
{
	oS << tE._id << std::endl
	   << tE._type << std::endl
	   << tE._values;

	return (oS);
}

// ---
std::istream& QGAMES::operator >> (std::istream& iS, QGAMES::PlatformGame::PocketElement& tE)
{
	iS >> tE._id >> tE._type >> tE._values;

	return (iS);
}

// ---
QGAMES::PlatformGame::PocketElement QGAMES::PlatformGame::Pocket::toAddElement 
	(const QGAMES::PlatformGame::PocketElement& pE)
{
	QGAMES::PlatformGame::PocketElement lT = _elements [_elements.size () - 1];
	for (int i = (_elements.size () - 2); i >= 0; i--)
		_elements [i + 1] = _elements [i];
	_elements [0] = pE;
	return (lT);
}

// ---
std::vector <int> QGAMES::PlatformGame::Pocket::listOfElementsType () const
{
	std::vector <int> result (_elements.size (), -1);
	for (int i = 0; i < (int) _elements.size (); i++)
		result [i] = _elements [i]._type;
	return (result);
}

// ---
bool QGAMES::PlatformGame::Pocket::isEmpty () const
{
	bool result = true;
	for (int i = 0; i < (int) _elements.size () && !result; result = (_elements [i++]._type != -1));
	return (result);
}

// ---
std::ostream& QGAMES::operator << (std::ostream& oS, const QGAMES::PlatformGame::Pocket& p)
{
	oS << (int) p._elements.size ();
	for (int i = 0; i < (int) p._elements.size (); i++)
		oS << std::endl << p._elements [i];

	return (oS);
}

// ---
std::istream& QGAMES::operator >> (std::istream& iS, QGAMES::PlatformGame::Pocket& p)
{
	int nE; iS >> nE;
	p._elements.resize (nE);
	for (int i = 0; i < nE; i++)
		iS >> p._elements [i];

	return (iS);	
}

// ---
std::ostream& QGAMES::operator << (std::ostream& oS, const QGAMES::PlatformGame::ElementDefinition& eD)
{
	oS << eD._id << std::endl
	   << eD._type << std::endl
	   << eD._worldId << std::endl
	   << eD._sceneId << std::endl;

	oS << (int) eD._positions.size () << std::endl;
	for (int i = 0; i < (int) eD._positions.size (); i++)
		oS << eD._positions [i] << std::endl;

	oS << eD._orientation << std::endl;

	oS << eD._state << std::endl
	   << eD._otherValues;

	return (oS);
}

// ---
std::istream& QGAMES::operator >> (std::istream& iS, QGAMES::PlatformGame::ElementDefinition& eD)
{
	iS >> eD._id 
	   >> eD._type 
	   >> eD._worldId
	   >> eD._sceneId;

	int nE; iS >> nE;
	eD._positions.resize (nE);
	for (int i = 0; i < nE; i++)
	{
		QGAMES::Position p; iS >> p;
		eD._positions [i] = p;
	}

	iS >> eD._orientation;

	iS >> eD._state
	   >> eD._otherValues;

	return (iS);
}

// ---
QGAMES::PlatformGame::Conf::Conf (int nP)
	: QGAMES::AdvancedArcadeGame::Conf (nP),
	  _pocket (),
	  _thingsToEat (),
	  _thingsToCatch (),
	  _badGuys (),
	  _exitDoors (),
	  _eatInfoLoadedOnce (),
	  _catchInfoLoadedOnce (),
	  _badGuysInfoLoadedOnce (),
	  _exitDoorsInfoLoadedOnce ()
{
	adjustToPlayers (nP);
}

// ---
QGAMES::PlatformGame::Conf::~Conf ()
{
	for (int i = 1; i <= _numberPlayers; i++)
	{
		deleteElementsDefinitionFor (i, _thingsToEat, _eatInfoLoadedOnce);
		deleteElementsDefinitionFor (i, _thingsToCatch, _catchInfoLoadedOnce);
		deleteElementsDefinitionFor (i, _badGuys, _badGuysInfoLoadedOnce);
		deleteElementsDefinitionFor (i, _exitDoors, _exitDoorsInfoLoadedOnce);
	}
}

// ---
QGAMES::PlatformGame::ThingsToEatDefinition QGAMES::PlatformGame::Conf::thingsToEatFor (int nP, int nS) const
{
	assert (nP > 0 && nP <= (int) _thingsToEat.size ());

	QGAMES::PlatformGame::ThingsToEatDefinition result;

	ThingsToEatDefinitionLoadedOnce::const_iterator i = _eatInfoLoadedOnce [nP - 1].find (nS);
	if (i != _eatInfoLoadedOnce [nP - 1].end () && (*i).second == true) // Has it been already loaded at least once?
	{
		// Then, just look for it...
		for (QGAMES::PlatformGame::ThingsToEatDefinition::const_iterator j = _thingsToEat [nP - 1].begin ();
				j != _thingsToEat [nP - 1].end (); j++)
			if ((*j).second -> _sceneId == nS)
				result [(*j).second -> _id] = (*j).second;
	}
	else
	{
		if ((__AGM game ()) -> existWorldForScene (nS)) 
		{
			result = defaultThingsToEatFor (nS);
			for (QGAMES::PlatformGame::ThingsToEatDefinition::const_iterator j = result.begin ();
					j != result.end (); j++)
				_thingsToEat [nP - 1][(*j).second -> _id] = (*j).second;
			_eatInfoLoadedOnce [nP - 1][nS] = true;
		}
	}

	return (result);
}

// ---
bool QGAMES::PlatformGame::Conf::existThingToEatFor (int nP, int nS, int id) const
{
	QGAMES::PlatformGame::ThingsToEatDefinition tE = thingsToEatFor (nP, nS);

	bool result = false;
	for (QGAMES::PlatformGame::ThingsToEatDefinition::const_iterator i = tE.begin (); 
			i != tE.end () && !result; i++)
		if ((*i).second -> _id == id)
			result = true;

	return (result);
}

// ---
QGAMES::PlatformGame::ThingsToCatchDefinition QGAMES::PlatformGame::Conf::thingsToCatchFor (int nP, int nS) const
{
	assert (nP > 0 && nP <= (int) _thingsToCatch.size ());

	QGAMES::PlatformGame::ThingsToCatchDefinition result;

	ThingsToCatchDefinitionLoadedOnce::const_iterator i = _catchInfoLoadedOnce [nP - 1].find (nS);
	if (i != _catchInfoLoadedOnce [nP - 1].end () && (*i).second == true) // Has it been already loaded at least once?
	{
		// Then, just look for it...
		for (QGAMES::PlatformGame::ThingsToCatchDefinition::const_iterator j = _thingsToCatch [nP - 1].begin ();
				j != _thingsToCatch [nP - 1].end (); j++)
			if ((*j).second -> _sceneId == nS)
				result [(*j).second -> _id] = (*j).second;
	}
	else
	{
		if ((__AGM game ()) -> existWorldForScene (nS)) 
		{
			result = defaultThingsToCatchFor (nS);
			for (QGAMES::PlatformGame::ThingsToCatchDefinition::const_iterator j = result.begin ();
					j != result.end (); j++)
				_thingsToCatch [nP - 1][(*j).second -> _id] = (*j).second;
			_catchInfoLoadedOnce [nP - 1][nS] = true;
		}
	}

	return (result);
}

// ---
bool QGAMES::PlatformGame::Conf::existThingToCatchFor (int nP, int nS, int id) const
{
	QGAMES::PlatformGame::ThingsToCatchDefinition tC = thingsToCatchFor (nP, nS);

	bool result = false;
	for (QGAMES::PlatformGame::ThingsToCatchDefinition::const_iterator i = tC.begin (); 
			i != tC.end () && !result; i++)
		if ((*i).second -> _id == id)
			result = true;

	return (result);
}

// ---
QGAMES::PlatformGame::ThingToCatchDefinition* QGAMES::PlatformGame::Conf::addThingToCatch 
	(int nP, int nW, int nS, int id, int t, 
	 const QGAMES::Positions& pos, int st, const QGAMES::SetOfOpenValues& sV)
{
	assert (!existThingToCatchFor (nP, nS, id)); // It should exit before (just to be sire)

	QGAMES::PlatformGame::ThingToCatchDefinition* nT = createEmptyThingToCatchDefinition ();
	nT -> _id = id; 
	nT -> _type = t;
	nT -> _positions = pos;
	nT -> _worldId = nW;
	nT -> _sceneId = nS;
	nT -> _state = st;
	nT -> _otherValues = sV;

	thingsToCatchFor (nP, nS); // To load the info if it hasn't been loaded already
	_thingsToCatch [nP - 1][nT -> _id] = nT; // Just added...

	return (nT); // What has been added!
}

// ---
QGAMES::PlatformGame::BadGuysDefinition QGAMES::PlatformGame::Conf::badGuysFor (int nP, int nS) const
{
	assert (nP > 0 && nP <= (int) _badGuys.size ());

	QGAMES::PlatformGame::BadGuysDefinition result;

	BadGuysDefinitionLoadedOnce::const_iterator i = _badGuysInfoLoadedOnce [nP - 1].find (nS);
	if (i != _badGuysInfoLoadedOnce [nP - 1].end () && (*i).second == true) // Has it been already loaded at least once?
	{
		// Then, just look for it...
		for (QGAMES::PlatformGame::BadGuysDefinition::const_iterator j = _badGuys [nP - 1].begin ();
				j != _badGuys [nP - 1].end (); j++)
			if ((*j).second -> _sceneId == nS)
				result [(*j).second -> _id] = (*j).second;
	}
	else
	{
		if ((__AGM game ()) -> existWorldForScene (nS)) 
		{
			result = defaultBadGuysFor (nS);
			for (QGAMES::PlatformGame::BadGuysDefinition::const_iterator j = result.begin ();
					j != result.end (); j++)
				_badGuys [nP - 1][(*j).second -> _id] = (*j).second;
			_badGuysInfoLoadedOnce [nP - 1][nS] = true;
		}
	}

	return (result);
}

// ---
bool QGAMES::PlatformGame::Conf::existBadGuyFor (int nP, int nS, int id) const
{
	QGAMES::PlatformGame::BadGuysDefinition bG = badGuysFor (nP, nS);

	bool result = false;
	for (QGAMES::PlatformGame::BadGuysDefinition::const_iterator i = bG.begin (); 
			i != bG.end () && !result; i++)
		if ((*i).second -> _id == id)
			result = true;

	return (result);
}

// ---
QGAMES::PlatformGame::ExitDoorsDefinition QGAMES::PlatformGame::Conf::exitDoorsFor (int nP, int nS) const
{
	assert (nP > 0 && nP <= (int) _exitDoors.size ());

	QGAMES::PlatformGame::ExitDoorsDefinition result;

	ExitDoorsDefinitionLoadedOnce::const_iterator i = _exitDoorsInfoLoadedOnce [nP - 1].find (nS);
	if (i != _exitDoorsInfoLoadedOnce [nP - 1].end () && (*i).second == true) // Has it been already loaded at least once?
	{
		// Then, just look for it...
		for (QGAMES::PlatformGame::ExitDoorsDefinition::const_iterator j = _exitDoors [nP - 1].begin ();
				j != _exitDoors [nP - 1].end (); j++)
			if ((*j).second -> _sceneId == nS)
				result [(*j).second -> _id] = (*j).second;
	}
	else
	{
		if ((__AGM game ()) -> existWorldForScene (nS)) 
		{
			result = defaultExitDoorsFor (nS);
			for (QGAMES::PlatformGame::ExitDoorsDefinition::const_iterator j = result.begin ();
					j != result.end (); j++)
				_exitDoors [nP - 1][(*j).second -> _id] = (*j).second;
			_exitDoorsInfoLoadedOnce [nP - 1][nS] = true;
		}
	}

	return (result);
}

// ---
bool QGAMES::PlatformGame::Conf::existExitDoorFor (int nP, int nS, int id) const
{
	QGAMES::PlatformGame::ExitDoorsDefinition eD = exitDoorsFor (nP, nS);

	bool result = false;
	for (QGAMES::PlatformGame::ExitDoorsDefinition::const_iterator i = eD.begin (); 
			i != eD.end () && !result; i++)
		if ((*i).second -> _id == id)
			result = true;

	return (result);
}

// ---
QGAMES::PlatformGame::ThingsToEatDefinition QGAMES::PlatformGame::Conf::defaultThingsToEatFor (int nS) const
{
	if (!(__AGM game ()) -> existWorldForScene (nS))
		return (QGAMES::PlatformGame::ThingsToEatDefinition ()); // The scene doesn't exist...

	QGAMES::PlatformScene* scn = 
		dynamic_cast <QGAMES::PlatformScene*> ((__AGM game ()) -> worldForScene (nS) -> scene (nS));
	assert (scn);
	return (scn -> thingsToEatDefinition ());
}

// ---
QGAMES::PlatformGame::ThingsToCatchDefinition QGAMES::PlatformGame::Conf::defaultThingsToCatchFor (int nS) const
{
	if (!(__AGM game ()) -> existWorldForScene (nS))
		return (QGAMES::PlatformGame::ThingsToCatchDefinition ());

	QGAMES::PlatformScene* scn = 
		dynamic_cast <QGAMES::PlatformScene*> ((__AGM game ()) -> worldForScene (nS) -> scene (nS));
	assert (scn);
	return (scn -> thingsToCatchDefinition ());
}

// ---
QGAMES::PlatformGame::BadGuysDefinition QGAMES::PlatformGame::Conf::defaultBadGuysFor (int nS) const
{
	if (!(__AGM game ()) -> existWorldForScene (nS))
		return (QGAMES::PlatformGame::BadGuysDefinition ());

	QGAMES::PlatformScene* scn = 
		dynamic_cast <QGAMES::PlatformScene*> ((__AGM game ()) -> worldForScene (nS) -> scene (nS));
	assert (scn);
	return (scn -> badGuysDefinition ());
}

// ---
QGAMES::PlatformGame::ExitDoorsDefinition QGAMES::PlatformGame::Conf::defaultExitDoorsFor (int nS) const
{
	if (!(__AGM game ()) -> existWorldForScene (nS))
		return (QGAMES::PlatformGame::ExitDoorsDefinition ());

	QGAMES::PlatformScene* scn = 
		dynamic_cast <QGAMES::PlatformScene*> ((__AGM game ()) -> worldForScene (nS) -> scene (nS));
	assert (scn);
	return (scn -> exitDoorsDefinition ());
}

// ---
void QGAMES::PlatformGame::Conf::adjustToPlayers (int nP)
{
	QGAMES::AdvancedArcadeGame::Conf::adjustToPlayers (nP);

	_pocket = std::vector <QGAMES::PlatformGame::Pocket> (nP, QGAMES::PlatformGame::Pocket ()); 

	for (int i = 0; i < (int) _thingsToEat.size (); deleteThingsToEatFor (i++ + 1));
	_thingsToEat = QGAMES::PlatformGame::ThingsToEatDefinitionPerPlayer
			(nP, QGAMES::PlatformGame::ThingsToEatDefinition ());
	_eatInfoLoadedOnce = QGAMES::PlatformGame::ThingsToEatDefinitionLoadedOncePerPlayer
			(nP, QGAMES::PlatformGame::ThingsToEatDefinitionLoadedOnce ());
	for (int i = 0; i < (int) _thingsToCatch.size (); deleteThingsToCatchFor (i++ + 1));
	_thingsToCatch = QGAMES::PlatformGame::ThingsToCatchDefinitionPerPlayer
			(nP, QGAMES::PlatformGame::ThingsToCatchDefinition ());
	_catchInfoLoadedOnce = QGAMES::PlatformGame::ThingsToCatchDefinitionLoadedOncePerPlayer
			(nP, QGAMES::PlatformGame::ThingsToCatchDefinitionLoadedOnce ());
	for (int i = 0; i < (int) _badGuys.size (); deleteBadGuysFor (i++ + 1));
	_badGuys = QGAMES::PlatformGame::BadGuysDefinitionPerPlayer
			(nP, QGAMES::PlatformGame::BadGuysDefinition ());
	_badGuysInfoLoadedOnce = QGAMES::PlatformGame::BadGuysDefinitionLoadedOncePerPlayer
			(nP, QGAMES::PlatformGame::BadGuysDefinitionLoadedOnce ());
	for (int i = 0; i < (int) _exitDoors.size (); deleteExitDoorsFor (i++ + 1));
	_exitDoors = QGAMES::PlatformGame::ExitDoorsDefinitionPerPlayer 
			(nP, QGAMES::PlatformGame::ExitDoorsDefinition ());
	_exitDoorsInfoLoadedOnce = QGAMES::PlatformGame::ExitDoorsDefinitionLoadedOncePerPlayer
			(nP, QGAMES::PlatformGame::ExitDoorsDefinitionLoadedOnce ());
}

// ---
void QGAMES::PlatformGame::Conf::cfgToStream (std::ostringstream& oS)
{
	QGAMES::AdvancedArcadeGame::Conf::cfgToStream (oS);

	// Save the pocket
	for (int i = 0; i < _numberPlayers; i++)
		oS << _pocket [i] << std::endl;

	// The data related with the things to eat...
	for (int i = 0; i < _numberPlayers; i++)
	{
		oS << (int) _thingsToEat [i].size () << std::endl;
		for (QGAMES::PlatformGame::ThingsToEatDefinition::const_iterator j = _thingsToEat [i].begin ();
				j != _thingsToEat [i].end (); j++)
			oS << *((*j).second) << std::endl;

		oS << (int) _eatInfoLoadedOnce [i].size () << std::endl;
		for (QGAMES::PlatformGame::ThingsToEatDefinitionLoadedOnce::const_iterator j = _eatInfoLoadedOnce [i].begin ();
				j != _eatInfoLoadedOnce [i].end (); j++)
			oS << (*j).first << std::endl << (*j).second << std::endl;
	}

	// The data related with the things to catch...
	for (int i = 0; i < _numberPlayers; i++)
	{
		oS << (int) _thingsToCatch [i].size () << std::endl;
		for (QGAMES::PlatformGame::ThingsToCatchDefinition::const_iterator j = _thingsToCatch [i].begin ();
				j != _thingsToCatch [i].end (); j++)
			oS << *((*j).second) << std::endl;

		oS << (int) _catchInfoLoadedOnce [i].size () << std::endl;
		for (QGAMES::PlatformGame::ThingsToCatchDefinitionLoadedOnce::const_iterator j = _catchInfoLoadedOnce [i].begin ();
				j != _catchInfoLoadedOnce [i].end (); j++)
			oS << (*j).first << std::endl << (*j).second << std::endl;
	}

	// The data related with the bad guys...
	for (int i = 0; i < _numberPlayers; i++)
	{
		oS << (int) _badGuys [i].size () << std::endl;
		for (QGAMES::PlatformGame::BadGuysDefinition::const_iterator j = _badGuys [i].begin ();
				j != _badGuys [i].end (); j++)
			oS << *((*j).second) << std::endl;

		oS << (int) _badGuysInfoLoadedOnce [i].size () << std::endl;
		for (QGAMES::PlatformGame::BadGuysDefinitionLoadedOnce::const_iterator j = _badGuysInfoLoadedOnce [i].begin ();
				j != _badGuysInfoLoadedOnce [i].end (); j++)
			oS << (*j).first << std::endl << (*j).second << std::endl;
	}

	// The data related with the doors...
	for (int i = 0; i < _numberPlayers; i++)
	{
		oS << (int) _exitDoors [i].size () << std::endl;
		for (QGAMES::PlatformGame::ExitDoorsDefinition::const_iterator j = _exitDoors [i].begin ();
				j != _exitDoors [i].end (); j++)
			oS << *((*j).second) << std::endl;

		oS << (int) _exitDoorsInfoLoadedOnce [i].size () << std::endl;
		for (QGAMES::PlatformGame::ExitDoorsDefinitionLoadedOnce::const_iterator j = _exitDoorsInfoLoadedOnce [i].begin ();
				j != _exitDoorsInfoLoadedOnce [i].end (); j++)
			oS << (*j).first << std::endl << (*j).second << std::endl;
	}
}

// ---
void QGAMES::PlatformGame::Conf::cfgFromStream (std::istringstream& iS)
{
	QGAMES::AdvancedArcadeGame::Conf::cfgFromStream (iS);

	// The pocket of the artists...
	_pocket.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _pocket [i];

	// The data related with the things to eat...
	for (int i = 0; i < (int) _thingsToEat.size (); deleteThingsToEatFor (i++ + 1)); _thingsToEat.clear (); 
	_thingsToEat.resize (_numberPlayers); _eatInfoLoadedOnce.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
	{
		int nTE; iS >> nTE; // The number of things to eat kept...
		for (int j = 0; j < nTE; j++)
		{
			QGAMES::PlatformGame::ThingToEatDefinition tE; iS >> tE;
			_thingsToEat [i][tE._id] = new QGAMES::PlatformGame::ThingToEatDefinition (tE);
		}

		iS >> nTE; // The info about eat loaded or not...
		for (int j = 0; j < nTE; j++)
		{
			int f, s; iS >> f >> s;
			_eatInfoLoadedOnce [i][f] = s;
		}
	}

	// The data related with the things to catch...
	for (int i = 0; i < (int) _thingsToCatch.size (); deleteThingsToCatchFor (i++ + 1)); _thingsToCatch.clear (); 
	_thingsToCatch.resize (_numberPlayers); _catchInfoLoadedOnce.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
	{
		int nTC; iS >> nTC; // The number of things to catch kept...
		for (int j = 0; j < nTC; j++)
		{
			QGAMES::PlatformGame::ThingToCatchDefinition tC; iS >> tC;
			_thingsToCatch [i][tC._id] = new QGAMES::PlatformGame::ThingToCatchDefinition (tC);
		}

		iS >> nTC; // The info about things loaded or not...
		for (int j = 0; j < nTC; j++)
		{
			int f, s; iS >> f >> s;
			_catchInfoLoadedOnce [i][f] = s;
		}
	}

	// The data related with the bad guys...
	for (int i = 0; i < (int) _badGuys.size (); deleteBadGuysFor (i++ + 1)); _badGuys.clear (); 
	_badGuys.resize (_numberPlayers); _badGuysInfoLoadedOnce.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
	{
		int nBG; iS >> nBG; // The number of bad guys kept...
		for (int j = 0; j < nBG; j++)
		{
			QGAMES::PlatformGame::BadGuyDefinition bG; iS >> bG;
			_badGuys [i][bG._id] = new QGAMES::PlatformGame::BadGuyDefinition (bG);
		}

		iS >> nBG; // The info about bad guys info loaded or not...
		for (int j = 0; j < nBG; j++)
		{
			int f, s; iS >> f >> s;
			_badGuysInfoLoadedOnce [i][f] = s;
		}
	}

	// The data related with the exit doors...
	for (int i = 0; i < (int) _exitDoors.size (); deleteExitDoorsFor (i++ + 1)); _exitDoors.clear (); 
	_exitDoors.resize (_numberPlayers); _exitDoorsInfoLoadedOnce.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
	{
		int nED; iS >> nED; // The number of exit doors kept...
		for (int j = 0; j < nED; j++)
		{
			QGAMES::PlatformGame::ExitDoorDefinition eD; iS >> eD;
			_exitDoors [i][eD._id] = new QGAMES::PlatformGame::ExitDoorDefinition (eD);
		}

		iS >> nED; // The info about exit doors info loaded or not...
		for (int j = 0; j < nED; j++)
		{
			int f, s; iS >> f >> s;
			_exitDoorsInfoLoadedOnce [i][f] = s;
		}
	}
}

// ---
void QGAMES::PlatformGame::Conf::cloneImplementation (const QGAMES::Game::Configuration* dt)
{
	assert (dt);
	
	const QGAMES::PlatformGame::Conf* cfg = dynamic_cast <const QGAMES::PlatformGame::Conf*> (dt);
	assert (cfg); // It has to be...

	_thingsToEat = QGAMES::PlatformGame::ThingsToEatDefinitionPerPlayer 
		(cfg -> _thingsToEat.size (), QGAMES::PlatformGame::ThingsToCatchDefinition ());
	for (int i = 0; i < (int) cfg -> _thingsToEat.size (); i++)
		for (QGAMES::PlatformGame::ThingsToEatDefinition::const_iterator j = cfg -> _thingsToEat [i].begin ();
				j != cfg -> _thingsToEat [i].end (); j++)
			_thingsToEat [i][(*j).first] = new QGAMES::PlatformGame::ThingToEatDefinition (*((*j).second));

	_thingsToCatch = QGAMES::PlatformGame::ThingsToCatchDefinitionPerPlayer
		(cfg -> _thingsToCatch.size (), QGAMES::PlatformGame::ThingsToCatchDefinition ());
	for (int i = 0; i < (int) cfg -> _thingsToCatch.size (); i++)
		for (QGAMES::PlatformGame::ThingsToCatchDefinition::const_iterator j = cfg -> _thingsToCatch [i].begin ();
				j != cfg -> _thingsToCatch [i].end (); j++)
			_thingsToCatch [i][(*j).first] = new QGAMES::PlatformGame::ThingToCatchDefinition (*((*j).second));

	_badGuys = QGAMES::PlatformGame::BadGuysDefinitionPerPlayer
			(cfg -> _badGuys.size (), QGAMES::PlatformGame::BadGuysDefinition ());
	for (int i = 0; i < (int) cfg -> _badGuys.size (); i++)
		for (QGAMES::PlatformGame::BadGuysDefinition::const_iterator j = cfg -> _badGuys [i].begin ();
				j != cfg -> _badGuys [i].end (); j++)
			_badGuys [i][(*j).first] = new QGAMES::PlatformGame::BadGuyDefinition (*((*j).second));

	_exitDoors = QGAMES::PlatformGame::ExitDoorsDefinitionPerPlayer
			(cfg -> _exitDoors.size (), QGAMES::PlatformGame::ExitDoorsDefinition ());
	for (int i = 0; i < (int) cfg -> _exitDoors.size (); i++)
		for (QGAMES::PlatformGame::ExitDoorsDefinition::const_iterator j = cfg -> _exitDoors [i].begin ();
				j != cfg -> _exitDoors [i].end (); j++)
			_exitDoors [i][(*j).first] = new QGAMES::PlatformGame::ExitDoorDefinition (*((*j).second));
}

// ---
void QGAMES::PlatformGame::Conf::removeElementDefinitionFor (int nP, int nS, int id,
	QGAMES::PlatformGame::ElementsDefinitionPerPlayer& lst, 
	QGAMES::PlatformGame::ElementsDefinitionLoadedOncePerPlayer& lstOnce)
{
	assert (nP > 0 && nP <= (int) lst.size ());

	QGAMES::PlatformGame::ElementsDefinition::const_iterator i = lst [nP - 1].begin ();
	while (i != lst [nP - 1].end ())
	{
		if ((*i).second -> _sceneId == nS && (*i).second -> _id == id)
		{
			delete ((*i).second);
			lst [nP - 1].erase (i);
			i = lst [nP - 1].begin ();
		}
		else
			i++;
	}
}

// ---
void QGAMES::PlatformGame::Conf::deleteElementsDefinitionFor (int nP, int nS,
		QGAMES::PlatformGame::ElementsDefinitionPerPlayer& lst,
		QGAMES::PlatformGame::ElementsDefinitionLoadedOncePerPlayer& lstOnce)
{
	assert (nP > 0 && nP <= (int) lst.size ());

	for (QGAMES::PlatformGame::ElementsDefinition::const_iterator i = lst [nP - 1].begin ();
				i != lst [nP - 1].end (); i++)
		if ((*i).second -> _sceneId == nS)
			delete ((*i++).second);

	QGAMES::PlatformGame::ElementsDefinitionLoadedOnce::iterator i;
	if ((i = lstOnce [nP - 1].find (nS)) != lstOnce [nP].end ())
		lstOnce [nP- 1].erase (i); // The same that setting it up to false...
}

// ---
void QGAMES::PlatformGame::Conf::deleteElementsDefinitionFor (int nP, 
		QGAMES::PlatformGame::ElementsDefinitionPerPlayer& lst,
		QGAMES::PlatformGame::ElementsDefinitionLoadedOncePerPlayer& lstOnce)
{
	assert (nP > 0 && nP <= (int) lst.size ());

	for (QGAMES::PlatformGame::ElementsDefinition::const_iterator i = lst [nP - 1].begin ();
				i != lst [nP - 1].end (); delete ((*i++).second));

	lst [nP - 1].clear ();
	lst [nP - 1] = QGAMES::PlatformGame::ElementsDefinition ();

	lstOnce [nP - 1].clear ();
	lstOnce [nP - 1] = QGAMES::PlatformGame::ElementsDefinitionLoadedOnce ();
}

// ---
QGAMES::ToKnowTileFrameInformation* QGAMES::PlatformGame::tileFrameInformation ()
{
	QGAMES::ToKnowTileFrameInformation* add = NULL;
	const QGAMES::MapBuilder::AddsOnList& addLst = (__AGM game ()) -> mapBuilder () -> addsOn ();
	for (QGAMES::MapBuilder::AddsOnList::const_iterator i = addLst.begin (); i != addLst.end () && !add; i++)
		if (dynamic_cast <QGAMES::PlatformTMXMapBuilder*> ((*i)))
			add = ((QGAMES::PlatformTMXMapBuilder*) (*i)) -> tileFrameInformation ();

	return (add);
}

// ---
QGAMES::MapBuilder* QGAMES::PlatformGame::createMapBuilder ()
{
	QGAMES::MapBuilder* result = new QGAMES::MapBuilder (parameter (__GAME_MAPSOBJECTSFILE__));
	result -> addAddsOn (new QGAMES::ObjectMapBuilderAddsOn (objectBuilder ()));
	result -> addAddsOn (new QGAMES::BackgroundObjectMapBuilder (objectBuilder ()));
	// The adds on to create the maps (tmx or whatever) whould be added later attending to the type of game...
	return (result);
}
