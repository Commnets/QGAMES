/** \addtogroup SabreWulf */
/*@{*/

#ifndef __GAMESTATES_HPP__
#define __GAMESTATES_HPP__

#include "Defs.hpp"
#include <Arcade/arcadeinclude.hpp>

/** A class to define text drawn
	during the presentation. */
class SabreWulfPresentationText : public QGAMES::ScoreObjectText
{
	public:
	SabreWulfPresentationText (const std::string& t = std::string (__NULL_STRING__));

	virtual Entity* clone () const
							{ return (new SabreWulfPresentationText (_text)); }

	protected:
	/** @see parent. */
	virtual int formToDrawLetters ()
							{ return (__SABREWULFLETTERS__); }
};

/** A class to define little text drawn
	during the presentation. */
class SabreWulfLittlefPresentationText : public QGAMES::ScoreObjectText
{
	public:
	SabreWulfLittlefPresentationText (const std::string& t = std::string (__NULL_STRING__));

	virtual Entity* clone () const
							{ return (new SabreWulfLittlefPresentationText (_text)); }

	protected:
	/** @see parent. */
	virtual int formToDrawLetters ()
							{ return (__SABREWULFLITTLELETTERS__); }
};

/** The presentation of the game.
	A big label with the name of the game appears in the middle of the screen.
	The communitty logo appears as well.
	After a couple of seconds, the logo start to fade out,
	and the label with the name of the game moves down a couple of pixels. */
class SabreWulfIntroState : public QGAMES::GameState 
{
	public:
	SabreWulfIntroState (QGAMES::Game* g)
		: QGAMES::GameState (__SABREWULFINTROSTATE__, g),
		  _counter (0),
		  _background (NULL),
		  _sabrewulfLogo (NULL),
		  _logo (NULL),
		  _text (NULL),
		  _fadeOut (false),
		  _counterAlpha (255),
		  _moveLogo (false),
		  _posLogo (0)
							{ }

	/** @see parent. */
	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	/** To count how long the logo is in the screen... */
	int _counter; 
	/** The background to be shown. */
	QGAMES::Form* _background;
	/** The logo of sabrewulf. */
	QGAMES::Form* _sabrewulfLogo;
	/** The commty logo to be shown... */
	QGAMES::Form* _logo; 
	/** The text refering to the author. */
	SabreWulfPresentationText* _text;
	/** The logo start to fade out after a couple of seconds.. */
	bool _fadeOut; 
	/** The level of fade out... */
	int _counterAlpha;
	/** Sabrewulf logo start to move. */
	bool _moveLogo;
	/** The position of the logo. */
	int _posLogo;
};

/** The state to select options of the game. 
	The options appears in the middle, rounded of the forest. */
class SabreWulfSelectionState : public QGAMES::GameState 
{
	public:
	SabreWulfSelectionState (QGAMES::Game* g);

	void nextOption ();
	void previousOption ();
	bool isMovingToAnOption () const
							{ return (_movingToOption != 0); }
	void optionSelected ();
	void optionAt (const QGAMES::Position& pos);

	/** @see parent. */
	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	/** The background to be shown. */
	QGAMES::Form* _background;
	/** The commty logo form, shown in this state */
	QGAMES::Form* _logo; 
	/** The options to be selected. */
	std::vector <std::string> _options;
	/** The levels potentially selected. */
	std::vector <std::string> _optionLevels;
	/** ...and the entites representing them. */
	std::vector <SabreWulfPresentationText*> _optionGraphics;
	/** ...and the graphics in the left. */
	std::vector <QGAMES::Form*> _optionForms;
	/** The current option selected. */
	int _currentOption;
	/** The current option related with the level. */
	int _currentOptionLevel;
	/** Counter for the things moving... */
	int _counterMovement;
	/** The selection moves little by little.
		This parameters marks how much is it moving. */
	int _positionOption;
	/** To indicate the type of movement. */
	int _movingToOption;
	/** The figure that is being shown. */
	int _figure;
	/** The type of control selected: 0 keyboard, 1 joystick. */
	int _typeOfControl;	
	/** Blinking attribute. */
	int _blinkAttr;
	/** Direction of the blinking. */
	int _blinkDirection;
	/** Counting time with no activite.
		This is done to move the state toi a demo situation. */
	int _counterNoActivity;
	/** When the final option is selected, it is time to start to exit. */
	bool _startToExit;
	/** A counter is needed to know when the existing time finishes. */
	int _counterExiting;
	/** When the input handler detects an special key has been pressed
		and the game stands in this state this variable is set to true. */
	bool _wantToExit;
};

/** If no option is selected for a coupe of seconds,
	a demos starts, playing music and showing different random 
	places of the maze. */
class SabreWulfDemoState : public QGAMES::GameState 
{
	public:
	SabreWulfDemoState (QGAMES::Game* g)
		: QGAMES::GameState (__SABREWULFDEMOSTATE__, g),
		  _wantToExit (false),
		  _soundPlaying (0),
		  _counter (0)
							{ }

	void setWantToExit (bool e)
							{ _wantToExit = e; }

	/** @see parent. */
	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void onExit ();

	private:
	/** When the input handler detects an special key has been pressed
		and the game stands in this state this variable is set to true. */
	bool _wantToExit;
	/** The sound being playing is random. It is needed to keep it in order to stop
		it when the state ends. */
	int _soundPlaying;
	/** Counter to change the room visited. */
	int _counter;
};

/** The game starts.
	Sabreman appears after a couple of seconds. */
