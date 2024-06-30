#include <BattleshipLike/bsactionblocks.hpp>
#include <BattleshipLike/bsscenes.hpp>
#include <BattleshipLike/bsgame.hpp>

// ---
const int BATTLESHIP::StdSpaceElementsSceneActionBlock::_FADEGRADES [18] =
	{ 0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255 };

// ---
BATTLESHIP::WaitTimeActionBlock::WaitTimeActionBlock (int id, const QGAMES::SceneActionBlockProperties& prps)
	: BATTLESHIP::SceneActionBlock (id, prps),
	  _timeToWait (__BD 1 /** second by default. */)
{
	if (prps.find (std::string (__BATTLESHIP_WAITTIMEBLOCKATTRSECONDSTOWAIT__)) != prps.end ())
		_timeToWait = __BD (std::atof ((*prps.find (__BATTLESHIP_WAITTIMEBLOCKATTRSECONDSTOWAIT__)).second.c_str ()));
}

//---
void BATTLESHIP::WaitTimeActionBlock::initialize ()
{
	BATTLESHIP::SceneActionBlock::initialize ();

	reStartAllCounters ();

	counter (_COUNTERTIME) -> initialize ((int) (_timeToWait * __BD game () -> framesPerSecond ()), 0, true, false);
}

// ---
void BATTLESHIP::WaitTimeActionBlock::updatePositions ()
{
	BATTLESHIP::SceneActionBlock::updatePositions ();

	BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (bG); // Just in case...
	if (bG -> isGamePaused () || bG -> isNotPlaying ())
		return; 

	if (counter (_COUNTERTIME) -> isEnd ())
		notify (QGAMES::Event (__BATTLESHIP_ACTIONBLOCKISOVER__, this));
}

// ---
__IMPLEMENTCOUNTERS__ (BATTLESHIP::WaitTimeActionBlock::Counters)
{
	addCounter (new QGAMES::Counter (BATTLESHIP::WaitTimeActionBlock::_COUNTERTIME, 
		0 /** It will be set later. */, 0, true, true)); 
}

// ---
BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties::Properties 
		(QGAMES::bdata tA, int p, bool tE, QGAMES::bdata tM, bool r, bool rll, 
		 const QGAMES::Position& pos, const QGAMES::Rectangle& mZ, QGAMES::bdata mTA)
	: _timeToAppear (tA),
	  _probabilityToAppear (p),
	  _toEmerge (tE),
	  _timeToMove (tM),
	  _regenerate (r),
	  _roll (rll),
	  _initialPosition (pos),
	  _movementZone (mZ),
	  _maxTimeAction (mTA)
{ 
	assert (_probabilityToAppear >= 0 && _probabilityToAppear <= 100);
	assert (_maxTimeAction == __BD -1 || _maxTimeAction > __BD 0);
	assert ((_regenerate != _roll) || (!_regenerate && !_roll)); // Or different or both equal to false...
}

// ---
BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties::Properties 
	(const std::map <std::string, std::string>& prps)
	: _timeToAppear (__BD 2),
	  _probabilityToAppear (100),
	  _toEmerge (false),
	  _timeToMove (__BD 0),
	  _regenerate (true),
	  _roll (false),
	  _initialPosition (QGAMES::Position::_noPoint),
	  _maxTimeAction (__BD -1)	
{
	if (prps.find (std::string (__BATTLESHIP_SPACEELMNTBLOCKATTRSECONDSTOAPPEAR__)) != prps.end ())
		_timeToAppear = __BD (std::atof ((*prps.find (__BATTLESHIP_SPACEELMNTBLOCKATTRSECONDSTOAPPEAR__)).second.c_str ()));
	if (prps.find (std::string (__BATTLESHIP_SPACEELMNTBLOCKATTRPRBTOAPPEAR__)) != prps.end ())
		_probabilityToAppear = std::atoi ((*prps.find (__BATTLESHIP_SPACEELMNTBLOCKATTRPRBTOAPPEAR__)).second.c_str ());
	if (prps.find (std::string (__BATTLESHIP_SPACEELMNTBLOCKATTRTOEMERGE__)) != prps.end ())
		_toEmerge = (*prps.find (__BATTLESHIP_SPACEELMNTBLOCKATTRTOEMERGE__)).second.c_str () == std::string (__YES_STRING__);
	if (prps.find (std::string (__BATTLESHIP_SPACEELMNTBLOCKATTRTOMOVE__)) != prps.end ())
		_timeToMove = __BD (std::atof ((*prps.find (__BATTLESHIP_SPACEELMNTBLOCKATTRTOMOVE__)).second.c_str ()));
	if (prps.find (std::string (__BATTLESHIP_SPACEELMNTBLOCKATTRREGENERATE__)) != prps.end ())
		_regenerate = (*prps.find (__BATTLESHIP_SPACEELMNTBLOCKATTRREGENERATE__)).second.c_str () == std::string (__YES_STRING__);
	if (prps.find (std::string (__BATTLESHIP_SPACEELMNTBLOCKATTRROLL__)) != prps.end ())
		_roll = (*prps.find (__BATTLESHIP_SPACEELMNTBLOCKATTRROLL__)).second.c_str () == std::string (__YES_STRING__);
	if (prps.find (std::string (__BATTLESHIP_SPACEELMNTBLOCKATTRINITPOSITION__)) != prps.end ())
		_initialPosition = QGAMES::Position ((*prps.find (__BATTLESHIP_SPACEELMNTBLOCKATTRINITPOSITION__)).second.c_str ());
	if (prps.find (std::string (__BATTLESHIP_SPACEELMNTBLOCKATTRMOVZONE__)) != prps.end ())
		_movementZone = QGAMES::Rectangle ((*prps.find (__BATTLESHIP_SPACEELMNTBLOCKATTRMOVZONE__)).second.c_str ());
	if (prps.find (std::string (__BATTLESHIP_SPACEELMNTBLOCKATTRMAXTIMEACTION__)) != prps.end ())
		_maxTimeAction = __BD (std::atof ((*prps.find (__BATTLESHIP_SPACEELMNTBLOCKATTRMAXTIMEACTION__)).second.c_str ()));

	// To validate...
	BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties 
		(_timeToAppear, _probabilityToAppear, _toEmerge, _timeToMove, _regenerate, _roll, 
		 _initialPosition, _movementZone, _maxTimeAction); 
}

