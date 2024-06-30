#include <AdvancedArcade/adenvconditions.hpp>

// ---
const int QGAMES::Rain::_fades [6] = { 0, 50, 100, 150, 200, 255 };
const int QGAMES::Wind::_fades [6] = { 0, 50, 100, 150, 200, 255 };
const int QGAMES::Lightning::_fades [6] = { 255, 200, 150, 100, 50, 0 };
const int QGAMES::Rainbow::_fades [6] = { 100, 80, 60, 40, 20, 0 };
const int QGAMES::Clouds::_fades [6] = { 0, 25, 50, 75, 100, 125 };

// ---
QGAMES::Thunder::Thunder (QGAMES::Scene* s, int sCh, QGAMES::Sound* nS)
	: QGAMES::EnvironmentalCondition (__QGAMES_THUNDERWHEATHER__, s),
	  _soundChannel (sCh), _sound (NULL)
{
	assert (nS != NULL);

	_sound = nS;
}

// ---
QGAMES::Thunder::Thunder (QGAMES::Scene* s, const std::map <std::string, std::string>& prps)
	: QGAMES::EnvironmentalCondition (__QGAMES_THUNDERWHEATHER__, s),
	  _soundChannel (0), _sound (NULL)
{
	if (prps.find (__QGAMES_THUNDERWHEATHERATTRSOUNDCHN__) != prps.end ())
		_soundChannel = std::atoi ((*prps.find (__QGAMES_THUNDERWHEATHERATTRSOUNDCHN__)).second.c_str ());
	int sId = -1;
	if (prps.find (__QGAMES_THUNDERWHEATHERATTRSOUND__) != prps.end ())
		sId = std::atoi ((*prps.find (__QGAMES_THUNDERWHEATHERATTRSOUND__)).second.c_str ());
	if (sId != -1)
		_sound = game () -> sound (sId);
}

// ---
void QGAMES::Thunder::initialize ()
{
	QGAMES::EnvironmentalCondition::initialize ();

	if (_sound) _sound -> play (_soundChannel);

	reStartAllCounters ();
	reStartAllOnOffSwitches ();
}

// ---
void QGAMES::Thunder::updatePositions ()
{
	QGAMES::EnvironmentalCondition::updatePositions ();

	if (!onOffSwitch (_SWITCHTHUNDERSTARTS) -> isOn ())
	{
		onOffSwitch (_SWITCHTHUNDERSTARTS) -> set (true);
		notify (QGAMES::Event (__QGAMES_THUNDERSTARTS__, this));
	}

	if (counter (_COUNTERTHUNDERON) -> isEnd () &&
		!onOffSwitch (_SWITCHTHUNDERFINISHES) -> isOn ())
	{
		onOffSwitch (_SWITCHTHUNDERFINISHES) -> set (true);
		notify (QGAMES::Event (__QGAMES_THUNDERFINISHES__, this));
	}
}

// ---
void QGAMES::Thunder::finalize ()
{
	QGAMES::EnvironmentalCondition::finalize ();

	if (_sound) _sound -> stop ();

	if (!onOffSwitch (_SWITCHTHUNDERFINISHES) -> isOn ())
	{
		onOffSwitch (_SWITCHTHUNDERFINISHES) -> set (true);
		notify (QGAMES::Event (__QGAMES_THUNDERFINISHES__, this));
	}
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::Thunder::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::Thunder::_COUNTERTHUNDERON, 
			QGAMES::Game::game () -> framesPerSecond () * 4, 0, true, false));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::Thunder::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::Thunder::_SWITCHTHUNDERSTARTS, false)); 
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::Thunder::_SWITCHTHUNDERFINISHES, false)); 
}

//---
QGAMES::Thunders::Properties::Properties (int p, QGAMES::bdata f, int sCh, int sId)
	: _probability (p),
	  _frecuency (f),
	  _soundChannel (sCh),
	  _soundId (sId)
{
	assert (_probability > 0 && _probability < 100);
	assert (_frecuency > __BD 0);

}

// ---
QGAMES::Thunders::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _probability (50),
	  _frecuency (__BD 1),
	  _soundChannel (__QGAMES_WHEATHERSOUNDCHANNEL__),
	  _soundId (__QGAMES_THUNDERWAVSOUND__)
{
	if (prps.find (std::string (__QGAMES_THUNDERSWHEATHERATTRPROB__)) != prps.end ())
		_probability = std::atoi ((*prps.find (std::string (__QGAMES_THUNDERSWHEATHERATTRPROB__))).second.c_str ());
	if (prps.find (std::string (__QGAMES_THUNDERSWHEATHERATTRFREQ__)) != prps.end ())
		_frecuency = __BD (std::atof ((*prps.find (std::string (__QGAMES_THUNDERSWHEATHERATTRFREQ__))).second.c_str ()));
	if (prps.find (std::string (__QGAMES_THUNDERSWHEATHERATTRSOUNDCHN__)) != prps.end ())
		_soundChannel = std::atoi ((*prps.find (std::string (__QGAMES_THUNDERSWHEATHERATTRSOUNDCHN__))).second.c_str ());
	if (prps.find (std::string (__QGAMES_THUNDERSWHEATHERATTRSOUND__)) != prps.end ())
		_soundId = (std::atoi ((*prps.find (std::string (__QGAMES_THUNDERSWHEATHERATTRSOUND__))).second.c_str ()));

	QGAMES::Thunders::Properties::Properties (_probability, _frecuency, _soundChannel, _soundId);
}

// ---
QGAMES::Thunders::Thunders (QGAMES::Scene* s, const QGAMES::Thunders::Properties& prps)
	: QGAMES::ComplexEnvironmentalCondition (__QGAMES_THUNDERSWHEATHER__, s, 
		QGAMES::EnvironmentalConditions ()),
	  _properties (prps),
	  _thunder (NULL)
{
	addCondition (_thunder = new QGAMES::Thunder (s, _properties._soundChannel, 
		 (_properties._soundId != -1) ? game () -> sound (_properties._soundId) : NULL));
}

// ---
void QGAMES::Thunders::setProperties (const QGAMES::Thunders::Properties& prps)
{
	_properties = prps;

	counter (_COUNTERTONEXTTHUNDER) -> 
		initialize ((int) (__BD game () -> framesPerSecond () * _properties._frecuency), 0, true, true);
}

// ---
void QGAMES::Thunders::initialize ()
{
	QGAMES::ComplexEnvironmentalCondition::initialize ();

	activateCondition (_thunder);
	
	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	counter (_COUNTERTONEXTTHUNDER) -> 
		initialize ((int) (__BD game () -> framesPerSecond () * _properties._frecuency), 0, true, true);
}

// ---
void QGAMES::Thunders::updatePositions ()
{
	QGAMES::ComplexEnvironmentalCondition::updatePositions ();

	if (onOffSwitch (_SWITCHCOUNTTONEXTTHUNDER) -> isOn () &&
		counter (_COUNTERTONEXTTHUNDER) -> isEnd () && (rand () % 100 < _properties._probability))
	{
		activateCondition (_thunder); // Starts back...

		onOffSwitch (_SWITCHCOUNTTONEXTTHUNDER) -> set (false);
	}
}

// ---
void QGAMES::Thunders::finalize ()
{
	QGAMES::ComplexEnvironmentalCondition::finalize ();

	desactivateCondition (_thunder);
}

