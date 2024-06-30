#include <DragonwindLike/dwartists.hpp>
#include <DragonwindLike/dwgame.hpp>
#include <DragonwindLike/dwlayers.hpp>
#include <iterator>

// ---
QGAMES::Entity* DRAGONWIND::MenuNinjaMale::clone () const
{
	QGAMES::Entity* result = new DRAGONWIND::MenuNinjaMale (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
QGAMES::Entity* DRAGONWIND::MenuNinjaFemale::clone () const
{
	QGAMES::Entity* result = new DRAGONWIND::MenuNinjaFemale (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
DRAGONWIND::DragonArtist::DragonArtist (int cId, const QGAMES::Forms& f, const QGAMES::PlatformArtist::Data& d)
	: QGAMES::PlatformArtist (cId, f, d),
	  _lookingDirection (0), // Looking to the right?
	  _dialogController (new DRAGONWIND::DragonArtist::DialogController (this)),
	  _lastCanExitWaterUp (false), 
	  _lastCanExitWaterDown (false), 
	  _lastCanExitWaterLeft (false), 
	  _lastCanExitWaterRight (false),
	  _lastCollisionAtDir ()
{
	setVisible (false); // By default all artists of this type of games are not visible
}

// ---
QGAMES::SetOfOpenValues DRAGONWIND::DragonArtist::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::PlatformArtist::runtimeValues ();

	int lE = result.lastOpenValueId ();

	// Where the artist is looking to...
	result.addOpenValue (lE + 1, QGAMES::OpenValue (lookingToDirection ()));

	return (result);
}

// ---
void DRAGONWIND::DragonArtist::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	setLookingToDirection (cCfg.openValue (lE).intValue ());
	cCfg.removeOpenValue (lE);

	QGAMES::PlatformArtist::initializeRuntimeValuesFrom (cCfg);
}

// ---
bool DRAGONWIND::DragonArtist::canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
{
	DRAGONWIND::DragonArtist::DragonState dSt = platformStateToDragonState ();

	if (!isPossiblePosInMap (position () + d))
		return (false);

	if (isHappy () || isClimbing ())
		return (true);

	// If the entity is falling (including floating) or jumping 
	// the movement is always possible unless collision with a solid tile happens
	// If during the movement a platform is found, then another thing will be said!
	if (isFalling () || isJumping ())
		return (!collisioningWithTile (__QGAMES_PTSOLIDTILETYPE__, d, a));

	// If the entity is swiming but it is also in a position to exit the water (in a direction),
	// the movement to that direction won't be possible. 
	if (isSwiming ())
	{
		const QGAMES::Vector& tD = (d == QGAMES::Vector::_cero) ? orientation () : d;
		if ((_lastCanExitWaterUp    && tD.posY () < __BD 0) || 
			(_lastCanExitWaterDown  && tD.posY () > __BD 0) ||
			(_lastCanExitWaterLeft  && tD.posX () < __BD 0) || 
			(_lastCanExitWaterRight && tD.posX () > __BD 0))
			return (false);
	}

	return (QGAMES::PlatformArtist::canMove (d, a));
}

// ---
bool DRAGONWIND::DragonArtist::isPossiblePosInMap (const QGAMES::Position& v)
{
	if (map () == NULL)	return (true); // Makes no sense but it is ok...

	int mVL, mVH, mVD;
	maxVisualDimensions (mVL, mVH, mVD);
	return (v.posX () > __BD 0 && v.posX () < __BD (map () -> width () - (mVL >> 1)) &&
			v.posY () > __BD -mVH && v.posY () < (__BD (map () -> height () + (mVH >> 4)))); // A little bit out...
}

// ---
bool DRAGONWIND::DragonArtist::isBlocked ()
{
	bool result = false;

	QGAMES::bdata m = (QGAMES::bdata) 0.0;
	QGAMES::Vector pv = QGAMES::Vector::_cero;
	adjustRelevants (m, pv); // To adjust the way the tile is considered...
	QGAMES::Layers rLayers = (__TM map ()) -> relevantLayers ();
	for (QGAMES::Layers::const_iterator i = rLayers.begin (); 
		i != rLayers.end () && !result; i++)
	{
		if ((*i) -> type () != __QGAMES_TILELAYER__ || 
			((*i) -> type () == __QGAMES_TILELAYER__ && ((QGAMES::PlatformTileLayer*) (*i)) -> transparent ())) 
			continue;

		result = true;
		QGAMES::Tiles tls = (__TM map ()) -> tilesAround (this, ((QGAMES::TileLayer*) (*i)), m, pv);
		for (int j = 0; j < (int) tls.size () && result; result &= isTileAnObstacle (tls [j++]));
	}

	return (result);
}

// ---
bool DRAGONWIND::DragonArtist::toStand (int lD, bool a)
{ 
	if (!isAlive () && !isHappy ())
		return (false);

	if (isStanding () && lD == lookingToDirection ())
		return (false); 

	setPlatformState (-1); // Just to force the change of looking to...
	setLookingToDirection (lD);
	setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_STAND)); 
	if (a) adjustPositionToBase (true); // Adjust to the base if any before...

	return (true);
}

// ---
bool DRAGONWIND::DragonArtist::toFall ()
{
	if (isFalling () || !isAlive ())
		return (false);

	setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_FALL));
	setFixBasePosition (); // None, because it is falling...

	return (true);
}

// ---
void DRAGONWIND::DragonArtist::initialize ()
{
	QGAMES::PlatformArtist::initialize ();

	// After parent's initialization, the position (including the fixBasePosition if any) is set back to null
	// The DragonArtists are set from definition during the DragonScene initialization
	// that happens before the artists running inside are initialized. 
	// So, all data related with positions would be lost, and it is necessary to reset them!

	setLookingToDirection (0); // It means to the right...

	if (definition ())
	{
		setPlatformState (definition () -> _state == -1 
			? dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_STAND) : definition () -> _state);
		setPosition (definition () -> _positions [0]); // The position is the first...
		if (definition () -> _positions.size () > 1)
			setFixBasePosition (definition () -> _positions [1]); 
	}
	else
	{
		setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_STAND));
		setPosition (QGAMES::Position::_cero);
	}

	// If at this point no base position has been fixed still (if needed) 
	// and the state is a platform state one is fixed at least by default!
	if (isOnAPlatformState () && fixBasePosition () == QGAMES::Position::_noPoint)
		setFixBasePosition (basePosition ());
}

