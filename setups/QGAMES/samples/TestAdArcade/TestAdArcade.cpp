#include "TestAdArcade.hpp"

// --------------------------------------------------------------------------------
// ---
TestAdArcade::WhenFocusDecorator::WhenFocusDecorator ()
	: QGAMES::Widget::ComplexDecorator ()
{
	addDecorator (new QGAMES::Widget::BlinkingBackgroundDecorator 
		(QGAMES::Color (__QGAMES_WHITECOLOR__), 4, 100, 200, 1));
	addDecorator (new QGAMES::Widget::RoundingDecorator 
		(QGAMES::Color (__QGAMES_WHITECOLOR__), 2, QGAMES::Color (__QGAMES_TRANSPARENTCOLOR__), 5));
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestAdArcade::ControlWidget::processEvent (const QGAMES::Event& e)
{
	if (e.code () == __QGAMES_WIDGETGETFOCUSEVENT__)
		((QGAMES::Widget*) e.data ()) -> setDecorator (new TestAdArcade::WhenFocusDecorator ());
	else
	if (e.code () == __QGAMES_WIDGETLOSESFOCUSEVENT__)
		((QGAMES::Widget*) e.data ()) -> setDecorator (NULL);
		
	QGAMES::SelectionOptionsGameState::MenuControlWidget::processEvent (e);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::Widget* TestAdArcade::GUISystemBuilder::createWidget (const QGAMES::GUISystemBuilder::WidgetDefinition& def)
{
	QGAMES::Widget* result = NULL;

	if (def._id == __QGAMES_STDGUISYSTEMMAINWIDGET__ ||
		def._id == __QGAMES_STDGUISYSTEMMAINCONFWIDGET__) 
		result = new TestAdArcade::ControlWidget (def._id);
	else
		result = QGAMES::StandardGUISystemBuilder::createWidget (def);

	return (result);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestAdArcade::InputHandlerBehaviour::onKeyboardStatus (const std::vector <bool>& kS)
{
	assert (game () -> activeState () && game () -> activeState () -> nestedState ());

	// The active state is always the Control State.
	// The very real state being executed the one nested to that other
	if (game () -> activeState () -> nestedState () -> type () != __GAMETEST_PLAYINGGAMESTATE__ ||
		(game () -> activeState () -> nestedState () -> type () == __GAMETEST_PLAYINGGAMESTATE__ &&
		 ((TestAdArcade::ControllingGameStates*) game () -> activeState ()) -> isStopped ()))
		return; // If no playing, or playing but stopped, then nothing to execute...

	// If using joystick has been configured, here makes no sense to continue...
	if (((TestAdArcade::Game::Conf*) game () -> configuration ()) -> useJoystick ())
		return;

	TestAdArcade::SpaceBattleShip* chr = 
		dynamic_cast <TestAdArcade::SpaceBattleShip*> (game () -> entity (__GAMETEST_BATTLESHIPID__));
	assert (chr); // Just in case...

	int dMov = 0x00;
	dMov |= kS [_keys [__QGAMES_KEYUP__]]		? 0x01 : 0x00;
	dMov |= kS [_keys [__QGAMES_KEYRIGHT__]]	? 0x02 : 0x00;
	dMov |= kS [_keys [__QGAMES_KEYDOWN__]]		? 0x04 : 0x00;
	dMov |= kS [_keys [__QGAMES_KEYLEFT__]]		? 0x08 : 0x00;
	switch (dMov)
	{
		case 0x01:
			chr -> toForward (); break;
		case 0x03:
			chr -> toTheRightForward (); break;
		case 0x02:
			chr -> toTheRight (); break;
		case 0x06:
			chr -> toTheRightBackward (); break;
		case 0x04:
			chr -> toBackward (); break;
		case 0x0C:
			chr -> toTheLeftBackward (); break;
		case 0x08:
			chr -> toTheLeft (); break;
		case 0x09:
			chr -> toTheLeftForward (); break;
		default:
			chr -> toStay (); break;
	}

	if (kS [_keys [__QGAMES_KEYFIRE__]])
		chr -> toShoot ();
}

// ---
void TestAdArcade::InputHandlerBehaviour::manageKeyOnState (QGAMES::GameState* st, int kP)
{
	assert (st && st -> nestedState ());

	// The active state is always the Control State.
	// The very real state being executed the one nested to that other
	if (st -> nestedState () -> type () != __GAMETEST_PLAYINGGAMESTATE__)
		return;

	// To pause / or to continue the game...
	if (kP == _keys [__QGAMES_KEYPAUSE__])
	{
		if (game () -> isGamePaused ()) game () -> continueGame ();
		else game () -> pauseGame ();
	}

	// To save the current status of the game...
	if (kP == _keys [__QGAMES_KEYSAVE__])
		((TestAdArcade::Game*) game ()) -> 
			saveConfiguration (QGAMES::StandardInitialSelectionOptionsGameState::_CONFFILENAME);
}

// ---
void TestAdArcade::InputHandlerBehaviour::manageJoystickMovementOnState (QGAMES::GameState* st, int nJ, const QGAMES::Vector& dr)
{
	assert (st && st -> nestedState ());

	// The active state is always the Control State.
	// The very real state being executed the one nested to that other
	if (st -> nestedState () -> type () != __GAMETEST_PLAYINGGAMESTATE__ ||
		(game () -> activeState () -> nestedState () -> type () == __GAMETEST_PLAYINGGAMESTATE__ &&
		 ((TestAdArcade::ControllingGameStates*) game () -> activeState ()) -> isStopped ()) ||
		nJ != 0)
		return; // If no playing, or playing but stopped, or the joystick is not the 0, then nothing to execute...

	// If using joystick hasn't been configured, here makes no sense to continue...
	if (!((TestAdArcade::Game::Conf*) game () -> configuration ()) -> useJoystick ())
		return;

	TestAdArcade::SpaceBattleShip* chr = 
		dynamic_cast <TestAdArcade::SpaceBattleShip*> (game () -> entity (__GAMETEST_BATTLESHIPID__));
	assert (chr); // Just in case...

	int dMov = 0x00;
	dMov |= (dr.posY () < 0) ? 0x01 : 0x00;
	dMov |= (dr.posX () > 0) ? 0x02 : 0x00;
	dMov |= (dr.posY () > 0) ? 0x04 : 0x00;
	dMov |= (dr.posX () < 0) ? 0x08 : 0x00;
	switch (dMov)
	{
		case 0x01:
			chr -> toForward (); break;
		case 0x03:
			chr -> toTheRightForward (); break;
		case 0x02:
			chr -> toTheRight (); break;
		case 0x06:
			chr -> toTheRightBackward (); break;
		case 0x04:
			chr -> toBackward (); break;
		case 0x0C:
			chr -> toTheLeftBackward (); break;
		case 0x08:
			chr -> toTheLeft (); break;
		case 0x09:
			chr -> toTheLeftForward (); break;
		default:
			chr -> toStay (); break;
	}
}

// ---
void TestAdArcade::InputHandlerBehaviour::manageJoystickButtonOnState (QGAMES::GameState* st, int nJ, bool p, int nB, int bF)
{
	assert (st && st -> nestedState ());

	// The active state is always the Control State.
	// The very real state being executed the one nested to that other
	if (st -> nestedState () -> type () != __GAMETEST_PLAYINGGAMESTATE__ ||
		(game () -> activeState () -> nestedState () -> type () == __GAMETEST_PLAYINGGAMESTATE__ &&
		 ((TestAdArcade::ControllingGameStates*) game () -> activeState ()) -> isStopped ()) ||
		nJ != 0)
		return; // If no playing, or playing but stopped, or the joystick is not the 0, then nothing to execute...

	TestAdArcade::SpaceBattleShip* chr = 
		dynamic_cast <TestAdArcade::SpaceBattleShip*> (game () -> entity (__GAMETEST_BATTLESHIPID__));
	assert (chr); // Just in case...
	if (!p && (nB == 0 || nB == 10)) // In some joysticks (nintendo like) this should be the fire button...
		chr -> toShoot ();
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestAdArcade::NumberOfLivesScoreObject::NumberOfLivesScoreObject ()
	: QGAMES::ScoreObject (__GAMETEST_LIVESSCOREOBJECTID__)
{
	_forms.insert (QGAMES::Forms::value_type (__GAMETEST_BATTLESHIPLIVEFORM__, 
		QGAMES::Game::game () -> form (__GAMETEST_BATTLESHIPLIVEFORM__)));
	setCurrentForm (__GAMETEST_BATTLESHIPLIVEFORM__); 
}

// ---
void TestAdArcade::NumberOfLivesScoreObject::drawOn (QGAMES::Screen* scr, const QGAMES::Position& p)
{
	for (int i = ((TestAdArcade::Game*) game ()) -> lives (); i > 0; i--)
		currentForm () -> drawOn (scr, 0, p + QGAMES::Position (__BD (10 + (i - 1) * 55), __BD 10, __BD 0));
}

// ---
TestAdArcade::NumberOfPointsScoreObject::NumberOfPointsScoreObject ()
	: QGAMES::ScoreObjectNumber (__GAMETEST_POINTSSCOREOBJECTID__, 0, QGAMES::Forms (), 16)
{ 
	_forms.insert (QGAMES::Forms::value_type (__QGAMES_RAVIE24BLUELETTERS__, 
		QGAMES::Game::game () -> form (__QGAMES_RAVIE24BLUELETTERS__)));
	setCurrentForm (__QGAMES_RAVIE24BLUELETTERS__); 
	setNumberLength (5);
	setSpace (0);
	setPosition (QGAMES::Position (__BD 10, __BD 60, __BD 0));
}

// ---
void TestAdArcade::NumberOfPointsScoreObject::updatePositions ()
{
	QGAMES::ScoreObjectNumber::updatePositions ();

	setNumber (((TestAdArcade::Game*) game ()) -> score ());
}

// ---
TestAdArcade::NumberOfShootingsScoreObject::NumberOfShootingsScoreObject ()
	: QGAMES::ScoreObjectNumber (__GAMETEST_SHOOTINGSREAMININGSCOREOBJECTID__, 0, QGAMES::Forms (), 16)
{ 
	_forms.insert (QGAMES::Forms::value_type (__QGAMES_RAVIE12GREENLETTERS__, 
		QGAMES::Game::game () -> form (__QGAMES_RAVIE12GREENLETTERS__)));
	setCurrentForm (__QGAMES_RAVIE12GREENLETTERS__); 
	setNumberLength (5);
	setSpace (0);
	setPosition (QGAMES::Position (__BD 10, __BD 110, __BD 0));
}

// ---
void TestAdArcade::NumberOfShootingsScoreObject::updatePositions ()
{
	QGAMES::ScoreObjectNumber::updatePositions ();

	setNumber (((TestAdArcade::Game*) game ()) -> shootingsRemaining ());
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
const int TestAdArcade::SpaceBattleShip::_POWERDATA [__GAMETEST_BATTLESHIPMAXPOWER__][5] =
	{ { 0, 2,   1, 50,  200 }, 
	  { 0, 5,   2, 50,  200 }, 
	  { 1, 10,  2, 50,  200 }, 
	  { 1, 15,  3, 50,  200 }, 
	  { 2, 20,  2, 50,  200 }, 
	  { 3, 30,  3, 75,  400 }, 
	  { 3, 50,  4, 75,  400 }, 
	  { 4, 60,  5, 75,  400 }, 
	  { 5, 60,  5, 100, 600 }, 
	  { 5, 100, 5, 100, 600 } 
	};

// ---
QGAMES::Entity* TestAdArcade::SpaceBattleShip::clone () const
{ 
	QGAMES::Entity* result = new TestAdArcade::SpaceBattleShip (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
bool TestAdArcade::SpaceBattleShip::canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
{
	bool lX = ((position () + d).posX () > (__MAPWIDTHINPIXELS___ - currentForm () -> 
					frameWidthForFrame (currentAspect ())) && goingRight ()) ||
			  ((position () + d).posX () < 0 && goingLeft ()); // Out of screen in x axis?
	bool lY = ((position () + d).posY () > (__MAPHEIGHTINPIXELS__ - currentForm () -> 
					frameHeightForFrame (currentAspect ())) && goingBackward ()) ||
			  ((position () + d).posY () < 0 && goingForward ()); // Out of screen in Y axis?
	return (!(lX || lY));
}

// ---
void TestAdArcade::SpaceBattleShip::toTheRight ()
{
	if (!exploding () && !died () && !goingOnlyRight ())
		setCurrentState (__GAMETEST_BATTLESHIPMOVINGRIGHTSTATE__);
}

// ---
void TestAdArcade::SpaceBattleShip::toTheRightForward ()
{
	if (!exploding () && !died () && !goingRightForward ())
		setCurrentState (__GAMETEST_BATTLESHIPMOVINGRIGHTFORWARDSTATE__);
}

// ---
void TestAdArcade::SpaceBattleShip::toTheRightBackward ()
{
	if (!exploding () && !died () && !goingRightBackward ())
		setCurrentState (__GAMETEST_BATTLESHIPMOVINGRIGHTBACKWARDSTATE__);
}

// ---
bool TestAdArcade::SpaceBattleShip::goingOnlyRight () const
{
	return (currentState () && 
		currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGRIGHTSTATE__);
}

// ---
bool TestAdArcade::SpaceBattleShip::goingRight () const
{
	return (currentState () && 
		(currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGRIGHTSTATE__ ||
		 currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGRIGHTFORWARDSTATE__ ||
		 currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGRIGHTBACKWARDSTATE__));
}

// ---
bool TestAdArcade::SpaceBattleShip::goingRightForward () const
{
	return (currentState () &&
		currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGRIGHTFORWARDSTATE__);
}

// ---
bool TestAdArcade::SpaceBattleShip::goingRightBackward () const
{
	return (currentState () &&
		currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGRIGHTBACKWARDSTATE__);
}

// ---
void TestAdArcade::SpaceBattleShip::toTheLeft ()
{ 
	if (!exploding () && !died () && !goingOnlyLeft ())
		setCurrentState (__GAMETEST_BATTLESHIPMOVINGLEFTSTATE__);
}

// ---
void TestAdArcade::SpaceBattleShip::toTheLeftForward ()
{ 
	if (!exploding () && !died () && !goingLeftForward ())
		setCurrentState (__GAMETEST_BATTLESHIPMOVINGLEFTFORWARDSTATE__);
}

// ---
void TestAdArcade::SpaceBattleShip::toTheLeftBackward ()
{ 
	if (!exploding () && !died () && !goingLeftBackward ())
		setCurrentState (__GAMETEST_BATTLESHIPMOVINGLEFTBACKWARDSTATE__);
}

// ---
bool TestAdArcade::SpaceBattleShip::goingOnlyLeft () const
{
	return (currentState () && 
		currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGLEFTSTATE__);
}

// ---
bool TestAdArcade::SpaceBattleShip::goingLeft () const
{
	return (currentState () && 
			(currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGLEFTSTATE__ ||
			 currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGLEFTFORWARDSTATE__ ||
			 currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGLEFTBACKWARDSTATE__));
}

// ---
bool TestAdArcade::SpaceBattleShip::goingLeftForward () const
{
	return (currentState () &&
		currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGLEFTFORWARDSTATE__);
}

// ---
bool TestAdArcade::SpaceBattleShip::goingLeftBackward () const
{
	return (currentState () &&
		currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGLEFTBACKWARDSTATE__);
}

// ---
void TestAdArcade::SpaceBattleShip::toForward ()
{
	if (!exploding () && !died () && !goingForward ())
		setCurrentState (__GAMETEST_BATTLESHIPMOVINGFORWARDSTATE__);
}

// ---
bool TestAdArcade::SpaceBattleShip::goingForward () const
{
	return (currentState () && 
			currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGFORWARDSTATE__ ||
			currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGLEFTFORWARDSTATE__ ||
			currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGRIGHTFORWARDSTATE__);
}

// ---
void TestAdArcade::SpaceBattleShip::toBackward ()
{ 
	if (!exploding () && !died () && !goingBackward ())
		setCurrentState (__GAMETEST_BATTLESHIPMOVINGBACKWARDSTATE__);
}

// ---
bool TestAdArcade::SpaceBattleShip::goingBackward () const
{
	return (currentState () && 
			currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGBACKWARDSTATE__ ||
			currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGLEFTBACKWARDSTATE__ ||
			currentState () -> id () == __GAMETEST_BATTLESHIPMOVINGRIGHTBACKWARDSTATE__);
}

// ---
void TestAdArcade::SpaceBattleShip::toShoot ()
{
	if (!exploding () && !died())
		notify (QGAMES::Event (__GAMETEST_TRYTOSHOOTEVENT__, this));
}

// ---
void TestAdArcade::SpaceBattleShip::toStay ()
{
	if (!exploding () && !died () && !staying ())
		setCurrentState (__GAMETEST_BATTLESHIPSTAYINGSTATE__);
}

// ---
bool TestAdArcade::SpaceBattleShip::staying () const
{
	return (currentState () && currentState () -> id () == __GAMETEST_BATTLESHIPSTAYINGSTATE__);
}

// ---
void TestAdArcade::SpaceBattleShip::toRotate ()
{
	if (!rotating () && !exploding () && !died ())
		setCurrentState (__GAMETEST_BATTLESHIPROTATINGSTATE__);
}

// ---
bool TestAdArcade::SpaceBattleShip::rotating () const
{
	return (currentState () && currentState () -> id () == __GAMETEST_BATTLESHIPROTATINGSTATE__);
}

// ---
void TestAdArcade::SpaceBattleShip::toExplode ()
{
	if (!exploding ())
	{
		QGAMES::Position ctr = centerPosition ();
		setCurrentState (__GAMETEST_BATTLESHIPEXPLODINGSTATE__);
		setPosition (ctr - QGAMES::Vector (
			__BD (currentForm () -> frameWidthForFrame (currentAspect ()) >> 1), 
			__BD (currentForm () -> frameHeightForFrame (currentAspect ()) >> 1), 
			__BD 0));
		game () -> sound (__QGAMES_EXPLOSIONWAVSOUND__) -> play (__GAMETEST_BATTLESHIPBKCHANNEL__); // Not loop...
	}
}

// ---
bool TestAdArcade::SpaceBattleShip::exploding () const
{
	return (currentState () && currentState () -> id () == __GAMETEST_BATTLESHIPEXPLODINGSTATE__);
}

// ---
void TestAdArcade::SpaceBattleShip::toDie ()
{
	if (!died ())
		setCurrentState (__GAMETEST_BATTLESHIPBLANKSTATE__);
}

// ---
bool TestAdArcade::SpaceBattleShip::died () const
{
	return (currentState () && currentState () -> id () == __GAMETEST_BATTLESHIPBLANKSTATE__);
}

// ---
void TestAdArcade::SpaceBattleShip::setPowerLevel (int pL, int mNS)
{
	// The powe level has to be into the limits
	// Otherwise it is adjusted automatically into them...
	((TestAdArcade::Game*) game ()) -> setweaponsLevel (_powerLevel = (pL >= 0) ? 
		((pL < __GAMETEST_BATTLESHIPMAXPOWER__) ? pL : (__GAMETEST_BATTLESHIPMAXPOWER__ - 1)) : 0);
	
	// Set the shootings remaining to the beginning...
	_shootingsRemaining = (mNS == -1) 
		? _POWERDATA [_powerLevel][4] : ((_POWERDATA [_powerLevel][4] < mNS) ? _POWERDATA [_powerLevel][4] : mNS);
}

// ---
QGAMES::Position TestAdArcade::SpaceBattleShip::centralShootingPosition () const
{
	return (position () + QGAMES::Vector (__BD (currentForm () -> frameWidthForFrame (currentAspect ()) >> 1), __BD 0, __BD 0));
}

// ---
QGAMES::Rectangle TestAdArcade::SpaceBattleShip::collisionZone () const
{
	assert (_currentForm);

	QGAMES::Vector offSet (
		__BD _currentForm -> frameXOffsetForFrame (_currentAspect), 
		__BD _currentForm -> frameYOffsetForFrame (_currentAspect),
		__BD 0);

	QGAMES::Vector size (
		__BD _currentForm -> frameWidthForFrame (_currentAspect),
		__BD _currentForm -> frameHeightForFrame (_currentAspect), 
		__BD 0);

	QGAMES::Vector lSize = __BD 0.25 * size;
	
	return (QGAMES::Rectangle (_data._position + offSet + lSize, _data._position + offSet + size - lSize));
}

// ---
void TestAdArcade::SpaceBattleShip::initialize ()
{
	setCurrentState (__GAMETEST_BATTLESHIPSTAYINGSTATE__);

	// The initial position is in the middle of the screen...
	setPosition (QGAMES::Position (
		__BD ((__GAMETEST_SCREENWIDTH__ - currentForm () -> frameWidthForFrame (currentAspect ())) >> 1),
		__BD ((__GAMETEST_SCREENHEIGHT__ - currentForm () -> frameHeightForFrame (currentAspect ())) >> 1),
		__BD 0));
	// ...looking to the right
	setOrientation (QGAMES::Vector (__BD 1, __BD 0, __BD 0));

	setPowerLevel (((TestAdArcade::Game*) game ()) -> weaponsLevel ());

	QGAMES::Artist::initialize ();
}

// ---
void TestAdArcade::SpaceBattleShip::updatePositions ()
{
	QGAMES::Artist::updatePositions ();

	if (exploding () && 
		((QGAMES::Entity::FormAnimation*) animation (currentState () -> animationId ())) -> end ())
		toDie (); // After exploding it dies...

	if (died ()) // After dying, the most probably situation will be to change the state
		notify (QGAMES::Event (__GAMETEST_BATTLESHIPDESTROYED__, this)); // A notification is sent when died...
}

// ---
void TestAdArcade::SpaceBattleShip::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Artist::drawOn (s, p);

#ifndef NDEBUG
	s -> drawRectangle (collisionZone (), QGAMES::Color (120,120,120,255));
#endif
}

// ---
void TestAdArcade::SpaceBattleShip::whenCollisionWith (QGAMES::Entity* e)
{
	if (dynamic_cast <TestAdArcade::Asteroid*> (e) && moving () &&
		((TestAdArcade::Asteroid*) e) -> moving ())
		toExplode (); // To avoid two explosions...
	else if (dynamic_cast <TestAdArcade::ShootingToCatch*> (e))
		setPowerLevel (powerLevel () + 1);

	QGAMES::Artist::whenCollisionWith (e);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::Entity* TestAdArcade::SpaceBattleShipShooting::clone () const
{ 
	QGAMES::Entity* result = new TestAdArcade::SpaceBattleShipShooting (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void TestAdArcade::SpaceBattleShipShooting::toMove (int type)
{
	if (!moving ())
	{
		assert (type < __GAMETEST_BATTLESHIPSHOOTINGNUMBERTYPES__);
		setVisible (true);
		setCurrentState (__GAMETEST_BATTLESHIPSHOOTINGMOVINGSTATE__ + type);
		game () -> sound (__GAMETEST_SHOOTINGMOVINGBKSOUND__) -> play (__GAMETEST_SHOOTINGBKCHANNEL__);
	}
}

// ---
bool TestAdArcade::SpaceBattleShipShooting::moving () const
{
	return (currentState () && 
		(currentState () -> id () >= __GAMETEST_BATTLESHIPSHOOTINGMOVINGSTATE__ &&
		 currentState () -> id () < __GAMETEST_BATTLESHIPSHOOTINGMOVINGSTATE__ + __GAMETEST_BATTLESHIPSHOOTINGNUMBERTYPES__));
}

// ---
void TestAdArcade::SpaceBattleShipShooting::toDissapear ()
{
	if (!dissapeared ())
	{
		setVisible (false);
		setCurrentState (__GAMETEST_BATTLESHIPSHOOTINGBLANKSTATE__);
		game () -> sound (__GAMETEST_SHOOTINGMOVINGBKSOUND__) -> stop (); // Just in case...
	}
}

// ---
bool TestAdArcade::SpaceBattleShipShooting::dissapeared () const
{
	return (currentState () && currentState () -> id () >= __GAMETEST_BATTLESHIPSHOOTINGBLANKSTATE__);
}

// ---
QGAMES::Rectangle TestAdArcade::SpaceBattleShipShooting::collisionZone () const
{
	assert (_currentForm);

	QGAMES::Vector offSet (
		__BD _currentForm -> frameXOffsetForFrame (_currentAspect), 
		__BD _currentForm -> frameYOffsetForFrame (_currentAspect),
		__BD 0);

	QGAMES::Vector size (
		__BD _currentForm -> frameWidthForFrame (_currentAspect),
		__BD _currentForm -> frameHeightForFrame (_currentAspect), 
		__BD 0);

	QGAMES::Vector lSize = __BD 0.25 * size;
	
	return (QGAMES::Rectangle (_data._position + offSet + lSize, _data._position + offSet + size - lSize));
}

// ---
void TestAdArcade::SpaceBattleShipShooting::initialize ()
{
	setVisible (false);
	// Not visible by default...
	setCurrentState (__GAMETEST_BATTLESHIPSHOOTINGBLANKSTATE__);

	QGAMES::Artist::initialize ();
}

// ---
void TestAdArcade::SpaceBattleShipShooting::updatePositions ()
{
	// When the shooting reaches the limitof the screen has to disappear.
	// The situation is notified from the shooting movement, and managed at processEvent method.
	// Then the state should change, changing also the movement active and unObserving the previous one.
	// The list of observers becomes unconsistent, therefore the change of the state can't be done in the same cycle.
	// This is the reason for this variable to exist. The situation is "written down" in this variable
	// and treated in the next execution of the updtePositions method.
	// It could also be done using a buoy, but this is likely a simplier way.
	if (_dissapearNextLoop)
	{
		toDissapear ();
		_dissapearNextLoop = false;
	}

	QGAMES::Artist::updatePositions ();
}

// ---
void TestAdArcade::SpaceBattleShipShooting::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	QGAMES::Artist::drawOn (s, p);

#ifndef NDEBUG
	s -> drawRectangle (collisionZone (), QGAMES::Color (120,120,120,255));
#endif
} 

// ---
void TestAdArcade::SpaceBattleShipShooting::whenCollisionWith (QGAMES::Entity* e)
{
	if (dynamic_cast <TestAdArcade::Asteroid*> (e) && !((TestAdArcade::Asteroid*) e) -> exploding ())
		notify (QGAMES::Event (__GAMETEST_SHOOTINGALREADYUSED__, this)); // The shooting only finishes when hitting an asteroid....
}

// ---
void TestAdArcade::SpaceBattleShipShooting::processEvent (const QGAMES::Event& evnt)
{
	switch (evnt.code ())
	{
		case __QGAMES_MOVEMENTNOTPOSSIBLE__:
			notify (QGAMES::Event (__GAMETEST_SHOOTINGOUTOFSCREEN__, this));
			break;

		default:
			QGAMES::Artist::processEvent (evnt);
			break;
	}
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
const int TestAdArcade::Asteroid::_ENERGY [__GAMETEST_ASTEROIDNUMBERTYPES__] =
	{ 100, 150, 150, 150, 200, 200 };

// ---
QGAMES::Entity* TestAdArcade::Asteroid::clone () const
{ 
	QGAMES::Entity* result = new TestAdArcade::Asteroid (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void TestAdArcade::Asteroid::toMove (int type)
{
	if (!moving ())
	{
		assert (type < __GAMETEST_ASTEROIDNUMBERTYPES__);
		
		setVisible (true);
		setCurrentState (__GAMETEST_ASTEROIDMOVINGSTATE__ + type);
		_initialEnergy = _energy = _ENERGY [type];

		// The speed of the movement can be changed a little bit...
		QGAMES::SimpleLinearMovement* mov = (QGAMES::SimpleLinearMovement*) currentMovement ();
		mov -> setSpeed (mov -> speed () + (__BD ((rand () % 5) - 2))); // Quicker or slower?
	}
}

// ---
bool TestAdArcade::Asteroid::moving () const
{
	return (currentState () && 
		(currentState () -> id () >= __GAMETEST_ASTEROIDMOVINGSTATE__ &&
		 currentState () -> id () < __GAMETEST_ASTEROIDMOVINGSTATE__ + __GAMETEST_ASTEROIDNUMBERTYPES__));
}

// ---
void TestAdArcade::Asteroid::toExplode ()
{
	if (!exploding ())
	{
		setCurrentState (__GAMETEST_ASTEROIDEXPLODINGSTATE__);
		game () -> sound (__QGAMES_EXPLOSIONWAVSOUND__) -> play (__GAMETEST_ASTEROIDBKCHANNEL__); // Not loop...
	}
}

// ---
bool TestAdArcade::Asteroid::exploding () const
{
	return (currentState () && currentState () -> id () == __GAMETEST_ASTEROIDEXPLODINGSTATE__);
}

// ---
void TestAdArcade::Asteroid::toDissapear ()
{
	if (!dissapeared ())
	{
		setVisible (false);
		setCurrentState (__GAMETEST_ASTEROIDBLANKSTATE__);
	}
}

// ---
bool TestAdArcade::Asteroid::dissapeared () const
{
	return (currentState () && currentState () -> id () == __GAMETEST_ASTEROIDBLANKSTATE__);
}

// ---
QGAMES::Rectangle TestAdArcade::Asteroid::collisionZone () const
{
	assert (_currentForm);

	QGAMES::Vector offSet (
		__BD _currentForm -> frameXOffsetForFrame (_currentAspect), 
		__BD _currentForm -> frameYOffsetForFrame (_currentAspect),
		__BD 0);

	QGAMES::Vector size (
		__BD _currentForm -> frameWidthForFrame (_currentAspect),
		__BD _currentForm -> frameHeightForFrame (_currentAspect), 
		__BD 0);

	QGAMES::Vector lSize = __BD 0.10 * size;
	
	return (QGAMES::Rectangle (_data._position + offSet + lSize, _data._position + offSet + size - lSize));
}

// ---
void TestAdArcade::Asteroid::initialize ()
{
	setVisible (false);
	// Not visible by default...
	setCurrentState (__GAMETEST_ASTEROIDBLANKSTATE__);

	QGAMES::Artist::initialize ();
}

// ---
void TestAdArcade::Asteroid::updatePositions ()
{
	// When the asteroid reaches the limit of the screen has to disappear.
	// The situation is notified from the asteroid movement, and managed at processEvent method.
	// Then the state should change, changing also the movement active and unObserving the previous one.
	// The list of observers becomes unconsistent, therefore the change of the state can't be done in the same cycle.
	// This is the reason for this variable to exist. The situation is "written down" in this variable
	// and treated in the next execution of the updtePositions method.
	// It could also be done using a buoy, but this is likely a simplier way.
	if (_dissapearNextLoop)
	{
		toDissapear ();
		_dissapearNextLoop = false;
	}

	if (exploding () && 
		((QGAMES::Entity::FormAnimation*) animation (currentState () -> animationId ())) -> end ())
		notify (QGAMES::Event (__GAMETEST_ASTEROIDALREADYEXPLODED__, this)); // To be reused...

	QGAMES::Artist::updatePositions ();
}

// ---
void TestAdArcade::Asteroid::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	QGAMES::Artist::drawOn (s, p);

#ifndef NDEBUG
	s -> drawRectangle (collisionZone (), QGAMES::Color (120,120,120,255));
#endif
}

// ---
void TestAdArcade::Asteroid::whenCollisionWith (QGAMES::Entity* e)
{
	// It explodes when a shooting hits it
	if (dynamic_cast <TestAdArcade::SpaceBattleShipShooting*> (e) &&
		(!exploding () && (_energy -= ((TestAdArcade::SpaceBattleShipShooting*) e) -> energy ()) < 0))
	{
		((TestAdArcade::Game*) game ()) -> addScore (_initialEnergy / 10); // Point linked to initial energy...
		toExplode ();
	}
	
	QGAMES::Artist::whenCollisionWith (e);
}

// ---
void TestAdArcade::Asteroid::processEvent (const QGAMES::Event& evnt)
{
	switch (evnt.code ())
	{
		case __QGAMES_MOVEMENTNOTPOSSIBLE__:
			notify (QGAMES::Event (__GAMETEST_ASTEROIDOUTOFSCREEN__, this));
			break;

		default:
			QGAMES::Artist::processEvent (evnt);
			break;
	}
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::Entity* TestAdArcade::ShootingToCatch::clone () const
{
	QGAMES::Entity* result = new TestAdArcade::ShootingToCatch (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void TestAdArcade::ShootingToCatch::toMove (int type)
{
	if (!moving ())
	{
		assert (type < __GAMETEST_SHOOTINGSTYPESTOCATCH__);
		setVisible (true);
		setCurrentState (__GAMETEST_SHOOTINGTOCATCHMOVINGSTATE__ + type);
	}
}

// ---
bool TestAdArcade::ShootingToCatch::moving () const
{
	return (currentState () && 
		(currentState () -> id () >= __GAMETEST_SHOOTINGTOCATCHMOVINGSTATE__ &&
		 currentState () -> id () < __GAMETEST_SHOOTINGTOCATCHMOVINGSTATE__ + __GAMETEST_SHOOTINGSTYPESTOCATCH__));
}

// ---
void TestAdArcade::ShootingToCatch::toDissapear ()
{
	if (!dissapeared ())
	{
		setVisible (false);
		setCurrentState (__GAMETEST_SHOOTINGTOCATCHBLANKSTATE__);
	}
}

// ---
bool TestAdArcade::ShootingToCatch::dissapeared () const
{
	return (currentState () && currentState () -> id () == __GAMETEST_SHOOTINGTOCATCHBLANKSTATE__);
}

// ---
void TestAdArcade::ShootingToCatch::initialize ()
{
	setVisible (false);
	// Not visible by default...
	setCurrentState (__GAMETEST_SHOOTINGTOCATCHBLANKSTATE__);

	QGAMES::Artist::initialize ();
}

// ---
void TestAdArcade::ShootingToCatch::updatePositions ()
{
	// When the shooting to catch reaches the limit of the screen has to disappear.
	// The situation is notified from the shooting to catch movement, and managed at processEvent method.
	// Then the state should change, changing also the movement active and unObserving the previous one.
	// The list of observers becomes unconsistent, therefore the change of the state can't be done in the same cycle.
	// This is the reason for this variable to exist. The situation is "written down" in this variable
	// and treated in the next execution of the updtePositions method.
	// It could also be done using a buoy, but this is likely a simplier way.
	if (_dissapearNextLoop)
	{
		toDissapear ();
		_dissapearNextLoop = false;
	}

	QGAMES::Artist::updatePositions ();
}

// ---
void TestAdArcade::ShootingToCatch::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	QGAMES::Artist::drawOn (s, p);

#ifndef NDEBUG
	s -> drawRectangle (collisionZone (), QGAMES::Color (120,120,120,255));
#endif
}

// ---
void TestAdArcade::ShootingToCatch::whenCollisionWith (QGAMES::Entity* e)
{
	if (dynamic_cast <TestAdArcade::SpaceBattleShip*> (e))
		notify (QGAMES::Event (__GAMETEST_SHOOTINGTOCATCHCAUGHT__, this));
}

// ---
void TestAdArcade::ShootingToCatch::processEvent (const QGAMES::Event& evnt)
{
	switch (evnt.code ())
	{
		case __QGAMES_MOVEMENTNOTPOSSIBLE__:
			notify (QGAMES::Event (__GAMETEST_SHOOTINGTOCATCHOUTOFSCREEN__, this));
			break;

		default:
			QGAMES::Artist::processEvent (evnt);
			break;
	}
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::Entity* TestAdArcade::EntityBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	QGAMES::Entity* result = NULL;

	if (def._id == __GAMETEST_BATTLESHIPID__)
		result = new TestAdArcade::SpaceBattleShip (def._id);
	else if (def._id >= __GAMETEST_BATTLESHIPSHOOTINGBASEID__ && 
			 def._id < (__GAMETEST_BATTLESHIPSHOOTINGBASEID__ + __GAMETEST_BATTLESHITPMAXSHOOTINGS__))
		result = new TestAdArcade::SpaceBattleShipShooting (def._id);
	else if (def._id >= __GAMETEST_ASTEROIDBASEID__ && 
			 def._id < (__GAMETEST_ASTEROIDBASEID__ + __GAMETEST_MAXASTEROIDS__))
		result = new TestAdArcade::Asteroid (def._id);
	else if (def._id == __GAMETEST_BATTLESHIPSHOOTINGTOCATCHID__)
		result = new TestAdArcade::ShootingToCatch (def._id);
	else
		result = QGAMES::AdvancedEntityBuilder::createEntity (def);

	return (result);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::Movement* TestAdArcade::MovementBuilder::createMovement (const QGAMES::MovementBuilder::MovementDefinition& def)
{
	QGAMES::Movement* result = NULL;

	if (def._id == __GAMETEST_BATTLESHIPRUNMOVEMENT__)
		result = new QGAMES::SimpleLinearMovement (def._id, def._variables);
	else if (def._id == __GAMETEST_BATTLESHIPSTAYMOVEMENT__)
		result = new QGAMES::NoMovement (def._id, def._variables);
	else if (def._id >= __GAMETEST_BATTLESHIPSHOOTINGMOVEMENTBASEID__ && 
			 def._id < (__GAMETEST_BATTLESHIPSHOOTINGMOVEMENTBASEID__ + __GAMETEST_BATTLESHITPMAXSHOOTINGS__))
		result = new QGAMES::SimpleLinearMovement (def._id, def._variables);
	else if (def._id >= __GAMETEST_BATTLESHIPSHOOTINGNOMOVBASEID__ && 
			 def._id < (__GAMETEST_BATTLESHIPSHOOTINGNOMOVBASEID__ + __GAMETEST_BATTLESHITPMAXSHOOTINGS__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	else if (def._id >= __GAMETEST_ASTEROIDMOVEMENTBASEID__ && 
			 def._id < (__GAMETEST_ASTEROIDMOVEMENTBASEID__ + __GAMETEST_MAXASTEROIDS__))
		result = new QGAMES::SimpleLinearMovement (def._id, def._variables);
	else if (def._id >= __GAMETEST_ASTEROIDNOMOVEMENTBASEID__ && 
			 def._id < (__GAMETEST_ASTEROIDNOMOVEMENTBASEID__ + __GAMETEST_MAXASTEROIDS__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	else if (def._id == __GAMETEST_SHOOTINGTOCATCHMOVEMENT__)
		result = new QGAMES::SimpleLinearMovement (def._id, def._variables);
	else if (def._id == __GAMETEST_SHOOTINGTOCATCHNOMOVEMENT__)
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
		result = QGAMES::AdvancedMovementBuilder::createMovement (def);

	return (result);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestAdArcade::World::initialize ()
{
	setScene (__GAMETEST_SCENEID__);

	QGAMES::World::initialize ();

	(__AT (__AGM game ()) -> artist (__GAMETEST_BATTLESHIPID__)) -> setMap (_activeScene -> activeMap ());
	for (int i = 0; i < __GAMETEST_BATTLESHITPMAXSHOOTINGS__; i++)
		(__AT (__AGM game ()) -> artist (__GAMETEST_BATTLESHIPSHOOTINGBASEID__ + i)) -> setMap (_activeScene -> activeMap ());
	for (int i = 0; i < __GAMETEST_MAXASTEROIDS__; i++)
		(__AT (__AGM game ()) -> artist (__GAMETEST_ASTEROIDBASEID__ + i)) -> setMap (_activeScene -> activeMap ());
	(__AT (__AGM game ()) -> artist (__GAMETEST_BATTLESHIPSHOOTINGTOCATCHID__)) -> setMap (_activeScene -> activeMap ());
}

// ---
void TestAdArcade::World::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __GAMETEST_LEVELCOMPLETE__)
		notify (evnt);

	QGAMES::World::processEvent (evnt);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
const QGAMES::bdata TestAdArcade::Scene::_LEVEL [__GAMETEST_MAXNUMBERLEVELS__][4] =
	{ { __BD 1,    __BD 50, __BD 10, __BD 60 },
	  { __BD 1,    __BD 55, __BD 15, __BD 60 },
	  { __BD 1,    __BD 60, __BD 20, __BD 60 },
	  { __BD 0.75, __BD 50, __BD 10, __BD 60 },
	  { __BD 0.75, __BD 60, __BD 20, __BD 80 },
	  { __BD 0.75, __BD 70, __BD 30, __BD 80 },
	  { __BD 0.5,  __BD 60, __BD 20, __BD 80 },
	  { __BD 0.5,  __BD 70, __BD 30, __BD 90 },
	  { __BD 0.25, __BD 70, __BD 30, __BD 90 },
	  { __BD 0.25, __BD 80, __BD 40, __BD 100 } 
	};

// ---
void TestAdArcade::Scene::setLevel (int l)
{
	_level = l;
	if (_level < 0) _level = 0;
	if (_level > __GAMETEST_MAXNUMBERLEVELS__) _level = __GAMETEST_MAXNUMBERLEVELS__;
	
	((TestAdArcade::Game*) game ()) -> setLevel (_level); 

	counter (_COUNTERTOASTEROID) -> 
		initialize ((int) (_LEVEL [_level][0] * game () -> framesPerSecond () / 
			(((TestAdArcade::Game::Conf*) game () -> configuration ()) -> difficulty () + 1)), 0, true, true);
	counter (_COUNTERBATLESHIPSHOOTING) -> initialize ((int) (_LEVEL [_level][0] * __BD 1.25 * game () -> 
		framesPerSecond ()), 0, true, true);
}

// ---
void TestAdArcade::Scene::initialize ()
{
	addEntity (__AT (__AGM game ()) -> artist (__GAMETEST_BATTLESHIPID__));

	_freeShootings.clear ();
	for (int i = 0; i < __GAMETEST_BATTLESHITPMAXSHOOTINGS__; i++)
	{
		QGAMES::Entity* shty = __AT (__AGM game ()) -> artist (__GAMETEST_BATTLESHIPSHOOTINGBASEID__ + i);
		_freeShootings.push_back (shty);
		addEntity (shty); // It will be initialized later...
	}

	_freeAsteroids.clear ();
	for (int i = 0; i < __GAMETEST_MAXASTEROIDS__; i++)
	{
		QGAMES::Entity* ast = __AT (__AGM game ()) -> artist (__GAMETEST_ASTEROIDBASEID__ + i);
		_freeAsteroids.push_back (ast);
		addEntity (ast); // It will be initialized later...
	}

	addEntity (_shootingToCatch = 
		(TestAdArcade::ShootingToCatch*) (__AGM game ()) -> artist (__GAMETEST_BATTLESHIPSHOOTINGTOCATCHID__));

	setMap (__GAMETEST_MAPID__);
	activeMap () -> layer (std::string (__GAMETEST_MAPLAYERBACKGROUND__)) -> setVisible (false);
	// Instead of changing the map definition file, the layer is set to invisible
	// This is because it is in my interest to test all properties as possible in definition...

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	setLevel (((TestAdArcade::Game*) game ()) -> level ()); // Of the current player...

	if (_timeInLevel == NULL)
		observe (_timeInLevel = game () -> 
			createLoopCounter (0, (int) (_LEVEL [_level][3] * game () -> framesPerSecond ()), true));
	else
		_timeInLevel -> initialize ((int) (_LEVEL [_level][3] * game () -> framesPerSecond ()), true);

	QGAMES::Scene::initialize ();
}

// ---
void TestAdArcade::Scene::updatePositions ()
{
	if (!onOffSwitch (_SWITCHCANSHOOT) -> isOn ())
	{
		if (counter (_COUNTERTOSHOOT) -> isEnd ())
			onOffSwitch (_SWITCHCANSHOOT) -> set (true);
	}

	// A new asteroid appears depending on the level
	if (counter (_COUNTERTOASTEROID) -> isEnd ())
	{
		if ((rand () % 100 < (int) (__BD _LEVEL [_level][1] * 
				(__BD 1 + (__BD ((TestAdArcade::Game::Conf*) game () -> configuration ()) -> difficulty ()) / __BD 10))) && 
			(int) _freeAsteroids.size () > 0)
		{
			TestAdArcade::Asteroid* ast = 
				(TestAdArcade::Asteroid*) (*_freeAsteroids.begin ()); 
			_freeAsteroids.pop_front ();
			ast -> toMove (rand () % __GAMETEST_ASTEROIDNUMBERTYPES__); // Start to move...
			ast -> setPosition (QGAMES::Position (
				__BD (rand () % 
					(__GAMETEST_SCREENWIDTH__ - ast -> currentForm () -> frameWidthForFrame (ast -> currentAspect ()))),
				__BD -100, __BD 0));
		}
	}

	// A new battleship shooting appears depending on the level 
	if (counter (_COUNTERBATLESHIPSHOOTING) -> isEnd ())
	{
		if ((rand () % 100 < (int) _LEVEL [_level][2]) && !_shootingToCatch -> isVisible ())
		{
			_shootingToCatch -> toMove (rand () % __GAMETEST_SHOOTINGSTYPESTOCATCH__); // Starts to move...
			_shootingToCatch -> setPosition (QGAMES::Position (
				__BD (rand () % 
					(__GAMETEST_SCREENWIDTH__ - 
						_shootingToCatch -> currentForm () -> frameWidthForFrame (_shootingToCatch -> currentAspect ()))),
				__BD -50, __BD 0));
		}
	}

	QGAMES::Scene::updatePositions ();
}

// ---
void TestAdArcade::Scene::finalize ()
{
	QGAMES::Scene::finalize ();

	removeEntity (__AT (__AGM game ()) -> artist (__GAMETEST_BATTLESHIPID__));

	_freeShootings.clear ();
	for (int i = 0; i < __GAMETEST_BATTLESHITPMAXSHOOTINGS__; i++)
		removeEntity (__AT (__AGM game ()) -> artist (__GAMETEST_BATTLESHIPSHOOTINGBASEID__ + i)); 

	_freeAsteroids.clear ();
	for (int i = 0; i < __GAMETEST_MAXASTEROIDS__; i++)
		removeEntity (__AT (__AGM game ()) -> artist (__GAMETEST_ASTEROIDBASEID__ + i)); 

	_shootingToCatch = NULL;
	removeEntity (__AT (__AGM game ()) -> artist (__GAMETEST_BATTLESHIPSHOOTINGTOCATCHID__));

	setMap (__MININT__);
}

// ---
void TestAdArcade::Scene::processEvent (const QGAMES::Event& evnt)
{
	switch (evnt.code ())
	{
		// This event comes always from battleship...
		case __GAMETEST_TRYTOSHOOTEVENT__:
			if (onOffSwitch (_SWITCHCANSHOOT) -> isOn ())
				tryToActivateNewShootingFrom ((TestAdArcade::SpaceBattleShip*) evnt.data ());
			break;

		// This event comes from a shoot
		case __GAMETEST_SHOOTINGOUTOFSCREEN__:
		// ...and this other when the shooting has collisioned with an asteroid
		case __GAMETEST_SHOOTINGALREADYUSED__:
			{
				TestAdArcade::SpaceBattleShipShooting* shty = 
					(TestAdArcade::SpaceBattleShipShooting*) evnt.data ();
				shty -> toDissapearNextLoop (); // The shooting will be free back!
				_freeShootings.push_back (shty); // In the list again...
			}

			break;

		// This event comes from an asteroid...(when it has left the screen)
		case __GAMETEST_ASTEROIDOUTOFSCREEN__:
		// ...and this other too...(when it has exploded)
		case __GAMETEST_ASTEROIDALREADYEXPLODED__:
			{
				TestAdArcade::Asteroid* ast = (TestAdArcade::Asteroid*) evnt.data ();
				ast -> toDissapearNextLoop ();
				_freeAsteroids.push_back (ast);

				// The asteroid has to dissapear, but the puntuation of the player is reduced also in 
				// a percentage of the initial energy of the asteroid...
				if (evnt.code () == __GAMETEST_ASTEROIDOUTOFSCREEN__)
					((TestAdArcade::Game*) game ()) -> addScore (-ast -> initialEnergy () / 10);
			}

			break;

		// This event comes from a shooting to catch...
		case __GAMETEST_SHOOTINGTOCATCHOUTOFSCREEN__:
			{
				// Appart of dissapear from the screen...
				// when a shooting to catch is not cought the powel level is reduced in 1 (if possible)...
				TestAdArcade::SpaceBattleShip* bS = 
					((TestAdArcade::SpaceBattleShip*) entity (__GAMETEST_BATTLESHIPID__));
				bS -> setPowerLevel (bS -> powerLevel () - 1, bS -> shootingsReaming ());
			}

		// ...and this when the shoothing to catch has collisioned with the battleship
		case __GAMETEST_SHOOTINGTOCATCHCAUGHT__:
			{
				TestAdArcade::ShootingToCatch* stc = (TestAdArcade::ShootingToCatch*) evnt.data ();
				stc -> toDissapearNextLoop ();
			}

			break;

		// This event comes from the loop counter...
		case __QGAMES_LOOPCOUNTEREVENT__:
			notify (QGAMES::Event (__GAMETEST_LEVELCOMPLETE__, this));
			break;

		default:
			QGAMES::Scene::processEvent (evnt);
			break;
	}
}

// ---
__IMPLEMENTCOUNTERS__ (TestAdArcade::Scene::Counters)
{
	addCounter (new QGAMES::Counter (_COUNTERTOSHOOT, 4, 0, true, true));
	addCounter (new QGAMES::Counter (_COUNTERTOASTEROID, (int) (0.5 * QGAMES::Game::game () -> framesPerSecond ()), 0, true, true));
	addCounter (new QGAMES::Counter (_COUNTERBATLESHIPSHOOTING, (int) (0.5 * QGAMES::Game::game () -> framesPerSecond ()), 0, true, true));
	// These two last one will depend on the level later...
}

// ---
__IMPLEMENTONOFFSWITCHES__ (TestAdArcade::Scene::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHCANSHOOT, false));
}

// ---
void TestAdArcade::Scene::tryToActivateNewShootingFrom (TestAdArcade::SpaceBattleShip* bS)
{
	// The number of shootings remaining...
	int nSR = bS -> shootingsReaming ();
	// The max number of shootings will depend on the level of power that the battleship has now.
	int mS = bS -> maxNumberOfShootings ();
	// The more power the battleship has the wider the shooting is
	// The width of the shooting is determinated by the number of instances in pararllel of the specific shoot.
	int s  = bS -> instancesPerShooting ();
	// Where the first shoot will be place?
	QGAMES::Position pS = bS -> centralShootingPosition ();
	if (s % 2 == 0) pS -= QGAMES::Vector (__BD 10, __BD 0, __BD 0);
	pS -= __BD ((s - 1) / 2) * QGAMES::Vector (__BD 20, __BD 0, __BD 0);

	// Is shooting be possible? Are there enough space? Has the battleship 
	if (((__GAMETEST_BATTLESHITPMAXSHOOTINGS__ - (int) _freeShootings.size ()) + s) <= mS && 
		(int) _freeShootings.size () >= s && nSR >= s)
	{
		onOffSwitch (_SWITCHCANSHOOT) -> set (false);
		for (int j = 0; j < s; j++)
		{
			TestAdArcade::SpaceBattleShipShooting* bSS = 
				(TestAdArcade::SpaceBattleShipShooting*) (*_freeShootings.begin ()); 
			_freeShootings.pop_front ();
			bSS -> setEnergy (bS -> shootingEnergy ());
			bSS -> toMove (bS -> typeOfShooting ()); // Start to move...
			bSS -> setPosition (pS - 
				QGAMES::Vector (__BD (bSS -> currentForm () -> frameWidth () >> 1), __BD 0, __BD 0));
			pS += QGAMES::Vector (__BD 20, __BD 0, __BD 0);

			bS -> reduceShootingsReaming (1);
		}
	}
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::World* TestAdArcade::WorldBuilder::createWorldObject (int no, const QGAMES::Scenes& s, 
		const QGAMES::WorldProperties& p)
{
	QGAMES::World* result = NULL;
	if (no == __GAMETEST_WORLDID__)
		result = new TestAdArcade::World (no, s, p);
	return (result);
}

// ---
QGAMES::Scene* TestAdArcade::WorldBuilder::createSceneObject (int ns, const QGAMES::Maps& m, 
		const QGAMES::Scene::Connections& cn, 
		const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
{
	QGAMES::Scene* result = NULL;
	if (ns == __GAMETEST_SCENEID__)
		result = new TestAdArcade::Scene (ns, m, cn, p, ePL);
	return (result);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestAdArcade::ControllingGameStates::ControllingGameStates (QGAMES::GameState* psSt)
	: QGAMES::AdvancedGameStateControl (__GAMETEST_CONTROLGAMESTATE__, psSt, 
		QGAMES::GameStates (), QGAMES::GameStateControl::FlowMap ())
{
	QGAMES::GameStates sts;
	sts [std::string (__QGAMES_GAMESTATELOADINGNAME__)] =
		game () -> state (std::string (__QGAMES_GAMESTATELOADINGNAME__));
	sts [std::string (__GAMETEST_INTROGAMESTATENAME__)] = 
		game () -> state (std::string (__GAMETEST_INTROGAMESTATENAME__));
	sts [std::string (__GAMETEST_PRESENTATIONGAMESTATENAME__)] = 
		game () -> state (std::string (__GAMETEST_PRESENTATIONGAMESTATENAME__));
	sts [std::string (__GAMETEST_SELECTIONGAMESTATENAME__)] = 
		game () -> state (std::string (__GAMETEST_SELECTIONGAMESTATENAME__));
	sts [std::string (__GAMETEST_HALLOFFAMEGAMESTATENAME__)] =
		game () -> state (std::string (__GAMETEST_HALLOFFAMEGAMESTATENAME__));
	sts [std::string (__GAMETEST_ABOUTTOSTARTGAMESTATENAME__)] =
		game () -> state (std::string (__GAMETEST_ABOUTTOSTARTGAMESTATENAME__));
	sts [std::string (__GAMETEST_PLAYINGGAMESTATENAME__)] =
		game () -> state (std::string (__GAMETEST_PLAYINGGAMESTATENAME__));
	sts [std::string (__GAMETEST_LEVELHASCOMPLETEDGAMESTATENAME__)] =
		game () -> state (std::string (__GAMETEST_LEVELHASCOMPLETEDGAMESTATENAME__));
	sts [std::string (__GAMETEST_PLAYERHASDIEDGAMESTATENAME__)] =
		game () -> state (std::string (__GAMETEST_PLAYERHASDIEDGAMESTATENAME__));
	sts [std::string (__GAMETEST_GAMEHASENDEDGAMESTATENAME__)] =
		game () -> state (std::string (__GAMETEST_GAMEHASENDEDGAMESTATENAME__));
	sts [std::string (__QGAMES_EXITGAMEGAMESTATENAME__)] =
		game () -> state (std::string (__QGAMES_EXITGAMEGAMESTATENAME__));
	setStates (sts);
}

// ---
std::string TestAdArcade::ControllingGameStates::nextGameState 
	(QGAMES::GameState* st, const QGAMES::Event& evnt)
{
	assert (st);

	std::string result (__NULL_STRING__);
	switch (st -> type ())
	{
		// From loading to intro
		case __QGAMES_GAMESTATELOADING__:
			result = std::string (__GAMETEST_INTROGAMESTATENAME__);
			break;

		// From intro to presentation...
		case __GAMETEST_INTROGAMESTATE__:
			result = std::string (__GAMETEST_PRESENTATIONGAMESTATENAME__);
			break;

		// From presentation to selection...
		case __GAMETEST_PRESENTATIONGAMESTATE__:
			result = std::string (__GAMETEST_SELECTIONGAMESTATENAME__);
			break;

		// From the selection to...depends on the option selected
		case __GAMETEST_SELECTIONGAMESTATE__:
			{
				TestAdArcade::SelectionGameState* gS = (TestAdArcade::SelectionGameState*) st;
				switch (gS -> lastOptionSelected ())
				{
					case __QGAMES_STDGUISYSTEMSTARTGAMEWDT__:
						result = std::string (__GAMETEST_ABOUTTOSTARTGAMESTATENAME__); // To start the game
						break;

					case __QGAMES_STDGUISYSTEMHALLOFFAMEWDT__:
						result = std::string (__GAMETEST_HALLOFFAMEGAMESTATENAME__); // To the hall of fame
						break;

					case __QGAMES_STDGUISYSTEMEXITWDT__:
						result = std::string (__QGAMES_EXITGAMEGAMESTATENAME__); // To exit
						break;

					default:
						assert (0); // It shouldn't be here...
						break;
				}
			}

			break;

		// After the hall of fame, then the selection is back...
		case __GAMETEST_HALLOFFAMEGAMESTATE__:
			result = std::string (__GAMETEST_SELECTIONGAMESTATENAME__);
			break;

		// After saying the game is about to start...it starts eventually!
		case __GAMETEST_ABOUTTOSTARTGAMESTATE__:
			result = std::string (__GAMETEST_PLAYINGGAMESTATENAME__);
			break;

		// Playing finishes just because only two reasons:
		// either the player has reached his goal and it is time to change the level
		// or the player has died, then it is time the next player to play or (if there is no more lives)
		// the game ended (so probably here we have the most complex logic in terms of game state flow of the game)
		case __GAMETEST_PLAYINGGAMESTATE__:
			{
				TestAdArcade::Playing* pS = (TestAdArcade::Playing*) st;
				switch (pS -> result ())
				{
					case TestAdArcade::Playing::Result::_PLAYERDIED:
						result = std::string (__GAMETEST_PLAYERHASDIEDGAMESTATENAME__);
						break;

					case TestAdArcade::Playing::Result::_LEVELACHIEVED:
						result = std::string (__GAMETEST_LEVELHASCOMPLETEDGAMESTATENAME__);
						break;

					default:
						assert (0); // It shouldn't be here even when the current result is still playing...
						break;
				}
			}

			break;

		// From player has died to game has ended or start back!
		// depending on the number of lives still to consume
		case __GAMETEST_PLAYERHASDIEDGAMESTATE__:
			{
				const TestAdArcade::Game* g = (const TestAdArcade::Game*) game ();
				const TestAdArcade::Game::Conf* cfg = (const TestAdArcade::Game::Conf*) g -> configuration ();
				if (g -> lives () == 1 && g -> currentPlayer () == cfg -> numberPlayers ())
					result = std::string (__GAMETEST_GAMEHASENDEDGAMESTATENAME__);
				else
					result = std::string (__GAMETEST_ABOUTTOSTARTGAMESTATENAME__);
			}

			break;

		// From player has completed the level to about to start the next
		case __GAMETEST_LEVELHASCOMPLETEDGAMESTATE__:
			result = std::string (__GAMETEST_ABOUTTOSTARTGAMESTATENAME__);
			break;

		// From game has ended to the hall of fame, with the results!!
		case __GAMETEST_GAMEHASENDEDGAMESTATE__:
			result = std::string (__GAMETEST_HALLOFFAMEGAMESTATENAME__);
			break;

		default:
			assert (0); // It shouldn't be here...
			break;
	}

	return (result);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
bool TestAdArcade::UntilReturnKeyOrMouseLeftButtonIsPressed::StopStateHandler::stopWhen (const QGAMES::Event& evnt)
{
	bool result = false;
	if (evnt.code () == __QGAMES_KEYBOARDEVENT__)
	{
		QGAMES::KeyBoardEventData* dt = (QGAMES::KeyBoardEventData*) evnt.data ();
		if (!dt -> _on && dt ->_internalCode == QGAMES::KeyCode::QGAMES_RETURN)
			result = true;
	}
	else
	if (evnt.code () == __QGAMES_MOUSEBUTTONEVENT__)
	{
		QGAMES::MouseButtonEventData* dt = (QGAMES::MouseButtonEventData*) evnt.data ();
		if (!dt -> _on && dt ->_internalCode == QGAMES::MouseButtonCode::QGAMES_BUTTONLEFT)
			result = true;
	}

	return (result);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestAdArcade::PresentationTitlesGameState::drawOn (QGAMES::Screen* s)
{
	// Change the intensity of the battleship when needed...
	QGAMES::ShowingFixFormGameState::Properties prps = 
		((QGAMES::ShowingFixFormGameState*) nestedState ()) -> properties ();
	prps._fade = (_titleToPaint == 3) ? _titleStatus [_titleToPaint] -> _fadeGrade : 0;
	((QGAMES::ShowingFixFormGameState*) nestedState ()) -> setProperties (prps);

	QGAMES::CreditTitlesGameState::drawOn (s);
}

// ---
QGAMES::ScoreObjectText* TestAdArcade::PresentationTitlesGameState::createText (const std::string& txt)
{
	if (txt.find (std::string ("Battleship")) != std::string::npos)
		return (new QGAMES::Ravie36GreenFont (txt)); // A little bit bigger than standard!
	else if (txt.find (std::string ("Press")) != std::string::npos)
		return (new QGAMES::Ravie24BlueFont (txt)); // In another color!
	return (new QGAMES::Ravie24OrangeFont (txt)); // The normal font...
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestAdArcade::PresentationGameState::PresentationGameState ()
	: TestAdArcade::UntilReturnKeyOrMouseLeftButtonIsPressed (__GAMETEST_PRESENTATIONGAMESTATE__, 
		new QGAMES::ShowingFixFormGameState (__GAMETEST_PRESENTATIONGAMESTATE__ + 1, // The toppest element is the little logo of community...
			QGAMES::ShowingFixFormGameState::Properties (), 
		new TestAdArcade::PresentationTitlesGameState (__GAMETEST_PRESENTATIONGAMESTATE__  + 2, // Credit titles just behind... 
			QGAMES::CreditTitlesGameState::Properties (),
		new QGAMES::RollingFormsGameState  (__GAMETEST_PRESENTATIONGAMESTATE__  + 3, // A rolling background behind the previous... 
			QGAMES::RollingFormsGameState::Properties (),
		new QGAMES::PlayingASoundGameState (__GAMETEST_PRESENTATIONGAMESTATE__  + 4))))), // ...and playing a sound in the background...
	_logoForm (NULL),
	_creditTitles (NULL),
	_rollingBackground (NULL),
	_playingSound (NULL)
{
	_logoForm = (QGAMES::ShowingFixFormGameState*) nestedState ();
	_creditTitles = (TestAdArcade::PresentationTitlesGameState*) _logoForm -> nestedState ();
	_rollingBackground = (QGAMES::RollingFormsGameState *) _creditTitles -> nestedState () -> nestedState ();
	_playingSound =  (QGAMES::PlayingASoundGameState*) _rollingBackground -> nestedState ();
		
	// Properties for the state showing the logo of comuunity...
	_logoForm -> setProperties (QGAMES::ShowingFixFormGameState::Properties 
		(__QGAMES_COMMTYNETSSMALLLOGO__,
		 255, /** Fade */ 
		 QGAMES::Position (__BD (__GAMETEST_SCREENWIDTH__ - 110) / __BD __GAMETEST_SCREENWIDTH__, 
						   __BD (__GAMETEST_SCREENHEIGHT__ - 44) / __BD __GAMETEST_SCREENHEIGHT__, 
						   __BD 0), /** Releative position in the screen. */
		 QGAMES::ShowingFixFormGameState::Properties::PosReference::_TOPLEFT /** Reference position of the form */
		));

	// Properties for the state showing the titles...
	std::vector <std::string> txts;
	txts.push_back (std::string (" "));
	txts.push_back (std::string ("Community Networks Presents"));
	txts.push_back (std::string ("An Ignacio Cea's production"));
	txts.push_back (std::string ("Battleship Commander"));
	txts.push_back (std::string ("Developed also by Ignacio Cea"));
	txts.push_back (std::string ("from his hidden laboratories"));
	txts.push_back (std::string ("and with the collaboration of many Internet's resources"));
	txts.push_back (std::string ("Press any key for menu"));
	_creditTitles -> setProperties (QGAMES::CreditTitlesGameState::Properties
		(txts,
		 29, /** Max number letter per line */
		 QGAMES::Position (__BD 0.5, __BD 0.5, __BD 0), /** Reference position in the screen */ 
		 QGAMES::CreditTitlesGameState::Properties::PosReference::_CENTER, /** Reference position of the text */
		 __BD 2 /** Seconds the text in the screen */
		));

	// For the rolling forms behind...
	std::vector <int> bks;
	bks.push_back (__GAMETEST_PRESENTATIONGSTATEBK1__);
	bks.push_back (__GAMETEST_PRESENTATIONGSTATEBK2__);
	bks.push_back (__GAMETEST_PRESENTATIONGSTATEBK3__);
	bks.push_back (__GAMETEST_PRESENTATIONGSTATEBK4__);
	_rollingBackground -> setProperties (QGAMES::RollingFormsGameState::Properties
		(bks,
		 1, /** Loops before moving */
		 __BD 3, /** Seconds before disappearing */ 
		 QGAMES::Vector (__BD 1, __BD 0, __BD 0), /** Movement */
		 2, /** Instances per background */
		 false /** The bks are not sequential */
		));

	// ...and for the sound...
	_playingSound -> setProperties (QGAMES::PlayingASoundGameState::Properties 
		(__GAMETEST_PRESENTATIONGSTATEBKSOUND__, 
		 __GAMETEST_GAMESTATEBKCHANNEL__
		));
}

// ---
TestAdArcade::PresentationGameState::~PresentationGameState ()
{
	delete (_playingSound);
	_playingSound = NULL;
	delete (_rollingBackground);
	_rollingBackground = NULL;
	delete (_creditTitles);
	_creditTitles = NULL;
	delete (_logoForm);
	_logoForm = NULL;
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestAdArcade::BattleshipMovingGameState::~BattleshipMovingGameState ()
{
	((TestAdArcade::SpaceBattleShip*) game () -> entity (__GAMETEST_BATTLESHIPID__)) -> 
		addControlStepsMonitor (NULL); // Just in case...
}

// ---
void TestAdArcade::BattleshipMovingGameState::onEnter ()
{
	QGAMES::ShowingEntityGameState::onEnter ();

	TestAdArcade::SpaceBattleShip* bS = 
		(TestAdArcade::SpaceBattleShip*) game () -> entity (__GAMETEST_BATTLESHIPID__);
	bS -> setPosition (QGAMES::Position (
		__BD (game () -> mainScreen () -> visualWidth () * 0.75),
		__BD (game () -> mainScreen () -> visualHeight () * 0.5), 
		__BD 0));

	_currentMonitor = NULL;
	_loadNextMonitor = true;
}

// ---
void TestAdArcade::BattleshipMovingGameState::updatePositions ()
{
	QGAMES::ShowingEntityGameState::updatePositions ();

	if (_loadNextMonitor)
	{
		_loadNextMonitor = false; // Done

		if (_currentMonitor)
			unObserve (_currentMonitor);
		TestAdArcade::SpaceBattleShip* bS = 
			(TestAdArcade::SpaceBattleShip*) game () -> entity (__GAMETEST_BATTLESHIPID__);
		_currentMonitor = game () -> characterMonitorBuilder () -> monitorFor 
			(__GAMETEST_PRESENTATIONMONITORBASEID__ + (rand () % __GAMETEST_PRESENTATIONMONITORNUMBERMOVS__), bS);
		bS -> addControlStepsMonitor (_currentMonitor); // Removes the previous control...
		observe (_currentMonitor);
	}
}

// ---
void TestAdArcade::BattleshipMovingGameState::onExit ()
{
	QGAMES::ShowingEntityGameState::onExit ();

	if (_currentMonitor)
		unObserve (_currentMonitor);
	TestAdArcade::SpaceBattleShip* bS = 
		(TestAdArcade::SpaceBattleShip*) game () -> entity (__GAMETEST_BATTLESHIPID__);
	bS -> addControlStepsMonitor (NULL); // Removes the control...
	_currentMonitor = NULL;
}

// ---
void TestAdArcade::BattleshipMovingGameState::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __QGAMES_CHRSTEPSMONITORLASTSTEP__)
		_loadNextMonitor = true;
	else
		QGAMES::GameState::processEvent (evnt);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestAdArcade::SelectionGameState::SelectionGameState ()
	: QGAMES::StandardInitialSelectionOptionsGameState (__GAMETEST_SELECTIONGAMESTATE__,
		new QGAMES::ShowingFixFormGameState (__GAMETEST_SELECTIONGAMESTATE__ + 1, 
			QGAMES::ShowingFixFormGameState::Properties (), 
		new BattleshipMovingGameState (__GAMETEST_SELECTIONGAMESTATE__ + 2, 
		new QGAMES::RollingFormsGameState (__GAMETEST_SELECTIONGAMESTATE__ + 3,
			QGAMES::RollingFormsGameState::Properties (),
		new QGAMES::PlayingASoundGameState (__GAMETEST_SELECTIONGAMESTATE__ + 4))))),
	  _logoForm (NULL),
	  _battleshipEntity (NULL),
	  _rollingBackground (NULL),
	  _playingSound (NULL)
{
	_logoForm = (QGAMES::ShowingFixFormGameState*) nestedState ();
	_battleshipEntity = (TestAdArcade::BattleshipMovingGameState*) _logoForm -> nestedState ();
	_rollingBackground = (QGAMES::RollingFormsGameState *) _battleshipEntity -> nestedState ();
	_playingSound =  (QGAMES::PlayingASoundGameState*) _rollingBackground -> nestedState ();
		
	// Properties for the state showing the logo of comuunity...
	_logoForm -> setProperties (QGAMES::ShowingFixFormGameState::Properties
		(__QGAMES_COMMTYNETSSMALLLOGO__,
		 255, /** Fade */ 
		 QGAMES::Position (__BD (__GAMETEST_SCREENWIDTH__ - 110) / __BD __GAMETEST_SCREENWIDTH__, 
						   __BD (__GAMETEST_SCREENHEIGHT__ - 44) / __BD __GAMETEST_SCREENHEIGHT__, 
						   __BD 0), /** Reference position in the screen */
		 QGAMES::ShowingFixFormGameState::Properties::PosReference::_TOPLEFT /** Reference position of the form */
		));

	// The battleship game state is configured by itself...

	// For the rolling forms behind...
	std::vector <int> bks;
	bks.push_back (__GAMETEST_BACKGROUNDFORM__);
	_rollingBackground -> setProperties (QGAMES::RollingFormsGameState::Properties
		(bks,
		 1, /** Loops before advancing */
		 __BD 99999, /* never disappear. */
		 QGAMES::Vector (__BD 0, __BD -1, __BD 0), /** Movement */
		 2, /** Instances per background */
		 false /** Not sequential. */
		));

	// ...and for the sound...
	_playingSound -> setProperties (QGAMES::PlayingASoundGameState::Properties
		(__GAMETEST_SELECTIONGSATEBKSOUND__,
		 __GAMETEST_GAMESTATEBKCHANNEL__
		));
}

// ---
TestAdArcade::SelectionGameState::~SelectionGameState ()
{
	delete (_playingSound);
	_playingSound = NULL;
	delete (_rollingBackground);
	_rollingBackground = NULL;
	delete (_battleshipEntity);
	_battleshipEntity = NULL;
	delete (_logoForm);
	_logoForm = NULL;
}

// ---
void TestAdArcade::SelectionGameState::onEnter ()
{
	QGAMES::StandardInitialSelectionOptionsGameState::onEnter ();

	// Always the selection menu is open, the current player is back to the first...
	((TestAdArcade::Game*) game ()) -> setCurrentPlayer (1);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestAdArcade::ShowingPlayerAboutToStart::actualizeText ()
{
	QGAMES::ShowingTextGameState::actualizeText ();
	std::string str = std::string ("Player ") + 
		std::to_string (((TestAdArcade::Game*) game ()) -> currentPlayer ());
	_text -> setMaxTextLength (str.length ()); _text -> setText (str);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestAdArcade::ShowingLevelCurrentPlayer::actualizeText ()
{
	QGAMES::ShowingTextGameState::actualizeText ();
	std::string str = std::string ("Level ") + 
		std::to_string (((TestAdArcade::Game*) game ()) -> level ());
	_text -> setMaxTextLength (str.length ()); _text -> setText (str);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestAdArcade::GameAboutToStart::GameAboutToStart ()
	: QGAMES::SpendingTimeGameState (__GAMETEST_ABOUTTOSTARTGAMESTATE__, __BD 3 /** Seconds maximum. */, 
		new TestAdArcade::UntilReturnKeyOrMouseLeftButtonIsPressed (__GAMETEST_ABOUTTOSTARTGAMESTATE__ + 1,
		new TestAdArcade::ShowingPlayerAboutToStart (__GAMETEST_ABOUTTOSTARTGAMESTATE__ + 2,
		new TestAdArcade::ShowingLevelCurrentPlayer (__GAMETEST_ABOUTTOSTARTGAMESTATE__ + 3,
		new QGAMES::ShowingFixFormGameState (__GAMETEST_ABOUTTOSTARTGAMESTATE__ + 4,
			QGAMES::ShowingFixFormGameState::Properties (),
		new QGAMES::ShowingFixFormGameState (__GAMETEST_ABOUTTOSTARTGAMESTATE__ + 5)))))),
	  _untilPressed (NULL),
	  _numberPlayer (NULL),
	  _levelPlayer (NULL),
	  _background (NULL),
	  _stars (NULL),
	  _guiSystem (NULL),
	  _nameIntroduced (false)
{
	_untilPressed = (TestAdArcade::UntilReturnKeyOrMouseLeftButtonIsPressed*) nestedState ();
	_numberPlayer = (TestAdArcade::ShowingPlayerAboutToStart*) _untilPressed -> nestedState ();
	_levelPlayer = (TestAdArcade::ShowingLevelCurrentPlayer*) _numberPlayer -> nestedState ();
	_background = (QGAMES::ShowingFixFormGameState*) _levelPlayer -> nestedState ();
	_stars = (QGAMES::ShowingFixFormGameState*) _background -> nestedState ();

	_background -> setProperties (QGAMES::ShowingFixFormGameState::Properties 
		(__GAMETEST_PLANETFORM2__, 
		 255, /** Fade */ 
		 QGAMES::Position (__BD 0.5, __BD 0.5, __BD 0), /** Reference position in the screen */
		 QGAMES::ShowingFixFormGameState::Properties::PosReference::_CENTER /** Reference position of the form */
		));

	_stars -> setProperties (QGAMES::ShowingFixFormGameState::Properties
		(__GAMETEST_BACKGROUNDFORM__,
		 255, /** Fade */
		 QGAMES::Position::_cero, /** Reference position in the screen. */
		 QGAMES::ShowingFixFormGameState::Properties::PosReference::_TOPLEFT /** Reference position of the form */
		));
}

// ---
TestAdArcade::GameAboutToStart::~GameAboutToStart ()
{
	delete (_stars);
	_stars = NULL;
	delete(_background);
	_background = NULL;
	delete (_levelPlayer);
	_levelPlayer = NULL;
	delete (_numberPlayer);
	_numberPlayer = NULL;
	delete (_untilPressed);
	_untilPressed = NULL;

	deleteGUISystem ();
}

// ---
void TestAdArcade::GameAboutToStart::onEnter ()
{
	QGAMES::SpendingTimeGameState::onEnter ();

	if (!(_nameIntroduced = 
			(((TestAdArcade::Game*) game ()) -> playerName () != std::string (__NULL_STRING__))))
	{
		_guiSystem = game () -> guiSystemBuilder () -> system (__GAMETEST_PLAYERNAMEGUISYSTEM__);
		assert (dynamic_cast <QGAMES::CompositeWidget*> (_guiSystem -> mainWidget ())); // Just in case...
		observe (_guiSystem -> mainWidget ());
	}
}

// ---
void TestAdArcade::GameAboutToStart::updatePositions ()
{
	// The parent method is not invoked because its behaviour has been changed...
	// Now the state finishes not only when the count down finishes but also when the name has already been introduced...
	QGAMES::GameState::updatePositions ();

	if (counter (_COUNTERTIME) -> isEnd () && _nameIntroduced)
		notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
}

// ---
void TestAdArcade::GameAboutToStart::onExit ()
{
	QGAMES::SpendingTimeGameState::onExit ();

	// When exiting the state, the name has been introduced,
	// or it was previously...
	if (_guiSystem)
		((TestAdArcade::Game*) game ()) -> setPlayerName (((QGAMES::IntroWidget*) 
			((QGAMES::CompositeWidget*) _guiSystem -> mainWidget ()) -> widget (__GAMETEST_PLAYERNAMEOPTIONID__)) -> text ());

	deleteGUISystem ();
}

// ---
void TestAdArcade::GameAboutToStart::processEvent (const QGAMES::Event& evnt)
{
	// The events from the nested states are only propagated when the name has already been introduced...or it was!
	if (_nameIntroduced ||
		(evnt.code () == __QGAMES_GAMENESTEDSTATEFINISHED__ &&
		((QGAMES::IntroWidget*) ((QGAMES::CompositeWidget*) _guiSystem -> mainWidget ()) -> 
			widget (__GAMETEST_PLAYERNAMEOPTIONID__)) -> text () != std::string (__NULL_STRING__)))
	{
		_nameIntroduced = true;
		notify (evnt);
	}
}

// ---
void TestAdArcade::GameAboutToStart::deleteGUISystem ()
{
	if (_guiSystem)
		unObserve (_guiSystem -> mainWidget ());

	delete (_guiSystem);
	_guiSystem = NULL;
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestAdArcade::Playing::Playing ()
	: QGAMES::GameState (__GAMETEST_PLAYINGGAMESTATE__, QGAMES::Game::game (),
		new QGAMES::RollingFormsGameState (__GAMETEST_PLAYINGGAMESTATE__ + 1, 
			QGAMES::RollingFormsGameState::Properties (), 
		new QGAMES::PlayingASoundGameState (__GAMETEST_PLAYINGGAMESTATE__ + 2))),
	  _result (TestAdArcade::Playing::Result::_STILLPLAYING),
	  _rollingBackground (NULL),
	  _playingSound (NULL)
{
	_rollingBackground = (QGAMES::RollingFormsGameState *) nestedState ();
	_playingSound =  (QGAMES::PlayingASoundGameState*) _rollingBackground -> nestedState ();

	// For the rolling forms behind...
	std::vector <int> bks;
	bks.push_back (__GAMETEST_BACKGROUNDFORM__);
	_rollingBackground -> setProperties (QGAMES::RollingFormsGameState::Properties
		(bks,
		 1, /** Loops before advancing */
		 __BD 99999, /* never disappear. */
		 QGAMES::Vector (__BD 0, __BD -1, __BD 0), /** Movement */
		 2, /** Instances per background */
		 false /** Not sequential. */
		));

	// ...and for the sound...
	_playingSound -> setProperties (QGAMES::PlayingASoundGameState::Properties
		(__GAMETEST_PLAYINGGSTATEBKSOUND__,
		 __GAMETEST_GAMESTATEBKCHANNEL__
		));
}

// ---
TestAdArcade::Playing::~Playing ()
{
	delete (_playingSound);
	_playingSound = NULL;
	delete (_rollingBackground);
	_rollingBackground = NULL;
}

// ---
void TestAdArcade::Playing::onEnter ()
{
	QGAMES::GameState::onEnter ();

	game () -> addScoreObjects ();

	observe (game () -> entity (__GAMETEST_BATTLESHIPID__));

	(__AGM game ()) -> setWorld (__GAMETEST_WORLDID__);

	QGAMES::World* wrld = (__AGM game ()) -> world (__GAMETEST_WORLDID__);
	wrld -> initialize ();
	observe (wrld);

	// Stops music if the configuration says so...
	if (!((TestAdArcade::Game::Conf*) game () -> configuration ()) -> musicOn ())
		_playingSound -> stopSound ();

	_result = TestAdArcade::Playing::Result::_STILLPLAYING;
}

// ---
void TestAdArcade::Playing::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	if (_result != TestAdArcade::Playing::Result::_STILLPLAYING)
		notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
}

// ---
void TestAdArcade::Playing::onExit ()
{
	QGAMES::GameState::onExit ();

	(__AGM game ()) -> world (__GAMETEST_WORLDID__) -> finalize ();
	(__AGM game ()) -> setWorld (__MININT__);

	unObserve ((__AGM game ()) -> world (__GAMETEST_WORLDID__));

	unObserve (game () -> entity (__GAMETEST_BATTLESHIPID__));

	game () -> removeScoreObjects ();
}

// ---
void TestAdArcade::Playing::processEvent (const QGAMES::Event& evnt)
{
	// Comming from the battleship...
	if (evnt.code () == __GAMETEST_BATTLESHIPDESTROYED__)
		_result = TestAdArcade::Playing::Result::_PLAYERDIED;
	// Comming from the world...
	else if (evnt.code () == __GAMETEST_LEVELCOMPLETE__)
		_result = TestAdArcade::Playing::Result::_LEVELACHIEVED;
	else
		QGAMES::GameState::processEvent (evnt);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestAdArcade::PlayerHasDied::PlayerHasDied (TestAdArcade::Playing* ply)
	: QGAMES::SpendingTimeGameState (__GAMETEST_PLAYERHASDIEDGAMESTATE__, __BD 3 /** Seconds before the state finishes */,
		new QGAMES::RollingFormsGameState (__GAMETEST_PLAYERHASDIEDGAMESTATE__ + 1, 
			QGAMES::RollingFormsGameState::Properties (), 
		new QGAMES::PlayingASoundGameState (__GAMETEST_PLAYERHASDIEDGAMESTATE__ + 2))),
	  _playing (ply),
	  _rollingBackground (NULL), 
	  _playingSound (NULL)
{
	assert (_playing);

	_rollingBackground = (QGAMES::RollingFormsGameState*) nestedState ();
	_playingSound = (QGAMES::PlayingASoundGameState*) _rollingBackground -> nestedState ();

	// Properties for the rolling forms behind...
	std::vector <int> bks;
	bks.push_back (__GAMETEST_BACKGROUNDFORM__);
	_rollingBackground -> setProperties (QGAMES::RollingFormsGameState::Properties
		(bks,
		 1, /** Loops before advancing */
		 __BD 99999, /* never disappear. */
		 QGAMES::Vector (__BD 0, __BD -1, __BD 0), /** Movement */
		 2, /** Instances per background */
		 false /** Not sequential. */
		));

	// Properties of the sound...
	_playingSound -> setProperties (QGAMES::PlayingASoundGameState::Properties
		(__GAMETEST_PLAYERHASDEADGSTATEBKSOUND__,
		 __GAMETEST_GAMESTATEBKCHANNEL__
		));
}

// ---
TestAdArcade::PlayerHasDied::~PlayerHasDied ()
{
	delete (_playingSound);
	_playingSound = NULL;
	delete (_rollingBackground);
	_rollingBackground = NULL;
}

// ---
void TestAdArcade::PlayerHasDied::onEnter ()
{
	QGAMES::SpendingTimeGameState::onEnter ();

	// To allign the background...
	_rollingBackground -> addoptStatusFrom (_playing -> rollingBackground ());
}

// ---
void TestAdArcade::PlayerHasDied::onExit ()
{
	QGAMES::SpendingTimeGameState::onExit ();

	((TestAdArcade::Game*) game ()) -> addLives (-1); // One life less for the current player
	((TestAdArcade::Game*) game ()) -> setNextPlayer (); // Change the player...if any
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestAdArcade::LevelCompleted::LevelCompleted (TestAdArcade::Playing* ply)
	: QGAMES::SpendingTimeGameState (__GAMETEST_LEVELHASCOMPLETEDGAMESTATE__, __BD 10 /** Seconds */, 
		new QGAMES::RollingFormsGameState (__GAMETEST_LEVELHASCOMPLETEDGAMESTATE__ + 1, 
			QGAMES::RollingFormsGameState::Properties (), 
		new QGAMES::PlayingASoundGameState (__GAMETEST_LEVELHASCOMPLETEDGAMESTATE__ + 2))),
	  _playing (ply),
	  _battleship (NULL), // It will be set in onEnter method...
	  _rollingBackground (NULL),
	  _playingSound (NULL)
{
	assert (_playing);

	_rollingBackground = (QGAMES::RollingFormsGameState*) nestedState ();
	_playingSound = (QGAMES::PlayingASoundGameState*) _rollingBackground -> nestedState ();

	// Properties for the rolling forms behind...
	std::vector <int> bks;
	bks.push_back (__GAMETEST_BACKGROUNDFORM__);
	_rollingBackground -> setProperties (QGAMES::RollingFormsGameState::Properties
		(bks,
		 1, /** Loops before advancing */
		 __BD 99999, /* never disappear. */
		 QGAMES::Vector (__BD 0, __BD -1, __BD 0), /** Movement */
		 2, /** Instances per background */
		 false /** Not sequential. */
		));

	// Set the properties for the sound...
	_playingSound -> setProperties (QGAMES::PlayingASoundGameState::Properties
		(__GAMETEST_LEVELHASCOMPLETEDGSTATBKSOUND__,
		 __GAMETEST_GAMESTATEBKCHANNEL__
		));
}

// ---
TestAdArcade::LevelCompleted::~LevelCompleted ()
{
	delete (_playingSound);
	_playingSound = NULL;
	delete (_rollingBackground);
	_rollingBackground = NULL;
}

// ---
void TestAdArcade::LevelCompleted::onEnter ()
{
	QGAMES::SpendingTimeGameState::onEnter ();

	_battleship = (TestAdArcade::SpaceBattleShip*) game () -> entity (__GAMETEST_BATTLESHIPID__);
	_battleship -> toRotate ();

	// To alling the background...
	_rollingBackground -> addoptStatusFrom (_playing -> rollingBackground ());
}

// ---
void TestAdArcade::LevelCompleted::updatePositions ()
{
	QGAMES::SpendingTimeGameState::updatePositions ();

	_battleship -> updatePositions ();
}

// ---
void TestAdArcade::LevelCompleted::drawOn (QGAMES::Screen* s)
{
	QGAMES::SpendingTimeGameState::drawOn (s);

	_battleship -> drawOn (s);
}

// ---
void TestAdArcade::LevelCompleted::onExit ()
{
	QGAMES::SpendingTimeGameState::onExit ();

	((TestAdArcade::Game*) game ()) -> setNextLevel ();
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestAdArcade::GameHasEnded::GameHasEnded ()
	: QGAMES::SpendingTimeGameState (__GAMETEST_GAMEHASENDEDGAMESTATE__, __BD 10 /** seconds */,
		new TestAdArcade::ShowingGameHasEnded (__GAMETEST_GAMEHASENDEDGAMESTATE__ + 1,
		new QGAMES::ShowingFixFormGameState (__GAMETEST_GAMEHASENDEDGAMESTATE__ + 2,
			QGAMES::ShowingFixFormGameState::Properties (),
		new QGAMES::ShowingFixFormGameState (__GAMETEST_GAMEHASENDEDGAMESTATE__ + 3,
			QGAMES::ShowingFixFormGameState::Properties (),
		new QGAMES::PlayingASoundGameState (__GAMETEST_GAMEHASENDEDGAMESTATE__ + 4))))),
	  _text (NULL),
	  _background (NULL),
	  _stars (NULL),
	  _playingSound (NULL)
{
	_text = (TestAdArcade::ShowingGameHasEnded*) nestedState ();
	_background = (QGAMES::ShowingFixFormGameState*) _text -> nestedState ();
	_stars = (QGAMES::ShowingFixFormGameState*) _background -> nestedState ();
	_playingSound = (QGAMES::PlayingASoundGameState*) _stars -> nestedState ();

	// Sets the behaviour of the background...
	_background -> setProperties (QGAMES::ShowingFixFormGameState::Properties
		(__GAMETEST_PLANETFORM3__,
		 255, /** Fade */
		 QGAMES::Position (__BD 0.5, __BD 0.5, __BD 0), /** Reference position in the screen */
		 QGAMES::ShowingFixFormGameState::Properties::PosReference::_CENTER /** Reference position of the form */
		));

	// Sets the behaviour of the background...
	_stars -> setProperties (QGAMES::ShowingFixFormGameState::Properties 
		(__GAMETEST_BACKGROUNDFORM__,
		255,/** Fade */
		QGAMES::Position::_cero, /** Relative position in the screen */
		QGAMES::ShowingFixFormGameState::Properties::PosReference::_TOPLEFT /** Reference poition of the form */
	   ));
	
	// Same for the playing sound at the background...
	_playingSound -> setProperties (QGAMES::PlayingASoundGameState::Properties
		(__GAMETEST_GAMEHASENDEDGSTATEBKSOUND__,
		 __GAMETEST_GAMESTATEBKCHANNEL__
		));
}

// ---
TestAdArcade::GameHasEnded::~GameHasEnded ()
{
	delete (_playingSound);
	_playingSound = NULL;
	delete (_stars);
	_stars = NULL;
	delete (_background);
	_background = NULL;
	delete (_text);
	_text = NULL;
}

// ---
void TestAdArcade::GameHasEnded::onExit ()
{
	QGAMES::SpendingTimeGameState::onExit ();

	// Copy the results of the game into the hall of fame...
	((TestAdArcade::Game*) game ()) -> actualizeHallOfFame ();
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
std::map <std::string, QGAMES::SetOfOpenValues> TestAdArcade::ShowingEntriesHallOfFame::orderHallOfFame 
	(const std::map <std::string, QGAMES::SetOfOpenValues>& hF)
{
	// list of scores, and per score which is the original entry...
	std::vector <int> scores;
	std::multimap <int, int> scoresPlayer;
	for (std::multimap <std::string, QGAMES::SetOfOpenValues>::const_iterator i = hF.begin ();
			i != hF.end (); i++)
	{
		scores.push_back ((*i).second.openValue (__GAMETEST_SCOREPLAYERPENVALUE__).intValue ());
		scoresPlayer.insert (std::multimap <int, int>::value_type ((*i).second.openValue (__GAMETEST_SCOREPLAYERPENVALUE__).intValue (),
			std::atoi ((*i).first.c_str ())));
	}

	// Order the scores...
	std::sort (scores.begin (), scores.end ());

	// Generates the new order...
	// But take care, if the are many equal scores, then the name of player can be repeated...
	std::map <std::string, QGAMES::SetOfOpenValues> result;
	for (std::vector <int>::const_iterator i = scores.begin (); i != scores.end (); i++)
		result.insert (std::map <std::string, QGAMES::SetOfOpenValues>::value_type (std::to_string ((*i)), 
			(*hF.find (std::to_string ((*scoresPlayer.find ((*i))).second))).second));
	return (result);
}

// ---
QGAMES::ScoreObjectText* TestAdArcade::ShowingEntriesHallOfFame::createTextFor 
	(const std::string& eK, const QGAMES::SetOfOpenValues& sV)
{
	std::string scr = std::to_string (sV.openValue (__GAMETEST_SCOREPLAYERPENVALUE__).intValue ());
	if (scr.length () < 5) 
		scr = std::string ("00000").substr (0, 5 - scr.length ()) + scr;
	return (new QGAMES::Ravie24OrangeFont (scr + std::string ("  ") + 
		sV.openValue (__GAMETEST_NAMEPLAYEROPENVALUE__).strValue ()));
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestAdArcade::HallOfFameGameState::HallOfFameGameState ()
	: TestAdArcade::UntilReturnKeyOrMouseLeftButtonIsPressed (__GAMETEST_HALLOFFAMEGAMESTATE__,
		new TestAdArcade::ShowingEntriesHallOfFame (__GAMETEST_HALLOFFAMEGAMESTATE__ + 1,
		new QGAMES::ShowingFixFormGameState (__GAMETEST_HALLOFFAMEGAMESTATE__ + 1, 
			QGAMES::ShowingFixFormGameState::Properties (), 
		new QGAMES::RollingFormsGameState (__GAMETEST_HALLOFFAMEGAMESTATE__ + 2,
			QGAMES::RollingFormsGameState::Properties (),
		new QGAMES::PlayingASoundGameState (__GAMETEST_HALLOFFAMEGAMESTATE__ + 3))))),
	  _hallOfFame (NULL),
	  _logoForm (NULL),
	  _rollingBackground (NULL),
	  _playingSound (NULL)
{
	_hallOfFame = (TestAdArcade::UntilReturnKeyOrMouseLeftButtonIsPressed*) nestedState ();
	_logoForm = (QGAMES::ShowingFixFormGameState*) _hallOfFame -> nestedState ();
	_rollingBackground = (QGAMES::RollingFormsGameState *) _logoForm -> nestedState ();
	_playingSound =  (QGAMES::PlayingASoundGameState*) _rollingBackground -> nestedState ();
		
	// Properties for the state showing the logo of comuunity...
	_logoForm -> setProperties (QGAMES::ShowingFixFormGameState::Properties (
		__QGAMES_COMMTYNETSSMALLLOGO__, 255 /** Fade */, 
			QGAMES::Position (__BD (__GAMETEST_SCREENWIDTH__ - 110) / __BD __GAMETEST_SCREENWIDTH__, 
							  __BD (__GAMETEST_SCREENHEIGHT__ - 44) / __BD __GAMETEST_SCREENHEIGHT__, 
							  __BD 0), QGAMES::ShowingFixFormGameState::Properties::PosReference::_TOPLEFT));

	// The battleship game state is configuration by itself...

	// For the rolling forms behind...
	std::vector <int> bks;
	bks.push_back (__GAMETEST_BACKGROUNDFORM__);
	_rollingBackground -> setProperties (QGAMES::RollingFormsGameState::Properties (
		bks, 1, __BD 99999 /* never disappear. */, QGAMES::Vector (__BD 0, __BD -1, __BD 0), 
			2 /** Instance */, false /** Not sequential */));

	// ...and for the sound...
	_playingSound -> setProperties (QGAMES::PlayingASoundGameState::Properties (
		__GAMETEST_HALLOFFAMESTATEBKSOUND__, __GAMETEST_GAMESTATEBKCHANNEL__));
}

// ---
TestAdArcade::HallOfFameGameState::~HallOfFameGameState ()
{
	delete (_playingSound);
	_playingSound = NULL;
	delete (_rollingBackground);
	_rollingBackground = NULL;
	delete (_logoForm);
	_logoForm = NULL;
	delete (_hallOfFame);
	_hallOfFame = NULL;
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestAdArcade::Game::Conf::actualizeHallOfFame ()
{
	int iS = (int) _hallOfFame.size ();
	for (int i = 0; i < _numberPlayers; i++)
	{
		QGAMES::OpenValues vals; 
		vals.insert (QGAMES::OpenValues::value_type (__GAMETEST_SCOREPLAYERPENVALUE__, QGAMES::OpenValue (_score [i])));
		vals.insert (QGAMES::OpenValues::value_type (__GAMETEST_NAMEPLAYEROPENVALUE__, QGAMES::OpenValue (_playerName [i])));
		_hallOfFame.insert (std::map <std::string, QGAMES::SetOfOpenValues>::value_type 
			(std::to_string (i + iS), QGAMES::SetOfOpenValues (vals)));
	}
}

// ---
void TestAdArcade::Game::Conf::adjustToPlayers (int nP)
{
	QGAMES::AdvancedArcadeGame::Conf::adjustToPlayers (nP);

	_score = std::vector <int> (nP, 0);
	_weaponsLevel = std::vector <int> (nP, 0);
	_level = std::vector <int> (nP, 0); // The first level is 1
	_playerName = std::vector <std::string> (nP, std::string (__NULL_STRING__));

	// Adjust the previous loaded elements to this game...
	_currentWorld = std::vector <int> (nP, __GAMETEST_WORLDID__);
	_currentScene = std::vector <int> (nP, __GAMETEST_SCENEID__);
	_numberLives = std::vector <int> (nP, __GAMETEST_MAXNUMBERLIVES__);
}

// ---
void TestAdArcade::Game::Conf::cfgToStream (std::ostringstream& oS)
{
	QGAMES::AdvancedArcadeGame::Conf::cfgToStream (oS);

	for (int i = 0; i < _numberPlayers; i++)
		oS << _score [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _weaponsLevel [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _level [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _playerName [i] << std::endl;
}

// ---
void TestAdArcade::Game::Conf::cfgFromStream (std::istringstream& iS)
{
	QGAMES::AdvancedArcadeGame::Conf::cfgFromStream (iS);

	_score.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _score [i];
	_weaponsLevel.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _weaponsLevel [i];
	_level.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _level [i];
	_playerName.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _playerName [i];
}

// ---
int TestAdArcade::Game::shootingsRemaining ()
{
	return (((TestAdArcade::SpaceBattleShip*) entity (__GAMETEST_BATTLESHIPID__)) -> shootingsReaming ());
}

// ---
void TestAdArcade::Game::addScoreObjects ()
{
	addScoreObject (new TestAdArcade::NumberOfLivesScoreObject ());
	addScoreObject (new TestAdArcade::NumberOfPointsScoreObject ());
	addScoreObject (new TestAdArcade::NumberOfShootingsScoreObject ());
}

// ---
void TestAdArcade::Game::removeScoreObjects ()
{
	const QGAMES::Entities scObjs = scoreObjects ();
	for (QGAMES::Entities::const_iterator i = scObjs.begin (); i != scObjs.end (); i++)
		delete ((*i).second);

	QGAMES::AdvancedArcadeGame::removeScoreObjects ();
}

// ---
void TestAdArcade::Game::initialize ()
{
	if (isInitialized ())
		return; // Only once...

	QGAMES::AdvancedArcadeGame::initialize ();
	if (!isInitialized ())
		return;

	mainScreen () -> windowAtCenter ();
	mainScreen () -> setIcon (form (__GAMETEST_BATTLESHIPGAMELOGO__));

	((QGAMES::LoadingGameState*) state (__QGAMES_GAMESTATELOADINGNAME__)) ->
		setProperties (QGAMES::ShowingFormBeingFulfilledGameState::Properties
			(__GAMETEST_PLANETFORM1__,
			 255, /** Fade */ 
			 QGAMES::Position (__BD 0.5, __BD 0.5, __BD 0), /** Releative position in the screen */
			 QGAMES::LoadingGameState::Properties::PosReference::_CENTER, /** Reference point of the form */
			 false, /** Direct fullfilment */
			 true /** Horizontal fullfilment */
			));

	addState (new TestAdArcade::IntroLogoGameState, std::string (__GAMETEST_INTROGAMESTATENAME__));
	addState (new TestAdArcade::PresentationGameState, std::string (__GAMETEST_PRESENTATIONGAMESTATENAME__));
	addState (new TestAdArcade::SelectionGameState, std::string (__GAMETEST_SELECTIONGAMESTATENAME__));
	addState (new TestAdArcade::HallOfFameGameState, std::string (__GAMETEST_HALLOFFAMEGAMESTATENAME__));
	addState (new TestAdArcade::GameAboutToStart, std::string (__GAMETEST_ABOUTTOSTARTGAMESTATENAME__));
	addState (new TestAdArcade::Playing, std::string (__GAMETEST_PLAYINGGAMESTATENAME__));
	addState (new TestAdArcade::LevelCompleted ((TestAdArcade::Playing*) state (std::string (__GAMETEST_PLAYINGGAMESTATENAME__))),
		std::string (__GAMETEST_LEVELHASCOMPLETEDGAMESTATENAME__));
	addState (new TestAdArcade::PlayerHasDied ((TestAdArcade::Playing*) state (std::string (__GAMETEST_PLAYINGGAMESTATENAME__))), 
		std::string (__GAMETEST_PLAYERHASDIEDGAMESTATENAME__));
	addState (new TestAdArcade::GameHasEnded, std::string (__GAMETEST_GAMEHASENDEDGAMESTATENAME__));
	addState (new TestAdArcade::ControllingGameStates (state (std::string (__QGAMES_GAMESTATESHOWENTITYNAME__))), 
		std::string (__GAMETEST_CONTROLGAMESTATENAME__));

	addWorld (__GAMETEST_WORLDID__);

	addArtist (__GAMETEST_BATTLESHIPID__);
	for (int i = 0; i < __GAMETEST_BATTLESHITPMAXSHOOTINGS__; i++)
		addArtist (__GAMETEST_BATTLESHIPSHOOTINGBASEID__ + i);
	for (int i = 0; i < __GAMETEST_MAXASTEROIDS__; i++)
		addArtist (__GAMETEST_ASTEROIDBASEID__ + i);
	addArtist (__GAMETEST_BATTLESHIPSHOOTINGTOCATCHID__);

	setState (__GAMETEST_CONTROLGAMESTATENAME__);
}

// ---
void TestAdArcade::Game::additionalTasks ()
{
	if (activeState () -> nestedState () -> type () == __GAMETEST_PLAYINGGAMESTATE__)
		QGAMES::AdvancedArcadeGame::additionalTasks ();
}
// --------------------------------------------------------------------------------
