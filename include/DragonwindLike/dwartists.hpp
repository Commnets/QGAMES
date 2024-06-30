/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: dwartist.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 06/04/2018 \n
 *	Description: Define the base of the artists used in a dragonwind like game.
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_ARTIST__
#define __DRAGONWIND_ARTIST__

#include <DragonwindLike/dwdefinitions.hpp>
#include <Platform/ptinclude.hpp>

namespace DRAGONWIND
{
	// They are refered in the character class definition...
	class Shooting;

	/** The male ninja used as background in the presentation menu. */
	class MenuNinjaMale : public QGAMES::Entity
	{
		public:
		MenuNinjaMale (int eId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::Entity::Data& d = Data ())
			: QGAMES::Entity (eId, f, d)
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent. */
		virtual void initialize ()
							{ setCurrentState (__DRAGONWIND_NINJAPRESENTATIONBASICSTATE__);
							  QGAMES::Entity::initialize (); }
	};

	/** The same but the female one. */
	class MenuNinjaFemale : public QGAMES::Entity
	{
		public:
		MenuNinjaFemale (int eId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::Entity::Data& d = Data ())
			: QGAMES::Entity (eId, f, d)
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent. */
		virtual void initialize ()
							{ setCurrentState (__DRAGONWIND_NINJAPRESENTATIONBASICSTATE__);
							  QGAMES::Entity::initialize (); }
	};

	/** All entities moving across a Dragonwind like game must inherit from this one.
		It is prepared to maintain different states of the character during the game and
		to calculate which tiles around the entity in each moment are important according to those states.
		By default all DragonArtists are NOT VISIBLE at construction time. 
		No DragonArtist can move when hitting a solid platform except if it is climbing or jumping (and moving up). */
	class DragonArtist : public QGAMES::PlatformArtist
	{
		public:
		/** Any artist of the game could try to say somthing. 
			To do so, a dialog controller should be used. It is implemented in the DragonArtist class. 
			Any dialog controller by that class is defined by a set of properties. 
			This is the class with them. 
			It is public and visible to anybody. It is not the case of the controller. */
		struct DialogProperties 
		{
			DialogProperties ()
					: _font (__QGAMES_COURIER10WHITELETTERS__),
					  _separation (1), // Pixel
					  _chrsPerLine (20), 
					  _rectangleColor (__QGAMES_GRAYCOLOR__),
					  _borderColor (__QGAMES_WHITECOLOR__),
					  _fadeLevel (100), // A little bit transparent
					  _offset (QGAMES::Vector::_cero),
					  _secondsOnScreen (__BD 1.0) // By default
								{ }

			DialogProperties (int f, int sp, int cPL, const QGAMES::Color& rC, const QGAMES::Color& bC, int fL,
				QGAMES::bdata sOS, const QGAMES::Vector& o = QGAMES::Vector::_cero)
					: _font (f),
					  _separation (sp), // In pixles...
					  _chrsPerLine (cPL),
					  _rectangleColor (rC),
					  _borderColor (bC),
					  _fadeLevel (fL),
					  _secondsOnScreen (sOS),
					  _offset (o)
							{ assert (sOS > 0 && 
									  (fL >= 0 && fL <= 255)); }

			int _font;
			int _separation;
			int _chrsPerLine;
			QGAMES::Color _rectangleColor;
			QGAMES::Color _borderColor;
			int _fadeLevel;
			QGAMES::bdata _secondsOnScreen;
			QGAMES::Vector _offset;
		};

		DragonArtist (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::PlatformArtist::Data& d = QGAMES::PlatformArtist::Data ());

		~DragonArtist ()
							{ delete (_dialogController); }

		/** @see parent. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		/** Controlling the direction that the artist is looking to. 
			For some specific artists it could make sense.
			Take care: The meaning could depend on the dragonwind like game implementation actually. 
			By default the value is 0. In the default implementation it means "looking to the right". */
		int lookingToDirection () const
							{ return (_lookingDirection); }
		void setLookingToDirection (int lD)
							{ _lookingDirection = lD; }

		/** To know whether another dragon entity is or not an enemy.
			By default all are. It can be overloaded. */
		virtual bool isEnemy (const DRAGONWIND::DragonArtist* art)
							{ return (true); }