// ---
void QGAMES::Thunders::processEvent (const QGAMES::Event& evnt)
{
	switch (evnt.code ())
	{
		// Starts back to count down to the next thunder...
		case __QGAMES_THUNDERFINISHES__:
			{
				onOffSwitch (_SWITCHCOUNTTONEXTTHUNDER) -> set (true); 

				desactivateCondition (_thunder); // Just in case...
			}

			break;

		default:
			break;
	}

	// Rethrow the event...
	notify (evnt);
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::Thunders::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::Thunders::_COUNTERTONEXTTHUNDER, QGAMES::Game::game () -> framesPerSecond (), 
			0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::Thunders::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::Thunders::_SWITCHCOUNTTONEXTTHUNDER, true)); 
}

// ---
QGAMES::Rain::Properties::Properties (QGAMES::bdata d, int sp, int sCh, int sId, int bF, int dF)
	: _density (d),
	  _speed (sp),
	  _soundChannel (sCh),
	  _soundId (sId),
	  _backgroundFormId (bF), // Means none
	  _dropFormId (dF)
{
	assert (_density > __BD 0);
	assert (_speed > 0);
}

// ---
QGAMES::Rain::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _density (__BD 1),
	  _speed (2),
	  _soundChannel (__QGAMES_WHEATHERSOUNDCHANNEL__),
	  _soundId (__QGAMES_RAINWAVSOUND__),
	  _backgroundFormId (-1), // Means none
	  _dropFormId (__QGAMES_DROPINRAINFORM__)
{
	if (prps.find (std::string (__QGAMES_RAINWHEATHERATTRDENSITY__)) != prps.end ())
		_density = __BD (std::atof ((*prps.find (std::string (__QGAMES_RAINWHEATHERATTRDENSITY__))).second.c_str ()));
	if (prps.find (std::string (__QGAMES_RAINWHEATHERATTRSPEED__)) != prps.end ())
		_speed = std::atoi ((*prps.find (std::string (__QGAMES_RAINWHEATHERATTRSPEED__))).second.c_str ());
	if (prps.find (std::string (__QGAMES_RAINWHEATHERATTRSOUNDCHN__)) != prps.end ())
		_soundChannel = std::atoi ((*prps.find (std::string (__QGAMES_RAINWHEATHERATTRSOUNDCHN__))).second.c_str ());
	if (prps.find (std::string (__QGAMES_RAINWHEATHERATTRSOUND__)) != prps.end ())
		_soundId = std::atoi ((*prps.find (std::string (__QGAMES_RAINWHEATHERATTRSOUND__))).second.c_str ());
	if (prps.find (std::string (__QGAMES_RAINWHEATHERATTRBK__)) != prps.end ())
		_backgroundFormId = std::atoi ((*prps.find (std::string (__QGAMES_RAINWHEATHERATTRBK__))).second.c_str ());
	if (prps.find (std::string (__QGAMES_RAINWHEATHERATTRDROP__)) != prps.end ())
		_dropFormId = std::atoi ((*prps.find (std::string (__QGAMES_RAINWHEATHERATTRDROP__))).second.c_str ());

	QGAMES::Rain::Properties::Properties (_density, _speed, _soundChannel, _soundId, _backgroundFormId, _dropFormId);
}

// ---
QGAMES::Rain::Rain (QGAMES::Scene* s, const QGAMES::Rain::Properties& prps)
	: QGAMES::EnvironmentalCondition (__QGAMES_RAINWHEATHER__, s, false /** Affect all time. */),
	  _properties (prps),
	  _densityFactorManager (),
	  _speedFactorManager (),
	  _drops (),
	  _width (0), _height (0) // Set in runtime...
{
	setDensityFactorManager (__BD 0.1, __BD 0.5);
	setSpeedFactorManager (__BD 0.5, __BD 1.0);
}

// ---
void QGAMES::Rain::setProperties (const QGAMES::Rain::Properties& prps)
{
	_properties = prps;

	setDensityFactorManager (__BD 0.1, __BD 0.5);
	setSpeedFactorManager (__BD 0.5, __BD 1.0);
}

// ---
void QGAMES::Rain::setDensityFactorManager (const QGAMES::bdata iF, QGAMES::bdata fF, 
		QGAMES::bdata i, bool up)
{
	_densityFactorManager = 
		QGAMES::DensityFactorManager (_properties._density, iF, fF, (int) _drops .size (), i, up);
	_densityFactorManager.initialize ();
}

// ---
void QGAMES::Rain::setSpeedFactorManager (const QGAMES::bdata iF, QGAMES::bdata fF, 
		QGAMES::bdata i, bool up)
{
	_speedFactorManager = 
		QGAMES::DensityFactorManager (__BD _properties._speed, iF, fF, 1 /** it doesn't matter */, i, up);
	_speedFactorManager.initialize ();
}

// ---
void QGAMES::Rain::initialize ()
{
	QGAMES::EnvironmentalCondition::initialize ();

	QGAMES::Form* drop = game () -> form (_properties._dropFormId);

	// Create the initial drops taking into account the density...
	_drops = QGAMES::Positions ();
	_width = scene () -> activeMap () -> width () ;
	_height = scene () -> activeMap () -> height ();
	int nC = (int) (_width / drop -> width () * _properties._density);
	int nR = (int) (_height / drop -> height () * _properties._density);
	for (int i = 1; i < nC; i++) // The first column is avoided...nothing is put there!
		for (int j = 0; j < nR; j++)
			if ((rand () % 10) < 4) // 40% probability...
				_drops.push_back (QGAMES::Position (i * (drop -> width () / _properties._density), 
					j * (drop -> height () / _properties._density), __BD 0));

	setDensityFactorManager (__BD 0.1, __BD 0.5);
	setSpeedFactorManager (__BD 0.5, __BD 1.0);

	if (_properties._soundId != -1)
		game () -> sound (_properties._soundId) -> play (_properties._soundChannel);

	reStartAllCounters ();
	reStartAllOnOffSwitches ();
}

// ---
void QGAMES::Rain::updatePositions ()
{
	QGAMES::EnvironmentalCondition::updatePositions ();

	if (!onOffSwitch (_SWITCHRAINSTARTS) -> isOn ()) 
	{
		onOffSwitch (_SWITCHRAINSTARTS) -> set (true);
		notify (QGAMES::Event (__QGAMES_RAINSTARTS__, this));
	}

	if (counter (_COUNTERTOMOVEFADE) -> isEnd ())
		counter (_COUNTERTOSHOW) -> isEnd ();

	if (counter (_COUNTERTOINCREMENTDENSITYANDSPEED) -> isEnd ())
	{
		_densityFactorManager.update ();
		_speedFactorManager.update ();
	}

	for (int i = 0; i < (int) _drops.size (); i++)
		if ((_drops [i] += QGAMES::Position (__BD 0, 
				__BD (10 * _properties._speed * _speedFactorManager.currentFactor ()), __BD 0)).posY () > _height)
			_drops [i] = QGAMES::Position (_drops [i].posX () + __BD ((rand () % 100) - 50), 
				__BD (rand () % 100), __BD 0);
}

// ---
void QGAMES::Rain::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::EnvironmentalCondition::drawOn (s, p);

	QGAMES::Form* drop = game () -> form (_properties._dropFormId);
	QGAMES::Form* background = (_properties._backgroundFormId == -1) // Not mandatory...
		? NULL : game () -> form (_properties._backgroundFormId);

	// Get the status of the things to draw...
	const std::vector <bool>& dS = _densityFactorManager.currentThingsStatus (); // Not copy...
	assert (dS.size () == _drops.size ());// Just in case, it should happen, but who knows!
	int fd = counter (_COUNTERTOSHOW) -> value ();
	for (int i = 0; i < (int) _drops.size (); i++)
		if (dS [i]) drop -> drawOn (s, 0, _drops [i], _fades [fd]);
	if (background) // A little bit transparent...
		background -> drawOn (s, 0, s -> position (), (int) (50 * _densityFactorManager.currentFactor ())); 
}