// ---
void DRAGONWIND::DragonArtist::updatePositions ()
{
	// Keeps the last situation to be used into the method "canMove"...
	_lastCanExitWaterUp = canExitWaterUp ();
	_lastCanExitWaterDown = canExitWaterDown ();
	_lastCanExitWaterRight = canExitWaterRight ();
	_lastCanExitWaterLeft = canExitWaterLeft ();

	// Same for other uses...
	_lastCollisionAtDir = _collisionAtDir;

	QGAMES::PlatformArtist::updatePositions ();

	if (position ().posY () >= __BD (map () -> height ()))
		whenGoingOutOfTheMap ();

	if (_dialogController)
		_dialogController -> update ();
}

// ---
void DRAGONWIND::DragonArtist::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (isVisible ()) 
	{
		QGAMES::PlatformArtist::drawOn (s, p);

		if (_dialogController)
			_dialogController ->  drawOn (s, centerPosition ()); 
	}
}

// ---
DRAGONWIND::DragonArtist::DragonState DRAGONWIND::DragonArtist::platformStateToDragonState (int pS) const
{
	assert (pS < 19);

	return ((DRAGONWIND::DragonArtist::DragonState) pS);
}

// ---
QGAMES::Tiles DRAGONWIND::DragonArtist::referenceTilesNow (const QGAMES::SetOfTiles& rT,
		const QGAMES::Vector& dr, const QGAMES::Vector& a)
{
	QGAMES::Tiles result;
	switch (platformStateToDragonState ())
	{
		// In these states the entity can either go down or to the right or to the left
		// But never can go up!
		case DRAGONWIND::DragonArtist::DragonState::_FALL:
		case DRAGONWIND::DragonArtist::DragonState::_FLOAT:
		case DRAGONWIND::DragonArtist::DragonState::_INTHEAIR:
		case DRAGONWIND::DragonArtist::DragonState::_DIEING:
		case DRAGONWIND::DragonArtist::DragonState::_DIED:
			if (dr.posX () != 0) result = (dr.posX () < 0) ? rT [LEFT] : rT [RIGHT];
			else result = rT [DOWN]; 
			break;

		// In these others the entity can go to any place...
		case DRAGONWIND::DragonArtist::DragonState::_JUMP:
		case DRAGONWIND::DragonArtist::DragonState::_JUMPANDATTACK:
		case DRAGONWIND::DragonArtist::DragonState::_JUMPTHROW:
		case DRAGONWIND::DragonArtist::DragonState::_HAPPY:
		case DRAGONWIND::DragonArtist::DragonState::_THROW:
		case DRAGONWIND::DragonArtist::DragonState::_SWIM:
			if (dr.posX () != 0) result = (dr.posX () < 0) ? rT [LEFT] : rT [RIGHT];
			else result = (dr.posY () < 0) ? rT [UP] : rT [DOWN];
			break;

		// In these states the entity can go either to the right or to the left
		// But it can't go up or down...
		case DRAGONWIND::DragonArtist::DragonState::_RUN:
		case DRAGONWIND::DragonArtist::DragonState::_STAND:
		case DRAGONWIND::DragonArtist::DragonState::_STANDATTACK:
		case DRAGONWIND::DragonArtist::DragonState::_RUNATTACK:
		case DRAGONWIND::DragonArtist::DragonState::_ROLL:
			result = (dr.posX () >= 0) ? rT [RIGHT] : rT [LEFT];
			break;

		// When slidding, only the tile in the left or right at the most bottom part is taken into account...
		case DRAGONWIND::DragonArtist::DragonState::_SLIDE:
			{
				result = (dr.posX () >= 0) ? rT [RIGHT] : rT [LEFT];
				if (result.size () > 1)
				{
					QGAMES::Tiles oR;
					oR.push_back (result [result.size () - 1]); // Just the last one...
					result = oR;
				}
			}

			break;

		// In this state the entiy can only go up or down, but never go to the right or to the left
		case DRAGONWIND::DragonArtist::DragonState::_CLIMB:
		case DRAGONWIND::DragonArtist::DragonState::_CLIMBFIXED:
			result = (dr.posY () < 0) ? rT [UP] : rT [DOWN];
			break;
 
		default:
			assert (0); // It should happen, just in case...
			break;
	}

	return (result);
}

// ---
void DRAGONWIND::DragonArtist::afterAllLayersTestedInWhenOnTile ()
{
	if (!_layerBase)
	{
		if (!_tileSolid)
		{
			if (isOnAPlatformState ())
				toFall (); // ...start to fall
		}
	}

	else
	{
		int hhT = ((QGAMES::TiledMap*) _layerBase -> map ()) -> tileHeight () >> 1;

		if (!isOnAPlatformState ())
		{
			// A base has been found, but the action to do will depend on
			// whether the artist is on the edge or not...

			// But also on the state
			// If the entity is jumping (or falling, including floating) 
			// and already going down and the base (where the feet are) is close to the base, then it is time to stand
			if (((isJumping () && orientation ().posY () > __BD 0) || isFalling ()))
			{
				if (isJumping ())
				{
					setFixBasePosition (QGAMES::Position ((_onTheEdge == 0) // If on the edge adjust a little bit...
							? basePosition ().posX () : _layerBasePosition.posX (), 
						_layerBasePosition.posY () + __BD (hhT >> 1), __BD 0)); // A little bit IN the base...
					toStand (lookingToDirection (), true); // And stands eventually!, but everything aligned with the base!
				}
				else
				if (isFalling ())
				{
					setFixBasePosition (QGAMES::Position ((_onTheEdge == 0) 
							? basePosition ().posX () : _layerBasePosition.posX (), 
						_layerBasePosition.posY () + __BD (hhT >> 1), __BD 0)); 
					toStand (lookingToDirection (), true); 
				}
			}
		}

		else
		{
			// The artist has changed the layer
			// So it could be needed to adjust the level of the platform!
			if (_lastLayerBase != _layerBase)
			{
				setFixBasePosition (QGAMES::Position (basePosition ().posX (), 
					_layerBasePosition.posY () + __BD (hhT >> 1), __BD 0)); 
				adjustPositionToBase (true); 
			}
		}
	}
}

