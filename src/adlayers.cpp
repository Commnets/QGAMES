#include <AdvancedArcade/adlayers.hpp>

// The static values...
const int QGAMES::AdvancedTileLayer::_fadeValues [] = 
	{ 0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 210, 220, 240, 255 };
#define __DWLNUMBEROFFADEVALUES__	15

// ---
QGAMES::BackgroundLayer::BackgroundLayer (int id, const std::string& n, const QGAMES::Objects& o, 
		QGAMES::Map* m, const QGAMES::LayerProperties& p)
	: QGAMES::ObjectLayer (id, n, o, m, p)
{
	assert (_objects.size () == 1); // No more than 1...it's only a background layer...
	assert (dynamic_cast <QGAMES::Background*> (((QGAMES::ObjectProxy*) (*_objects.begin ())) -> ptr ())); 
	// And it has to be a background...
}

// ---
QGAMES::FormBasedTileLayerEffect::FormBasedTileLayerEffect (QGAMES::Layer* ly, 
	const QGAMES::LayerProperties& pts, int spd, QGAMES::bdata prb)
		: QGAMES::Layer::VisualEffect (ly, pts),
		  _formsStatus (),
		  _form (NULL),
		  _numberAspects (0),
		  _bbX1 (0), _bbY1 (0), _bbZ1 (0), _bbX2 (0), _bbY2 (0), _bbZ2 (0),
		  _numberStatus (0),
		  _changeSpeed (spd),
		  _probabilityToAppear (prb),
		  _counter (0),
		  _sqrWidth (0), _sqrHeight (0), _sqrDepth (0),
		  _tileWidth (0), _tileHeight (0), _tileDepth (0)
{
	assert (_layer -> existsProperty (std::string (__QGAMES_VEFFECTFORMSATTRFORM__)) &&
			_layer -> existsProperty (std::string (__QGAMES_VEFFECTFORMSATTRNUMASPECTS__)) &&
			_layer -> existsProperty (std::string (__QGAMES_VEFFECTFORMATTRSQAURE__)));
	assert (_changeSpeed > 0 && (_probabilityToAppear >= __BD 0 && _probabilityToAppear <= __BD 1.0));
	assert (dynamic_cast <QGAMES::TileLayer*> (ly)); // It must be a tiled one...

	_form = _layer -> game () -> 
		form (std::atoi (_layer -> layerProperty (std::string (__QGAMES_VEFFECTFORMSATTRFORM__)).c_str ()));
	_numberAspects = 
		std::atoi (_layer -> layerProperty (std::string (__QGAMES_VEFFECTFORMSATTRNUMASPECTS__)).c_str ());
	std::vector <int> sqrV = 
		QGAMES::getElementsFromAsInt (_layer -> layerProperty (__QGAMES_VEFFECTFORMATTRSQAURE__), ',', 6);
	_bbX1 = sqrV [0]; _bbY1 = sqrV [1]; _bbZ1 = sqrV [2]; _bbX2 = sqrV [3]; _bbY2 = sqrV [4]; _bbZ2 = sqrV [5];
	_sqrWidth = _bbX2 - _bbX1 + 1; _sqrHeight = _bbY2 - _bbY1 + 1; _sqrDepth = _bbZ2 - _bbZ1 + 1;
	_numberStatus = _sqrWidth * _sqrHeight * ((_sqrDepth == 0) ? 1 : _sqrDepth); 
	_formsStatus = QGAMES::FormBasedTileLayerEffect::FormsStatus 
		(_numberStatus, QGAMES::FormBasedTileLayerEffect::FormStatus ());

	_tileWidth = ((QGAMES::TiledMap*)  _layer -> map ()) -> tileWidth ();
	_tileHeight = ((QGAMES::TiledMap*)  _layer -> map ()) -> tileHeight ();
	_tileDepth = ((QGAMES::TiledMap*)  _layer -> map ()) -> tileDepth ();
}