// ---
void QGAMES::Rain::finalize ()
{
	QGAMES::EnvironmentalCondition::finalize ();

	if (_properties._soundId != -1)
		game () -> sound (_properties._soundId) -> stop ();

	notify (QGAMES::Event (__QGAMES_RAINFINISHES__, this));
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::Rain::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::Rain::_COUNTERTOMOVEFADE, 3, 0, true, true));
	addCounter (new QGAMES::Counter 
		(QGAMES::Rain::_COUNTERTOSHOW, 6, 0, true, false));
	addCounter (new QGAMES::Counter 
		(QGAMES::Rain::_COUNTERTOINCREMENTDENSITYANDSPEED, 
			QGAMES::Game::game () -> framesPerSecond () / 2, 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::Rain::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::Rain::_SWITCHRAINSTARTS, false)); 
}

// ---
QGAMES::Earthquake::Properties::Properties (int i, int sCh, int sId)
	: _intensity (i),
	  _soundChannel (sCh),
	  _soundId (sId)
{
	assert (_intensity > 0);
}

// ---
QGAMES::Earthquake::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _intensity (1),
	  _soundChannel (__QGAMES_WHEATHERSOUNDCHANNEL__),
	  _soundId (__QGAMES_EARTHQUAKEWAVSOUND__)
{
	if (prps.find (std::string (__QGAMES_EARTHQUAKEWHEATHERATTRINTEN__)) != prps.end ())
		_intensity = std::atoi ((*prps.find (std::string (__QGAMES_EARTHQUAKEWHEATHERATTRINTEN__))).second.c_str ());
	if (prps.find (std::string (__QGAMES_EARTHQUAKEWHEATHERATTRSDCHN__)) != prps.end ())
		_soundChannel = std::atoi ((*prps.find (std::string (__QGAMES_EARTHQUAKEWHEATHERATTRSDCHN__))).second.c_str ());
	if (prps.find (std::string (__QGAMES_EARTHQUAKEWHEATHERATTRSOUND__)) != prps.end ())
		_soundId = std::atoi ((*prps.find (std::string (__QGAMES_EARTHQUAKEWHEATHERATTRSOUND__))).second.c_str ());

	QGAMES::Earthquake::Properties::Properties (_intensity, _soundChannel, _soundId);
}

// ---
void QGAMES::Earthquake::initialize ()
{
	QGAMES::EnvironmentalCondition::initialize ();

	_lastVector = QGAMES::Position::_cero;

	if (_properties._soundId != -1)
		game () -> sound (_properties._soundId) -> play (_properties._soundChannel);
}

// ---
void QGAMES::Earthquake::updatePositions ()
{
	QGAMES::EnvironmentalCondition::updatePositions ();

	if (!onOffSwitch (_SWITCHEARTHQUAKESTARTS) -> isOn ()) 
	{
		onOffSwitch (_SWITCHEARTHQUAKESTARTS) -> set (true);
		notify (QGAMES::Event (__QGAMES_EARTHQUAKESTARTS__, this));
	}

	QGAMES::Screen* sc = game () -> mainScreen ();
	QGAMES::Vector nV = QGAMES::Vector (__BD ((rand () % 5) * _properties._intensity),
		__BD ((rand () % 5) * _properties._intensity), __BD 0);
	sc -> setPosition (sc -> position () + nV - _lastVector);
	_lastVector = nV;
}

// ---
void QGAMES::Earthquake::finalize ()
{
	QGAMES::EnvironmentalCondition::finalize ();

	QGAMES::Screen* sc = scene () -> game () -> mainScreen ();
	sc -> setPosition (sc -> position () - _lastVector);

	if (_properties._soundId)
		game () -> sound (_properties._soundId) -> stop ();

	notify (QGAMES::Event (__QGAMES_EARTHQUAKEFINISHES__, this));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::Earthquake::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::Earthquake::_SWITCHEARTHQUAKESTARTS, false)); 
}

// ---
QGAMES::Wind::Properties::Properties (QGAMES::bdata d, int sp, int sCh, int sId, int tId)
	: _density (d),
	  _speed (sp),
	  _soundChannel (sCh),
	  _soundId (sId),
	  _traceFormId (tId)
{
	assert (_density > __BD 0);
	assert (_speed > 0);
}

// ---
QGAMES::Wind::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _density (__BD 1),
	  _speed (2),
	  _soundChannel (__QGAMES_WHEATHERSOUNDCHANNEL__),
	  _soundId (__QGAMES_WINDWAVSOUND__),
	  _traceFormId (__QGAMES_WINDTRACEFORM__)
{
	if (prps.find (std::string (__QGAMES_WINDWHEATHERATTRDENSITY__)) != prps.end ())
		_density = __BD (std::atof ((*prps.find (std::string (__QGAMES_WINDWHEATHERATTRDENSITY__))).second.c_str ()));
	if (prps.find (std::string (__QGAMES_WINDWHEATHERATTRSPEED__)) != prps.end ())
		_speed = std::atoi ((*prps.find (std::string (__QGAMES_WINDWHEATHERATTRSPEED__))).second.c_str ());
	if (prps.find (std::string (__QGAMES_WINDWHEATHERATTRSOUNDCHN__)) != prps.end ())
		_soundChannel = std::atoi ((*prps.find (std::string (__QGAMES_WINDWHEATHERATTRSOUNDCHN__))).second.c_str ());
	if (prps.find (std::string (__QGAMES_WINDWHEATHERATTRSOUND__)) != prps.end ())
		_soundId = std::atoi ((*prps.find (std::string (__QGAMES_WINDWHEATHERATTRSOUND__))).second.c_str ());
	if (prps.find (std::string (__QGAMES_WINDWHEATHERATTRTRACE__)) != prps.end ())
		_traceFormId = std::atoi ((*prps.find (std::string (__QGAMES_WINDWHEATHERATTRTRACE__))).second.c_str ());

	QGAMES::Wind::Properties::Properties (_density, _speed, _soundChannel, _soundId, _traceFormId);
}

// ---
QGAMES::Wind::Wind (QGAMES::Scene* s, const QGAMES::Wind::Properties& prps)
	: QGAMES::EnvironmentalCondition (__QGAMES_WINDWHEATHER__, s, false /** Affects all time. */),
	  _properties (prps)
{
	setDensityFactorManager (__BD 0.1, __BD 0.5);
	setSpeedFactorManager (__BD 0.5, __BD 1.0);
}

// ---
void QGAMES::Wind::setProperties (const QGAMES::Wind::Properties& prps)
{
	_properties = prps;

	setDensityFactorManager (__BD 0.1, __BD 0.5);
	setSpeedFactorManager (__BD 0.5, __BD 1.0);
}

// ---
void QGAMES::Wind::setDensityFactorManager (const QGAMES::bdata iF, QGAMES::bdata fF, 
		QGAMES::bdata i, bool up)
{
	_densityFactorManager = 
		QGAMES::DensityFactorManager (_properties._density, iF, fF, (int) _traces.size (), i, up);
	_densityFactorManager.initialize ();
}

// ---
void QGAMES::Wind::setSpeedFactorManager (const QGAMES::bdata iF, QGAMES::bdata fF, 
		QGAMES::bdata i, bool up)
{
	_speedFactorManager = 
		QGAMES::DensityFactorManager (__BD _properties._speed, iF, fF, 1 /** it foesn't matter */, i, up);
	_speedFactorManager.initialize ();
}

