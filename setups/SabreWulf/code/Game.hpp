/** \addtogroup SabreWulf */
/*@{*/

#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "Sabreman.hpp"
#include "MtEntities.hpp"
#include "ObjEntities.hpp"
#include "ScObjects.hpp"
#include <Arcade/arcadeinclude.hpp>

/** Class to represente Sabrewulf Game. */
class SabreWulfGame : public QGAMES::ArcadeGame
{
	public:
	struct Level
	{
		Level ()
			: _level (0), 
			  _maxTimeToAppearNasties (0),
			  _maxNumberOfAppearances (0),
			  _maxNumberOfAppearingMonsters (0),
			  _percentUpPerFood (0),
			  _pointsUpPerFood (0),
			  _percentDownPerMonster (0),
			  _pointsUpWhenKillMonster (0),
			  _secondsInNotNormalState (0),
			  _secondsBetweenBlinks (0.0f),
			  _numberOfBlinks (0)
							{ }

		int _level;
		int _maxTimeToAppearNasties;
		int _maxNumberOfAppearances;
		int _maxNumberOfAppearingMonsters;
		int _percentUpPerFood;
		int _pointsUpPerFood;
		int _percentDownPerMonster;
		int _pointsUpWhenKillMonster;
		int _secondsInNotNormalState;
		float _secondsBetweenBlinks;
		int _numberOfBlinks;
		int _secondsToGrowOrchild;
		int _secondsOrchildBig;
	};

	// To define a list of levels...
	typedef std::vector <Level> Levels;

	// Constructors & Destructors
	SabreWulfGame ();

	/** Reset the initial values of the game. 
		Any time the game starts, this method is invoked. 
		The score elements have to had been created before. 
		Otherwise it won't work. */
	void resetGame ();
	
	/** Information about a level.
		Assertion of the level required is done before. */
	const Level& levelInfo () const
							{ return (_levels [_level]); }
	/** To know the current level. */
	int level () const
							{ return (_level); }
	/** To set a new level. */
	void setLevel (int l);

	/** To set whether the game is or not controlled by joystick. */
	void setJoystick (bool j);

	/** To know the current maze place. */
	int mazePlaceNumber () const;
	/** To set the maze place number. */
	void setMazePlaceNumber (int nR);

	/** To know the energy available. */
	int energyAvailable () const
							{ return (_energyPercentage); }
	/** To set the energy available. */
	void setEnergyAvailable (int e);

	/** To know the number of lives. */
	int numberOfLives () const
							{ return (_numberOfLives); }
	/** To set the number of lives. */
	void setNumberOfLives (int nL);

	/** To know the number of seconds from the beggining. */
	int numberOfSeconds () const
							{ return (_numberOfSeconds); }
	/** To set the number of seconds. */
	void setNumberOfSeconds (int nS);

	/** To know the number of points. */
	int numberOfPoints () const
							{ return (_scoreNumber); }
	/** To set the number of points. */
	void setNumberOfPoints (int nP); 

	/** To know the percentage of the game played. */
	int percentagePlayed () const;

	/** To indicate the main character is managing the sword. */
	void fight ();
	/** To indicate the main character is no longer managing the sword. */
	void noFight ();
	/** To know whether the main character is or not fighting. */
	bool isFighting ();

	virtual void updatePositions ();
	virtual void addScoreObjects ();
	virtual void removeScoreObjects ();
	virtual void additionalTasks ()
							{ /** Nothing...the default behaviour is deleted. */ }

	// ---
	// To activate or desactivate to draw the limits of the maze
	// This is an option just valid in debug mode...
	#ifndef NDEBUG
	void onOffDrawLimits ();
	#endif
	// ---

	/** @see parent. */
	virtual void drawOn (QGAMES::Screen* s);

	private:
	// @see parent
	virtual QGAMES::EntityBuilder* createEntityBuilder ();
	virtual QGAMES::MovementBuilder* createMovementBuilder ();
	virtual QGAMES::InputHandler* createInputHandler ();
	virtual QGAMES::Screens createScreens ();
	virtual QGAMES::WorldBuilder* createWorldBuilder ();
	virtual QGAMES::MapBuilder* createMapBuilder ();

	/* To initialize the game. */
	virtual void initialize ();
	/** To finalize the game. */
	virtual void finalize ();
	/** To detect the collisions among the basic objects in the game. */
	virtual void detectCollisions ();

	// Implementation
	/** When player collisions with the orchild. */
	void playerCollisionWithOrchild ();
	/** When player collisions with an object. */
	void playerCollisionWithObject (SabreWulfObject* obj);
	/** When player collisions with the nasties. */
	void playerCollisionWithNastie (SabreWulfNastie* nt);
	/** When player collisions with the fire. */
	void playerCollisionWithFire ();
	/** When player collisions with an inmortal. */
	void playerCollisionWithInmortal (SabreWulfInmortal* iM);
	/** When player collisions with the guardian. */
	void playerCollisionWithGuardian ();
	/** When the player dies. */
	void playerDies ();

	private:
	/** The levels. */
	Levels _levels;
	/** The number of the level. */
	int _level;
	/** The frame rounding the score objects. */
	SabreWulfScoreFrame* _scoreFrame;
	/** The object to reflect the lives. */
	SabreWulfEnergy* _energy;
	/** The object to reflect the energy. */
	SabreWulfLives* _lives;
	/** The time counter. */
	SabreWulfTimeCounter* _timeCounter;
	/** The score counter. */
	SabreWulfScoreCounter* _scoreCounter;
	/** Percentage of energy. */
	int _energyPercentage;
	/** The number of lives. */
	int _numberOfLives;
	/** The current number of seconds. */
	int _numberOfSeconds;
	/** THe current score number. */
	int _scoreNumber;

	// Implementation
	/** To count down the energy. */
	int _countEnergy;
	/** A counter to count when to change a second. */
	int _counterPerSecond;
	/** To count in debug mode the number of collisions. */
	int _n;

};

#endif
  
// End of the file
/*@}*/