// ---
void QGAMES::FormBasedTileLayerEffect::initialize ()
{
	_formsStatus = 
		QGAMES::FormBasedTileLayerEffect::FormsStatus (_numberStatus, QGAMES::FormBasedTileLayerEffect::FormStatus ());
}

// ---
void QGAMES::FormBasedTileLayerEffect::updatePositions ()
{
	if (_counter++ < _changeSpeed)
		return;

	_counter = 0;
	for (int i = 0; i < _numberStatus; i++)
	{
		if (!_formsStatus [i]._changing && ((rand () % 100) <= (_probabilityToAppear * 100)))
			_formsStatus [i]._changing = true; // The aspect will be at this point always 0
		else
		if (_formsStatus [i]._changing)
			if (++_formsStatus [i]._aspect >=  _numberAspects)
				_formsStatus [i] = QGAMES::FormBasedTileLayerEffect::FormStatus (); // A new one...
	}
}

// ---
void QGAMES::FormBasedTileLayerEffect::drawOn (Screen* s, const Position& pos)
{
	for (int i = 0; i < _numberStatus; i++)
	{
		if (_formsStatus [i]._changing)
			_form -> frame (_formsStatus [i]._aspect) -> drawOn (s, 
				pos + QGAMES::Vector (__BD ((_bbX1 + (i % _sqrWidth)) * _tileWidth), 
					__BD ((_bbY1 + (i / _sqrWidth)) * _tileHeight), __BD 0));
	}
}

// ---
QGAMES::AdvancedTileLayer::AdvancedTileLayer (int c, const std::string& n, const QGAMES::Tiles& t, QGAMES::Map* m, 
				QGAMES::TileLayer::VisualOrientation o, const QGAMES::LayerProperties& p, bool oT)
	: QGAMES::TileLayer (c, n, t, m, o , p, oT),
	  _originalPosition (QGAMES::Position::_noPoint),
	  _originalPositionSet (false),
	  _fadeGrade (__DWLNUMBEROFFADEVALUES__ - 1)
{
	// Nothing else to do
}

// ---
QGAMES::SetOfOpenValues QGAMES::AdvancedTileLayer::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::TileLayer::runtimeValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_fadeGrade));

	return (result);
}

// ---
void QGAMES::AdvancedTileLayer::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_fadeGrade = cCfg.openValue (lE).intValue ();
	cCfg.removeOpenValue (lE);

	QGAMES::TileLayer::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::AdvancedTileLayer::disappear (int ml)
{
	if (ml == -1)
	{
		if (_fadeGrade != 0)
			setFade (_fadeGrade = 0);
	}
	else
	{
		onOffSwitch (_SWITCHAPPEARON) -> set (false);
		onOffSwitch (_SWITCHDISAPPEARON) -> set (true);
		counter (_COUNTERTOFADEVALUES) -> initialize (ml, 0, true, true);
	}
}

// ---
bool QGAMES::AdvancedTileLayer::isDisappearing ()
{
	return (onOffSwitch (_SWITCHDISAPPEARON) -> isOn ());
}

// ---
void QGAMES::AdvancedTileLayer::appear (int ml)
{
	if (ml == -1)
	{
		if (_fadeGrade < (__DWLNUMBEROFFADEVALUES__ - 1))
			setFade (_fadeGrade = __DWLNUMBEROFFADEVALUES__ - 1);
	}
	else
	{
		onOffSwitch (_SWITCHAPPEARON) -> set (true);
		onOffSwitch (_SWITCHDISAPPEARON) -> set (false);
		counter (_COUNTERTOFADEVALUES) -> initialize (ml, 0, true, true);
	}
}

// ---
bool QGAMES::AdvancedTileLayer::isAppearing ()
{
	return (onOffSwitch (_SWITCHAPPEARON) -> isOn ());
}