// ---
QGAMES::SetOfOpenValues BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));

	result.addOpenValue (0, QGAMES::OpenValue (_timeToAppear));
	result.addOpenValue (1, QGAMES::OpenValue (_probabilityToAppear));
	result.addOpenValue (2, QGAMES::OpenValue (_toEmerge));
	result.addOpenValue (3, QGAMES::OpenValue (_timeToMove));
	result.addOpenValue (4, QGAMES::OpenValue (_regenerate));
	result.addOpenValue (5, QGAMES::OpenValue (_roll));
	std::stringstream sS; sS << _initialPosition;
	result.addOpenValue (6, QGAMES::OpenValue (sS.str ()));
	result.addOpenValue (7, QGAMES::OpenValue (_maxTimeAction));

	return (result);
}

// ---
void BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties::fromSetOfOpenValues 
	(const QGAMES::SetOfOpenValues& oV)
{
	assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));
	assert (oV.existOpenValue (0) && oV.existOpenValue (1) && oV.existOpenValue (2) &&
			oV.existOpenValue (3) && oV.existOpenValue (4) && oV.existOpenValue (5) &&
			oV.existOpenValue (6));

	_timeToAppear = oV.openValue (0).bdataValue ();
	_probabilityToAppear = oV.openValue (1).intValue ();
	_toEmerge = oV.openValue (2).boolValue ();
	_timeToMove = oV.openValue (3).bdataValue ();
	_regenerate = oV.openValue (4).boolValue ();
	_roll = oV.openValue (5).boolValue ();
	_initialPosition = QGAMES::Position (oV.openValue (6).strValue ());
	_maxTimeAction = oV.openValue (7).bdataValue ();
}

// ---
QGAMES::Position BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties::absInitialPositionFor 
	(BATTLESHIP::SpaceElement* spc)
{
	assert (spc);

	int fW, fH, fD;
	spc -> maxVisualDimensions (fW, fH, fD);
	QGAMES::Rectangle r = (_movementZone == QGAMES::Rectangle::_noRectangle) ? spc -> getMovingZone () : _movementZone;

	return (r.pos1 () + 
				(_initialPosition == QGAMES::Position::_noPoint // exists?
				? QGAMES::Vector (__BD ((rand () % ((int) r.width () - (fW >> 1))) + (fW >> 1)), __BD (fH >> 1), __BD 0)
				: QGAMES::Position (r.width () * _initialPosition.posX (),
							 		r.height () * _initialPosition.posY (), __BD 0)));
}

// ---
BATTLESHIP::StdSpaceElementsSceneActionBlock::StdSpaceElementsSceneActionBlock (int id,
		BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties* prps)
	: BATTLESHIP::SceneActionBlock (id),
	  _spaceElements (), // The action block is not the owner ever...
	  _properties (prps)
{
	assert (_properties != NULL);

	addBuoy (_FINALIZESPACEELEMENTBUOY, new BATTLESHIP::StdSpaceElementsSceneActionBlock::ToFinalizeSpaceElement);
}
	
// ---
void BATTLESHIP::StdSpaceElementsSceneActionBlock::setProperties 
	(BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties* prps)
{
	assert (prps != NULL);

	delete (_properties);

	_properties = prps;
}

// ---
QGAMES::SetOfOpenValues BATTLESHIP::StdSpaceElementsSceneActionBlock::runtimeValues () const
{
	assert (_properties);

	QGAMES::SetOfOpenValues result = BATTLESHIP::SceneActionBlock::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	result.addSetOfOpenValues (lNE + 1, _properties -> asSetOfOpenValues ());

	// Space elements' information if any...
	result.addOpenValue (lE + 1, QGAMES::OpenValue ((int) _spaceElements.size ()));
	QGAMES::SetOfOpenValues sEs;
	for (int i = 0; i < (int) _spaceElements.size (); i++)
	{
		QGAMES::SetOfOpenValues sE; 
		sE.addOpenValue (0, _spaceElements [i] -> id ()); // The element used...
		sE.addSetOfOpenValues (0, _spaceElements [i] -> runtimeValues ()); // ...and its data
		sEs.addSetOfOpenValues (i, sE);

		/** It is important to keep the d of the elements used 
			in the block, because things like the movement id are direclty related with the element itself. */
	}

	result.addSetOfOpenValues (lNE + 2, sEs);

	return (result);
}