		/** @see parent. */
		virtual bool canMove (const QGAMES::Vector& d, const QGAMES::Vector& a);

		/** @see parent. 
			As some type of dragon artists can jump, the limits overflow
			a little bit the upper limit of the map. 
			When the artist is out of the lower limit of the map, 
			the artists die (@see updatePositions method in this class). */
		virtual bool isPossiblePosInMap (const QGAMES::Position& v);

		/** To determine whether the artist is or not totally blocked,
			it is that all tiles around are either a base or solid. */
		bool isBlocked ();

		/** To know whether the artist is or not doing something. 
			They can be overloaded by any class inheriting this one,
			but a very common default behaviour is given. */
		virtual bool isStanding () const
							{ DragonState dS = platformStateToDragonState ();
							  return (dS == _STAND || dS == _STANDATTACK); }
		virtual bool isRunning () const
							{ DragonState dS = platformStateToDragonState ();
							  return (dS == _RUN || dS == _RUNATTACK); }
		virtual bool isJumping () const
							{ DragonState dS = platformStateToDragonState ();
							  return (dS == _JUMP || dS == _JUMPANDATTACK || dS == _JUMPTHROW); }
		virtual bool isClimbing () const
							{ DragonState dS = platformStateToDragonState ();
							  return (dS == _CLIMB || dS == _CLIMBFIXED); }
		virtual bool isSlidding () const
							{ return (platformStateToDragonState () == _SLIDE); }
		virtual bool isFalling () const
							{ DragonState dS = platformStateToDragonState ();
							  return (dS == _FALL || dS == _FLOAT); }
		virtual bool isThrowing () const
							{ return (platformStateToDragonState () == _THROW); }
		virtual bool isDieing () const 
							{ return (platformStateToDragonState () == _DIEING); }
		virtual bool hasDied () const 
							{ return (platformStateToDragonState () == _DIEING && currentAnimation () -> end ()); }
		virtual bool isDied () const 
							{ return (platformStateToDragonState () == _DIED); }
		virtual bool isAlive () const
							{ return (!isDieing () && !isDied () && !isHappy ()); } // Happy is like not being alive...
		virtual bool isSwiming () const
							{ return (platformStateToDragonState () == _SWIM); }
		virtual bool isHappy () const
							{ return (platformStateToDragonState () == _HAPPY); }
		virtual bool isRolling () const
							{ return (platformStateToDragonState () == _ROLL); }
		virtual bool isInTheAir () const
							{ return (platformStateToDragonState () == _INTHEAIR); }
		virtual bool isAttacking () const
							{ return (platformStateToDragonState () == _STANDATTACK ||
									  platformStateToDragonState () == _RUNATTACK ||
									  platformStateToDragonState () == _JUMPANDATTACK); }
		virtual bool isOnAPlatformState () const
							{ return (isStanding () || isRunning () || isSlidding () || isRolling () || 
									  isThrowing () || isDieing () || isDied ()); }

		// To manage the actions when possible. 
		// They can be overload, but a default behaviour is always defined for each one
		/** ToStand can be used from many other different states. 
			Some of them will be also states valid only on a platform, 
			so an adjustment to the position could be required
			to maintain always the base at the same place. This is what the second parameter indicates. 
			By default it is nor needed. 
			The first parameter is the direction to look to when standing. 
			Returns true if it was possible to stand, and false in other circunstance. */
		virtual bool toStand (int lD, bool a = false);
		/** Same situation (regarding the adjustment to the base) deferred.
			This way of doing is requiered (usually) when the state has to be changed processing 
			an event related with the current one...
			Take into account that it is not possible to change the direction the entity is looking to 
			when using this method */
		virtual bool toStandDeferred (bool a = false) 
							{ setPlatformStateDeferred (dragonStateToPlatformState (_STAND), a); return (true); }
		/** Start to fall.
			Returns true if it was possible and false if it wasn't */
		virtual bool toFall ();

		/** When something has to be said.
			Beautiful and simple!. */
		virtual void toSay (const std::string& txt, const DialogProperties& prps = DialogProperties ())
							{ _dialogController -> addDialog (txt, prps); }

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_noPoint);

