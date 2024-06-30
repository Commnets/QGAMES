/** \ingroup PACMAN */
/*@{*/

/**	
 *	@file	
 *	File: pcpacman.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: Definition of the main artist = pacman. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMAN_PACMAN__
#define __PACMAN_PACMAN__

#include <PacManLike/pcdefs.hpp>
#include <PacManLike/pcartists.hpp>

#include <advancedArcade/adinclude.hpp>

namespace PACMAN
{
	/** What the player controls! \n
		@see Artist and PacmanElement for more detail. */
	class PacMan : public Artist
	{
		public:
		enum class Status 
			{ _NOTDEFINED = 0, _STOPPED = 1, _MOVING = 2, _CHASING = 3 };

		PacMan (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

		virtual QGAMES::Entity* clone () const override
							{ return (new PacMan (id (), forms (), data ())); }

		virtual void toStand () override;
		virtual void toMove (const QGAMES::Vector& d) override;

		// Additional behaviour of pacman...
		/** Going for the enemies. */
		void toChase (bool c);
		bool isChasing () const
							{ return (_status == Status::_CHASING); }
		void toChaseDeferred (bool c);

		virtual bool isEnemy (const PacmanElement* elmnt) const override;

		virtual bool isAlive () const override
							{ return (_alive); }
		void setAlive (bool a)
							{ _alive = a; }
		virtual bool isStanding () const override
							{ return (_status == Status::_STOPPED); }
		virtual bool isMoving () const override
							{ return (_status == Status::_MOVING || _status == Status::_CHASING); }

		/** To know / change the number of points obteined by pacman. */
		int score () const
							{ return (_score); }
		void setScore (int s);

		/** To know / change what fruit have already been eaten by pacman. */
		const std::map <int, bool> fruitsEaten () const
							{ return (_fruitsEaten); }
		void setFruitsEaten (const std::map <int, bool>& fE);
		void addFruitEaten (int fE)
							{ _fruitsEaten [fE] = true; setFruitsEaten (_fruitsEaten); }

		/** To know the internal status. */
		Status status () const
							{ return (_status); }

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		virtual void whenCollisionWith (QGAMES::Entity* e) override;

		protected:
		/** In this case, the status is changed indirectly using e.g. toStand, toMove or toChase methods. */
		void setStatus (const Status& st);

		/** For Pacman the target position is always the limit of the maze in the current direction of the movement. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;

		virtual void whatToDoWhenStopStatusIsRequested (const QGAMES::Vector& d) override;
		virtual void whatToDoWhenMovementStatusIsRequested (const QGAMES::Vector& d) override;
		virtual void whatToDoOnCurrentPosition () override;
		virtual void setStateToStandLookingTo (const QGAMES::Vector& d) override;
		virtual void setStateToMoveTo (const QGAMES::Vector& d) override;

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
		/** To adapt the speed of the pacman when moving. \n
			This mthod is invokd usually from whatToDoWhenStopStatusIsRequested and whatToDoWhenMovementStatusIsRequested. */
		void adaptSpeed ();
		
		/** For Pacman is a little bit different that the standard one as the movements made by the player 
			in the joystick or keyboard have to be taken into account to recalculate the path in the maze. */
		virtual QGAMES::MazeModel::PathInMaze& recalculatePathInMaze 
			(const QGAMES::Vector& mD = QGAMES::Vector::_noPoint) override;

		virtual QGAMES::Color trackingColor () const override
							{ return (QGAMES::Color (0xFF, 0xFF, 0x00, 0x80));  /** Yellow. */}

		private:
		/** To indicate whether the pacman is alive. */
		bool _alive;
		/** The score of the pacman. It is get and put back into the game configuration. */
		int _score;
		/** The fruits eaten. */
		std::map <int, bool> _fruitsEaten;
		/** The status of the pacman. */
		Status _status;

		// Implementation
		/** The last status of pacman. */
		Status _lastStatus;
		/** To indicate whether the pacman has eaten or not something. \n
			It is used to adapt the speed in every movement e.g. */
		bool _hasEaten;
		/** The last score notified (in multiples) */
		int _lastMulScoreNotified;
	};
}

#endif
  
// End of the file
/*@}*/