// ---
bool QGAMES::AdvancedTileLayer::transparent (float p)
{
	return (!isVisible () || 
			(isVisible () && _fadeGrade <= ((float) __DWLNUMBEROFFADEVALUES__ * p)));
}

// ---
bool QGAMES::AdvancedTileLayer::solid (float p)
{
	return (isVisible () && 
			_fadeGrade >= ((float) __DWLNUMBEROFFADEVALUES__ * p) - 1);
}

// ---
QGAMES::Vector QGAMES::AdvancedTileLayer::vectorMoved (const QGAMES::Tile& tl) const
{ 
	QGAMES::Vector result = QGAMES::Vector::_cero;

	if (movement () != NULL)
		result += movement () -> vectorMoved (); // The tile is not taken into account at all...
	if (tileUpdater () != NULL)
		result += tileUpdater () -> updateDirection (tl) * __BD tileUpdater () -> pixelsUpdated (tl); // But here it is...

	return (result);
}

// ---
void QGAMES::AdvancedTileLayer::initialize ()
{
	QGAMES::TileLayer::initialize ();

	_fadeGrade = __DWLNUMBEROFFADEVALUES__ - 1;

	if (!_originalPositionSet)
	{
		_originalPositionSet = true;
		_originalPosition = position ();
	}

	setMovement (createMovement ()); // It could be null. Moves by default (if not null)

	setPosition (_originalPosition); // In ISOMETRIC draws the original position can be different than 0...

	setTileUpdater (createTileUpdater ()); // It could be null. Updates by default (if not null)

	setVisualEffect (createVisualEffect ()); // It could be null

	reStartAllCounters ();
	reStartAllOnOffSwitches ();
}

// ---
void QGAMES::AdvancedTileLayer::updatePositions ()
{
	QGAMES::TileLayer::updatePositions ();

	int fG = _fadeGrade;

	// Appearing?
	if (onOffSwitch (_SWITCHAPPEARON) -> isOn ())
	{
		if (counter (_COUNTERTOFADEVALUES) -> isEnd ())
			_fadeGrade++;
		if (_fadeGrade >= __DWLNUMBEROFFADEVALUES__)
		{
			_fadeGrade = __DWLNUMBEROFFADEVALUES__ - 1;
			onOffSwitch (_SWITCHAPPEARON) -> set (false);
		}
	}
	else
	// ..or disappearing?
	if (onOffSwitch (_SWITCHDISAPPEARON) -> isOn ())
	{
		if (counter (_COUNTERTOFADEVALUES) -> isEnd ())
			_fadeGrade--;
		if (_fadeGrade < 0)
		{
			_fadeGrade = 0;
			onOffSwitch (_SWITCHDISAPPEARON) -> set (false);
		}
	}

	// Has _fadeGrade changed?...
	if (_fadeGrade != fG)
		setFade (AdvancedTileLayer::_fadeValues [_fadeGrade]); // ..actualize the tiles!
}