// ---
void QGAMES::Wind::initialize ()
{
	QGAMES::EnvironmentalCondition::initialize ();

	QGAMES::Form* trace = game () -> form (_properties._traceFormId);

	_traces = QGAMES::Positions ();
	_width = scene () -> activeMap () -> width () ;
	_height = scene () -> activeMap () -> height ();
	int nC = (int) (_width / trace -> width () * _properties._density);
	int nR = (int) (_height / trace -> height () * _properties._density);
	for (int i = 0; i < nC; i++) // The first row is avoided...nothing is put there!
		for (int j = 1; j < nR; j++)
			if ((rand () % 10) < 4) // 40% probability...
				_traces.push_back (QGAMES::Position (i * (trace -> width () / _properties._density), 
					j * (trace -> height () / _properties._density), __BD 0));

	setDensityFactorManager (__BD 0.1, __BD 0.5);
	setSpeedFactorManager (__BD 0.5, __BD 1.0);

	if (_properties._soundId != -1)
		game () -> sound (_properties._soundId) -> play (_properties._soundChannel);

	reStartAllCounters ();
	reStartAllOnOffSwitches ();
}

// ---
void QGAMES::Wind::updatePositions ()
{
	QGAMES::EnvironmentalCondition::updatePositions ();

	if (!onOffSwitch (_SWITCHWINDSTARTS) -> isOn ()) 
	{
		onOffSwitch (_SWITCHWINDSTARTS) -> set (true);
		notify (QGAMES::Event (__QGAMES_WINDSTARTS__, this));
	}

	if (counter (_COUNTERTOINCREMENTDENSITYANDSPEED) -> isEnd ())
	{
		_densityFactorManager.update ();
		_speedFactorManager.update ();
	}

	if (counter (_COUNTERTOMOVEFADE) -> isEnd ())
		counter (_COUNTERTOSHOW) -> isEnd ();

	QGAMES::Form* trace = game () -> form (_properties._traceFormId);

	for (int i = 0; i < (int) _traces.size (); i++)
		if ((_traces [i] -= QGAMES::Position (__BD (10 * _properties._speed * _speedFactorManager.currentFactor ()), 
				__BD 0, __BD 0)).posX () < - trace -> width ())
			_traces [i] = QGAMES::Position (__BD (_width - rand () % 100), 
				_traces [i].posY () + __BD ((rand () % 100) - 50), __BD 0);
}

// ---
void QGAMES::Wind::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::EnvironmentalCondition::drawOn (s, p);

	QGAMES::Form* trace = game () -> form (_properties._traceFormId);

	const std::vector <bool>& dS = _densityFactorManager.currentThingsStatus (); // Not copy...
	assert (dS.size () == _traces.size ());// Just in case, it should happen, but who knows!
	int fd = counter (_COUNTERTOSHOW) -> value ();
	for (int i = 0; i < (int) _traces.size (); i++)
		if (dS [i]) trace -> drawOn (s, 0, _traces [i], _fades [fd]);
}

// ---
void QGAMES::Wind::finalize ()
{
	QGAMES::EnvironmentalCondition::finalize ();

	if (_properties._soundId != -1)
		game () -> sound (_properties._soundId) -> stop ();

	notify (QGAMES::Event (__QGAMES_WINDFINISHES__, this));
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::Wind::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::Wind::_COUNTERTOMOVEFADE, 3, 0, true, true));
	addCounter (new QGAMES::Counter 
		(QGAMES::Wind::_COUNTERTOSHOW, 6, 0, true, false));
	addCounter (new QGAMES::Counter 
		(QGAMES::Wind::_COUNTERTOINCREMENTDENSITYANDSPEED, 
			QGAMES::Game::game () -> framesPerSecond () / 2, 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::Wind::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::Wind::_SWITCHWINDSTARTS, false)); 
}

// ---
QGAMES::Lightning::Lightning (QGAMES::Scene* s, int sCh, QGAMES::Sound* nS)
	: QGAMES::EnvironmentalCondition (__QGAMES_LIGHTNINGWHEATHER__, s),
	  _thPoints (),
	  _soundChannel (sCh), _sound (NULL)
{
	assert (nS);

	_sound = nS;
}

// ---
QGAMES::Lightning::Lightning (QGAMES::Scene* s, const std::map <std::string, std::string>& prps)
	: QGAMES::EnvironmentalCondition (__QGAMES_LIGHTNINGWHEATHER__, s),
	  _thPoints (),
	  _soundChannel (0), _sound (NULL)
{
	if (prps.find (__QGAMES_LIGHTNINGWHEATHERATTRSDCHN__) != prps.end ())
		_soundChannel = std::atoi ((*prps.find (__QGAMES_LIGHTNINGWHEATHERATTRSDCHN__)).second.c_str ());
	int sId = -1;
	if (prps.find (__QGAMES_LIGHTNINGWHEATHERATTRSOUND__) != prps.end ())
		sId = std::atoi ((*prps.find (__QGAMES_LIGHTNINGWHEATHERATTRSOUND__)).second.c_str ());
	if (sId != -1)
		_sound = game () -> sound (sId);
}

// ---
bool QGAMES::Lightning::hasCollisionWith (QGAMES::Artist* a)
{
	assert (a);
		
	bool result = false;
	QGAMES::Rectangle rct = a -> collisionZone ();
	for (int i = 0; i < (int) (_thPoints.size () - 1) && !result; i++)
		if (rct.intersectsWith (QGAMES::Rectangle (_thPoints [i], _thPoints [i + 1], QGAMES::Vector::_zNormal)))
			result = true; // The light is affecting the artist...

	return (result);
}

// ---
void QGAMES::Lightning::initialize ()
{
	if (_sound) _sound -> play (_soundChannel);

	_thPoints = QGAMES::Lightning::ThunderPoints ();
	QGAMES::Screen* sc = game () -> mainScreen ();
	QGAMES::Position pS (sc -> position ().posX () + __BD (rand () % sc -> visualWidth ()), 
		__BD sc -> position ().posY (), __BD 0); // Initial point at the top of the screen...
	QGAMES::Position pE (sc -> position ().posX () + __BD (rand () % sc -> visualWidth ()), 
		__BD sc -> position ().posY () + sc -> visualHeight (), __BD 0); // End point at the bottom part of the screen...
	QGAMES::bdata vr = (pE - pS).module () * __BD 0.40; 
	_thPoints.push_back (pS);
	builtBolt (pS, pE, _thPoints, vr);

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	// The notification happens after the lightning has been built!
	QGAMES::EnvironmentalCondition::initialize ();
}

// ---
void QGAMES::Lightning::updatePositions ()
{
	QGAMES::EnvironmentalCondition::updatePositions ();

	if (!onOffSwitch (_SWITCHLIGHTNINGSTARTS) -> isOn ())
	{
		onOffSwitch (_SWITCHLIGHTNINGSTARTS) -> set (true);
		notify (QGAMES::Event (__QGAMES_LIGHTNINGSTARTS__, this));
	}

	if (counter (_COUNTERLIGHTNINGON) -> isEnd () &&
		counter (_COUNTERTOFADEOFF) -> isEnd () &&
		counter (_COUNTERFADEOFF) -> isEnd () &&
		!onOffSwitch (_SWITCHLIGHTNINGFINISHES) -> isOn ())
	{
		onOffSwitch (_SWITCHLIGHTNINGFINISHES) -> set (true);
		notify (QGAMES::Event (__QGAMES_LIGHTNINGFINISHES__, this));
	}
}