// ---
void DRAGONWIND::DragonArtist::drawOnLimits (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::PlatformArtist::drawOnLimits (s, p);

	QGAMES::bdata m = (QGAMES::bdata) 0.0;
	QGAMES::Vector pv (__BD 0, __BD 0, __BD 0);
	adjustRelevants (m, pv); 
	int ePosX, ePosY, ePosZ, ntW, ntH, ntD;
	tilesOcuppied (ePosX, ePosY, ePosZ, ntW, ntH, ntD, QGAMES::Position::_cero, m, pv); // Tiles ocuppied...

	int tWidth = (__TM map ()) -> tileWidth ();
	int tHeight = (__TM map ()) -> tileHeight ();
	int mWidth = map () -> width () / tWidth;
	int mHeight = map () -> height () / tHeight; 
	int nE = ePosY * mWidth + ePosX;
	std::vector <int> up, down, right, left, front, back;
	relevantTilePositions (ntW, ntH, ntD, 
		up, down, right, left, front, back); // up,...back, fullfilled!

	for (int i = 0; i < ntH; i++)
	{
		for (int j = 0; j < ntW; j++)
		{
			int iPos = nE + (i * mWidth + j);

			// Has the tile to be fullfilled?
			int dr = 0; // No direction
			int iP = i * ntW + j; // The cell to be double checked
			if (std::find (up.begin (), up.end (), iP) != up.end ()) dr = 1; // Cell Up
			else if (std::find (right.begin (), right.end (), iP) != right.end ()) dr = 2; // Cell Right
			else if (std::find (down.begin (), down.end (), iP) != down.end ()) dr = 3; // Cell Down
			else if (std::find (left.begin (), left.end (), iP) != left.end ()) dr = 4; // Cell Left

			// Draws the rectangle...
			QGAMES::Position pos1 (__BD ((iPos % mWidth) * tWidth), 
				__BD ((iPos / mWidth) * tHeight), __BD 0);
			QGAMES::Position pos2 = pos1 + QGAMES::Vector (__BD tWidth, __BD tHeight, __BD 0);
			s -> drawRectangle (QGAMES::Rectangle (pos1, pos2, QGAMES::Vector::_zNormal), 
					__QGAMES_SHADOWCOLOR__, (dr != 0) ? true : false);
		}
	}

	QGAMES::Position bPos (__BD (((int) basePosition ().posX () / tWidth) * tWidth), 
		__BD (((int) basePosition ().posY () / tHeight) * tHeight), __BD 0); 
	s -> drawRectangle (QGAMES::Rectangle 
		(bPos, bPos + QGAMES::Vector (__BD tWidth, __BD tHeight, __BD 0), QGAMES::Vector::_zNormal), 
			__QGAMES_SHADOWCOLOR__, true);

	if (canClimbUp ())
	{
		QGAMES::Rectangle r = collisionZone ();
		r = QGAMES::Rectangle (r.pos1 (), r.pos1 () + QGAMES::Vector (__BD 8, __BD 8, __BD 0));
		s -> drawRectangle (r, __QGAMES_REDCOLOR__, true);
	}

	if (canClimbDown ())
	{
		QGAMES::Rectangle r = collisionZone ();
		r = QGAMES::Rectangle (r.pos2 (), r.pos2 () + QGAMES::Vector (__BD 8, __BD 8, __BD 0));
		s -> drawRectangle (r, __QGAMES_REDCOLOR__, true);
	}

	if (canSwim ())
		s -> drawCircle (position (), QGAMES::Vector::_zNormal, 10, 10, 
			canExitWater () ? __QGAMES_YELLOWCOLOR__ :  __QGAMES_BLUECOLOR__, true);
}

// ---
QGAMES::Position DRAGONWIND::DragonArtist::baseWithXPositionAdjusted ()
{
	if (basePosition () == QGAMES::Position::_noPoint)
		return (basePosition ());

	QGAMES::Position oB = basePosition ();
	setFixBasePosition ();
	QGAMES::Position nB = basePosition ();
	setFixBasePosition (oB);
	
	QGAMES::Position result (nB.posX (), oB.posY (), __BD 0);
	return (result);
}

// ---
void DRAGONWIND::DragonArtist::Dialog::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	// First of all, The rectangle containing everything is drawn...
	s -> drawRectangle (_roundingRectangle + _properties._offset + p, _properties._rectangleColor, true);
	s -> drawRectangle (_roundingRectangle + _properties._offset + p, _properties._borderColor, false);

	// The all the texts are drawn...
	QGAMES::Position pos = p + _properties._offset + 
		QGAMES::Vector (__BD _properties._separation, __BD _properties._separation, __BD 0);
	for (std::vector <QGAMES::ScoreObjectText*>::const_iterator i = _internalTexts.begin (); 
		i != _internalTexts.end (); i++)
	{
		(*i) -> drawOn (s, pos);
		pos += QGAMES::Vector (__BD 0, __BD ((*i) -> visualHeight () + _properties._separation), __BD 0);
	}
}

// ---
void DRAGONWIND::DragonArtist::Dialog::createInternalTexts ()
{
	deleteInternalTexts ();
	
	std::string cTxt = _text;
	while (cTxt != std::string (__NULL_STRING__))
	{
		std::string p = QGAMES::trim (cTxt.substr (0, _properties._chrsPerLine));
		cTxt = (_properties._chrsPerLine >= (int) cTxt.length ()) 
			? std::string (__NULL_STRING__) : cTxt.substr (_properties._chrsPerLine);

		QGAMES::ScoreObjectText* iTxt = QGAMES::Game::game () -> textBuilder () -> createText (_properties._font, p);
		iTxt -> setAlphaLevel (_properties._fadeLevel);
		_internalTexts.push_back (iTxt);
	}

	// Now the size of the rectangle is calculated...
	int w = 0; int h = _properties._separation;
	for (std::vector <QGAMES::ScoreObjectText*>::const_iterator i = _internalTexts.begin ();
			i != _internalTexts.end (); i++)
	{
		if ((*i) -> visualLength () > w)
			w = (*i) -> visualLength ();
		h += (*i) -> visualHeight () + _properties._separation;
	}

	_roundingRectangle = QGAMES::Rectangle (QGAMES::Position::_cero, 
		QGAMES::Position (__BD (w + (2 * _properties._separation)), __BD h, __BD 0));
}

// ---
void DRAGONWIND::DragonArtist::Dialog::deleteInternalTexts ()
{
	for (std::vector <QGAMES::ScoreObjectText*>::const_iterator i = _internalTexts.begin ();
			i != _internalTexts.end (); delete ((*i++)));

	_internalTexts.clear (); // No more...
	_internalTexts = std::vector <QGAMES::ScoreObjectText*> ();
}

// ---
DRAGONWIND::DragonArtist::DialogController::~DialogController ()
{
	while (!_dialogs.empty ())
	{
		delete (_dialogs.front ());
		_dialogs.pop ();
	}
}

// ---
void DRAGONWIND::DragonArtist::DialogController::update ()
{
	if (_dialogs.empty ())
		return; // With no dialogs, nothing else to do...

	if (_loopsCounter++ >= 
		_dialogs.back () -> properties ()._secondsOnScreen * QGAMES::Game::game () -> framesPerSecond ())
	{
		delete (_dialogs.front ()); 
		_dialogs.pop ();

		_loopsCounter = 0;

		// The notification is done to potentailly linked activities...
		// Notice that the notification is done from the owner (the dragonartist)
		_owner -> notify (QGAMES::Event (__DRAGONWIND_DIALOGHASFINISHED__, _owner));
	}
}