// ---
void BATTLESHIP::StdSpaceElementsSceneActionBlock::initializeRuntimeValuesFrom 
	(const QGAMES::SetOfOpenValues& cfg)
{
	assert (_properties);

	int lE = cfg.lastOpenValueId ();
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existOpenValue (lE) && 
			cfg.existSetOfOpenValues (lNE) && cfg.existSetOfOpenValues (lNE - 1));

	QGAMES::SetOfOpenValues cCfg = cfg;

	// Gets space's element information if any...
	int nSE = cCfg.openValue (lE).intValue ();
	cCfg.removeOpenValue (lE);
	if (nSE != 0) 
	{
		std::vector <int> sEIds (nSE);
		QGAMES::SetOfOpenValues sEs = cCfg.setOfOpenValues (lNE);
		for (int i = 0; i < nSE; i++)
			sEIds [i] = sEs.setOfOpenValues (i).openValue (0).intValue (); // The id of the space element got...

		getAndConfigureSpaceElements (sEIds); // With the right id to recover...
		assert (nSE == (int) _spaceElements.size ()); // Just in case...
		std::for_each (_spaceElements.begin (), _spaceElements.end (),
				[=](BATTLESHIP::SpaceElement* i) { i -> toFreeze (); observe (i); });

		for (int i = 0; i < nSE; i++)
			_spaceElements [i] -> initializeRuntimeValuesFrom (sEs.setOfOpenValues (i).setOfOpenValues (0));
	}

	cCfg.removeSetOfOpenValues (lNE);

	_properties -> fromSetOfOpenValues (cCfg.setOfOpenValues (lNE - 1));
	cCfg.removeSetOfOpenValues (lNE - 1);

	BATTLESHIP::SceneActionBlock::initializeRuntimeValuesFrom (cCfg);
}

// ---
void BATTLESHIP::StdSpaceElementsSceneActionBlock::initialize ()
{
	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	_spaceElements = BATTLESHIP::SpaceElements (); // Not defined so far...

	BATTLESHIP::SceneActionBlock::initialize ();

	// Even if the seconds have been saved, 
	// they are set back to the quantity defined in the configuration...
	counter (_COUNTERTOAPPEAR) -> initialize 
		((int) (((_properties -> _timeToAppear < __BD 0) 
			? __BD ((rand () % (int) - _properties -> _timeToAppear) + __BD 1)
			: _properties -> _timeToAppear) * __BD game () -> framesPerSecond ()), 0, true, true);

	// Same with the time to move...
	counter (_COUNTERTOMOVE) -> initialize 
		((int) (((_properties -> _timeToMove < __BD 0) 
			? __BD ((rand () % (int) - _properties -> _timeToMove) + __BD 1)
			: _properties -> _timeToMove) * __BD game () -> framesPerSecond ()), 0, true, true);

	// Same with the maximum time the block will be active
	if (_properties -> _maxTimeAction != __BD -1)
		counter (_COUNTERMAXTIMEACTION) -> initialize
			((int) (_properties -> _maxTimeAction * __BD game () -> framesPerSecond ()), 0, true, false); // one way counter...

	// and the switch, set back to false..
	onOffSwitch (_SWITCHELEMENTSINSCENE) -> set (false);
}