// ---
void QGAMES::Lightning::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::EnvironmentalCondition::drawOn (s, p);

	QGAMES::Color c (255, 255, 255, _fades [counter (_COUNTERFADEOFF) -> value ()]);
	for (int i = 0; i < (int) (_thPoints.size () - 1); i++)
	{
#ifndef NDEBUG
		s -> drawRectangle (QGAMES::Rectangle (_thPoints [i], _thPoints [i + 1]), __QGAMES_REDCOLOR__, false);
#endif 
		s -> drawLine (_thPoints [i], _thPoints [i + 1], c, 2);
	}
}

// ---
void QGAMES::Lightning::finalize ()
{
	QGAMES::EnvironmentalCondition::finalize ();

	if (_sound) _sound -> stop ();

	if (!onOffSwitch (_SWITCHLIGHTNINGFINISHES) -> isOn ())
	{
		onOffSwitch (_SWITCHLIGHTNINGFINISHES) -> set (true);
		notify (QGAMES::Event (__QGAMES_LIGHTNINGFINISHES__, this));
	}
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::Lightning::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::Lightning::_COUNTERLIGHTNINGON, QGAMES::Game::game () -> framesPerSecond (), 
			0, true, false));
	addCounter (new QGAMES::Counter 
		(QGAMES::Lightning::_COUNTERTOFADEOFF, 3, 0, true, true));
	addCounter (new QGAMES::Counter 
		(QGAMES::Lightning::_COUNTERFADEOFF, 6, 0, true, false));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::Lightning::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::Lightning::_SWITCHLIGHTNINGSTARTS, false)); 
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::Lightning::_SWITCHLIGHTNINGFINISHES, false)); 
}

// ---
// A reference to the web page with the algorithm to set up a lightning!
// http://stackoverflow.com/questions/13730007/create-a-java-point-array-that-moves-randomly-toward-a-defined-end-point
void QGAMES::Lightning::builtBolt (const QGAMES::Position& pS, const QGAMES::Position& pE,
		QGAMES::Lightning::ThunderPoints& pts, QGAMES::bdata vr)
{
	QGAMES::bdata ln = (pE - pS).module ();
	if (ln < 50) // Minimum length to finish in pixels...
		pts.push_back (pE);
	else
	{
		QGAMES::Position pM (__BD (((pS.posX () + pE.posX ()) / 2) + 
			((rand () % ((int) vr * 2)) - (int) vr)),
				__BD ((pS.posY () + pE.posY ()) / 2), __BD 0);
		builtBolt (pS, pM, pts, vr * __BD 0.55);
		builtBolt (pM, pE, pts, vr * __BD 0.55);
	}
}

//---
QGAMES::Lightnings::Properties::Properties (int p, QGAMES::bdata f, int sCh, int sId)
	: _probability (p),
	  _frecuency (f),
	  _soundChannel (sCh),
	  _soundId (sId)
{
	assert (_probability > 0 && _probability < 100);
	assert (_frecuency > __BD 0);

}

// ---
QGAMES::Lightnings::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _probability (50),
	  _frecuency (__BD 1),
	  _soundChannel (__QGAMES_WHEATHERSOUNDCHANNEL__),
	  _soundId (__QGAMES_THUNDERWAVSOUND__)
{
	if (prps.find (std::string (__QGAMES_LIGHTNINGSWHEATHERATTRPROB__)) != prps.end ())
		_probability = std::atoi ((*prps.find (std::string (__QGAMES_LIGHTNINGSWHEATHERATTRPROB__))).second.c_str ());
	if (prps.find (std::string (__QGAMES_LIGHTNINGSWHEATHERATTRFREQ__)) != prps.end ())
		_frecuency = __BD (std::atof ((*prps.find (std::string (__QGAMES_LIGHTNINGSWHEATHERATTRFREQ__))).second.c_str ()));
	if (prps.find (std::string (__QGAMES_LIGHTNINGSWHEATHERATTRSDCHN__)) != prps.end ())
		_soundChannel = std::atoi ((*prps.find (std::string (__QGAMES_LIGHTNINGSWHEATHERATTRSDCHN__))).second.c_str ());
	if (prps.find (std::string (__QGAMES_LIGHTNINGSWHEATHERATTRSOUND__)) != prps.end ())
		_soundId = (std::atoi ((*prps.find (std::string (__QGAMES_LIGHTNINGSWHEATHERATTRSOUND__))).second.c_str ()));

	QGAMES::Lightnings::Properties::Properties (_probability, _frecuency, _soundChannel, _soundId);
}

// ---
QGAMES::Lightnings::Lightnings (QGAMES::Scene* s, const QGAMES::Lightnings::Properties& prps)
	: QGAMES::ComplexEnvironmentalCondition (__QGAMES_LIGHTNINGSWHEATHER__, s, 
		QGAMES::EnvironmentalConditions ()),
	  _properties (prps),
	  _lightning (NULL)
{
	addCondition (_lightning = new QGAMES::Lightning (s, _properties._soundChannel, 
		(_properties._soundId != -1) ? game () -> sound (_properties._soundId) : NULL)); 
}

// ---
void QGAMES::Lightnings::setProperties (const QGAMES::Lightnings::Properties& prps)
{
	_properties = prps;

	counter (_COUNTERTONEXTLIGHTNING) -> 
		initialize ((int) (__BD game () -> framesPerSecond () * _properties._frecuency), 0, true, true);
}

// ---
void QGAMES::Lightnings::initialize ()
{
	QGAMES::ComplexEnvironmentalCondition::initialize ();

	activateCondition (_lightning);

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	counter (_COUNTERTONEXTLIGHTNING) -> 
		initialize ((int) (__BD game () -> framesPerSecond () * _properties._frecuency), 0, true, true);
}

// ---
void QGAMES::Lightnings::updatePositions ()
{
	QGAMES::ComplexEnvironmentalCondition::updatePositions ();

	if (onOffSwitch (_SWITCHCOUNTTONEXTLIGHTNING) -> isOn () &&
		counter (_COUNTERTONEXTLIGHTNING) -> isEnd () &&
		__1IN2PROBABLE__) // When a lightning finishes, 50% probability to start up the next...
	{
		activateCondition (_lightning);
		onOffSwitch (_SWITCHCOUNTTONEXTLIGHTNING) -> set (false);
	}
}

// ---
void QGAMES::Lightnings::finalize ()
{
	QGAMES::ComplexEnvironmentalCondition::finalize ();

	desactivateCondition (_lightning);
}

// ---
void QGAMES::Lightnings::processEvent (const QGAMES::Event& evnt)
{
	switch (evnt.code ())
	{
		case __QGAMES_LIGHTNINGFINISHES__:
			{
				onOffSwitch (_SWITCHCOUNTTONEXTLIGHTNING) -> set (true); 

				desactivateCondition (_lightning);
			}

			break;

		default:
			break;
	}

	notify (evnt);
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::Lightnings::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::Lightnings::_COUNTERTONEXTLIGHTNING, QGAMES::Game::game () -> framesPerSecond (), 
			0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::Lightnings::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::Lightnings::_SWITCHCOUNTTONEXTLIGHTNING, true)); 
}

// ---
QGAMES::Rainbow::Rainbow (QGAMES::Scene* s, QGAMES::bdata sc, QGAMES::Form* rF)
	: QGAMES::EnvironmentalCondition (__QGAMES_RAINBOWWHEATHER__, s),
	  _secondsOnScreen (sc),
	  _rainbow (rF)
{
	assert (_rainbow != NULL);
}