// ---
void DRAGONWIND::DragonArtist::DialogController::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	if (!_dialogs.empty ())
		_dialogs.front () -> drawOn (s, p);
}

// ---
DRAGONWIND::Character::Behaviour::Behaviour (const QGAMES::SetOfOpenValues& oV)
	: _canBeHurmedByEnemies (true), _canBeHurmedBySimilars (false), _canBeHurmedByFlyingMonsters (true),
	  _canBeHurmedByLightning (true),
	  _gunsHurming (),
	  _energyLostPerLoop (__BD 1 / __BD QGAMES::Game::game () -> framesPerSecond ()),
	  _energyLostPerExplosion (__BD 100),
	  _pointsGivenWhenDie (10), 
	  _shootingEnergy (5),
	  _automaticMovement (-1),
	  _weakness (1)
	  // Default values...
{
	_gunsHurming.push_back (-1); // By default all...

	if (oV.existOpenValue (__DRAGONWIND_BHVOPENVALUEHURMEDEMISPARAM__))
		_canBeHurmedByEnemies = QGAMES::toUpper (std::string 
			(oV.openValue (__DRAGONWIND_BHVOPENVALUEHURMEDEMISPARAM__).strValue ())) == std::string (__YES_STRING__);
	if (oV.existOpenValue (__DRAGONWIND_BHVOPENVALUEHURMEDSIMSPARAM__))
		_canBeHurmedBySimilars = QGAMES::toUpper (std::string 
			(oV.openValue (__DRAGONWIND_BHVOPENVALUEHURMEDSIMSPARAM__).strValue ())) == std::string (__YES_STRING__);
	if (oV.existOpenValue (__DRAGONWIND_BHVOPENVALUEHURMEDFLYSPARAM__))
		_canBeHurmedByFlyingMonsters = QGAMES::toUpper (std::string 
			(oV.openValue (__DRAGONWIND_BHVOPENVALUEHURMEDFLYSPARAM__).strValue ())) == std::string (__YES_STRING__);
	if (oV.existOpenValue (__DRAGONWIND_BHVOPENVALUEHURMEDLIGHTPARAM__))
		_canBeHurmedByLightning = QGAMES::toUpper (std::string 
			(oV.openValue (__DRAGONWIND_BHVOPENVALUEHURMEDLIGHTPARAM__).strValue ())) == std::string (__YES_STRING__);
	if (oV.existOpenValue (__DRAGONWIND_BHVOPENVALUEGUNSHURMPARAM__))
		_gunsHurming = QGAMES::getElementsFromAsInt (oV.openValue (__DRAGONWIND_BHVOPENVALUEGUNSHURMPARAM__).strValue (), ',');
	if (oV.existOpenValue (__DRAGONWIND_BHVOPENVALUEENERGYLOSTPARAM__))
		_energyLostPerLoop = __BD std::stof (oV.openValue (__DRAGONWIND_BHVOPENVALUEENERGYLOSTPARAM__).strValue ());
	if (oV.existOpenValue (__DRAGONWIND_BHVOPENVALUEENERGYTPEXPARAM__))
		_energyLostPerExplosion = __BD std::stof (oV.openValue (__DRAGONWIND_BHVOPENVALUEENERGYTPEXPARAM__).strValue ());
	if (oV.existOpenValue (__DRAGONWIND_BHVOPENVALUEPTSWHENDIEPARAM__))
		_pointsGivenWhenDie = std::stoi (oV.openValue (__DRAGONWIND_BHVOPENVALUEPTSWHENDIEPARAM__).strValue ());
	if (oV.existOpenValue (__DRAGONWIND_BHVOPENVALUESHTENERGYPARAM__))
		_shootingEnergy = std::stoi (oV.openValue (__DRAGONWIND_BHVOPENVALUESHTENERGYPARAM__).strValue ());
	if (oV.existOpenValue (__DRAGONWIND_BHVOPENVALUEAUTOMOVPARAM__))
		_automaticMovement = std::stoi (oV.openValue (__DRAGONWIND_BHVOPENVALUEAUTOMOVPARAM__).strValue ());
	if (oV.existOpenValue (__DRAGONWIND_BHVOPENVALUEWEAKNESSPARAM__))
		_weakness = __BD std::stof (oV.openValue (__DRAGONWIND_BHVOPENVALUEWEAKNESSPARAM__).strValue ());

	// Just to check the values...
	DRAGONWIND::Character::Behaviour::Behaviour 
		(_canBeHurmedByEnemies, _canBeHurmedBySimilars, _canBeHurmedByFlyingMonsters, _canBeHurmedByLightning, 
		  _gunsHurming, _energyLostPerLoop, _energyLostPerExplosion, _pointsGivenWhenDie, 
		  _shootingEnergy, _automaticMovement, _weakness);
}

// ---
QGAMES::SetOfOpenValues DRAGONWIND::Character::Behaviour::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result (std::string (__DRAGONWIND_RUNTIMEVALUESBEHAVIOURTYPE__));

	result.addOpenValue (0, QGAMES::OpenValue (_canBeHurmedByEnemies));
	result.addOpenValue (1, QGAMES::OpenValue (_canBeHurmedBySimilars));
	result.addOpenValue (2, QGAMES::OpenValue (_canBeHurmedByFlyingMonsters));
	result.addOpenValue (3, QGAMES::OpenValue (_canBeHurmedByLightning));
	std::string gHStr = std::to_string (_gunsHurming [0]);
	for (int i = 1; i < (int) _gunsHurming.size (); 
			gHStr += std::string (",") + std::to_string (_gunsHurming [i++]));
	result.addOpenValue (4, QGAMES::OpenValue (gHStr));
	result.addOpenValue (5, QGAMES::OpenValue (_energyLostPerLoop));
	result.addOpenValue (6, QGAMES::OpenValue (_energyLostPerExplosion));
	result.addOpenValue (7, QGAMES::OpenValue (_pointsGivenWhenDie));
	result.addOpenValue (8, QGAMES::OpenValue (_shootingEnergy));
	result.addOpenValue (9, QGAMES::OpenValue (_automaticMovement));
	result.addOpenValue (10, QGAMES::OpenValue (_weakness));

	return (result);
}