// ---
void BATTLESHIP::StdSpaceElementsSceneActionBlock::updatePositions ()
{
	BATTLESHIP::SceneActionBlock::updatePositions ();

	if (!onOffSwitch (_SWITCHELEMENTSUNDERCONTROL) -> isOn ())
		return; // No continue...

	// State 1...Time to appear?
	if ((!onOffSwitch (_SWITCHELEMENTSINSCENE) -> isOn () && // There is none already in the scene...
		 counter (_COUNTERTOAPPEAR) -> isEnd () &&  // ..and it is the time to appear...
		 (__RANDOM__ (100) < _properties -> _probabilityToAppear)) || // ...and the probability is the right one...
		(onOffSwitch (_SWITCHELEMENTSINSCENE) -> isOn () && 
		 _spaceElements.empty () && onOffSwitch (_SWITCHELEMENTSUNDERCONTROL) -> isOn ())) // or in the screen, but nothing really 
																						  // (because it comes from any internal value saved e.g)...
	{
		if (!_spaceElements.empty ())
			std::for_each (_spaceElements.begin (), _spaceElements.end (), 
				[=](BATTLESHIP::SpaceElement* i) { unObserve (i); });

		getAndConfigureSpaceElements (); // The first free...

		std::for_each (_spaceElements.begin (), _spaceElements.end (),
				[=](BATTLESHIP::SpaceElement* i) { i -> toFreeze (); observe (i); });

		onOffSwitch (_SWITCHELEMENTSINSCENE) -> set (true);
		onOffSwitch (_SWITCHELEMENTSEMERGING) -> set (_properties -> _toEmerge);
		onOffSwitch (_SWITCHELEMENTSTOMOVE) -> set (!_properties -> _toEmerge);
		onOffSwitch (_SWITCHELEMENTSMOVING) -> set (false);

		notify (QGAMES::Event (__BATTLESHIP_SPACEELEMENTACTIONBLOCKELMNTCREATED__, this));

		if (_properties -> _timeToAppear < __BD 0)
			counter (_COUNTERTOAPPEAR) -> initialize 
				((int) (((rand () % (int) -_properties -> _timeToAppear) + 1) * __BD game () -> framesPerSecond ()), 0, true, true);
	}

	// State 2...Emerging?
	if (onOffSwitch (_SWITCHELEMENTSINSCENE) -> isOn () && 
		onOffSwitch (_SWITCHELEMENTSEMERGING) -> isOn ())
	{
		if (counter (_COUNTEREMERGINGGRADE) -> isEnd ())
		{
			onOffSwitch (_SWITCHELEMENTSEMERGING) -> set (false);
			onOffSwitch (_SWITCHELEMENTSTOMOVE) -> set (true);
			onOffSwitch (_SWITCHELEMENTSMOVING) -> set (false);

			notify (QGAMES::Event (__BATTLESHIP_SPACEELEMENTACTIONBLOCKELMNTMEMERGED__, this));
		}
		else
			std::for_each (_spaceElements.begin (), _spaceElements.end (),
				[=](BATTLESHIP::SpaceElement* i) 
					{ i -> setAlphaLevel (_FADEGRADES [counter (_COUNTEREMERGINGGRADE) -> value ()]); });
	}

	// State 3...Moving finally!
	if (onOffSwitch (_SWITCHELEMENTSINSCENE) -> isOn () &&
		onOffSwitch (_SWITCHELEMENTSTOMOVE) -> isOn () &&
		!onOffSwitch (_SWITCHELEMENTSMOVING) -> isOn () &&
		counter (_COUNTERTOMOVE) -> isEnd ())
	{
		if (_properties -> _timeToMove < __BD 0)
			counter (_COUNTERTOMOVE) -> initialize 
				((int) (((rand () % (int) -_properties -> _timeToMove) + 1) * __BD game () -> framesPerSecond ()), 0, true, true);

		onOffSwitch (_SWITCHELEMENTSTOMOVE) -> set (false);
		onOffSwitch (_SWITCHELEMENTSMOVING) -> set (true);

		notify (QGAMES::Event (__BATTLESHIP_SPACEELEMENTACTIONBLOCKELMNTMOVING__, this));

		startToMoveSpaceElements ();
	}

	// If paused or on hold, no more thing to do...(speacilly to count down the time)
	BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (bG); // Just in case...
	if (bG -> isGamePaused () || bG -> isNotPlaying ())
		return; 

	// When the time of the action block is over (if any) a notification is sent up!
	// Just when there is no more elements to move!
	if (_properties -> _maxTimeAction != __BD -1 && counter (_COUNTERMAXTIMEACTION) -> isEnd () && _spaceElements.empty ())
		notify (QGAMES::Event (__BATTLESHIP_SPACEELEMENTACTIONBLOCKTIMEOVER__, this));
}

// ---
void BATTLESHIP::StdSpaceElementsSceneActionBlock::finalize ()
{
	BATTLESHIP::SceneActionBlock::finalize ();

	std::for_each (_spaceElements.begin (), _spaceElements.end (),
		[=](BATTLESHIP::SpaceElement* i) { (i) -> setVisible (false); unObserve (i); });
	_spaceElements.clear (); // No longer needed, the action is not the owner...
}