// ---
QGAMES::Layer::MovementBehaviour* QGAMES::AdvancedTileLayer::createMovement ()
{
	int tp = -1;
	if (existsProperty (std::string (__QGAMES_LAYERTYPOFMOVEMENT__))) 
		tp = std::stoi (layerProperty (__QGAMES_LAYERTYPOFMOVEMENT__));

	QGAMES::Layer::MovementBehaviour* m = NULL;

	switch (tp)
	{
		// Circular movements...
		case __QGAMES_LAYERTYPEOFMOVCIRCULAR__:
		case __QGAMES_LAYERTYPEOFMOVBCIRCULAR__:
		case __QGAMES_LAYERTYPEOFMOVCCIRCULAR__:
			{
				// Basic circular movement, just in case the parameters are not well defined...
				QGAMES::Layer::CircularMovementBehaviour* cM = 
					new QGAMES::Layer::CircularMovementBehaviour (0, 0, 0, (__BD 2 * __PI)); // Full loop by default...

				// This variable will be used later to determine the max number of movements...
				int nCycles = existsProperty (std::string (__QGAMES_LAYERMOVNUMBERCYCLES__))
					? std::stoi (layerProperty (std::string (__QGAMES_LAYERMOVNUMBERCYCLES__)))
					: __QGAMES_LAYERMOVDEFAULTNUMBERCYCLES__;

				// ...then set the rest of the variables!
				cM -> setSpeed (existsProperty (std::string (__QGAMES_LAYERMOVEMENTSPEED__)) 
					? std::stoi (layerProperty (std::string (__QGAMES_LAYERMOVEMENTSPEED__))) 
					: __QGAMES_LAYERMOVEMENTDEFAULTSPEED__);
				cM -> setRadix (existsProperty (std::string (__QGAMES_LAYERCIRCULARMOVRADIX__)) 
					? std::stoi (layerProperty (std::string (__QGAMES_LAYERCIRCULARMOVRADIX__))) 
					: __QGAMES_LAYERCIRCULARMOVDEFAULTRADIX__);
				cM -> setInitialAngle (existsProperty (std::string (__QGAMES_LAYERCIRCULARMOVINITANGLE__)) 
					? __BD std::stof (layerProperty (std::string (__QGAMES_LAYERCIRCULARMOVINITANGLE__))) * (__PI / __BD 180)
					: __QGAMES_LAYERCIRCULARMOVDEFINITANGLE__);
				QGAMES::bdata mAng = existsProperty (std::string (__QGAMES_LAYERCIRCULTARMOVFINALANGLE__)) 
					? __BD std::stof (layerProperty (std::string (__QGAMES_LAYERCIRCULTARMOVFINALANGLE__))) * (__PI / __BD 180) 
					: cM -> initialAngle () + (__BD 2 * __PI);
				cM -> setMaxAngle (mAng); // To make a full loop...
				cM -> setAngleStep (existsProperty (std::string (__QGAMES_LAYERCIRCULARMOVANGLESTEP__)) 
					? __BD std::stoi (layerProperty (std::string (__QGAMES_LAYERCIRCULARMOVANGLESTEP__))) * (__PI / __BD 180) 
					: __QGAMES_LAYERCIRCULARMOVDEFANGLESTEP__);
				bool cW = true;
				cM -> setClockwise ((existsProperty (std::string (__QGAMES_LAYERCIRCULATMOVCLOCKWISE__)) &&
					std::string (layerProperty (__QGAMES_LAYERCIRCULATMOVCLOCKWISE__)) == std::string ("FALSE")) ? false : true);

				switch (tp)
				{
					case __QGAMES_LAYERTYPEOFMOVCIRCULAR__:
						m = cM; // Pure circular...
						break;
					case __QGAMES_LAYERTYPEOFMOVBCIRCULAR__:
						{
							// Going back and forth for ever...
							if (nCycles == -1)
								m = new QGAMES::Layer::BounceMovementBehaviour (cM); 
							// or just a number of times...
							else
								m = new QGAMES::Layer::BounceNTimesMovementBehaviour (nCycles, cM); 
						}

						break;
					case __QGAMES_LAYERTYPEOFMOVCCIRCULAR__:
						{
							QGAMES::Layer::MovementBehaviours movs; movs.push_back (cM);
							m = new QGAMES::Layer::SetOfMovementsBehaviour (movs, true); // Cyclic...
						}

						break;

					default:
						assert (0); // It shouldn't be here...
						break;
				}
			}

			break;

		// Linear movements...
		case __QGAMES_LAYERTYPEOFMOVSTRAIGHT__:
		case __QGAMES_LAYERTYPEOFMOVBSTRAIGHT__:
		case __QGAMES_LAYERTYPEOFMOVSTRAIGHTCICLIC__:
			{
				// Creates an almost empty linear behaviour...
				QGAMES::Layer::LinearMovementBehaviour* lM = 
					new QGAMES::Layer::LinearMovementBehaviour (0, 0, QGAMES::Position::_cero);

				// This variable will be used later to determine the max number of movements...
				int nCycles = existsProperty (std::string (__QGAMES_LAYERMOVNUMBERCYCLES__))
					? std::stoi (layerProperty (std::string (__QGAMES_LAYERMOVNUMBERCYCLES__)))
					: __QGAMES_LAYERMOVDEFAULTNUMBERCYCLES__;

				// ...and then sets the rest of the parameters!
				lM -> setSpeed (existsProperty (std::string (__QGAMES_LAYERMOVEMENTSPEED__)) 
					? std::stoi (layerProperty (std::string (__QGAMES_LAYERMOVEMENTSPEED__))) 
					: __QGAMES_LAYERMOVEMENTDEFAULTSPEED__);
				lM  -> setSteps (existsProperty (std::string (__QGAMES_LAYERSTRAIGHTMOVLENGTH__))
					? std::stoi (layerProperty (std::string (__QGAMES_LAYERSTRAIGHTMOVLENGTH__))) 
					: __QGAMES_LAYERSTRAIGHTDEFMOVLENGTH__);
				std::string defDir (__QGAMES_LAYERSTRAIGHTDEFMOVDIRECTION__);
				if (existsProperty (std::string (__QGAMES_LAYERSTRAIGHTMOVDIRECTION__)))
					defDir = layerProperty (std::string (__QGAMES_LAYERSTRAIGHTMOVDIRECTION__));
				std::vector <QGAMES::bdata> dir = QGAMES::getElementsFromAsBDATA (defDir, ',', 3);
				lM -> setDirection (QGAMES::Vector (dir [0], dir [1], dir [2]));

				switch (tp)
				{
					case __QGAMES_LAYERTYPEOFMOVSTRAIGHT__:
						m = lM;
						break;
					case __QGAMES_LAYERTYPEOFMOVBSTRAIGHT__:
						{
							// Going back and forth for ever...
							if (nCycles == -1)
								m = new QGAMES::Layer::BounceMovementBehaviour (lM);
							// or just a number of times...
							else
								m = new QGAMES::Layer::BounceNTimesMovementBehaviour (nCycles, lM);
						}
						
						break;
					case __QGAMES_LAYERTYPEOFMOVSTRAIGHTCICLIC__:
						{
							QGAMES::Layer::MovementBehaviours movs; movs.push_back (lM);
							m = new QGAMES::Layer::SetOfMovementsBehaviour (movs, true); // Cyclic...
						}

						break;

					default:
						assert (0); // It shouldn't be here...
						break;
				}
			}

			break;

		default:
			break; // None...
	}

	return (m);
}

