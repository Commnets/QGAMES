/** \addtogroup SabreWulf */
/*@{*/

#ifndef __ENTITIES_HPP__
#define __ENTITIES_HPP__

#include "Defs.hpp"
#include <Arcade/arcadeinclude.hpp>

class SabreWulfMonster;
class SabreWulfInmortal;

/** This class defines the sabreman. */
class Sabreman : public QGAMES::Artist
{
	public:
	/** The different states a sabreman can be in. */ 
	enum InternalState { 
		_NORMAL = 0, _FAST, _STOPPED, _INMORTAL, _GOINGBACK };

	// Constructors & Destructores
	Sabreman (const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

	virtual Entity* clone () const
							{ return (new Sabreman (_forms, _data)); }

	/** To determinate whether the sabreman can or not move.
		If the sabreman is in one of the moving zones the movemenet is possible. */
	virtual bool canMove (const QGAMES::Vector& d, const QGAMES::Vector& a);

	/** @see parent. */
	virtual QGAMES::Rectangle collisionZone () const;

	/** To get the position of the character to take into a count when moving
		or collisioning with some one. */
	QGAMES::Position referenceZone (const QGAMES::Vector& d) const;

	/** To set and to know whether the player is or not fighting. */
	void setFighting (bool f);
	bool isFighting () const
							{ return (_fighting); }
	/** To set & to know the state of the sabreman. */
	void setInternalState (InternalState st);
	InternalState internalState () const
							{ return (_internalState); }
	/** To set sabreman is carrying a piece of the amulet. */
	void setAmultePiece (int nP);
	/** To know whether sabreman is carrying or not a piece of the amulet. */
	bool isCarryingPiece (int nP) const;
	/** To know whether sabreman is or not carrying the full amulet. */
	bool isCarryingAmulet () const;
	/** To leave all amulet pieces. */
	void leaveAmulets ();
	/** To know whether sabreman can or not kill a monster. */
	bool couldKill (SabreWulfMonster* m);
	/** To know whether sabreman can or not damage an inmortal. */
	bool couldDamage (SabreWulfInmortal* iM);
	/** To know whether sabreman can or not die. */
	bool canDie () const
							{ return (_internalState == _NORMAL); }
	/** To know whether sabreman is or not inmortal. */
	bool isInmortal () const
							{ return (!canDie ()); }

	/** @see parent.
		To activate or desactivate if the sabreman is visible.  
		It is used when the game is in some states. */
	virtual void setVisible (bool v);
	/** To know the last orientation when move. */
	const QGAMES::Vector& lastOrientation () const
							{ return (_lastMov); }
	/** To know the orientation of the las monster collision with sabreman. */
	const QGAMES::Vector& lastMonsterOrientation () const
							{ return (_lastMonsterOrientation); }
	/** To know the maze where sabreman is. */
	int mazePlaceNumber () const;

	virtual void initialize ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

	// ---
	// To activate or desactivate to draw the limits of the maze
	// This is an option just valid in debug mode...
	#ifndef NDEBUG
	void onOffDrawLimits ();
	#endif
	// ---

	/** @see parent. */
	virtual void whenCollisionWith (QGAMES::Entity* e);

	private:
	/** This method is invoked when either the state or the 
		fighting status change, to fix the form and frame for sabreman. */
	void determinateFormAndFrames ();
	/** To know whether a monster is or not comming by the back. */
	bool isMonsterCommingBack (SabreWulfMonster* mt);

	private:
	/** To determinate whether the player is or not fighting. */
	bool _fighting;
	/** To determinate the status of the player.
		It is: The sabreman has been tocuhed by any special type of orchild mainly. */
	InternalState _internalState;
	/** To know whether sabreman is or not carrying the different pieces of the amulet. */
	std::vector <bool> _amuletPieces;

	// Implementation
	/** Counter to count steps to change the aspect. */
	int _counter;
	/** To know when to make a sound (for walking). */
	bool _walkingSound;
	/** To indicate whether the sabreman is or not blinking. */
	bool _blinking;
	/** To indicate whether the blink is or not on. */
	bool _blinkingState;
	/** To count how many seconds before blinking. */
	int _counterBeforeBlinking;
	/** To count the number of blinkings. */
	int _counterBlinkings;
	/** To count the time in the state of blinking. */
	int _counterBlinks;
	/** To count the time sabreman is stopped when that is his status!. */
	int _counterStopped;
	/** To count how long sabreman is inmortal after being picked by an yellow orchild. */
	int _counterAfterStopped;
	/** To know whether the orchilds affect or not sabreman.
		Normally after being affected by a yellow orchild sabremans stops and
		just a couple of microseconds after that no other orchilds affects him. */
	bool _yellowOrchildsAffects;
	/** To keep track of the movement when changing the aspect. */
	QGAMES::Vector _lastMov;
	/** The initial and final frame.
		It is something that depens on the aspect of the character. */
	int _initialFrameRight, _finalFrameRight;
	int _initialFrameLeft, _finalFrameLeft;
	int _initialFrameUp, _finalFrameUp;
	int _initialFrameDown, _finalFrameDown;
	int _cInitialFrame, _cFinalFrame;
	/** The last orientation used by the character. */
	QGAMES::Vector _lastOrientation;
	/** The last orientation of the monster collision with sabreman. */
	QGAMES::Vector _lastMonsterOrientation;

	// ---
	// The variable to control when to draw or not the limits of the maze
	// It makes only sense in debug mode...
	#ifndef NDEBUG
	bool _onOffDrawLimits;
	#endif
	// ---
};

#endif
  
// End of the file
/*@}*/