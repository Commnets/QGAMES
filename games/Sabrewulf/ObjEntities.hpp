/** \addtogroup SabreWulf */
/*@{*/

#ifndef __OBJENTITIES_HPP__
#define __OBJENTITIES_HPP__

#include "Worlds.hpp"
#include "Defs.hpp"
#include <Arcade/arcadeinclude.hpp>

/** This class describes the objects.
	The orchild appear in every room, but it status
	is manage at world level. */
class SabreWulfObject : public QGAMES::Artist
{
	public:
	enum Type { 
		_CHEST, _RING, _BALL, _HAT, 
		_DIAMOND, _LIFE, _DOLLARS, _SYRINGE,
		_AMULET };

	// Constructors & Destructores
	SabreWulfObject (int id, const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

	virtual Entity* clone () const
							{ return (new SabreWulfObject (_id, _forms, _data)); }

	/** To know the type of object. */
	Type type () const;
	/** To know which piece of the amulet th eobject is
		just in case the object is an amulet!!.
		Otherwise the number returned will be __MININT__ .*/
	int amuletPieceNumber () const;

	/** Objects never move. */
	virtual bool canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
							{ return (false); }

	/** @see parent. */
	virtual QGAMES::Rectangle collisionZone () const;

	virtual void initialize ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_noPoint);
	/** To init the object from a object counter. */
	void actualizeFrom (SabreWulfWorld::ObjectCounter* ct);

	/** @see parent. */
	virtual void whenCollisionWith (QGAMES::Entity* e);

	private:
	void setObjectAspect ();

	private:
	/** The counter the object is representing. */
	SabreWulfWorld::ObjectCounter* _counter;
};

#endif
  
// End of the file
/*@}*/