// ---
QGAMES::Rainbow::Rainbow (QGAMES::Scene* s, const std::map <std::string, std::string>& prps)
	: QGAMES::EnvironmentalCondition (__QGAMES_RAINBOWWHEATHER__, s),
	  _secondsOnScreen (__QGAMES_RAINBOWWHEATHERDEFATTRSECS__),
	  _rainbow (NULL)
{
	if (prps.find (__QGAMES_RAINBOWWHEATHERATTRSECONDS__) != prps.end ())
		_secondsOnScreen = __BD (std::atof ((*prps.find (__QGAMES_RAINBOWWHEATHERATTRSECONDS__)).second.c_str ()));

	int rId = __QGAMES_RAINBOWFORM__;
	if (prps.find (__QGAMES_RAINBOWWHEATHERATTRFORM__) != prps.end ())
		rId = std::atoi ((*prps.find (__QGAMES_RAINBOWWHEATHERATTRFORM__)).second.c_str ());

	_rainbow = game () -> form (rId);
}

// ---
void QGAMES::Rainbow::initialize ()
{
	QGAMES::EnvironmentalCondition::initialize ();

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	counter (_COUNTERRAINBOWON) -> 
		initialize ((int) (_secondsOnScreen * game () -> framesPerSecond ()), 0, true, true);
}

// ---
void QGAMES::Rainbow::updatePositions ()
{
	QGAMES::EnvironmentalCondition::updatePositions ();

	if (!onOffSwitch (_SWITCHRAINBOWSTARTS) -> isOn ())
	{
		onOffSwitch (_SWITCHRAINBOWSTARTS) -> set (true);
		notify (QGAMES::Event (__QGAMES_RAINBOWSTARTS__, this));
	}
	
	if (counter (_COUNTERRAINBOWON) -> isEnd () &&
		counter (_COUNTERTOFADEOFF) -> isEnd () && 
		counter (_COUNTERFADEOFF) -> isEnd () &&
		!onOffSwitch (_SWITCHRAINBOWFINISHES) -> isOn ())
	{
		onOffSwitch (_SWITCHRAINBOWFINISHES) -> set (true);
		notify (QGAMES::Event (__QGAMES_RAINBOWFINISHES__, this)); 
	}
}

// ---
void QGAMES::Rainbow::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::EnvironmentalCondition::drawOn (s, p);
	_rainbow -> drawOn (s, 0, QGAMES::Position::_cero, _fades [counter (_COUNTERFADEOFF) -> value ()]);
}

// ---
void QGAMES::Rainbow::finalize ()
{
	QGAMES::EnvironmentalCondition::finalize ();

	if (!onOffSwitch (_SWITCHRAINBOWFINISHES) -> isOn ())
	{
		onOffSwitch (_SWITCHRAINBOWFINISHES) -> set (true);
		notify (QGAMES::Event (__QGAMES_RAINBOWFINISHES__, this)); 
	}
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::Rainbow::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::Rainbow::_COUNTERRAINBOWON, QGAMES::Game::game () -> framesPerSecond () * 3, 
			0, true, false));
	addCounter (new QGAMES::Counter 
		(QGAMES::Rainbow::_COUNTERTOFADEOFF, 3, 0, true, true));
	addCounter (new QGAMES::Counter 
		(QGAMES::Rainbow::_COUNTERFADEOFF, 6, 0, true, false));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::Rainbow::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::Rainbow::_SWITCHRAINBOWSTARTS, false)); 
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::Rainbow::_SWITCHRAINBOWFINISHES, false)); 
}

// ---
QGAMES::Storm::Storm (QGAMES::Scene* s, 
	QGAMES::Thunder* t, QGAMES::Lightning* l, QGAMES::Rain* r, QGAMES::Rainbow* rb)
	: QGAMES::ComplexEnvironmentalCondition (__QGAMES_STORMWHEATHER__, s, 
		QGAMES::EnvironmentalConditions ()),
	  _state (QGAMES::Storm::State::_NONE),
	  _thunder (NULL),
	  _lightning (NULL),
	  _rain (NULL),
	  _rainbow (NULL)
{
	assert (t != NULL && l != NULL && r != NULL && rb != NULL);

	addCondition (_thunder = t);
	addCondition (_lightning = l);
	addCondition (_rain = r); 
	addCondition (_rainbow = rb);
}

// ---
void QGAMES::Storm::initialize ()
{
	QGAMES::ComplexEnvironmentalCondition::initialize ();

	desactivateCondition (_thunder);
	desactivateCondition (_rain);
	desactivateCondition (_lightning);
	desactivateCondition (_rainbow);

	_state = QGAMES::Storm::State::_NONE;

	reStartAllCounters ();
	reStartAllOnOffSwitches ();
}

// ---
void QGAMES::Storm::updatePositions ()
{
	QGAMES::ComplexEnvironmentalCondition::updatePositions ();

	// Is it time to change the state?
	if (counter (_COUNTERTOCHANGESTATE) -> isEnd () &&
		_state != QGAMES::Storm::State::_RAINBOW && // When there is no rainbow (because it finishes alone!
		(rand () % 10) < 4) // 40% probability of changing...
	{
		QGAMES::Storm::State oldState = _state;
		_state = nextState ();
		if (_state == oldState) 
			return; // Nothing has changed...

		// Depending on the new state...
		switch (_state)
		{
			// When nothing happens so far...
			case QGAMES::Storm::State::_NONE:
				{
					// Desactive evrything if it was previously active...
					if (isActive (_thunder)) desactivateCondition (_thunder);
					if (isActive (_lightning)) desactivateCondition (_lightning);
					if (isActive (_rain)) desactivateCondition (_rain);
					if (isActive (_rainbow)) desactivateCondition (_rainbow);
				}

				break; 

			// When it is raining...
			case QGAMES::Storm::State::_SMOOTHRAIN:
			case QGAMES::Storm::State::_RAIN:
				{
					// Desactive evrything if it was previously active, except raning...
					if (isActive (_thunder)) desactivateCondition (_thunder);
					if (isActive (_lightning)) desactivateCondition (_lightning);
					if (isActive (_rainbow)) desactivateCondition (_rainbow);
					if (isActive (_rain))
						_rain -> adaptDensityFactorManager (__BD 0.10, 
							__BD (_state == QGAMES::Storm::State::_SMOOTHRAIN ? 0.5 : 1), __BD 0.1);
					else
					{
						activateCondition (_rain);
						_rain -> setDensityFactorManager (__BD 0.10, 
							__BD (_state == QGAMES::Storm::State::_SMOOTHRAIN ? 0.5 : 1), __BD 0.1);
					}

					_rain -> setSpeedFactorManager (__BD 0.5, __BD 1.0, __BD 0.1);
				}
			
				break;

			// When there is thunders...
			case QGAMES::Storm::State::_THUNDERS:
				{
					// Desactive evrything if it was previously active, except thunders...
					if (isActive (_rain)) desactivateCondition (_rain);
					if (isActive (_lightning)) desactivateCondition (_lightning);
					if (isActive (_rainbow)) desactivateCondition (_rainbow);
					if (!isActive (_thunder)) activateCondition (_thunder);
					else _thunder -> initialize ();
				}

				break;

			// When appart raining there are thunders...
			case QGAMES::Storm::State::_RAINWITHTHUNDERS:
				{
					// Desactive evrything if it was previously active, except thunders and rain...
					if (isActive (_lightning)) desactivateCondition (_lightning);
					if (isActive (_rainbow)) desactivateCondition (_rainbow);
					if (isActive (_rain))
						_rain -> adaptDensityFactorManager (__BD 0.10, __BD 1.0, __BD 0.1);
					else
					{
						activateCondition (_rain);
						_rain -> setDensityFactorManager (__BD 0.10, __BD 1.0, __BD 0.1);
					}

					_rain -> adaptSpeedFactorManager (__BD 0.5, __BD 1.0, __BD 0.1);
				
					if (!isActive (_thunder)) activateCondition (_thunder);
					else _thunder -> initialize ();
				}

				break;

			// When appart raining there are lightnings...
			case QGAMES::Storm::State::_RAINWITHLIGHTNING:
				{
					// Desactive evrything if it was previously active, except lightning and rain...
					if (isActive (_thunder)) desactivateCondition (_thunder);
					if (isActive (_rainbow)) desactivateCondition (_rainbow);
					if (isActive (_rain))
						_rain -> adaptDensityFactorManager (__BD 0.10, __BD 1.0, __BD 0.1);
					else
					{
						activateCondition (_rain);
						_rain -> setDensityFactorManager (__BD 0.10, __BD 1.0, __BD 0.1);
					}

					_rain -> setSpeedFactorManager (__BD 0.5, __BD 1.0, __BD 0.1);
				
					if (!isActive (_lightning)) activateCondition (_lightning);
					else _lightning -> initialize ();
				}

				break;

			// When there is a big rainbow...
			case QGAMES::Storm::State::_RAINBOW:
				{
					// Desactive evrything if it was previously active, except _rainbow...
					if (isActive (_rain)) desactivateCondition (_rain);
					if (isActive (_lightning)) desactivateCondition (_lightning);
					if (isActive (_thunder)) desactivateCondition (_thunder);
					if (!isActive (_rainbow)) activateCondition (_rainbow);
					else _rainbow -> initialize ();
				}

				break;

			default:
				break;
		}
	}
	// Normal running...
	else
	{
		switch (_state)
		{
			// When there are lightnings
			case QGAMES::Storm::State::_RAINWITHLIGHTNING:
				{
					if (onOffSwitch (_SWITCHCOUNTTONEXTLIGHTNING) -> isOn () &&
						counter (_COUNTERTONEXTLIGHTNING) -> isEnd () &&
						rand () % 10 < 4) // When a lightning finishes, 40% probability to start up the next...
					{
						_lightning -> initialize (); // Starts back...
						onOffSwitch (_SWITCHCOUNTTONEXTLIGHTNING) -> set (false);
					}
				}

				break;

			// When there are thunders...
			case QGAMES::Storm::State::_THUNDERS:
			case QGAMES::Storm::State::_RAINWITHTHUNDERS:
				{
					if (onOffSwitch (_SWITCHCOUNTTONEXTTHUNDER) -> isOn () &&
						counter (_COUNTERTONEXTTHUNDER) -> isEnd () &&
						rand () % 10 < 4) // When a thunder finishes, 40% probability to start up the next...
					{
						_thunder -> initialize (); // Starts back...
						onOffSwitch (_SWITCHCOUNTTONEXTTHUNDER) -> set (false);
					}
				}

				break;

			default:
				break;
		}
	}
}

