/** \ingroup PACMAN */
/*@{*/

/**	
 *	@file	
 *	File: pcmonsters.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: General definition for any monster of the game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMAN_MONSTERS__
#define __PACMAN_MONSTERS__

#include <PacManLike/pcdefs.hpp>
#include <PacManLike/pcartists.hpp>

#include <advancedArcade/adinclude.hpp>

namespace PACMAN
{
	/** 
	  *	What the machine controls. \n
	  *	A monster moves through the maze chasing likely pacman. \n
	  *	Every monster has be on different states: \n
	  *	_NOTDEFINED	:	It is only a transient state, usually at the beginning of any game. \n
	  *	_ATHOME		:	When the monster starts the movement. Phisically it could be in th house space or not. \n
	  * _EXITINGHOME:	When it is already moving but it is still in the home space. \n
	  *					See definition of a map to know how to declare a hom space. \n
	  *	_CHASING	:	Moving through the maze and pursuiting the target. \n
	  *	_RUNNINGAWAY:	Moving through the maze and running away from the target (defined in the method runAwayMazePosition). \n
	  *	_TOBEEATEN	:	When it is in the situation to be eaten.
	  *	_BEINGEATEN	:	That happened, and the monster has to move back home to recover itself.
	  *	What is the position to run away o the element to chase has to be defined later. \n
	  * Every monster can be or not under an "elroy condition". It could mean many things and thy have to defined later.
	  */
	class Monster : public Artist
	{
		public:
		// The different status a monster cab ne in...
		enum class Status 
			{ _NOTDEFINED = 0, _ATHOME = 1, _EXITINGHOME = 2, _CHASING = 3, _RUNNINGWAY = 4, _TOBEEATEN = 5, _BEINGEATEN = 6 };

		Monster (int cId, int mN, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

		virtual bool isEnemy (const PacmanElement* elmnt) const override;

		/** The number of the monster. */
		int monsterNumber () const
							{ return (_monsterNumber); }

		/** To define whether there is or not a preferred direction to move to at exiting home. 
			It gotta be detailed later. */
		virtual QGAMES::Vector preferredDirectionAtStartingToMove () const = 0;

		/** The points associated to the monster. */
		int points () const
						{ return (_points); }
		void setPoints (int p)
							{ _points = p; }

		/** To know whether the monster is or not at home. */
		bool isAtHome () const;
		/** To know whether the monster is or not in a tunnel. */
		bool isInATunnelHall () const;
		/** To know the direction to start any movement. 
			This is also used when the monsters stops. */
		QGAMES::Vector directionToStartMovement () const;

		virtual void toStand () override;
		virtual void toMove (const QGAMES::Vector& d) override;
		void toChase (bool f);
		void toChaseDeferred (bool o);
		void toBeThreaten (bool f);
		/** The last parameter are the loops per blink */
		void toBlink (bool b, int bp);

		/** The elroy condition change things in the speed and maybe in the behaviour. 
			To accept or not the change depends on the type of monster. 
			By default it is not accepted and it always -1, meaning no condition aceepted. */
		virtual bool hasElroyPossibility () const
							{ return (false); }
		virtual void setElroyCondition (int nC) 
							{ _elroyCondition = -1; }
		int elroyCondition () const
							{ return (_elroyCondition); }

		virtual bool isAlive () const override
							{ return (_status != Status::_BEINGEATEN); }
		virtual bool isStanding () const override
							{ return (_status == Status::_ATHOME); }
		virtual bool isMoving () const override
							{ return (!isStanding () && _status != Status::_NOTDEFINED); }
		bool isDangerous () const
							{ return (isAlive () && 
								(_status == Status::_CHASING || _status == Status::_RUNNINGWAY || _status == Status::_EXITINGHOME)); }
		bool isChasing () const 
							{ return (_status == Status::_CHASING); }
		bool isRunningAway () const
							{ return (_status == Status::_RUNNINGWAY); }
		bool canReverseMovement () const;

		// To know the status...
		Status status () const
							{ return (_status); }

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		virtual void whenCollisionWith (QGAMES::Entity* e) override;

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters () override
							{ return (new Counters); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches)
		virtual QGAMES::OnOffSwitches* createOnOffSwitches () override
							{ return (new OnOffSwitches); }

		void setStatus (const Status& st);

		/** To know the last status. */
		Status lastStatus () const
							{ return (_lastStatus); }

		virtual void whatToDoWhenStopStatusIsRequested (const QGAMES::Vector& d) override;
		virtual void whatToDoWhenMovementStatusIsRequested (const QGAMES::Vector& d) override;
		virtual void whatToDoOnCurrentPosition () override
							{ }

		/** Define a buoy to chase within the inEveryLoop method. */
		class ToChaseBuoy final : public QGAMES::Buoy
		{
			public:
			ToChaseBuoy ()
				: QGAMES::Buoy (__PACMAN_TOCHASEBUOYID__, (QGAMES::bdata) 0)
							{ /** Nothing else to do. */ }

			void setChasing (bool c)
							{ _chasing = c; }

			virtual void* treatFor (QGAMES::Element* e) override;

			private:
			bool _chasing;
		};

		// Implementation
		/** To adapt the speed of the monster to the different cicunstances. 
			It could be invoked from many different places but from "whatToDoWhenMovementStatusIsRequested" method mainly. */
		virtual void adaptSpeed () = 0;
		/** The monsters can not enter back their home unless they have been eaten!. */
		virtual QGAMES::MazeModel::PathInMaze& recalculatePathInMaze 
			(const QGAMES::Vector& mD = QGAMES::Vector::_noPoint) override;
		/** Where to run. */
		virtual QGAMES::MazeModel::PositionInMaze runAwayMazePosition () const;

		protected:
		/** The number of the monster. */
		int _monsterNumber;
		/** The points associated to the monster. */
		int _points;
		/** The status of the monster. */
		Status _status;
		/** The last status of the monster. */
		Status _lastStatus;
		/** Whether the monster is in "elroy condition", meaning changes in the behaviour. 
			-1 means no elroy condition. */
		int _elroyCondition;

		// The counters and the switches
		static const int _COUNTERBLINKSITUATION = 0;
		static const int _SWITCHBLINKACTIVE = 0;
		static const int _SWITCHBLINKSITUATION = 1;
	};

	/** 
	  * The standard monster always pursuit a pacman. \n
	  *	So, the method setReferenceArtist controls this. \n
	  * The target position depends on the state, but for standard monsters all are calculated in the same way.
	  */
	class StandardMonster : public Monster
	{
		public:
		StandardMonster (int cId, int mN, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: Monster (cId, mN, f, d)
							{ }

		/** Left by default in standard monsters. */
		virtual QGAMES::Vector preferredDirectionAtStartingToMove () const override
							{ return (QGAMES::Vector (__BD -1, __BD 0, __BD 0)); }

		/** By default the referenced artist is pacman only */
		virtual void setReferenceArtists (const std::vector <Artist*>& r) override;

		/** The speed within the tunnel path has to b taken into account. */
		virtual void updatePositions () override;

		protected:
		/** The target position in a standard monster for most of the internal status except chasing is calculated in the same way. \
			Just only the target position when chasing will depend on the specific monster. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;

		virtual void setStateToStandLookingTo (const QGAMES::Vector& d) override;
		virtual void setStateToMoveTo (const QGAMES::Vector& d) override;

		// Implementation
		virtual void adaptSpeed () override;
	};

	/** Blinky, the red monster. Your shadow. */
	class Blinky final : public StandardMonster
	{
		public:
		static const int _NUMBER = 0;

		Blinky (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: StandardMonster (cId, _NUMBER, f, d)
							{ }

		virtual QGAMES::Entity* clone () const override
							{ return (new Blinky (id (), forms (), data ())); }

		virtual bool hasElroyPossibility () const override
							{ return (true); }
		virtual void setElroyCondition (int eC) override; 

		protected:
		/** When chasing Blinky's target position is the position where pacman is,
			in other cicunstances is the run away position. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;

		virtual QGAMES::Color trackingColor () const override
							{ return (QGAMES::Color (0xFF, 0x00, 0x00, 0x80)); /** Red. */ }
	};

	/** Pinky, the pink monster. Pure speed. */
	class Pinky final : public StandardMonster
	{
		public:
		static const int _NUMBER = 1;

		Pinky (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: StandardMonster (cId, _NUMBER, f, d)
							{ }

		virtual QGAMES::Entity* clone () const override
							{ return (new Pinky (id (), forms (), data ())); }

		protected:
		/** When chasing Pinky's target position is 4 positions ahead pacman's,
			otherwise if the run away position in the maze. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;

		virtual QGAMES::Color trackingColor () const override
							{ return (QGAMES::Color (0xFF, 0x8E, 0xB1, 0x80)); /** Pink. */ }
	};

	/** Inky, the blue/cyan monster. Bashful. */
	class Inky final : public StandardMonster
	{
		public:
		static const int _NUMBER = 2;

		Inky (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: StandardMonster (cId, _NUMBER, f, d)
							{ }

		virtual QGAMES::Entity* clone () const override
							{ return (new Inky (id (), forms (), data ())); }

		/** Inky has blinky and pacman as references. */
		virtual void setReferenceArtists (const std::vector <Artist*>& r) override;

		protected:
		/** When chasing Inky's target position is twice the distance between the 2 next positions of pacman and blinky's 
			In other circunstance is the run away position. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;

		virtual QGAMES::Color trackingColor () const override
							{ return (QGAMES::Color (0x00, 0xFF, 0xFF, 0x80)); /** Cyan. */ }
	};

	/** Clyde, the orange monster. Just the opposite: pokey*/
	class Clyde final : public StandardMonster
	{
		public:
		static const int _NUMBER = 3;

		Clyde (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: StandardMonster (cId, _NUMBER, f, d)
							{ }

		virtual QGAMES::Entity* clone () const override
							{ return (new Clyde (id (), forms (), data ())); }

		protected:
		/** When chasing, Clyde's target depends on the distance to Pacman:
			If it is 8 positions or greater, then goes for Pacman, otherwhise it is the run away position.
			When no chaing the targt position is the run away. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;

		virtual QGAMES::Color trackingColor () const override
							{ return (QGAMES::Color (0xFF, 0xA5, 0x00)); /** Orange. */ }
	};
}

#endif
  
// End of the file
/*@}*/