// ---
void DRAGONWIND::Character::Behaviour::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	assert (oV.name () == std::string (__DRAGONWIND_RUNTIMEVALUESBEHAVIOURTYPE__));
	assert (oV.existOpenValue (0) && oV.existOpenValue (1) && oV.existOpenValue (2) &&
			oV.existOpenValue (3) && oV.existOpenValue (4) && oV.existOpenValue (5) &&	
			oV.existOpenValue (6) && oV.existOpenValue (7) && oV.existOpenValue (8) &&	
			oV.existOpenValue (9) && oV.existOpenValue (10));

	_canBeHurmedByEnemies = oV.openValue (0).boolValue ();
	_canBeHurmedBySimilars = oV.openValue (1).boolValue ();
	_canBeHurmedByFlyingMonsters = oV.openValue (2).boolValue ();
	_canBeHurmedByLightning = oV.openValue (3).boolValue ();
	_gunsHurming = QGAMES::getElementsFromAsInt (oV.openValue (4).strValue (), ',');
	_energyLostPerLoop = oV.openValue (5).bdataValue ();
	_energyLostPerExplosion = oV.openValue (6).bdataValue ();
	_pointsGivenWhenDie = oV.openValue (7).intValue ();
	_shootingEnergy = oV.openValue (8).intValue ();
	_automaticMovement = oV.openValue (9).intValue ();
	_weakness = oV.openValue (10).bdataValue ();
}

// ---
bool DRAGONWIND::Character::Behaviour::canBeHurmedBy (int st)
{
	bool result = false;
	for (std::vector <int>::const_iterator i = _gunsHurming.begin (); 
			i != _gunsHurming.end () && !result; i++)
		if ((*i) == -1 || (*i) == st)
			result = true;
	return (result);
}

// ---
DRAGONWIND::Character::Character (int cId, DRAGONWIND::Character::Behaviour* bhv, 
	const QGAMES::Forms& f,	const QGAMES::PlatformArtist::Data& d)
			: DRAGONWIND::DragonArtist (cId, f, d),
			  _behaviour (bhv),
			  _energy (__BD 100), // Always full at construction time
			  _losingEnergy (true),
			  _score (0),
			  _shootings (), // Empty by default...
			  _energyToAdd (__BD 0),
			  _hitsBy (), // Nobody has hit the entity so far...
			  _exitingTheWater (false)
{ 
	assert (bhv);

	// Adds a buoy to be abble to "die" out of the process event manipulation...
	assert (!buoys ().empty ()); // Just in case...
	buoys ().insert (QGAMES::Buoys::value_type
		(__DRAGONWIND_DIEDEFEREDBUOYID__, new DRAGONWIND::Character::ToDieBuoy ()));
}

// ---
QGAMES::SetOfOpenValues DRAGONWIND::Character::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = DRAGONWIND::DragonArtist::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_energy));
	result.addOpenValue (lE + 2, QGAMES::OpenValue (_losingEnergy));
	result.addSetOfOpenValues (lNE + 1, _behaviour 
		? _behaviour -> asSetOfOpenValues () 
		: QGAMES::SetOfOpenValues (std::string (__DRAGONWIND_RUNTIMEVALUESBEHAVIOURTYPE__)));

	return (result);
}

// --
void DRAGONWIND::Character::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1) &&
			cfg.existSetOfOpenValues (lNE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_losingEnergy = cCfg.openValue (lE).boolValue ();
	cCfg.removeOpenValue (lE);

	setEnergy (cCfg.openValue (lE - 1).bdataValue ());
	cCfg.removeOpenValue (lE - 1);

	QGAMES::SetOfOpenValues bOV = cCfg.setOfOpenValues (lNE);
	if (_behaviour && bOV.numberValues () != 0)
		_behaviour -> fromSetOfOpenValues (bOV);
	cCfg.removeSetOfOpenValues (lNE);

	DRAGONWIND::DragonArtist::initializeRuntimeValuesFrom (cCfg);
}

// ---
bool DRAGONWIND::Character::isPlatformStatePossible (int pS) const
{
	return (platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_STAND ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_STANDATTACK ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_RUN ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_RUNATTACK ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_FALL ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_JUMP ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_JUMPANDATTACK ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_THROW ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_JUMPTHROW ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_DIEING ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_DIED);
	// Not all characters can be happy and can roll or swim or climb...just only ninja!
}

// ---
void DRAGONWIND::Character::setBehaviour (DRAGONWIND::Character::Behaviour* bhv)
{ 
	assert (bhv); 
	
	delete (_behaviour); 

	_behaviour = bhv; 
}

// ---
void DRAGONWIND::Character::setEnergy (QGAMES::bdata e)
{
	QGAMES::bdata lE = _energy;

	_energy = e;

	if (_energy > __BD 100) _energy = __BD 100;
	if (_energy < __BD 0) _energy = __BD 0;

	if (_energy != lE)
	{
		QGAMES::SetOfOpenValues oV;
		oV.addOpenValue (__DRAGONWIND_ENTITYENERGYVALUEPARAMETER__, QGAMES::OpenValue (_energy));
		notify (QGAMES::Event (__DRAGONWIND_ENTITYENERGYACTUALIZED__, this, oV)); // Notify the new level of energy...
	}
}

// ---
void DRAGONWIND::Character::setScore (int s)
{
	if (s == _score)
		return; 

	_score = s;

	QGAMES::SetOfOpenValues oV;
	oV.addOpenValue (__DRAGONWIND_GAMESCOREVALUEPARAMETER__, QGAMES::OpenValue (score ()));
	notify (QGAMES::Event (__DRAGONWIND_GAMESCOREACTUALIZED__, this, oV)); // Notify the new level of energy...
}

// ---
void DRAGONWIND::Character::loadShootings (const QGAMES::Entities& sth)
{
	for (QGAMES::Entities::const_iterator i = sth.begin (); i != sth.end (); i++)
	{
		DRAGONWIND::Shooting* sth = dynamic_cast <DRAGONWIND::Shooting*> ((*i).second);
		if (sth) 
		{
			sth -> setVisible (false);
			QGAMES::Observers obs = sth -> observers ();
			for (QGAMES::Observers::const_iterator j = obs.begin (); j != obs.end (); j++)
				if (dynamic_cast <DRAGONWIND::Character*> ((*j)))
					(*j) -> unObserve (sth); // Just because the shooting could be sometyhing reused 
											 // from anyother one...
			observe (sth); 
			_shootings [sth -> id ()] = sth;
		}
	}
}

// ---
DRAGONWIND::Shooting* DRAGONWIND::Character::firstAvailableShooting ()
{
	DRAGONWIND::Shooting* result = NULL;
	for (QGAMES::Entities::const_iterator i = _shootings.begin (); 
		i != _shootings.end () && !result; i++)
		if (!(*i).second -> isVisible ())
			result = dynamic_cast <DRAGONWIND::Shooting*> ((*i).second);

	return (result); 
}