// ---
void QGAMES::Storm::finalize ()
{
	QGAMES::ComplexEnvironmentalCondition::finalize ();

	desactivateCondition (_thunder);
	desactivateCondition (_rain);
	desactivateCondition (_lightning);
	desactivateCondition (_rainbow);
}

// ---
void QGAMES::Storm::processEvent (const QGAMES::Event& evnt)
{
	switch (evnt.code ())
	{
		// Starts back to count down to the next lightning...
		case __QGAMES_LIGHTNINGFINISHES__:
			onOffSwitch (_SWITCHCOUNTTONEXTLIGHTNING) -> set (true); 
			break;

		case __QGAMES_THUNDERFINISHES__:
			_thunder -> initialize (); // Starts back...
			break;

		case __QGAMES_RAINBOWFINISHES__:
			desactivateCondition (_rainbow);
			_state = QGAMES::Storm::State::_NONE;
			break;

		default:
			break;
	}

	// Rethrow the event...
	notify (evnt);
}

// ---
QGAMES::Storm::State QGAMES::Storm::nextState () const
{
	QGAMES::Storm::State result = _state;
	switch (_state)
	{
		case QGAMES::Storm::State::_NONE:
			// 40% probability to start thunders, 60% start to rain...
			result = (rand () % 10 < 4) 
				? QGAMES::Storm::State::_THUNDERS : QGAMES::Storm::State::_SMOOTHRAIN;
			break;

		case QGAMES::Storm::State::_SMOOTHRAIN:
			// 20% probability stop, 24% probability rainbow, 46% start to rain heavy...
			result = (rand () % 10 < 2) 
				? QGAMES::Storm::State::_NONE 
				: (rand () % 10 < 3) ? QGAMES::Storm::State::_RAIN : QGAMES::Storm::State::_RAINBOW;
			break;

		case QGAMES::Storm::State::_RAIN:
			// 20% probability to go back to smooth rain, 80% start to rain with thunders...
			result = (rand () % 10 < 2) 
				? QGAMES::Storm::State::_SMOOTHRAIN : QGAMES::Storm::State::_RAINWITHTHUNDERS;
			break;

		case QGAMES::Storm::State::_THUNDERS:
			// 20% probability stop, 80% start to rain heavyly...
			result = (rand () % 10 < 2) 
				? QGAMES::Storm::State::_NONE : QGAMES::Storm::State::_RAINWITHTHUNDERS;
			break;

		case QGAMES::Storm::State::_RAINWITHTHUNDERS:
			// 20% probability stop, 80% lightnings to start...
			result = (rand () % 10 < 2) 
				? QGAMES::Storm::State::_RAIN : QGAMES::Storm::State::_RAINWITHLIGHTNING;
			break;

		case QGAMES::Storm::State::_RAINWITHLIGHTNING: 
			// Always go back to rain with just only thunders...
			result = QGAMES::Storm::State::_RAINWITHTHUNDERS;
			break;

		case QGAMES::Storm::State::_RAINBOW:
			// Always stop...
			result = QGAMES::Storm::State::_NONE;
			break;
	}

	return (result);
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::Storm::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::Storm::_COUNTERTOCHANGESTATE, QGAMES::Game::game () -> framesPerSecond () * 5 /* seconds. */, 
			0, true, true));
	addCounter (new QGAMES::Counter 
		(QGAMES::Storm::_COUNTERTONEXTLIGHTNING, QGAMES::Game::game () -> framesPerSecond (), 
			0, true, true));
	addCounter (new QGAMES::Counter 
		(QGAMES::Storm::_COUNTERTONEXTTHUNDER, QGAMES::Game::game () -> framesPerSecond (), 
			0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::Storm::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::Storm::_SWITCHCOUNTTONEXTLIGHTNING, false)); 
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::Storm::_SWITCHCOUNTTONEXTTHUNDER, false)); 
}

// ---
QGAMES::Clouds::Properties::Properties (QGAMES::bdata d, int sp, int sCh, int sId, int cF1, int cF2, int cF3)
	: _density (d),
	  _speed (sp),
	  _soundChannel (sCh),
	  _soundId (sId),
	  _cloudFormId1 (cF1), _cloudFormId2 (cF2), _cloudFormId3 (cF3)
{
	assert (_density > __BD 0);
	assert (_speed > 0);
}