// ---
void BATTLESHIP::StdSpaceElementsSceneActionBlock::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __BATTLESHIP_SPACEELEMENTOUTOFSCENE__ && _properties -> _roll)
	{
		BATTLESHIP::SpaceElement* spc = static_cast <BATTLESHIP::SpaceElement*> (evnt.data ());
		assert (spc); // Just in case...

		// The element is out of the scene, but it has to be rolled to any other part
		// That new position is calculated following the opposite direction of the current trend...
		// up to the intersection with the moving zone (but taking into account what the elements mesures)
		QGAMES::Rectangle mZ = spc -> getMovingZone (); // Includes the max dimensions...

		// When there are abrupts changes in the direction followed by the ufo
		// A not expected limit can be reached. 
		// In that situations the trend (calculated before the movement changed) could indicate another way!
		// In case of discrepancies between orientation and trend, orientation is stronger!

		// To determine next position when a moving zone limit has been reached and rolling is needed
		// the direction followed is used, getting the opposite sid of the moving zone
		// but two special situations could happen:
		// 1.- The down or up limit have been reached, and the trend is horizontal
		// 2.- The right or left limit have been reached, and the trend is vertical
		// These two cases are strange actually, but the calculation could generate these unexpectd results!

		QGAMES::Vector trend = QGAMES::Vector::_cero;
		if (spc -> trendDirection () == QGAMES::Vector::_cero)
		{
			if (spc -> orientation () != QGAMES::Vector::_cero)
				trend = spc -> orientation ();
		}
		else
		if (spc -> orientation () == QGAMES::Vector::_cero)
			trend = spc -> trendDirection ();
		// None is 0!
		else
		{
			QGAMES::bdata a = spc -> trendDirection ().angleWith (spc -> orientation ());
			trend = (a >= __BD 0 && a <= (__PI / __BD 2)) ? spc -> trendDirection () : spc -> orientation ();
		}

		// Really a new position has to be calculated, and is it possible?
		QGAMES::Position nPos = spc -> position ();
		if (!mZ.hasIn (nPos + trend))
		{
			QGAMES::Vector nTrend = QGAMES::Vector (trend).normalize (); // That trend normalized!

			// The limit reached is the one in the bottom or the one in the top, and the trend is horizontal...
			// Then a position in the left, or right is choosen!
			if ((spc -> position ().posY () == mZ.pos2 ().posY () || spc -> position ().posY () == mZ.pos1 ().posY ()) &&
				(nTrend == QGAMES::Vector::_xNormal || nTrend == -QGAMES::Vector::_xNormal))
				nPos = QGAMES::Position ((nTrend == QGAMES::Vector::_xNormal) 
					? mZ.pos1 ().posX () : mZ.pos2 ().posX (),(mZ.pos1 ().posY () + mZ.pos2 ().posY ()) / __BD 2, __BD 0); 
			// The limit reached is the on on the right or the one on the left and the trend is vertical...
			// The a position in the top or the bottom is choosen!
			else
			if ((spc -> position ().posX () == mZ.pos2 ().posX () || spc -> position ().posX () == mZ.pos1 ().posX ()) &&
				(nTrend == QGAMES::Vector::_yNormal || nTrend == -QGAMES::Vector::_yNormal))
				nPos = QGAMES::Position ((mZ.pos1 ().posX () + mZ.pos2 ().posX ()) / __BD 2, 
					(nTrend == QGAMES::Vector::_yNormal) ? mZ.pos1 ().posY () : mZ.pos2 ().posY (), __BD 0);
			// Other circunstances...
			else
			{
				// The one choosen is the most away one!
				QGAMES::Positions pos = mZ.intersectionWith (spc -> position (), nTrend); 
				assert ((int) pos.size () == 2); 
				if (pos [0] != pos [1])
					nPos = ((pos [0] - spc -> position ()).module2 () > 
							(pos [1] - spc -> position ()).module2 ()) ? pos [0] : pos [1];
				// ...unless they are the same, what means the spaceship is in a corner, 
				// so the other corner has to be selected!
				else
				{
					if (pos [0] == mZ.pos1 () || pos [0] == mZ.pos2 ())	
						nPos = (nTrend.posX () > __BD 0) ? mZ.pos4 () : mZ.pos3 ();
					else if (pos [0] == mZ.pos3 () || pos [0] == mZ.pos4 ())
						nPos = (nTrend.posX () > __BD 0) ? mZ.pos1 () : mZ.pos2 ();
					else
						assert (0); // Big mistakes in calculations, so it shouldn't be here at all!
				}
			}

			spc -> restartTrendDirection ();
		}

		assert (mZ.hasIn (nPos + trend)); // Just to be sure...

		spc -> setVisible (true);
		spc -> setPosition (nPos);
		spc -> toForgetDeferreStates ();
	}
	else
	if (evnt.code () == __BATTLESHIP_SPACEELEMENTOUTOFSCENE__ || 
		evnt.code () == __BATTLESHIP_SPACEELEMENTCAUGHT__ ||
		evnt.code () == __BATTLESHIP_SPACEELMNTHASDESTROYED__)
	{
		// The event comes always from a space element...
		BATTLESHIP::SpaceElement* spc = static_cast <BATTLESHIP::SpaceElement*> (evnt.data ());
		assert (spc); // Just in case...

		BATTLESHIP::StdSpaceElementsSceneActionBlock::ToFinalizeSpaceElement* b = 
			dynamic_cast <BATTLESHIP::StdSpaceElementsSceneActionBlock::ToFinalizeSpaceElement*> (buoy (_FINALIZESPACEELEMENTBUOY));
		assert (b); // just in case...

		b -> setSpaceElement (spc);

		b -> active (true);
	}

	BATTLESHIP::SceneActionBlock::processEvent (evnt);
}

// ---
BATTLESHIP::SpaceElements BATTLESHIP::StdSpaceElementsSceneActionBlock::getListValidSpaceElementsTypeFrom 
	(const std::vector <int>& lst, int nE, const QGAMES::Entities& eties) 
{
	BATTLESHIP::SpaceElements result;

	auto validator = isElementValidForActionBlock ();

	if (lst.empty ())
	{
		for (QGAMES::Entities::const_iterator i = eties.begin (); 
				i != eties.end () && result.size () != nE; i++)
		{
			BATTLESHIP::SpaceElement* spc = dynamic_cast <BATTLESHIP::SpaceElement*> ((*i).second);
			if (validator (spc))
				result.push_back (spc);
		}
	}
	else
	{
		for (std::vector <int>::const_iterator i = lst.begin (); 
				i != lst.end () && result.size () != nE; i++)
		{
			QGAMES::Entities::const_iterator j;
			if ((j = eties.find ((*i))) != eties.end ())
			{
				BATTLESHIP::SpaceElement* spc = dynamic_cast <BATTLESHIP::SpaceElement*> ((*j).second);
				if (validator (spc))
					result.push_back (spc);
			}
		}
	}

	return (result);
}