// ---
bool DRAGONWIND::Character::isAlive () const
{
	return (DRAGONWIND::DragonArtist::isAlive () |
		onOffSwitch (_SWITCHTODIEWHENPOSSIBLE) -> isOn ()); 
}

// ---
bool DRAGONWIND::Character::toDie ()
{
	if (isDieing () || isDied () || onOffSwitch (_SWITCHTODIEWHENPOSSIBLE) -> isOn ())
		return (false); 

	if (!isOnAPlatformState () && !isSwiming ()) 
		onOffSwitch (_SWITCHTODIEWHENPOSSIBLE) -> set (true); 
	else
	{
		setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_DIEING));
		adjustPositionToBase (); // Because the character is already in a platform, 
								 // it is needed to adjust him/her to the it...

		notify (QGAMES::Event (__DRAGONWIND_CHARACTERHASDIED__, this)); 
	}

	return (true);
}

// ---
bool DRAGONWIND::Character::toMove (const QGAMES::Vector& dr)
{
	if (!isAlive ())
		return (false);

	// What to do when moving is a little bit different depending on the state the artist is on...
	// When falling, jumping or swiming:
	// Changes the direction the entity is looking to and also the direction of the movement is changed!
	if (isFalling () || isJumping () || isSwiming ())
	{
		// Wanted to go to the right and now looking to the left, or the other way around?
		// So is the user wanting to change the orientation?
		if ((dr.posX () > __BD 0 && lookingToDirection () != 0) ||
			(dr.posX () < __BD 0 && lookingToDirection () != 1)) 
		{
			QGAMES::PhysicsIncremetalMovement* mov =
				dynamic_cast <QGAMES::PhysicsIncremetalMovement*> (currentMovement ());
			assert (mov); 

			if (canMove (dr, QGAMES::Vector::_cero))
			{
				// Axis 0 = X; Change the direction the entity is looking to...
				// Change the direction the entity is looking at...
				// ...and adapt just the animation (take into account that the state doesn't change)
				mov -> changeDirectionInAxis (0); 
				setLookingToDirection (lookingToDirection () == 0 ? 1 : 0); 
				setCurrentAnimation 
					(animationIdForState (platformStateToDragonState (platformState ()), lookingToDirection ()));
			}
		}
	}

	else
	if (isStanding () && dr.posX () != __BD 0)
	{
		setLookingToDirection ((dr.posX () > __BD 0) ? 0 : 1); 
		setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_RUN)); 
		setFixBasePosition (baseWithXPositionAdjusted ());
	}

	else
	if (isRunning ())
	{
		if (dr.posX () > __BD 0 && lookingToDirection () != 0)
		{
			// It is needed to pass through this state,
			// for the change to happen!
			toStand (0); 
			setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_RUN));
			setFixBasePosition (baseWithXPositionAdjusted ());
		}
		else 
		if (dr.posX () < __BD 0 && lookingToDirection () != 1)
		{
			toStand (1); 
			setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_RUN));
			setFixBasePosition (baseWithXPositionAdjusted ());
		}
		else 
		if (dr.posX () == __BD 0)
		{
			toStand (lookingToDirection (), true); 
			setFixBasePosition (baseWithXPositionAdjusted ());
		}
	}

	else 
	if (isClimbing ())
	{
		// If the movement (going up) is not possible, then better don't move!
		if (dr.posY () < __BD 0 && !canClimbUp ())
			setMove (QGAMES::Vector::_cero); 
		else
		// if the movement is not possible (going down or to the sides), then better to fall!
		if ((dr.posY () > __BD 0  && !canClimbDown ()) ||
			(dr.posX () != __BD 0 && !canClimb ()))
			toFall (); 
		else
		{
			if (dr == QGAMES::Vector::_cero && 
				platformStateToDragonState () != DRAGONWIND::DragonArtist::DragonState::_CLIMBFIXED)
				setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_CLIMBFIXED));
			else 
			if (dr != QGAMES::Vector::_cero &&
				platformStateToDragonState () != DRAGONWIND::DragonArtist::DragonState::_CLIMB)
				setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_CLIMB));

			setMove (dr);
		}
	}

	// If slidding there is no possibility to change the movement...

	return (true);
}

// ---
bool DRAGONWIND::Character::toClimb (const QGAMES::Vector& dr)
{
	if (!isAlive ())
		return (false);

	if ((!isClimbing () && ((dr.posY () < __BD 0 && canClimbUp ()) ||
							(dr.posY () > __BD 0 && canClimbDown ()))) ||
		(isClimbing () && platformStateToDragonState () == DRAGONWIND::DragonArtist::DragonState::_CLIMBFIXED &&
			dr.posX () != __BD 0 && canClimb ()))
	{
		setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_CLIMB));
		setFixBasePosition (); 
		setMove (dr);
	}

	return (true);
}

// ---
bool DRAGONWIND::Character::toJump (int i, bool dP)
{
	if (!isAlive ())
		return (false);

	if (isJumping () || isSlidding ())
		return (false);

	// The jump is only possible when the entity is either on a platform...
	// ...or exiting the water!
	if (isOnAPlatformState () || _exitingTheWater)
	{
		if (!dP) // Meanins pure jumping...
		{
			setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_JUMP));
			// The movemenet parameters set when the platform state is set
			// are modified using the next instruction...
			// The direction is "incremented" attending to the impulse
			// and the acceleration doesn't change!
			QGAMES::bdata imp = __BD i / __BD 15;  
			if (imp > __BD 2) imp = __BD 2; if (imp < __BD 1.2) imp = __BD 1.2;
			setMove (direction () * imp, acceleration ()); 
			// The movement now has no maximum speed to alert when reached!
			QGAMES::PhysicsIncremetalMovement* mov =
				dynamic_cast <QGAMES::PhysicsIncremetalMovement*> (currentMovement ());
			assert (mov); // Just in case
			// There is no base when jumping...
			setFixBasePosition ();
		}
		else // Means slidding instead...
		{
			setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_SLIDE));
			// The impulse is modified...
			QGAMES::bdata imp = __BD (i / 15);  
			if (imp > __BD 2) imp = __BD 2; if (imp < __BD 1.2) imp = __BD 1.2;
			setMove (direction () * imp, acceleration ());
			QGAMES::PhysicsIncremetalMovement* mov =
				dynamic_cast <QGAMES::PhysicsIncremetalMovement*> (currentMovement ());
			assert (mov); // Just in case
			mov -> setMinSpeed (__BD 2);
			// The movement has to be adjusted to the previous base position 
			// as it is also a movement base on a platform...
			adjustPositionToBase ();
		}
	}

	else 
	if (isSwiming ())
	{
		// Jump when swiming means trying to exit the water!
		if (canExitWater () && !dP) 
		{
			_exitingTheWater = true;

			QGAMES::PhysicsIncremetalMovement* mov =
				dynamic_cast <QGAMES::PhysicsIncremetalMovement*> (currentMovement ());
			assert (mov); // Just in case

			// Fix the previous movement to something like running...
			QGAMES::Vector cD = mov -> directionInAxis ();
			QGAMES::bdata cS = mov -> currentSpeed ().module ();
			mov -> setSpeed (QGAMES::Vector (((__BD _data._physics [0] / __BD 3) * __BD ((lookingToDirection () == 0) ? 1 : -1)) * cD.posX (), 
				__BD -_data._physics [0] / __BD 4 * cD.posY (), __BD 0 * cD.posZ ()));
			mov -> setAcceleration (QGAMES::Vector::_cero);
			mov -> stopImpulses ();

			toJump (i, dP); // Double to get out of the water...
		}
	}

	return (true);
}