// ---
QGAMES::TileLayer::TileUpdater* QGAMES::AdvancedTileLayer::createTileUpdater ()
{
	int tp = -1;
	if (existsProperty (std::string (__QGAMES_LAYERTYPOFUPDATER__))) 
		tp = std::stoi (layerProperty (__QGAMES_LAYERTYPOFUPDATER__));

	QGAMES::TileLayer::TileUpdater* u = NULL;

	switch (tp)
	{
		// Wave like update...
		case __QGAMES_LAYERTYPEOFUPDWAVE__:
			{
				QGAMES::TileLayer::WaveTileUpdater::Properties prps;

				prps._loopsToUpdate = (existsProperty (std::string (__QGAMES_LAYERUPDLOOPSTO__)) 
					? std::stoi (layerProperty (std::string (__QGAMES_LAYERUPDLOOPSTO__))) 
					: __QGAMES_LAYERUPSDEFAULTLOOPSTO__);
				std::string tl (__QGAMES_LAYERUPDDEFAULTLISTOFTILES__);
				if (existsProperty (std::string (__QGAMES_LAYERUPDLISTOFTILES__)))
					tl = layerProperty (std::string (__QGAMES_LAYERUPDLISTOFTILES__));
				std::vector <int> tls = QGAMES::getElementsFromAsInt (tl, ',');
				prps._tilesToWave = tls;
				prps._acceleration = __BD (existsProperty (std::string (__QGAMES_LAYERWAVEUPDACCELERATION__)) 
					? std::stof (layerProperty (std::string (__QGAMES_LAYERWAVEUPDACCELERATION__))) 
					: __QGAMES_LAYERWAVEUPDDEFACCELERATION__);
				prps._maximumSpeed = __BD (existsProperty (std::string (__QGAMES_LAYERWAVEUPDSPEED__)) 
					? std::stoi (layerProperty (std::string (__QGAMES_LAYERWAVEUPDSPEED__))) 
					: __QGAMES_LAYERWAVEUPDDEFAULTSPEED__);

				u = new QGAMES::TileLayer::WaveTileUpdater (prps); 
			}

			break;

		// Like flowing the tiles...
		case __QGAMES_LAYERTYPEOFUPDFLOW__:
			{
				QGAMES::TileLayer::FlowTileUpdater::Properties prps;

				prps._loopsToUpdate = (existsProperty (std::string (__QGAMES_LAYERUPDLOOPSTO__)) 
					? std::stoi (layerProperty (std::string (__QGAMES_LAYERUPDLOOPSTO__))) 
					: __QGAMES_LAYERUPSDEFAULTLOOPSTO__);
				std::string tl (__QGAMES_LAYERUPDDEFAULTLISTOFTILES__);
				if (existsProperty (std::string (__QGAMES_LAYERUPDLISTOFTILES__)))
					tl = layerProperty (std::string (__QGAMES_LAYERUPDLISTOFTILES__));
				std::vector <int> tls = QGAMES::getElementsFromAsInt (tl, ',');
				prps._tilesToFlow = tls;

				u = new QGAMES::TileLayer::FlowTileUpdater (prps); 
			}

			break;

		default:
			break; // None...
	}

	return (u);
}