// ---
QGAMES::Clouds::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _density (__BD 1),
	  _speed (2),
	  _soundChannel (__QGAMES_WHEATHERSOUNDCHANNEL__),
	  _soundId (__QGAMES_WINDWAVSOUND__),
	  _cloudFormId1 (__QGAMES_CLOUDS1FORM__), 
	  _cloudFormId2 (__QGAMES_CLOUDS2FORM__),
	  _cloudFormId3 (__QGAMES_CLOUDS3FORM__)
{
	if (prps.find (std::string (__QGAMES_CLOUDSWHEATHERATTRDENSITY__)) != prps.end ())
		_density = __BD (std::atof ((*prps.find (std::string (__QGAMES_CLOUDSWHEATHERATTRDENSITY__))).second.c_str ()));
	if (prps.find (std::string (__QGAMES_CLOUDSWHEATHERATTRSPEED__)) != prps.end ())
		_speed = std::atoi ((*prps.find (std::string (__QGAMES_CLOUDSWHEATHERATTRSPEED__))).second.c_str ());
	if (prps.find (std::string (__QGAMES_CLOUDSWHEATHERATTRSOUNDCHN__)) != prps.end ())
		_soundChannel = std::atoi ((*prps.find (std::string (__QGAMES_CLOUDSWHEATHERATTRSOUNDCHN__))).second.c_str ());
	if (prps.find (std::string (__QGAMES_CLOUDSWHEATHERATTRSOUND__)) != prps.end ())
		_soundId = std::atoi ((*prps.find (std::string (__QGAMES_CLOUDSWHEATHERATTRSOUND__))).second.c_str ());
	std::vector <int> fC = 
		{ __QGAMES_CLOUDS1FORM__, __QGAMES_CLOUDS2FORM__, __QGAMES_CLOUDS3FORM__ }; // Since C++11
	if (prps.find (std::string (__QGAMES_CLOUDSWHEATHERATTRCLOUDS__)) != prps.end ())
		fC = QGAMES::getElementsFromAsInt (((*prps.find 
			(std::string (__QGAMES_CLOUDSWHEATHERATTRCLOUDS__))).second.c_str ()), ',', 3);
	_cloudFormId1 = fC [0]; _cloudFormId2 = fC [1]; _cloudFormId3 = fC [2];

	QGAMES::Clouds::Properties (_density, _speed, _soundChannel, _soundId, 
		_cloudFormId1, _cloudFormId2, _cloudFormId3);
}

// ---
QGAMES::Clouds::Clouds (QGAMES::Scene* s, const QGAMES::Clouds::Properties& prps)
	: QGAMES::EnvironmentalCondition (__QGAMES_CLOUDSWHEATHER__, s),
	  _properties (prps),
	  _clouds (),
	  _densityFactorManager (),
	  _speedFactorManager (),
	  _width (0), _height (0), // Set at runtime...
	  _formWidth (0), _formHeight (0), // Set at runtime as well...
	  _cloudForms (),
	  _formToDraw (0)
{
	_cloudForms [0] = game () -> form (_properties._cloudFormId1);
	_cloudForms [1] = game () -> form (_properties._cloudFormId2);
	_cloudForms [2] = game () -> form (_properties._cloudFormId1);

	_formToDraw = rand () % 3; // Form to be drawn...

	setDensityFactorManager (__BD 0.1, __BD 0.5);
	setSpeedFactorManager (__BD 0.5, __BD 1.0);
}

// ---
void QGAMES::Clouds::setDensityFactorManager (const QGAMES::bdata iF, QGAMES::bdata fF, 
		QGAMES::bdata i, bool up)
{
	_densityFactorManager = 
		QGAMES::DensityFactorManager (_properties._density, iF, fF, (int) _clouds.size (), i, up);
	_densityFactorManager.initialize ();
}

// ---
void QGAMES::Clouds::setSpeedFactorManager (const QGAMES::bdata iF, QGAMES::bdata fF, 
		QGAMES::bdata i, bool up)
{
	_speedFactorManager = 
		QGAMES::DensityFactorManager (__BD _properties._speed, iF, fF, 1 /** it foesn't matter */, i, up);
	_speedFactorManager.initialize ();
}

// ---
void QGAMES::Clouds::initialize ()
{
	QGAMES::EnvironmentalCondition::initialize ();

	_formWidth = (_cloudForms [0] -> width () + _cloudForms [1] -> width () 
		+ _cloudForms [2] -> width ()) / 3;
	_formHeight = (_cloudForms [0] -> height () + _cloudForms [1] -> height () 
		+ _cloudForms [2] -> height ()) / 3;
	_formToDraw = rand () % 3;

	_clouds = QGAMES::Positions ();
	_width = scene () -> activeMap () -> width () ;
	_height = scene () -> activeMap () -> height ();
	int nC = (int) (__BD _width / __BD _formWidth * _properties._density);
	int nR = (int) (__BD _height / __BD _formHeight * _properties._density);
	for (int i = 0; i < nC; i++) 
		for (int j = 0; j < nR; j++)
			if ((rand () % 10) < 4) // 40% probability of putting a cloud!
				_clouds.push_back (QGAMES::Position (i * (_formWidth / _properties._density), 
					j * (_formHeight / _properties._density), __BD 0));

	setDensityFactorManager (__BD 0.1, __BD 0.5);
	setSpeedFactorManager (__BD 0.5, __BD 1.0);

	if (_properties._soundId != -1)
		game () -> sound (_properties._soundId) -> play (_properties._soundChannel);

	reStartAllCounters ();
	reStartAllOnOffSwitches ();
}

// ---
void QGAMES::Clouds::updatePositions ()
{
	QGAMES::EnvironmentalCondition::updatePositions ();

	if (!onOffSwitch (_SWITCHCLOUDSSTARTS) -> isOn ()) 
	{
		onOffSwitch (_SWITCHCLOUDSSTARTS) -> set (true);
		notify (QGAMES::Event (__QGAMES_CLOUDSSTARTS__, this));
	}

	if (counter (_COUNTERTOINCREMENTDENSITYANDSPEED) -> isEnd ())
	{
		_densityFactorManager.update ();
		_speedFactorManager.update ();
	}

	if (counter (_COUNTERTOMOVEFADE) -> isEnd ())
		counter (_COUNTERTOSHOW) -> isEnd ();

	for (int i = 0; i < (int) _clouds.size (); i++)
		if ((_clouds [i] -= QGAMES::Position (__BD (_properties._speed * _speedFactorManager.currentFactor ()), 
				__BD 0, __BD 0)).posX () < -_formWidth)
			_clouds [i] = QGAMES::Position (__BD (_width - rand () % 100), 
				_clouds [i].posY () + __BD ((rand () % 100) - 50), __BD 0);
}

// ---
void QGAMES::Clouds::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::EnvironmentalCondition::drawOn (s, p);

	// Get the status of the this to draw...
	const std::vector <bool>& dS = _densityFactorManager.currentThingsStatus (); // Not copy...
	assert (dS.size () == _clouds.size ());// Just in case, it should happen, but who knows!
	int fd = counter (_COUNTERTOSHOW) -> value ();
	for (int i = 0; i < (int) _clouds.size (); i++)
		if (dS [i]) _cloudForms [_formToDraw] -> drawOn (s, 0, _clouds [i], _fades [fd]);
}

// ---
void QGAMES::Clouds::finalize ()
{
	QGAMES::EnvironmentalCondition::finalize ();

	if (_properties._soundId != -1)
		game () -> sound (_properties._soundId) -> stop ();

	notify (QGAMES::Event (__QGAMES_CLOUDSFINISHES__, this));
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::Clouds::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::Clouds::_COUNTERTOMOVEFADE, 3, 0, true, true));
	addCounter (new QGAMES::Counter 
		(QGAMES::Clouds::_COUNTERTOSHOW, 6, 0, true, false));
	addCounter (new QGAMES::Counter 
		(QGAMES::Clouds::_COUNTERTOINCREMENTDENSITYANDSPEED, 
			QGAMES::Game::game () -> framesPerSecond () / 2, 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::Clouds::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::Clouds::_SWITCHCLOUDSSTARTS, false)); 
}
