/** \ingroup Battleship */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: bsscoreobjects.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/11/2019 \n
 *	Description: Defines the base for the score objects accesible for a Battleship game.
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIP_SCOREOBJECTS__
#define __BATTLESHIP_SCOREOBJECTS__

#include <BattleshipLike/bsdefinitions.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace BATTLESHIP
{
	/** Class to represent the energy of a entiy (usually the player). 
		The score object has to observe the main artist to get updated when his/her energy changes. 
		The form used to represent the energy can be changed (there is one by default). 
		That form has to have two frames: One representing the bar empty (first) and the other other full. 
		Take into account that several instances of this class can be set.
		It is interesting e.g. if several players can play simultaneosuly. */
	class EntityEnergy : public QGAMES::ScoreObject
	{
		public:
		EntityEnergy (int pId = 0, int eF = __QGAMES_BLUEENERGYBARFORM__);

		/** To set the % of the energy consumed. */
		void setPercentage (int p);
		/** To know how % of the energy has been consumed. */
		int percentage () const
							{ return (_percentage); }

		/** @see parent. */
		virtual Entity* clone () const
							{ return (new EntityEnergy (id () - __BATTLESHIP_SPACEELMNTENERGYSCOBJID__, _energyForm)); }

		/** @see parent. */
		virtual void initialize ();
		virtual void drawOn (QGAMES::Screen* s, 
			const QGAMES::Position& p = QGAMES::Position::_noPoint);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		/** The form used to represent the energy. */
		int _energyForm;

		// Implementation
		/** The percentage of the energy that has been consumed. */
		int _percentage;
	};

	/** Class to represent the lives the main artist still has. 
		The score object has to observe the main character to get updated one the number of 
		his/her lives changes too. 
		The form used to represent the lives can be changed. One is provided by default.
		The form has to have as many frames as typ of artist are used in the game (by default two). */
	class BattleshipLives : public QGAMES::ScoreObject
	{
		public:
		// The score object can be set for male or female aspect...male by default!
		BattleshipLives (int tA = 0, int lF = __BATTLESHIP_BATTLESHIPLIVESFORM__);

		/** @see parent. */
		virtual Entity* clone () const
							{ return (new BattleshipLives (_typeArtist, _livesForm)); }

		/** To set the number of lives. */
		void setLives (int l);
		/** To know the number of lives. */
		int lives () const
							{ return (_lives); }

		/** Type of artist represented */
		int typeArtist () const
							{ return (_typeArtist); }
		void setTypeArtist (bool tA) 
							{ _typeArtist = tA; }

		/** @see parent. */
		virtual void initialize ();
		virtual void drawOn (QGAMES::Screen* s, 
			const QGAMES::Position& p = QGAMES::Position::_noPoint);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		/** The form used to represent the lives. */
		int _livesForm;
		/** The type of artist represented. */
		int _typeArtist;

		// Implementation
		/** The number of lives. */
		int _lives;
	};

	/** Class to represent one thing being carried by the player. 
		The form used to represent the things being carried can be changed.
		One is provided by default.
		The form has to have as many frames as object can be carried by the character. 
		Take into account that there can be many objects of this type.
		It is interesting e.g if several players are playing simultaneously. */
	class BattleshipThingBeingCarried : public QGAMES::ScoreObject
	{
		public:
		BattleshipThingBeingCarried (int pId = 0, int tC = __BATTLESHIP_BATTLESHIPTHINGTOCARRYFORM__);

		/** @see parent. */
		virtual Entity* clone () const
						{ return (new BattleshipThingBeingCarried (id () - __BATTLESHIP_BATTLESHIPTHINGBEINGCARRIEDID__)); }

		/** To set the aspect of the form. */
		int thing () const
						{ return (_thing); }
		void setThing (int f);

		/** @see parent. */
		virtual void initialize ();
		virtual void drawOn (QGAMES::Screen* s, 
			const QGAMES::Position& p = QGAMES::Position::_noPoint);

		protected:
		/** The form used to represent the different things being carried. */
		int _thingsForm;

		// Implementation
		/** The specific thing.
			Has to be in the range of all forms possible. */
		int _thing;
	};

	/** Class to represent all things being carried. */
	class BattleshipThingsBeingCarried : public QGAMES::ScoreObject
	{
		public:
		BattleshipThingsBeingCarried (int nT = __BATTLESHIP_NUMBERTHINGSBEINGCARRIED__);
		~BattleshipThingsBeingCarried ();

		/** @see parent. */
		virtual Entity* clone () const;

		/** @see parent. */
		virtual void setPosition (const QGAMES::Position& p);

		/** To set the things. */
		std::vector <int> things () const;
		void setThings (const std::vector <int>& t);

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s, 
			const QGAMES::Position& p = QGAMES::Position::_noPoint);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		/** To create the specific element to represent the things being carried.
			By default it returns an instance of the previous class, but
			it can be overloaded if any. */
		virtual BattleshipThingBeingCarried* createThingBeingCarried (int id)
								{ return (new BattleshipThingBeingCarried (id)); }

		protected:
		// Implementation
		std::vector <BattleshipThingBeingCarried*> _things;

		/** Levels of fade. */
		static const int _FADELEVEL [25];
		/** Counter to count when to change the level of fading.*/
		static const int _COUNTERTOFADE = 0;
		/** Counter to count the level of fade applied to the form in use. */
		static const int _COUNTERFADELEVEL = 1;
		/** Switch to indicate whether the fade is getting darker or lighter. */
		static const int _SWITCHFADERLIGHTER = 0;
	};

	/** A class to represent the number of shootings available.
		This class should observe the main artist to reflect any change there. 
		The font used to represent the number of left shoots can be changed. */
	class ShootsLeftCounter : public QGAMES::ScoreObjectNumber
	{
		public:
		ShootsLeftCounter (int pId = 0, int lF = __QGAMES_RAVIE24ORANGELETTERS__);

		/** @see parent. */
		virtual Entity* clone () const
								{ return (new ShootsLeftCounter (id () - __BATTLESHIP_SPACEELMNTSHOOTINGSSTILLLEFTID__)); }

		/** @see parent. */
		virtual void initialize ();
		virtual void drawOn (QGAMES::Screen* s, 
			const QGAMES::Position& p = QGAMES::Position::_noPoint);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		/** @see parent.
			The type of letter could be redefined. */
		virtual int formToDrawDigits ()
								{ return (_font); }

		protected:
		/** The font used. */
		int _font;

		// Implementation
		/** The number of shootings still left. */
		int _shootingsLeft;
	};

	/** A class to represent the time counter. 
		The font of the time counter can be changed. */
	class TimeCounter : public QGAMES::ScoreObjectText
	{
		public:
		TimeCounter (int f = __QGAMES_RAVIE24BLUELETTERS__);

		/** @see parent. */
		virtual Entity* clone () const
								{ return (new TimeCounter ()); }

		/** To convert a time passed as parameter into something to be written. */
		void setSeconds (int nS);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		/** @see parent. */
		virtual int formToDrawLetters ()
								{ return (_font); }

		protected:
		/** The font used. */
		int _font;
	};

	/** A class to represent the score counter. 
		The font used can be changed. */
	class ScoreCounter : public QGAMES::ScoreObjectNumber
	{
		public:
		ScoreCounter (int f = __QGAMES_RAVIE24BLUELETTERS__);

		/** @see parent. */
		virtual Entity* clone () const
								{ return (new ScoreCounter ()); }

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		/** @see parent. */
		virtual int formToDrawDigits ()
								{ return (_font); }

		protected:
		/** The font used. */
		int _font;
	};

	/** A class to represent the a fix text in the screen. 
		It used e.g. to display the name of the player in use now. */
	class FixText : public QGAMES::ScoreObjectText
	{
		public:
		FixText (int id, const std::string& fT, int f = __QGAMES_RAVIE12ORANGELETTERS__);

		/** @see parent. */
		virtual Entity* clone () const
								{ return (new FixText (_id, _text, _font)); }

		protected:
		/** @see parent. */
		virtual int formToDrawLetters ()
								{ return (_font); }

		protected:
		/** The font used. */
		int _font;
	};
}

#endif
  
// End of the file
/*@}*/	