// ---
bool DRAGONWIND::Character::toShoot (int i, bool uP, int tS)
{
	if (!isAlive () || isSwiming () || (!isSwiming () && _exitingTheWater))
		return (false); 

	if (!isPossibleToShoot ())
		return (false); 

	if (numberShootingsAvailable () <= 0)
		return (false); 

	DRAGONWIND::Shooting* sht = firstAvailableShooting ();
	if (!sht)
		return (false); 

	// Creates a new definition object!
	QGAMES::PlatformGame::ElementDefinition* sDef = new QGAMES::PlatformGame::ElementDefinition;
	sDef -> _id = 0; // Not reusable...
	sDef -> _sceneId = map () -> scene () -> id ();
	sDef -> _worldId = map () -> scene () -> world () -> id ();
	sDef -> _type = (tS == -1) ? typeOfShootingAvailable () : tS; 
	QGAMES::Positions pos;
	pos.push_back (shootingPositionForType (sDef -> _type));
	sDef -> _positions = pos;

	sht -> setFromDefinition (sDef); // Sets the type of shooting...
	sht -> toLaunchFrom (this, uP /** Pressing up or not */ ? 2 : lookingToDirection (), i /** The force. */);

	// One shooting less...and notify the situation...
	reduceShootingsAvailable (1);

	observe (sht);

	return (true);
}

// ---
void DRAGONWIND::Character::whenWheatherCondition (QGAMES::EnvironmentalCondition* eC)
{
	DRAGONWIND::DragonArtist::whenWheatherCondition (eC);

	// Only the lightning could affect...
	if (isAlive () && // Nothing to do when the character is already died...
		dynamic_cast <QGAMES::Lightning*> (eC) &&
		((QGAMES::Lightning*) eC) -> hasCollisionWith (this) &&
		behaviour () -> _canBeHurmedByLightning)
		setEnergyToAdd (-100); // It is time to die...if possible...
}

// ---
void DRAGONWIND::Character::initialize ()
{
	DRAGONWIND::DragonArtist::initialize ();

	_energy = __BD 100;
	_energyToAdd = __BD 0;
	_score = 0;

	// Restart the shootings...
	for (int i = 0; i < (int) _shootings.size (); i++)
	{
		DRAGONWIND::Shooting* sth = (DRAGONWIND::Shooting*) _shootings [i];
		((DRAGONWIND::Shooting*) _shootings [i]) -> setFromNoDefinition ();
		((DRAGONWIND::Shooting*) _shootings [i]) -> setVisible (false);
	}

	_hitsBy = std::map <DRAGONWIND::Character*, int> (); // Hits clear...

	_exitingTheWater = false;

	reStartAllOnOffSwitches ();
}

// ---
void DRAGONWIND::Character::inEveryLoop ()
{
	DRAGONWIND::DragonArtist::inEveryLoop ();

	if (_losingEnergy)
		setEnergy (energy () -_behaviour -> _energyLostPerLoop + _energyToAdd);
	_energyToAdd = 0; // Once it's been used becomes useless...

	if (energy () == __BD 0)
	{
		notify (QGAMES::Event (__DRAGONWIND_ENTITYENERGYIS0__, this)); // The energy is 0 eventually...
		
		// ...and it has to die only if it possible...and it is not still alive!
		if (isAlive () && 
			(_behaviour -> _canBeHurmedByEnemies || _behaviour -> _canBeHurmedBySimilars))
			toDie ();
	}
}

// ---
void DRAGONWIND::Character::updatePositions ()
{
	if (!isVisible ())
		return;

	if (onOffSwitch (_SWITCHTODIEWHENPOSSIBLE) -> isOn () && isOnAPlatformState ())
	{
		onOffSwitch (_SWITCHTODIEWHENPOSSIBLE) -> set (false);
		setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_DIEING));

		notify (QGAMES::Event (__DRAGONWIND_CHARACTERHASDIED__, this));
	}

	DRAGONWIND::DragonArtist::updatePositions ();

	if (isBlocked ())
		toDie ();

	if (isDieing () && hasDied ())
		setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_DIED));

	if (isAlive () && canSwim () && !isSwiming () && !canExitWater () && !_exitingTheWater &&
		isPlatformStatePossible (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_SWIM))) 
	{
		setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_SWIM));
		setFixBasePosition (); 
	}

	if (!canSwim ())
		_exitingTheWater = false;

	if (_layerWater != NULL)
	{
		DRAGONWIND::LiquidTileLayer* lL = 
			dynamic_cast <DRAGONWIND::LiquidTileLayer*> (_layerWater);
		assert (lL);
		if (lL -> isPoison ())
			setEnergyToAdd (__BD -(behaviour () -> _energyLostPerLoop * 10));
	}
}

