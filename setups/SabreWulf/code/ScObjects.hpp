/** \addtogroup SabreWulf */
/*@{*/

#ifndef __SCENTITIES_HPP__
#define __SCENTITIES_HPP__

#include "Worlds.hpp"
#include "SabreWulf.hpp"
#include "Defs.hpp"
#include <Arcade/arcadeinclude.hpp>

/** Class to represent the frame around all the information about the game. */
class SabreWulfScoreFrame : public QGAMES::ScoreObject
{
	public:
	// Constructors
	SabreWulfScoreFrame ();

	virtual Entity* clone () const
							{ return (new SabreWulfScoreFrame ()); }

	/** To draw the frame. 
		@see parent. */
	virtual void drawOn (QGAMES::Screen* s, 
		const QGAMES::Position& p = QGAMES::Position::_noPoint);

	private:
	/** The number of color the frame is. */
	int _color;
};

/** Class to represent the bar of energy. */
class SabreWulfEnergy : public QGAMES::ScoreObject
{
	public:
	// Constructors
	SabreWulfEnergy ();

	virtual Entity* clone () const
							{ return (new SabreWulfEnergy ()); }

	/** To set the % of the bar of energy visible. */
	void setPercentage (int p);
	/** To know how % of the bar of energy has been consumed. */
	int percentage () const
							{ return (_percentage); }

	/** To initialize the bar of energy.
		Always 0% consumed. */
	virtual void initialize ();
	/** To draw the bar of energy.
		A part (below the percentage consumed will be shown as an eaten chicken,
		whilst the other part will be shown as a empty bar. 
		@see parent. */
	virtual void drawOn (QGAMES::Screen* s, 
		const QGAMES::Position& p = QGAMES::Position::_noPoint);

	private:
	/** The percentrage of the bar of energy that has been consumed. */
	int _percentage;
};

/** Class to represent the lives. */
class SabreWulfLives : public QGAMES::ScoreObject
{
	public:
	// Constructors
	SabreWulfLives ();

	virtual Entity* clone () const
							{ return (new SabreWulfLives ()); }

	/** To set the number of lives. */
	void setLives (int l);
	/** To know the number of lives. */
	int lives () const
							{ return (_lives); }

	/** To initialize the lives. */
	virtual void initialize ();
	/** To draw the lives.
		@see parent. */
	virtual void drawOn (QGAMES::Screen* s, 
		const QGAMES::Position& p = QGAMES::Position::_noPoint);

	private:
	/** The number of lives. */
	int _lives;
};

/** A class to represent the time counter. 
	The time core only makes the time to pass. */
class SabreWulfTimeCounter : public QGAMES::ScoreObjectText
{
	public:
	// Constructors & Destructors
	/** Just to create the time counter. It only adds the right form. 
		This entity is not created so far from an entity builder. */
	SabreWulfTimeCounter ();

	virtual Entity* clone () const
							{ return (new SabreWulfTimeCounter ()); }

	/** To convert a time passed as parameter in seconds,
		into a text to be shown in the score object. */
	void setSeconds (int nS);

	protected:
	/** @see parent. */
	virtual int formToDrawLetters ()
							{ return (__SABREWULFLETTERS__); }
};

/** A class to represent the score counter. 
	This counter adds points any time a monster is destroyed, 
	any time something is gotten, and any time something is eaten. */
class SabreWulfScoreCounter : public QGAMES::ScoreObjectNumber
{
	public:
	// Constructors & Destructors
	/** Just to create the score counter. It only adds the right form. 
		This entity is not created so far from an entity builder. */
	SabreWulfScoreCounter ();

	virtual Entity* clone () const
							{ return (new SabreWulfScoreCounter ()); }

	protected:
	/** @see parent. */
	virtual int formToDrawDigits ()
							{ return (__SABREWULFLETTERS__); }
};

#endif
  
// End of the file
/*@}*/