		protected:
		enum DragonState
		{
			_STAND = 0, 
			_STANDATTACK = 1, 
			_RUN = 2, 
			_RUNATTACK = 3, 
			_CLIMB = 4,
			_CLIMBFIXED = 5,
			_FALL = 6,
			_FLOAT = 7,
			_INTHEAIR = 8,
			_JUMP = 9, 
			_JUMPANDATTACK = 10, 
			_SLIDE = 11,
			_SWIM = 12,
			_THROW = 13, 
			_JUMPTHROW = 14,
			_DIEING = 15,
			_DIED = 16,
			_HAPPY = 17,
			_ROLL = 18
		};

		/** To translate a dragon state into a platform one (used at parent level) and the opposite. 
			By default it is a literal translation of the enum value defined. */
		DragonState platformStateToDragonState () const
							{ return (platformStateToDragonState (platformState ())); } 
		virtual DragonState platformStateToDragonState (int pS) const;
		virtual int dragonStateToPlatformState (DragonState dS) const
							{ return ((int) dS); }

		/** @see parent. */
		virtual QGAMES::Tiles referenceTilesNow (const QGAMES::SetOfTiles& rT, 
			const QGAMES::Vector& dr, const QGAMES::Vector& a);

		/** @see parent. 
			By default basically: 
			When there is no layer under, then starts to fall, and when there is, then stands. */
		virtual void afterAllLayersTestedInWhenOnTile ();

		/** What to do when the artists is finally out of the "map".
			Imagine e.g. it falls out of the map. 
			What happen? Should it be removed from the game or does it mean it "dies"?
			It is something that depends on the artist. 
			By default, juist only becomes invisible. */
		virtual void whenGoingOutOfTheMap ()
							{ setVisible (false); }

		/** @see parent. */
		virtual void drawOnLimits (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_noPoint);

		/** To recalculate the position X of the base. */
		QGAMES::Position baseWithXPositionAdjusted ();

		protected:
		/** Looking to? 
			The meaning could depend on the implementation and on the specific artist. 
			General convenctions could be 0 to the right, 1 to the left,... 
			Remember that this variable could reflect also other orientations like looking up, down,...and is opened!!
			This is the reason to choose a number instead a enum. */
		int _lookingDirection; 

		/** To represent a dialog, that is: a text plus a set of properties. */
		class Dialog
		{
			public:
			Dialog (const std::string& txt, const DialogProperties& prps = DialogProperties ())
				: _text (txt), _properties (prps),
				  _internalTexts (),
				  _roundingRectangle (QGAMES::Rectangle::_noRectangle)
							{ createInternalTexts (); }

			~Dialog ()
							{ deleteInternalTexts (); }

			const std::string& text () const
							{ return (_text); }
			const DialogProperties& properties () const
							{ return (_properties); }

			void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

			protected:
			void createInternalTexts ();
			void deleteInternalTexts ();

			protected:
			std::string _text;
			DialogProperties _properties;

			// Implementation
			std::vector <QGAMES::ScoreObjectText*> _internalTexts;
			QGAMES::Rectangle _roundingRectangle;
		};

		/** All dialogs are managed by a dialog controller. \n
			Every artist has an object of this type. */
		class DialogController
		{
			public:
			DialogController (DragonArtist* o)
				: _dialogs (),
				  _loopsCounter (0),
				  _owner (o)
								{ assert (_owner); }

			~DialogController ();

			void addDialog (const std::string& txt, const DialogProperties& prps = DialogProperties ())
								{ _dialogs.push (new Dialog (txt, prps)); }

			void update ();
			void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

			protected:
			std::queue <Dialog*> _dialogs;

			// Implementation
			int _loopsCounter;
			DragonArtist* _owner;
		};