// ---
void DRAGONWIND::Character::whenCollisionWith (QGAMES::Entity* e)
{
	if (!isAlive ())
		return; 

	// When there is a collision with a flock of something similar...
	DRAGONWIND::FlockOfMonsters* fM = dynamic_cast <DRAGONWIND::FlockOfMonsters*> (e);
	if (fM) 
	{
		for (QGAMES::Entities::const_iterator i = fM -> entities ().begin (); i != fM -> entities ().end (); i++)
			if ((*i).second -> isVisible () && (*i).second -> hasCollisionWith (this))
				whenCollisionWith ((*i).second); 
		// ...look for the collision with the specific member of the flock!
		// The flock can be visible and any of the member invisible. This is the reason to check it.
	}

	// If the collision has been with a flying monster (it is one members of the flock)...
	// Take in account when swiming the character can't be affected
	DRAGONWIND::FlyingMonster* flM = dynamic_cast <DRAGONWIND::FlyingMonster*> (e);
	if (flM && _behaviour -> _canBeHurmedByFlyingMonsters && !isSwiming ()) 
		setEnergyToAdd (__BD -flM -> properties ()._shootingEnergy); //...reduces the energy in the amount defined

	// Has the entity collisioned with a shooting and the shooting doesn't belong to him / her?
	DRAGONWIND::Shooting* sth = dynamic_cast <DRAGONWIND::Shooting*> (e);
	if (sth && sth -> owner () != NULL && sth -> owner () != this) // Exists and the owner is not me...
	{
		if ((_behaviour -> _canBeHurmedByEnemies && isEnemy (sth -> owner ()) ||
		     _behaviour -> _canBeHurmedBySimilars && !isEnemy (sth -> owner ())) &&
			_behaviour -> canBeHurmedBy (sth -> definition () -> _type)) // supports the specific type...
		{
				setEnergyToAdd (__BD (-sth -> owner () -> _behaviour -> _shootingEnergy) * 
					_behaviour -> _weakness); // Decreases the energy...
				hitBy (sth -> owner ()); // Write down who hit me!
		}
	}
}

// ---
DRAGONWIND::Character* DRAGONWIND::Character::whoHitTheMost () const
{
	DRAGONWIND::Character* result = NULL;

	int nH = 0;
	for (std::map <DRAGONWIND::Character*, int>::const_iterator i = _hitsBy.begin (); 
			i != _hitsBy.end (); i++)
		if ((*i).second > nH)
			result = (*i).first;

	return (result);
}

// ---
void DRAGONWIND::Character::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __QGAMES_MOVEMENTNOTPOSSIBLE__)
	{
		if (isFalling () || isJumping ())
		{
			int cAffected = 0x00;
			if (orientation ().posX () != __BD 0)
				cAffected |= 0x01; // Change the x...
			if (isJumping () && orientation ().posY () < __BD 0 && 
					_lastCollisionAtDir [QGAMES::ArtistInATiledMap::TilePosition::UP])
				cAffected |= 0x02; // ...and they are jumping and going up!

			if (cAffected != 0x00)
			{
				if (cAffected & 0x02)
				{
					QGAMES::PhysicsIncremetalMovement* m = 
						dynamic_cast <QGAMES::PhysicsIncremetalMovement*> (currentMovement ());
					assert (m);
					m -> setSpeed (QGAMES::Vector 
						(m -> currentSpeed ().posX (), __BD 0, m -> currentSpeed ().posZ ())); // Stops moving up...
				}

				if (cAffected & 0x01)
					toMove (QGAMES::Vector 
						(-orientation ().posX (), orientation ().posY (), orientation ().posZ ())); // Changes direction...
			}
		}
	}
	else
	if (evnt.code () == __QGAMES_MOVEMENTSPEEDLIMITREACHED__)
	{
		if (isSlidding ())
		{
			// could the character stand?
			if (overTileWhenState (__QGAMES_PTSOLIDTILETYPE__, DRAGONWIND::DragonArtist::DragonState::_STAND))
				toDieDeferred (); // No, there is many solid things around. So, it has to die...
			else
				toStandDeferred (true); // Adjusting the base position...
		}
	}
	else
	if (evnt.code () == __DRAGONWIND_SHOOTINGTOEND__)
		((DRAGONWIND::Shooting*) evnt.data ()) -> setVisible (false); // To liberate the shooting...
	else
		DRAGONWIND::DragonArtist::processEvent (evnt);
}

// ---
__IMPLEMENTONOFFSWITCHES__ (DRAGONWIND::Character::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (DRAGONWIND::Character::_SWITCHTODIEWHENPOSSIBLE, false));
}

// ---
void DRAGONWIND::Character::afterAllLayersTestedInWhenOnTile ()
{
	if (!_layerBase)
	{
		if (_tileSolid)
		{
			toJump (3, false); // Out...
		}
		else
			DRAGONWIND::DragonArtist::afterAllLayersTestedInWhenOnTile ();
	}
	else
		DRAGONWIND::DragonArtist::afterAllLayersTestedInWhenOnTile ();
}

// ---
bool DRAGONWIND::Character::overTileWhenState (int tT, DRAGONWIND::DragonArtist::DragonState st)
{
	QGAMES::Entity::FormAnimation* an = 
		dynamic_cast <QGAMES::Entity::FormAnimation*> (animation (animationIdForState (st, lookingToDirection ())));
	assert (an); 

	int fId = currentForm () -> id ();
	int aId = currentAspect ();

	// Sets temporaly the new aspect and check whether it will or not be on a type of tile...
	setCurrentForm (an -> formId ());
	setCurrentAspect (an -> initialAspectId ()); 
	bool result = overTile (tT);

	setCurrentForm (fId);
	setCurrentAspect (aId);

	return (result);
}

// ---
bool DRAGONWIND::Character::overTileWhenState (const std::vector <int>& tT, 
	DRAGONWIND::DragonArtist::DragonState st)
{
	QGAMES::Entity::FormAnimation* an = 
		dynamic_cast <QGAMES::Entity::FormAnimation*> (animation (animationIdForState (st, lookingToDirection ())));
	assert (an); // Just in case...

	int fId = currentForm () -> id ();
	int aId = currentAspect ();

	setCurrentForm (an -> formId ());
	setCurrentAspect (an -> initialAspectId ()); 
	bool result = overTile (tT);

	setCurrentForm (fId);
	setCurrentAspect (aId);

	return (result);
}

// ---
QGAMES::Position DRAGONWIND::Character::shootingPositionForType (int t) const
{
	return ((t == __DRAGONWIND_NINJATHINGSBOMBTYPE__ || t == __DRAGONWIND_BADGUYTHINGPOLLBALLTYPE__ ) 
		? centerPosition () : position ());
}

// ---
void DRAGONWIND::Character::hitBy (DRAGONWIND::Character* chr)
{
	if (_hitsBy.find (chr) == _hitsBy.end ()) 
		_hitsBy [chr] = 0;
	_hitsBy [chr]++; 
}

// ---
void* DRAGONWIND::Character::ToDieBuoy::treatFor (QGAMES::Element* e)
{
	DRAGONWIND::Character* chr = dynamic_cast <DRAGONWIND::Character*> (e);
	assert (chr);

	chr -> toDie ();

	return (this);
}

// ---
void DRAGONWIND::Character::toDieDeferred ()
{
	DRAGONWIND::Character::ToDieBuoy* b = 
		dynamic_cast <DRAGONWIND::Character::ToDieBuoy*> (buoy (__DRAGONWIND_DIEDEFEREDBUOYID__));
	assert (b); // Just in case...

	b -> active (true); 
}
