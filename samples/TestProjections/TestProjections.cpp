#include "TestProjections.hpp"
#include <SDL/sdlinclude.hpp>

const QGAMES::Position TestProjections::Game::_iPoint (__BD 50, __BD 50, __BD 0);
const int TestProjections::Game::_separation = 20;
const int TestProjections::Game::_length = 700;
const int TestProjections::Game::_it = 
	TestProjections::Game::_length / TestProjections::Game::_separation;

#define __GAMEICON__			70000

// --------------------------------------------------------------------------------
// ---
TestProjections::FramerateScoreObject::FramerateScoreObject ()
	: QGAMES::ScoreObjectNumber (100, 60, QGAMES::Forms (), 16)
{
	_forms.insert (QGAMES::Forms::value_type (10000, 
		QGAMES::Game::game () -> form (10000)));
	setCurrentForm (10000);
	setPosition (QGAMES::Position (__BD 10, __BD 10, __BD 0));
}

// ---
void TestProjections::FramerateScoreObject::updatePositions ()
{
	QGAMES::ScoreObjectNumber::updatePositions ();

	setNumber (game () -> currentLoopsPerSecond ());
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestProjections::AnglesScoreObject::AnglesScoreObject ()
	: QGAMES::ScoreObjectText (101, std::string (__NULL_STRING__), QGAMES::Forms (), 0)
{
	_forms.insert (QGAMES::Forms::value_type (10000, 
		QGAMES::Game::game () -> form (10000)));
	setCurrentForm (10000);
	setPosition (QGAMES::Position (__BD 10, __BD 40, __BD 0));
}

// ---
void TestProjections::AnglesScoreObject::updatePositions ()
{
	QGAMES::ScoreObjectText::updatePositions ();

	QGAMES::Camera::Status st = game () -> mainScreen () -> currentCamera () -> currentStatus ();
	std::string txt = 
		std::string ("P:")  + std::to_string ((int) (st._pitchAngle * __BD 100)) +
		std::string (",Y:") + std::to_string ((int) (st._yawAngle * __BD 100)) +
		std::string (",R:") + std::to_string ((int) (st._rollAngle * __BD 100)) + 
		std::string (",Z:") + std::to_string ((int) (dynamic_cast <TestProjections::Game*> (game ()) -> zCoord ()));

	setMaxTextLength (txt.length ());
	setText (txt);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestProjections::InputHandlerBehaviour::onKeyboardEvent (QGAMES::KeyBoardEventData* dt)
{
	if (dt -> _internalCode == QGAMES::KeyCode::QGAMES_ESCAPE && !dt -> _on)
		game () -> exitGame (); 
}
// --------------------------------------------------------------------------------

// ---
TestProjections::Game::Game ()
	: QGAMES::Game (new SDLGame ()),
	  _zCoord (__BD 0), _cAngle (__BD 0), _inc (__BD 0), _counter (0)
{ 
#ifdef NDEBUG
	// If the data parameter exists, then changes the default temporal file...
	// It makes sen se only when running release version of the app!
	if (dynamic_cast <QGAMES::InZipResourceReader*> (QGAMES::ResourceReader::resourceReader ()) != NULL)
		((QGAMES::InZipResourceReader*) QGAMES::ResourceReader::resourceReader ()) -> 
			setTempLocation (parameter (std::string (__GAME_DATADIRPROPERTYNAME__)));
#endif
}

// ---
void TestProjections::Game::initialize ()
{
	if (isInitialized ())
		return; // Only once...

	// Normal initialization...
	QGAMES::Game::initialize ();
	if (!isInitialized ())
		return;

	// Moves the main screen to the center..
	mainScreen () -> windowAtCenter ();
	// ...and set the id icon...
	mainScreen () -> setIcon (form (__GAMEICON__));

	// Adds the score objects...
	addScoreObjects ();

	// Sets the camera to project in orthogonal
	mainScreen () -> currentCamera () -> setProjectType (QGAMES::Camera::ProjectionType::_CONIC);
	mainScreen () -> setPosition (-QGAMES::Position::_cero);

	_zCoord = __BD 0;
	_cAngle = __BD 0;
	_inc = __BD 1;
}

// ---
void TestProjections::Game::updatePositions ()
{
	QGAMES::Game::updatePositions ();

	if (_counter++ >= 1)
	{
		_counter = 0;

		_cAngle += _inc;
		if (_cAngle >= __BD 60 || _cAngle <= __BD -60)
			_inc *= __BD -1;

		// Rotating...
		// Pitch (OX)
//		mainScreen () -> currentCamera () -> pitch (_inc * __PI / __BD 180);
		// Raw (OY)
		mainScreen () -> currentCamera () -> yaw (_inc * __PI / __BD 180);
		// Rotate (OZ)
//		mainScreen () -> currentCamera () -> rotate (_inc * __PI / __BD 180);

		_zCoord += __BD 6 * _inc;
	}
}

// ---
void TestProjections::Game::drawOn (QGAMES::Screen* s)
{
	const QGAMES::Color C1 (0, 255, 255, 100);
	const QGAMES::Color C2 (255, 255, 255, 255);
	const QGAMES::Color C3 (255, 255, 0, 100);

	// The net...
	for (int i = 0; i <= _it; i++)
		s -> drawLine (TestProjections::Game::_iPoint + QGAMES::Position (__BD (i * _separation), 0, _zCoord), 
					   TestProjections::Game::_iPoint + QGAMES::Position (__BD (i * _separation), __BD _length, _zCoord), 
					   C1);
	for (int i = 0; i <= _it; i++)
		s -> drawLine (TestProjections::Game::_iPoint + QGAMES::Position (__BD 0, __BD (i * _separation), _zCoord), 
					   TestProjections::Game::_iPoint + QGAMES::Position (__BD _length, __BD (i * _separation), _zCoord), 
					   C1);
	
	// The vector...
	s -> drawLine (TestProjections::Game::_iPoint + QGAMES::Position (__BD (_length >> 1), __BD (_length >> 1), _zCoord), 
				   TestProjections::Game::_iPoint + QGAMES::Position (__BD (_length >> 1), __BD (_length >> 1), __BD _zCoord - __BD 100), 
				   C2);

	// A room
	s -> drawRectangle (TestProjections::Game::_iPoint + 
		QGAMES::Rectangle (QGAMES::Position (__BD 100, __BD 100, __BD 100),
						   QGAMES::Position (__BD 100, __BD 400, __BD 1000), QGAMES::Vector::_xNormal), C1, true);
	s -> drawRectangle (TestProjections::Game::_iPoint + 
		QGAMES::Rectangle (QGAMES::Position (__BD 100, __BD 100, __BD 100),
						   QGAMES::Position (__BD 100, __BD 400, __BD 1000), QGAMES::Vector::_xNormal), C2, false);
	s -> drawRectangle (TestProjections::Game::_iPoint + 
		QGAMES::Rectangle (QGAMES::Position (__BD 100, __BD 100, __BD 1000),
						   QGAMES::Position (__BD 500, __BD 400, __BD 1000), QGAMES::Vector::_zNormal), C1, true);
	s -> drawRectangle (TestProjections::Game::_iPoint + 
		QGAMES::Rectangle (QGAMES::Position (__BD 100, __BD 100, __BD 1000),
						   QGAMES::Position (__BD 500, __BD 400, __BD 1000), QGAMES::Vector::_zNormal), C2, false);
	s -> drawRectangle (TestProjections::Game::_iPoint + 
		QGAMES::Rectangle (QGAMES::Position (__BD 500, __BD 100, __BD 100),
						   QGAMES::Position (__BD 500, __BD 400, __BD 1000), QGAMES::Vector::_xNormal), C1, true);
	s -> drawRectangle (TestProjections::Game::_iPoint + 
		QGAMES::Rectangle (QGAMES::Position (__BD 500, __BD 100, __BD 100),
						   QGAMES::Position (__BD 500, __BD 400, __BD 1000), QGAMES::Vector::_xNormal), C2, false);
	s -> drawRectangle (TestProjections::Game::_iPoint + 
		QGAMES::Rectangle (QGAMES::Position (__BD 100, __BD 100, __BD 100),
						   QGAMES::Position (__BD 500, __BD 400, __BD 100), QGAMES::Vector::_zNormal), C3, true);
	s -> drawRectangle (TestProjections::Game::_iPoint + 
		QGAMES::Rectangle (QGAMES::Position (__BD 100, __BD 100, __BD 100),
						   QGAMES::Position (__BD 500, __BD 400, __BD 100), QGAMES::Vector::_zNormal), C2, false);

	// The floor
	int nB = 20;
	int bZ = (1000 - 100) / nB;
	int bX = (500 - 100) / nB;
	for (int i = 0; i <= bZ; i++)
		s -> drawLine (
			TestProjections::Game::_iPoint + QGAMES::Position (__BD 100, __BD 400, __BD (100 + (nB * i))),
			TestProjections::Game::_iPoint + QGAMES::Position (__BD 500, __BD 400, __BD (100 + (nB * i))), C3);
	for (int i = 0; i <= bX; i++)
		s -> drawLine (
			TestProjections::Game::_iPoint + QGAMES::Position (__BD (100 + (i * nB)), __BD 400, __BD 100),
			TestProjections::Game::_iPoint + QGAMES::Position (__BD (100 + (i * nB)), __BD 400, __BD 1000), C3);

/*
	// Simple lines beyond the limits...
	s -> drawLine (QGAMES::Position (__BD 200, __BD 100, __BD 100),
				   QGAMES::Position (__BD 200, __BD 100, __BD -400), C2); // Out
	s -> drawLine (QGAMES::Position (__BD 600, __BD 100, __BD 100), 
				   QGAMES::Position (__BD 600, __BD 100, __BD -400), C2); // Out
	s -> drawLine (QGAMES::Position (__BD 200, __BD 500, __BD 100), 
				   QGAMES::Position (__BD 200, __BD 500, __BD -400), C2); // Out
	s -> drawLine (QGAMES::Position (__BD 600, __BD 500, __BD 100), 
				   QGAMES::Position (__BD 600, __BD 500, __BD -400), C2); // Out
*/
/*
	// Set of lines beyond the limits...
	// Up / Left
	for (int i = 0; i < 200; i++)
		s -> drawLine (QGAMES::Position (__BD 200, __BD 100, __BD 100), 
					   QGAMES::Position (__BD 200, __BD (100 - 3 * i), __BD -(200 + 5 * i)), C2); 

	// Up / Right
	for (int i = 0; i < 200; i++)
		s -> drawLine (QGAMES::Position (__BD 600, __BD 100, __BD 100), 
					   QGAMES::Position (__BD 600, __BD (100 - 3 * i), __BD -(200 + 5 * i)), C2); 

	// Down / Left
	for (int i = 0; i < 200; i++)
		s -> drawLine (QGAMES::Position (__BD 200, __BD 500, __BD 100), 
					   QGAMES::Position (__BD 200, __BD (500 + 3 * i), __BD -(200 + 5 * i)), C2);

	// Down / Right
	for (int i = 0; i < 200; i++)
		s -> drawLine (QGAMES::Position (__BD 600, __BD 500, __BD 100), 
					   QGAMES::Position (__BD 600, __BD (500 + 3 * i), __BD -(200 + 5 * i)), C2);
*/
/*
	// A triangle beyond the limits...
	s -> drawTriangle (
			QGAMES::Position (__BD 300, __BD 300, __BD 1000),
			QGAMES::Position (__BD 100, __BD 100, __BD -200),
			QGAMES::Position (__BD 700, __BD 500, __BD -550), C1, true);
*/

	QGAMES::Game::drawOn (s);
}

// ---
void TestProjections::Game::finalize ()
{
	removeScoreObjects ();

	QGAMES::Game::finalize ();
}

// ---
void TestProjections::Game::addScoreObjects ()
{
	addScoreObject (new TestProjections::FramerateScoreObject ());
	addScoreObject (new TestProjections::AnglesScoreObject ());
}
