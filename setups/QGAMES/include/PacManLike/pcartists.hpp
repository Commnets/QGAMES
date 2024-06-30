/** \ingroup PACMAN */
/*@{*/

/**	
 *	@file	
 *	File: pcartists.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: any PACMAN element/artist should inherit from this classes. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMAN_ARTISTS__
#define __PACMAN_ARTISTS__

#include <PacManLike/pcdefs.hpp>

#include <advancedArcade/adinclude.hpp>

namespace PACMAN
{
	class Map;

	/** Any element in this game to interact with. */
	class PacmanElement : public QGAMES::ArtistInATiledMap
	{
		public:
		PacmanElement () = delete;

		PacmanElement (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: QGAMES::ArtistInATiledMap (cId, f, d)
							{ setVisible (false); }

		PacmanElement (const Artist&) = delete;

		PacmanElement& operator = (const Artist&) = delete;

		/** To know which is the current position in the maze. 
			This method makes only sense when map has been set. */
		QGAMES::MazeModel::PositionInMaze currentMazePosition () const
							{ return (mapPositionToMazePosition (position ())); }
		/** To set the position of the element in based of the mazePosition. */
		void setPositionFromMazePosition (const QGAMES::MazeModel::PositionInMaze& p)
							{ setPosition (mazePositionToMapPosition (p) - 
								QGAMES::Vector (__BD (visualLength () >> 1), __BD (visualHeight () >> 1), __BD 0)); }
		/** To know which the next grid position will be (from the current position and following the orientation if any). */
		QGAMES::MazeModel::PositionInMaze nextXGridPosition (int x) const;
		/** To know which is the current ocuppied by the pacman element. */
		int currentMazeArea () const;

		/** To know whether another Pacman Element is or not my enemy. 
			It has to be defined later in detail as it depends on the type of pacman element. */
		virtual bool isEnemy (const PacmanElement* elmnt) const = 0;

		// To know which is current state of the pacmen element in terms of movement...
		/** To identify whether the artist is or not alive. */
		virtual bool isAlive () const = 0;
		/** To identify whether it is standing or not. */
		virtual bool isStanding () const = 0;
		/** To identify whether it is moving or not. */
		virtual bool isMoving () const = 0;

		protected:
		// Implementation
		// Used many times inside...
		/** Returns the PacMan map used. */
		inline const Map* pMap () const; // Not defined here to avoid recursive #include
		/** Same than above. */
		inline Map* pMap ();
		/** To convert a map position into a maze position. 
			It relies in the information of the map. */
		inline QGAMES::MazeModel::PositionInMaze mapPositionToMazePosition (const QGAMES::Position& p) const;
		/** The opposite. It gives back position in the map from maze position. \n
			The position returned is the center of the tile representing that maze position. */
		inline QGAMES::Position mazePositionToMapPosition (const QGAMES::MazeModel::PositionInMaze& p) const;
		/** Returns true when a map position is aligned exactly with an equivalent maze position. \n
			This method is very useful to determine when is time for the artist to change (e.g.) the path followed. */
		bool doesPositionMatchesTile (const QGAMES::Position& p) const;
	};

	/** 
	  * All artists in pacman moves and also in the same way. \n
	  *	The center of any artist always moves from the center of a tile to the center of the next one
	  *	following the direction of the movement in _path. \n 
	  *	So, the variable _direction should indicate that dirction of movement. \n
	  *	When the artist is stopped, the variable _direction should be 0, 
	  *	but the variable orientation should indicate to which place the element is looking to. \n
	  * Any artist has a set of other artists as reference for different purpouses: e.g. to follow it.
	  */
	class Artist : public PacmanElement
	{
		public:
		friend class MazeMovement;

		Artist (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

		// It is not needed to redefine the methods to save / recover the status of the artist 
		// As the game always starts with them in their initial positions

		/** The very basic actions in an artist 
			They hav to be implemented later. */
		virtual void toStand () = 0;
		virtual void toMove (const QGAMES::Vector& d) = 0;

		/** To freeze the movement. */
		void toFreeze (bool s)
							{ _frozen = s; }

		/** To get the last maze position of the artist. */
		const QGAMES::MazeModel::PositionInMaze lastMazePosition () const
							{ return (_lastMazePosition); }

		/** To load other artists taken as a reference for different purposes. */
		const std::vector <Artist*>& referenceArtists () const
							{ return (_referenceArtists); }
		virtual void setReferenceArtists (const std::vector <Artist*>& r)
							{ _referenceArtists = r; }

		/** When it is needed to change the direction of the movement, 
			this method should be invoked. The artist will change when and if possible. */
		void changeDirectionWhenPossibleTo (const QGAMES::Vector& d);

		/** To know what is the next expected position of the artist in the map. \n
			The parameter indicates how many steps in advance you want to know. \n
			The standard implementation of PacMan only calculats 2 steps in advance no more, 
			so any query beyond that number will return _noPosition. */
		QGAMES::MazeModel::PositionInMaze nextMazePosition (int n = 0) const
							{ return (_pathInMaze.empty () 
								? currentMazePosition () 
								: ((n < (int) _pathInMaze.size ()) ? _pathInMaze [n] : QGAMES::MazeModel::_noPosition)); }

		/** A little bit reduced from the original one (75%). */
		virtual QGAMES::Rectangle collisionZone () const
							{ return (PacmanElement::collisionZone ().scale (__BD 0.75)); }

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void drawOn (QGAMES::Screen* scr, const QGAMES::Position& p = QGAMES::Position::_noPoint) override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		protected:
		// Method to be overload per type of artist
		/** What is the target maze position of the artist. 
			The target will depend on the artists itself, and even (in the case of monsters e.g.) on the status of it!. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const = 0;

		// To control the aspect and what to do when stopping and moving
		/** What to do when the end of the planed path is reached. \n
			The parameter is the orientation requested. It should be never _noPosition. */
		virtual void whatToDoWhenStopStatusIsRequested (const QGAMES::Vector&) = 0;
		/** What to do when it is the beginning of a new requested movement. \n
			The parameter is the orientation requested. It should be never _noPosition. */
		virtual void whatToDoWhenMovementStatusIsRequested (const QGAMES::Vector&) = 0;
		/** What to do when artists is on a specific position in the maze.
			What it is beneath the artist could modify actually the behaviour: e.g. Pacman eaten a power ball! \n */
		virtual void whatToDoOnCurrentPosition () = 0;
		/** To change the state of the artist to stand looking to a direction. */
		virtual void setStateToStandLookingTo (const QGAMES::Vector&) = 0;
		/** To change the state of the artist to move looking to a direction. */
		virtual void setStateToMoveTo (const QGAMES::Vector&) = 0;

		/** Define a buoy to stop within the inEveryLoop method. */
		class ToStopBuoy final : public QGAMES::Buoy
		{
			public:
			ToStopBuoy ()
				: QGAMES::Buoy (__PACMAN_TOSTOPBUOYID__, (QGAMES::bdata) 0)
							{ /** Nothing else to do. */ }

			virtual void* treatFor (QGAMES::Element* e) override;
		};

		/** The method to stop deferred. */
		void toStopDeferred ();

		// Implementation
		// Used many times inside...
		/** To recalculate the path to follow (usually 2 steps more) from the current position. \n
			It doesn't matter whether it is aligned with the maze position, but usually it is invoked in that situation. 
			A mandatory direction can be passed. It will be taken into account if possible (by default it is not). */
		virtual QGAMES::MazeModel::PathInMaze& recalculatePathInMaze (const QGAMES::Vector& mD = QGAMES::Vector::_noPoint);
		/** To know whether the current situation in the path implies entering or not a tunnel. \n
			It is useful to know e.g whether to move (leap) the position of the artist. */
		bool isArtistEnteringATunnel () const;
		/** To know whether the reference artists of the this on have or not change their positions. \n
			In some cases, when this happens, the artist could vary it path. */
		bool hasReferenceArtistsPositionChanged () const; 

		/** To change the color when tracking. */
		virtual QGAMES::Color trackingColor () const
							{ return (__QGAMES_REDCOLOR__); }

		protected:
		QGAMES::MazeModel::PositionInMaze _lastMazePosition;
		std::vector <QGAMES::MazeModel::PositionInMaze> _pathInMaze;
		std::vector <Artist*> _referenceArtists;

		// Implementation
		QGAMES::Vector _nextDirectionWhenPossible;
		std::map <int, QGAMES::MazeModel::PositionInMaze> _referenceArtistLastMazePositions;
		bool _frozen;
	};

	/** The things than can be cought in the maze. \n
		@see PacmanElement documentation for more details. */
	class Thing : public Artist
	{
		public:
		// The different status a fruit Thing in the maze be in...
		enum class Status 
			{ _NOTSHOWN = 0, _SHOWN = 1, _SHOWNANDMOVING = 2, _EATEN = 3 /** Or caught. */ };

		Thing (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: Artist (cId, f, d),
			  _type (0), _points (200),
			  _status (Status::_NOTSHOWN)
							{ setVisible (false); }

		virtual bool isEnemy (const PacmanElement* elmnt) const override
							{ return (false); } // By default any thing wouldn't be an enemy ever...
		virtual bool isAlive () const override // It is alive while it is shown and not eaten!
							{ return (_status != Status::_NOTSHOWN && _status != Status::_EATEN); } 
		virtual bool isStanding () const override
							{ return (_status == Status::_SHOWN || _status == Status::_EATEN); }
		virtual bool isMoving () const override
							{ return (_status == Status::_SHOWNANDMOVING); }

		/** To know and change the type of thing to be caught. 
			Whn the type is set a new internal state should be also set, and it depends on many thing even if it is moving e.g. */
		int type () const
							{ return (_type); }
		void setType (int t)
							{ _type = t; }

		/** To set and know the number of points assigned to this element.
			Those points should be given to the one eatening/catching it. */
		int points () const
							{ return (_points); }
		void setPoints (int p)
							{ _points = p; }

		void toDisappear ();
		virtual void toStand () override;
		virtual void toMove (const QGAMES::Vector& d) override;
		void toBeEaten ();

		// To know the status...
		Status status () const
							{ return (_status); }

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void drawOn (QGAMES::Screen* scr, const QGAMES::Position& p = QGAMES::Position::_noPoint) override;

		virtual void whenCollisionWith (QGAMES::Entity* e) override;

		protected:
		/** To set the status. */
		void setStatus (const Status& st);

		/** To know the last status. */
		Status lastStatus () const
							{ return (_lastStatus); }

		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;
		virtual void whatToDoWhenStopStatusIsRequested (const QGAMES::Vector& d) override;
		virtual void whatToDoWhenMovementStatusIsRequested (const QGAMES::Vector& d) override;
		virtual void whatToDoOnCurrentPosition () override
							{ /** Nothing. */ }

		/** The things when moving, never go back. */
		virtual QGAMES::MazeModel::PathInMaze& recalculatePathInMaze 
			(const QGAMES::Vector& mD = QGAMES::Vector::_noPoint) override;

		virtual QGAMES::Color trackingColor () const override
							{ return (__QGAMES_WHITECOLOR__); }

		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters () override
							{ return (new Counters); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches)
		virtual QGAMES::OnOffSwitches* createOnOffSwitches () override
							{ return (new OnOffSwitches); }

		protected:
		/** The type of element. 
			When it is set, what to do will also depend on the status. */
		int _type;
		/** How many point are given to the eater. */
		int _points;
		/** The status of the thing. */
		Status _status;
		/** The last status of the thing. */
		Status _lastStatus;

		// The counters and the switches
		static const int _COUNTERPOINTSVISIBLE = 0;
		static const int _SWITCHPOINTSVISIBLE = 0;
	};
}

#include <PacManLike/pcpacman.hpp>
#include <PacManLike/pcfruit.hpp>
#include <PacManLike/pcmonsters.hpp>

#endif
  
// End of the file
/*@}*/