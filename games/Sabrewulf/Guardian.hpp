/** \addtogroup SabreWulf */
/*@{*/

#ifndef __GUARDIAN_HPP__
#define __GUARDIAN_HPP__

#include "Defs.hpp"
#include <Arcade/arcadeinclude.hpp>

/** This class describes the guardian of the ext door. 
	It is really an object that can kill the player, but
	when sabreman is carrying all amulet pieces, then he moves aways from 
	the exit door for sabreman to exit and finishes game. */
class SabreWulfGuardian : public QGAMES::Artist
{
	public:
	// Constructors & Destructors
	SabreWulfGuardian (const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

	virtual Entity* clone () const
							{ return (new SabreWulfGuardian (_forms, _data)); }

	/** To move the guardian, or not */
	void setMoves (bool m)
							{ _moves = m; _posMoved = 0; }
	/** To know whether the guardian is or not moving. */
	bool isMoving () const
							{ return (_moves); }

	/** Guardian never move....automatically
		His position will be change as sabreman carries the four amulet pieces. */
	virtual bool canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
							{ return (false); }

	/** @see parent. */
	virtual QGAMES::Rectangle collisionZone () const;

	virtual void initialize ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_noPoint);

	/** @see parent. */
	virtual void whenCollisionWith (QGAMES::Entity* e);

	private:
	/** Defines whether the guardian moves or not. */
	bool _moves;

	// Implementation
	int _counter; // To count positions when he moves
	int _cAspect; // The current aspect of the guardian
	int _posMoved; // The number of pixels moved to the right
	int _maxPosMoved; // The maximum number of position to move
};

#endif
  
// End of the file
/*@}*/