// ---
__IMPLEMENTCOUNTERS__ (BATTLESHIP::StdSpaceElementsSceneActionBlock::Counters)
{
	addCounter (new QGAMES::Counter (BATTLESHIP::StdSpaceElementsSceneActionBlock::_COUNTERTOAPPEAR, 
		0 /** It will be set later too. */, 0, true, true)); 
	addCounter (new QGAMES::Counter (BATTLESHIP::StdSpaceElementsSceneActionBlock::_COUNTEREMERGINGGRADE, 
		sizeof (BATTLESHIP::StdSpaceElementsSceneActionBlock::_FADEGRADES) / sizeof (int), 0, true, false)); 
	addCounter (new QGAMES::Counter (BATTLESHIP::StdSpaceElementsSceneActionBlock::_COUNTERTOMOVE, 
		0 /** It will be set later too. */, 0, true, true)); 
	addCounter (new QGAMES::Counter (BATTLESHIP::StdSpaceElementsSceneActionBlock::_COUNTERMAXTIMEACTION, 
		0 /** It will be set later too. */, 0, true, true)); 
}

// ---
__IMPLEMENTONOFFSWITCHES__ (BATTLESHIP::StdSpaceElementsSceneActionBlock::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(BATTLESHIP::StdSpaceElementsSceneActionBlock::_SWITCHELEMENTSINSCENE, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(BATTLESHIP::StdSpaceElementsSceneActionBlock::_SWITCHELEMENTSEMERGING, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(BATTLESHIP::StdSpaceElementsSceneActionBlock::_SWITCHELEMENTSTOMOVE, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(BATTLESHIP::StdSpaceElementsSceneActionBlock::_SWITCHELEMENTSMOVING, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(BATTLESHIP::StdSpaceElementsSceneActionBlock::_SWITCHELEMENTSUNDERCONTROL, true)); // By default...
}

// ---
void* BATTLESHIP::StdSpaceElementsSceneActionBlock::ToFinalizeSpaceElement::treatFor (QGAMES::Element* e)
{
	assert (_spaceElement);

	BATTLESHIP::StdSpaceElementsSceneActionBlock* blk =
		dynamic_cast <BATTLESHIP::StdSpaceElementsSceneActionBlock*> (e);
	assert (blk);

	_spaceElement -> toHide ();
	// Definitively...it is hidden...
	_spaceElement -> setVisible (false);

	blk -> unObserve (_spaceElement); 

	blk -> _spaceElements = BATTLESHIP::SpaceElements 
		(blk -> _spaceElements.begin (), std::remove_if (blk -> _spaceElements.begin (), blk -> _spaceElements.end (), 
			[this](BATTLESHIP::SpaceElement* i) -> bool { return (i == _spaceElement); }));

	// Are all elements off? is then the list fully empty?
	if (blk -> _spaceElements.empty ())
	{
		// Has the block to be regenerated?
		if (blk -> _properties -> _regenerate)
		{
			// starts again...
			blk -> onOffSwitch (_SWITCHELEMENTSINSCENE) -> set (false);
			blk -> onOffSwitch (_SWITCHELEMENTSTOMOVE) -> set (false);
			blk -> onOffSwitch (_SWITCHELEMENTSMOVING) -> set (false); 
		}
		// ...in any other circunstance, nothing else to do...
		else
			blk -> onOffSwitch (_SWITCHELEMENTSUNDERCONTROL) -> set (false);
	}

	return (NULL);
}

// ---
BATTLESHIP::StdAsteroidSceneActionBlock::Properties::Properties 
		(QGAMES::bdata tA, int pA, bool tE, QGAMES::bdata tM, bool r, bool rll,
		 const QGAMES::Position& pos, const QGAMES::Rectangle& mZ, QGAMES::bdata mTA,
		 int t, int mT, int p, QGAMES::bdata s)
	: BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties (tA, pA, tE, tM, r, rll, pos, mZ, mTA),
	  _type (t),
	  _movementType (mT),
	  _points (p),
	  _speed (s)
{
	assert (_type == -1 ||
			(_type >= 0 && _type < __BATTLESHIPDATAGAME__ -> _numberOfAsteroidTypes)); 
	assert (_points != 0);
	assert (_speed != __BD 0); 
}

// ---
BATTLESHIP::StdAsteroidSceneActionBlock::Properties::Properties (const std::map <std::string, std::string>& prps)
	: BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties (prps),
	  _type (-1), 
	  _movementType (0),
	  _points (10),
	  _speed (__BD 1)
{
	if (prps.find (std::string (__BATTLESHIP_ASTEROIDBLOCKATTRTYPE__)) != prps.end ())
		_type = std::atoi ((*prps.find (__BATTLESHIP_ASTEROIDBLOCKATTRTYPE__)).second.c_str ());
	if (prps.find (std::string (__BATTLESHIP_ASTEROIDBLOCKATTRTYPEMOVEMENT__)) != prps.end ())
		_movementType = std::atoi ((*prps.find (__BATTLESHIP_ASTEROIDBLOCKATTRTYPEMOVEMENT__)).second.c_str ());
	if (prps.find (std::string (__BATTLESHIP_ASTEROIDBLOCKATTRPOINTSGIVEN__)) != prps.end ())
		_points = std::atoi ((*prps.find (__BATTLESHIP_ASTEROIDBLOCKATTRPOINTSGIVEN__)).second.c_str ());
	if (prps.find (std::string (__BATTLESHIP_ASTEROIDBLOCKATTRSPEED__)) != prps.end ())
		_speed = __BD (std::atof ((*prps.find (__BATTLESHIP_ASTEROIDBLOCKATTRSPEED__)).second.c_str ()));

	BATTLESHIP::StdAsteroidSceneActionBlock::Properties 
		(_timeToAppear, _probabilityToAppear, _toEmerge, _timeToMove, _regenerate, _roll,
		  _initialPosition, _movementZone, _maxTimeAction, _type, _movementType, _points, _speed);
}

// ---
QGAMES::SetOfOpenValues BATTLESHIP::StdAsteroidSceneActionBlock::Properties::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result = 
		BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties::asSetOfOpenValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_type));
	result.addOpenValue (lE + 2, QGAMES::OpenValue (_movementType));
	result.addOpenValue (lE + 3, QGAMES::OpenValue (_points));
	result.addOpenValue (lE + 4, QGAMES::OpenValue (_speed));

	return (result);
}