class SabreWulfStartState : public QGAMES::GameState 
{
	public:
	SabreWulfStartState (QGAMES::Game* g)
		: QGAMES::GameState (__SABREWULFSTARTSTATE__, g),
		  _counter (0)
							{ }

	/** @see parent. */
	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void onExit ();

	private:
	/** Counter to appear sabreman. */
	int _counter;
};

/** The status while the game goes. */
class SabreWulfPlayingState : public QGAMES::GameState 
{
	public:
	SabreWulfPlayingState (QGAMES::Game* g)
		: QGAMES::GameState (__SABREWULFPLAYINGSTATE__, g)
							{ }

	/** @see parent. */
	virtual void onEnter ();
	virtual void drawOn (QGAMES::Screen* s);
};

/** Sabreman has found a piece of the amulet. */
class SabreWulfAmuletState : public QGAMES::GameState 
{
	public:
	SabreWulfAmuletState (QGAMES::Game* g)
		: QGAMES::GameState (__SABREWULFAMULETSTATE__, g),
		  _textGot (NULL),
		  _textPending (NULL),
		  _counter (0)
							{ }

	/** @see parent. */
	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	/** The text refering to the number of amultes you have got. */
	SabreWulfLittlefPresentationText* _textGot;
	/** The text refering to the number of amulet pieces still pending. */
	SabreWulfLittlefPresentationText* _textPending;
	/** Counter to maintain the pieces on the screen. */
	int _counter;
};

/** The state of the game when sabreman is hit and die flying. */
class SabreWulfFlyingDie : public QGAMES::GameState
{
	public:
	SabreWulfFlyingDie (QGAMES::Game* g)
		: QGAMES::GameState (__SABREWULFFLYDIESTATE__, g),
		  _diePosition (QGAMES::Position::_cero),
		  _dieOrientation (QGAMES::Vector::_cero),
		  _dieLimitPosition (QGAMES::Position::_cero),
		  _counter (0),
		  _satDown (false)
							{ }

	/** @see parent. */
	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	/** The position where the player dies, and the start
		point to fly and the remain sat. */
	QGAMES::Position _diePosition;
	/** To know the orientation of the sabreman and in consequence the
		direction to move him whilst he is flying. */
	QGAMES::Vector _dieOrientation;
	/** The limit position to fly to when dieing. */
	QGAMES::Position _dieLimitPosition;
	/** A counter need to know when the state finishes. */
	int _counter;
	/** To indicate whether sabreman is already sat down or not. */
	bool _satDown;
};

/** The state of the game when sabreman dies. */
class SabreWulfDieState : public QGAMES::GameState 
{
	public:
	SabreWulfDieState (QGAMES::Game* g)
		: QGAMES::GameState (__SABREWULFDIESTATE__, g),
		  _counter (0)
							{ }

	/** @see parent. */
	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	/** A counter need to know when the state finishes. */
	int _counter;
};

/** The state of the game when sabreman dies for ever.... */
class SabreWulfDieForEverState : public QGAMES::GameState 
{
	public:
	SabreWulfDieForEverState (QGAMES::Game* g)
		: QGAMES::GameState (__SABREWULFDIEFOREVERSTATE__, g),
		  _counter (0),
		  _diePosition (QGAMES::Position::_cero),
		  _dieOrientation (QGAMES::Vector::_cero)
							{ }

	/** @see parent. */
	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);

	private:
	/** A counter need to know when the state finishes. */
	int _counter;
	/** The position where the player dies. */
	QGAMES::Position _diePosition;
	/** To know the orientation of the sabreman and in consequence the
		direction to move him whilst he is flying. */
	QGAMES::Vector _dieOrientation;
};

/** The state when game finishes. */
class SabreWulfEndState : public QGAMES::GameState 
{
	public:
	SabreWulfEndState (QGAMES::Game* g)
		: QGAMES::GameState (__SABREWULFENDSTATE__, g),
		  _textEndGame (NULL),
		  _textPercentageGame (NULL),
		  _counter (0),
		  _wantToExit (false)
							{ }

	void setWantToExit (bool e)
							{ _wantToExit = e; }

	/** @see parent. */
	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	/** The text refering to the end of the game. */
	SabreWulfLittlefPresentationText* _textEndGame;
	/** The text refering to the percentage of the game finished. */
	SabreWulfLittlefPresentationText* _textPercentageGame;
	/** Counter to maintain the pieces on the screen. */
	int _counter;
	/** When the input handler detects an special key has been pressed
		and the game stands in this state this variable is set to true. */
	bool _wantToExit;
};

/** The state of the game when sabreman find the exit.*/
class SabreWulfWinState : public QGAMES::GameState 
{
	public:
	SabreWulfWinState (QGAMES::Game* g)
		: QGAMES::GameState (__SABREWULFENDSTATE__, g),
		  _textNextGame (NULL),
		  _textCopyright (NULL),
		  _counter (0),
		  _wantToExit (false)
							{ }

	void setWantToExit (bool e)
							{ _wantToExit = e; }

	/** @see parent. */
	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	/** The text refering to the next game. */
	SabreWulfLittlefPresentationText* _textNextGame;
	/** The text refering to the copyright of the game. */
	SabreWulfLittlefPresentationText* _textCopyright;
	/** Counter to maintain the pieces on the screen. */
	int _counter;
	/** When the input handler detects an special key has been pressed
		and the game stands in this state this variable is set to true. */
	bool _wantToExit;
};

#endif
  
// End of the file
/*@}*/