		// Implementaion
		DialogController* _dialogController;
		bool _lastCanExitWaterUp, _lastCanExitWaterDown, _lastCanExitWaterLeft, _lastCanExitWaterRight;
		std::vector <bool> _lastCollisionAtDir;
	};

	/** The main characters inherits from this one (bad guys and ninjas, etc. the ones that move really) 
		That type of characters define attributes related with their behaviour when shooting or receive shootings e.g.
		These attributes have a default value but they can be changed depending on the game/player situation.
		All these attributes are grouped around a nested class called Behaviour. */
	class Character : public DragonArtist
	{
		public:
		struct Behaviour
		{
			Behaviour ()
				: _canBeHurmedByEnemies (true), // Can be damaged by enemies? (if it can, it can also die)
				  _canBeHurmedBySimilars (false), // Can be damaged by similars? (if it can, it can also die)
				  _canBeHurmedByFlyingMonsters (true), // Can be damaged by flying monsters (if it can, it can also die)
				  _canBeHurmedByLightning (true), // Can be damaged by the lightnings (it can die only for this reason)
				  _gunsHurming (), // Meaning all... (-1 added later)
				  _energyLostPerLoop (__BD 1 / __BD QGAMES::Game::game () -> framesPerSecond ()), // 1% per second (100 seconds live)
				  _energyLostPerExplosion (__BD 100), // Energy lost when an explosion affects him...
				  _pointsGivenWhenDie (10), // When die, the number of points given to the enemy who killed him/her
				  _shootingEnergy (5), // Shooting energy (it will be substracted from the enemy hit)
				  _automaticMovement (-1), // Means none by default (ready for ninja's)
				  _weakness (1) // Meaning normal...
							{ _gunsHurming.push_back (-1); }

			Behaviour (bool cE, bool cS, bool cF, bool cL, const std::vector <int>& gH, 
					   QGAMES::bdata eL, QGAMES::bdata ePE, int p, int s, 
					   int aM = -1, QGAMES::bdata wk = __BD 1)
				: _canBeHurmedByEnemies (cE),
				  _canBeHurmedBySimilars (cS),
				  _canBeHurmedByFlyingMonsters (cF),
				  _canBeHurmedByLightning (cL),
				  _gunsHurming (gH),
				  _energyLostPerLoop (eL),
				  _energyLostPerExplosion (ePE),
				  _pointsGivenWhenDie (p),
				  _shootingEnergy (s),
				  _automaticMovement (aM), 
				  _weakness (wk)
							{ assert (!_gunsHurming.empty ()); }

			/** 
			  * The behaviour can be set from a set of values.
			  * The name of the values are always a number (the id), 
			  * and the value is a string so they must interpreted.
			  * This is used specially in the Bad Guys, when it is set from the description in the .tmx file
			  * The meaning of those are:
			  * 0 (__DRAGONWIND_BHVOPENVALUEHURMEDEMISPARAM__)	: Can be hurmed by enemies? (YES | NO)
			  * 1 (__DRAGONWIND_BHVOPENVALUEHURMEDSIMSPARAM__)	: Can be hurmed by Similars? (YES | NO)
			  * 2 (__DRAGONWIND_BHVOPENVALUEHURMEDFLYSPARAM__)	: Can be hurmed by flying monsters? (YES | NO)
			  * 3 (__DRAGONWIND_BHVOPENVALUEHURMEDLIGHTPARAM__)	: Can be hurmed by lightning? (YES | NO)
			  * 4 (__DRAGONWIND_BHVOPENVALUEGUNSHURMPARAM__)	: The list of the type of guns affecting the character if any (empty, -1, ...)
			  * 5 (__DRAGONWIND_BHVOPENVALUEENERGYLOSTPARAM__)	: The energy lost (in percentage so between 0 and 100) per loop
			  * 6 (__DRAGONWIND_BHVOPENVALUEPTSWHENDIEPARAM__)	: When it is died, how many point are given to the hunter?
			  * 7 (__DRAGONWIND_BHVOPENVALUESHTENERGYPARAM__)	: The energy (between 0 and 100) the shootings of the entity has?
			  * 8 (__DRAGONWIND_BHVOPENVALUEAUTOMOVPARAM__)		: The block id associated to that guy (if any, defined in dwworlds.xml). -1 means none
			  * 9 (__DRAGONWIND_BHVOPENVALUEWEAKNESSPARAM__)	: The weakness of the guy. 1 means normal. 2 weaker. 
			  */
			Behaviour (const QGAMES::SetOfOpenValues& oV);

			/** To get the properties as a set of openvalues. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			/** To check whether it can or not be damaged by a type of shooting. */
			bool canBeHurmedBy (int st);

			bool _canBeHurmedByEnemies, _canBeHurmedBySimilars, _canBeHurmedByFlyingMonsters;
			bool _canBeHurmedByLightning;
			std::vector <int> _gunsHurming; // empty means none, -1 means all...
			QGAMES::bdata _energyLostPerLoop;
			QGAMES::bdata _energyLostPerExplosion;
			int _pointsGivenWhenDie;
			int _shootingEnergy;
			int _automaticMovement; // -1 means none...
			QGAMES::bdata _weakness; // Optional. By default just 1...
		};

		// The behaviour is declared as a pointer to allow further redefinitions
		// By default a standard Behaviour object is set...
		Character (int cId, Behaviour* bhv = new Behaviour (), const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::PlatformArtist::Data& d = QGAMES::PlatformArtist::Data ());
		~Character ()
							{ delete (_behaviour); _behaviour = NULL; }

		/** @see parent. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		/** @see parent. */
		virtual bool isPlatformStatePossible (int pS) const;

		const Behaviour* behaviour () const
							{ return (_behaviour); }
		Behaviour* behaviour () 
							{ return (_behaviour); }
		void setBehaviour (Behaviour* bhv);

		/** Manages the energy. 
			Always between 0 an 100. When 0, it will be notified in the inEveryLoop method. 
			It shouldn't be usual to invoke setEnergy directly, only internal methods use it, but here you have it! */
		QGAMES::bdata energy () const
							{ return (_energy); }
		void setEnergy (QGAMES::bdata e);
		/** The energy is actually only modified in the method inEveryLoop, so this method only "takes note"
			of the amount of energy to add (or substract) to (from) the total in the inEveryLoop execution. 
			This method can be called from many different parts in a single loop.
			e.g. Just living reduce the energy, but it is e.g. also reduced when the character collisions with a bee. */
		virtual void setEnergyToAdd (QGAMES::bdata e)
							{ _energyToAdd += e; } 
		/** Restore level of energy. */
		void restoreLevelOfEnergy ()
							{ _energyToAdd = 100 - _energy; }
		/** Stops / Continue losing energy.
			In some situations, the characters stops loosing energy.
			e.g. It could be shields or something against any problem. */
		bool losingEnergy () const
							{ return (_losingEnergy); }
		void setLosingEnergy (bool sL)
							{ _losingEnergy = sL; }

		/** Manages the score. */
		int score () const
							{ return (_score); }
		virtual void setScore (int s);

		/** To manage the shootings. */
		void loadShootings (const QGAMES::Entities& sth); // Add new shootings to the list of those...
		Shooting* firstAvailableShooting (); // Null when none...
		void clearShootings ()
							{ _shootings = QGAMES::Entities (); }

		/** @see parent. */
		virtual bool isAlive () const;
		
		// Managing different actions of the character...
		/** To die if it not already dieing. 
			To die is only possible when the artist is on platform.
			If it is not, then a "take note" process is run. 
			Returns true when possible and false in any other circunstance. */
		virtual bool toDie ();
		/** To move to a direction.
			When standing it will mean to start to run.
			When running it could mean to change the direction of the movement.
			When jumping means to change the direction of the fall (to do the game more dinamic).
			When climbing this is to change a little bit the direction of the movement.
			and so on and so forth...
			The paremeter indicates the direction of the movement. 
			Returns true when possible and false in any other circunstance. */
		virtual bool toMove (const QGAMES::Vector& dr);
		/** To climb through an appropiate media.
			The vector indicates the direction of the initial movement.
			This method is valid only just when the artists is not already climbing 
			(other state or stopped climbing before) 
			Returns true when possible and false in any other circunstance. */
		virtual bool toClimb (const QGAMES::Vector& dr);
		/** To jump 
			Jumps in the current direction, taking into account the impulse
			and other circunstances around the character if they exist. 
			The other parameter indicates whether the down orientation is selected or not. 
			Jumping with that variable to true means to slide instead. 
			It is a very similar movement (in terms of physics even). 
			Returns true when possible and false when it is not. */
		virtual bool toJump (int i, bool dP);
		/** To Shoot if possible. 
			The type of shooting is optional. If not define, the method typeOfShootingAvailable is called instead 
			Returns true if it was possible to shoot and false in any other circunstance. */
		virtual bool toShoot (int i, bool uP, int tS = -1);

		/** @see parent. 
			The condition affects any character in different ways. 
			By default, only lightning can affect any character ¡ even killing him/her. */
		virtual void whenWheatherCondition (QGAMES::EnvironmentalCondition* eC);

		/** @see parent. */
		virtual void initialize ();
		virtual void inEveryLoop ();
		virtual void updatePositions ();

		/** @see parent. */
		virtual void whenCollisionWith (QGAMES::Entity* e);

		/** To know the entity who hit the entity the most. 
			If there are many, then the first one is returned. */
		DRAGONWIND::Character* whoHitTheMost () const;

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
							{ return (new OnOffSwitches ()); }

		/** @see parent. */
		virtual void afterAllLayersTestedInWhenOnTile ();

		/** To determinate which is the animation looking to a specific orientation when in a specific state. */
		virtual int animationIdForState (DragonState st, int lD) = 0;

		/** To check whether the artist will or not be on a type of tile before having certain type of state. */
		virtual bool overTileWhenState (int tT, DragonState st);
		/** And the same for a set of type of tiles. */
		virtual bool overTileWhenState (const std::vector <int>& tT, DragonState st);

		// Related with shooting...
		/** To know wether is possible to shoot or not. 
			By default it is always possible. */
		virtual bool isPossibleToShoot () const
							{ return (true); }
		/** To know which type of shooting has to be shooted right now. 
			It can depend on a formula or depend on the elements the character has in the pocket e.g. 
			By default it returns always __DRAGONWIND_NINJATHINGARROWTYPE__.
			This number is used to create a definition element passed to the shooting and used to create 
			actually the shooting aspect. */
		virtual int typeOfShootingAvailable () const
							{ return (__DRAGONWIND_NINJATHINGARROWTYPE__); }
		/** To know how many shootings are still available, if any. 
			By default it returns __MAXINT__, which means inifite really. */
		virtual int numberShootingsAvailable () const
							{ return (__MAXINT__); }
		/** Reduce the number of shootings available, if it is possible.
			By default, it makes no sense, because the number of shootings available are inifnite. */
		virtual void reduceShootingsAvailable (int nS)
							{ }
		/** The position where a shoot starts. 
			It will depend on the type of shooting and to the orientation of the character. 
			It can be rewritten in later classes. 
			By default, bombs like start from the center and the rest from the top part of the character. */
		virtual QGAMES::Position shootingPositionForType (int t) const;

		/** To consider (take note) a new hit by a character. */
		void hitBy (DRAGONWIND::Character* chr);

		/** Define a buoy to die within the inEveryLoop method. */
		class ToDieBuoy : public QGAMES::Buoy
		{
			public:
			ToDieBuoy ()
				: QGAMES::Buoy (__DRAGONWIND_DIEDEFEREDBUOYID__, (QGAMES::bdata) 0)
								{ /** Nothing else to do. */ }

			virtual void* treatFor (QGAMES::Element* e);
		};

		/** The method to die deferred. */
		void toDieDeferred ();

		protected:
		/** How the character behaves when shooting or receive shootings. */
		Behaviour* _behaviour;
		/** The energy that the character has. It goes from 100 (when initialized) to 0. */
		QGAMES::bdata _energy;
		/** To manage whether the character reduced energy or not. */
		bool _losingEnergy;
		/** The number of points the character has. */
		int _score;

		/** A reference to the shootings available for the character.
			The method to add them verifies they are all shootings. */
		QGAMES::Entities _shootings;

		// Implementation
		/** The level of energy to add or subtract next in the next execution of inEveryMethod. */
		QGAMES::bdata _energyToAdd;
		std::map <DRAGONWIND::Character*, int> _hitsBy;
		bool _exitingTheWater;

		static const int _SWITCHTODIEWHENPOSSIBLE = 0;
	};
}

#include <DragonwindLike/dwshootings.hpp>
#include <DragonwindLike/dwninjas.hpp>
#include <DragonwindLike/dwfood.hpp>
#include <DragonwindLike/dwthings.hpp>
#include <DragonwindLike/dwflyingmonsters.hpp>
#include <DragonwindLike/dwbadguys.hpp>

#endif
  
// End of the file
/*@}*/