// ---
void BATTLESHIP::StdAsteroidSceneActionBlock::Properties::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	int lE = oV.lastOpenValueId ();

	assert (oV.existOpenValue (lE) && oV.existOpenValue (lE - 1) &&
			oV.existOpenValue (lE - 2) && oV.existOpenValue (lE - 3));

	QGAMES::SetOfOpenValues cCfg = oV;
	_speed = cCfg.openValue (lE).bdataValue ();
	cCfg.removeOpenValue (lE);
	_points = cCfg.openValue (lE - 1).intValue ();
	cCfg.removeOpenValue (lE - 1);
	_movementType = cCfg.openValue (lE - 2).intValue ();
	cCfg.removeOpenValue (lE - 2);
	_type = cCfg.openValue (lE - 3).intValue ();
	cCfg.removeOpenValue (lE - 3);

	BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties::fromSetOfOpenValues (cCfg);
}

// ---
void BATTLESHIP::StdAsteroidSceneActionBlock::getAndConfigureSpaceElements (const std::vector <int>& elmts)
{
	assert (elmts.empty () || (int) elmts.size () == _numberOfAsteroids);

	BATTLESHIP::PlayingScene* scn = dynamic_cast <BATTLESHIP::PlayingScene*> (scene ());
	assert (scn); // just in case...

	_spaceElements = getListValidSpaceElementsTypeFrom (elmts, _numberOfAsteroids, scn -> asteroids ());

	for (BATTLESHIP::SpaceElements::const_iterator i = _spaceElements.begin (); 
			i != _spaceElements.end (); i++)
	{
		assert (dynamic_cast <BATTLESHIP::Asteroid*> ((*i)) != NULL); // Just in case...

		(*i) -> initialize (); 

		(*i) -> setVisible (true);

		BATTLESHIP::StdAsteroidSceneActionBlock::Properties* prps = 
			dynamic_cast <BATTLESHIP::StdAsteroidSceneActionBlock::Properties*> (properties ());
		assert (prps);

		// Which type will it be?
		((BATTLESHIP::Asteroid*) (*i)) -> setType (prps -> _type); // -1 means random...
		// ...and how many point will it give to the battleship?
		((BATTLESHIP::Asteroid*) (*i)) -> 
			setPoints (prps -> _points < 0 ? ((rand () % -prps -> _points) + 1) : prps -> _points);

		(*i) -> setPosition (_properties -> absInitialPositionFor ((*i)));
	}
}

// ---
void BATTLESHIP::StdAsteroidSceneActionBlock::startToMoveSpaceElements ()
{
	if (_spaceElements.empty ())
		return;

	BATTLESHIP::StdAsteroidSceneActionBlock::Properties* prps = 
		dynamic_cast <BATTLESHIP::StdAsteroidSceneActionBlock::Properties*> (properties ());
	assert (prps);

	for (BATTLESHIP::SpaceElements::const_iterator i = _spaceElements.begin (); 
			i != _spaceElements.end (); i++)
	{
		if ((*i) -> isAlive ())
		{
			(*i) -> toMove (prps -> _movementType, QGAMES::Vector (__BD 0, __BD 1, __BD 0) /** movement orientation. */);

			// Sets the speed
			// The more difficult the game is the higger the speed will be...
			QGAMES::bdata mS = (prps -> _speed < __BD 0) // If negative means random up to the positive value...
				? __BD ((rand () % ((int) -prps -> _speed)) + 1) : prps -> _speed;
			BATTLESHIP::Game::Conf* cfg = 
				dynamic_cast <BATTLESHIP::Game::Conf*> (scene () -> game () -> configuration ());
			assert (cfg); // Just to be sure...
			mS += __BD cfg -> difficulty () / __BD 10;

			QGAMES::SimpleLinearMovement* lM = 
				dynamic_cast <QGAMES::SimpleLinearMovement*> ((*i) -> currentMovement ());
			assert (lM); // Just in case...
			lM -> setSpeed (lM -> speed () * mS); 
		}
	}
}

// ---
BATTLESHIP::StdThingToCathActionBlock::Properties::Properties 
		(QGAMES::bdata tA, int pA, bool tE, QGAMES::bdata tM, bool r, bool rll, 
		 const QGAMES::Position& pos, const QGAMES::Rectangle& mZ, QGAMES::bdata mTA,
		 int eT, int mT, QGAMES::bdata s)
	: BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties (tA, pA, tE, tM, r, rll, pos, mZ, mTA),
	  _type (eT),
	  _movementType (mT),
	  _speed (s)
{ 
	assert (_type == -1 || // Either random...
			(_type >= 0 && _type < __BATTLESHIP_NUMBEROFTHINGTOCATCHTYPES__)); // ...or never bigger than the max allowed...
}