// ---
QGAMES::Layer::VisualEffect* QGAMES::AdvancedTileLayer::createVisualEffect ()
{
	int tVE = -1;
	if (existsProperty (std::string (__QGAMES_LAYERTYPEOFVISUALEFFECT__))) 
		tVE = std::stoi (layerProperty (__QGAMES_LAYERTYPEOFVISUALEFFECT__));

	QGAMES::Layer::VisualEffect* result = NULL;

	switch (tVE)
	{
		case __QGAMES_LAYERRVISUALEFFECTFORMON__:
			{
				int sC = existsProperty (std::string (__QGAMES_LAYERVEFFECTFORMONSPEED__))
					? std::stoi (layerProperty (std::string (__QGAMES_LAYERVEFFECTFORMONSPEED__)))
					: __QGAMES_LAYERVEFFECTFORMONDEFSPEED_;
				QGAMES::bdata pA = existsProperty (std::string (__QGAMES_LAYERVEFFECTFORMONPTOAPPEAR__))
					? __BD (std::stof (layerProperty (std::string (__QGAMES_LAYERVEFFECTFORMONPTOAPPEAR__))))
					: __QGAMES_LAYERVEFFECTFORMONDEFSPEED_;

				result = new QGAMES::FormBasedTileLayerEffect (this, properties (), sC, pA);	
			}

			break;

		default:
			break; // None...
	}

	return (result);
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::AdvancedTileLayer::Counters)
{
	// This is the default value, but when appear or disappear 
	// this variable will be initialized!
	addCounter (new QGAMES::Counter 
		(QGAMES::AdvancedTileLayer::_COUNTERTOFADEVALUES, 3, 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::AdvancedTileLayer::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (QGAMES::AdvancedTileLayer::_SWITCHAPPEARON, false)); 
	addOnOffSwitch (new QGAMES::OnOffSwitch (QGAMES::AdvancedTileLayer::_SWITCHDISAPPEARON, false)); 
}