// ---
BATTLESHIP::StdThingToCathActionBlock::Properties::Properties (const std::map <std::string, std::string>& prps)
	: BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties (prps),
	  _type (-1), 
	  _movementType (0),
	  _speed (__BD 1)
{
	if (prps.find (std::string (__BATTLESHIP_THINGTOCATCHBLOCKATTRTYPE__)) != prps.end ())
		_type = std::atoi ((*prps.find (__BATTLESHIP_THINGTOCATCHBLOCKATTRTYPE__)).second.c_str ());
	if (prps.find (std::string (__BATTLESHIP_THINGTOCATCHBLOCKATTRTYPEMOVEMENT__)) != prps.end ())
		_movementType = std::atoi ((*prps.find (__BATTLESHIP_THINGTOCATCHBLOCKATTRTYPEMOVEMENT__)).second.c_str ());
	if (prps.find (std::string (__BATTLESHIP_THINGTOCATCHBLOCKATTRSPEED__)) != prps.end ())
		_speed = __BD (std::atof ((*prps.find (__BATTLESHIP_THINGTOCATCHBLOCKATTRSPEED__)).second.c_str ()));

	BATTLESHIP::StdThingToCathActionBlock::Properties 
		(_timeToAppear, _probabilityToAppear, _toEmerge, _timeToMove, _regenerate, _roll,
		 _initialPosition, _movementZone, _maxTimeAction, _type, _movementType, _speed);
}

// ---
QGAMES::SetOfOpenValues BATTLESHIP::StdThingToCathActionBlock::Properties::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result = 
		BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties::asSetOfOpenValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_type));
	result.addOpenValue (lE + 2, QGAMES::OpenValue (_movementType));
	result.addOpenValue (lE + 3, QGAMES::OpenValue (_speed));

	return (result);
}

// ---
void BATTLESHIP::StdThingToCathActionBlock::Properties::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	int lE = oV.lastOpenValueId ();

	assert (oV.existOpenValue (lE) && oV.existOpenValue (lE - 1) &&
			oV.existOpenValue (lE - 2));

	QGAMES::SetOfOpenValues cCfg = oV;
	_speed = cCfg.openValue (lE).bdataValue ();
	cCfg.removeOpenValue (lE);
	_movementType = cCfg.openValue (lE - 1).intValue ();
	cCfg.removeOpenValue (lE - 1);
	_type = cCfg.openValue (lE - 2).intValue ();
	cCfg.removeOpenValue (lE - 2);

	BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties::fromSetOfOpenValues (cCfg);
}

// ---
void BATTLESHIP::StdThingToCathActionBlock::getAndConfigureSpaceElements (const std::vector <int>& elmts)
{
	assert (elmts.empty () || (int) elmts.size () == _numberOfThingsToCatch);

	BATTLESHIP::PlayingScene* scn = dynamic_cast <BATTLESHIP::PlayingScene*> (scene ());
	assert (scn); // just in case...

	_spaceElements = getListValidSpaceElementsTypeFrom (elmts, _numberOfThingsToCatch, scn -> thingsToCatch ());

	for (BATTLESHIP::SpaceElements::const_iterator i = _spaceElements.begin (); 
			i != _spaceElements.end (); i++)
	{
		assert (dynamic_cast <BATTLESHIP::ThingToCatch*> ((*i)) != NULL); // Just in case...

		(*i) -> initialize (); 

		(*i) -> setVisible (true);

		BATTLESHIP::StdThingToCathActionBlock::Properties* prps = 
			dynamic_cast <BATTLESHIP::StdThingToCathActionBlock::Properties*> (properties ());
		assert (prps);

		// Which aspect will it have?
		((BATTLESHIP::ThingToCatch*) (*i)) -> setType (prps -> _type); // -1 means random

		(*i) -> setPosition (_properties -> absInitialPositionFor ((*i)));
	}
}

// ---
void BATTLESHIP::StdThingToCathActionBlock::startToMoveSpaceElements ()
{
	if (_spaceElements.empty ())
		return;

	BATTLESHIP::StdThingToCathActionBlock::Properties* prps = 
		dynamic_cast <BATTLESHIP::StdThingToCathActionBlock::Properties*> (properties ());
	assert (prps);

	for (BATTLESHIP::SpaceElements::const_iterator i = _spaceElements.begin (); 
			i != _spaceElements.end (); i++)
	{
		if ((*i) -> isAlive ())
		{
			(*i) -> toMove (prps -> _movementType, QGAMES::Vector (__BD 0, __BD 1, __BD 0));

			QGAMES::bdata mS = (prps -> _speed < __BD 0) 
				? __BD ((rand () % ((int) -prps -> _speed)) + 1) : prps -> _speed;
			BATTLESHIP::Game::Conf* cfg = 
				dynamic_cast <BATTLESHIP::Game::Conf*> (scene () -> game () -> configuration ());
			assert (cfg); // Just to be sure...
			mS += __BD cfg -> difficulty () / __BD 10;

			QGAMES::SimpleLinearMovement* lM = 
				dynamic_cast <QGAMES::SimpleLinearMovement*> ((*i) -> currentMovement ());
			assert (lM); 
			lM -> setSpeed (lM -> speed () * mS); 
		}
	}
}
