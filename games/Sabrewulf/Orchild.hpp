/** \addtogroup SabreWulf */
/*@{*/

#ifndef __OENTITIES_HPP__
#define __OENTITIES_HPP__

#include "Worlds.hpp"
#include "Defs.hpp"
#include <Arcade/arcadeinclude.hpp>

/** This class describes the orchild.
	The orchild appear in every room, but it status
	is manage at world level. */
class Orchild : public QGAMES::Artist
{
	public:
	enum Type { 
		_RED = 0, _PINK, _BLUE, _YELLOW, _WHITE };

	// Constructors & Destructores
	Orchild (const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

	virtual Entity* clone () const
							{ return (new Orchild (_forms, _data)); }

	/** To know whether the orchild is or not already big.
		This is used to know if the orchild i.e. affects or not to the player. */
	bool isBig () const;
	/** To know the type of orchild. */
	Type type () const;

	/** Orchilds never move. */
	virtual bool canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
							{ return (false); }

	/** @see parent. */
	virtual QGAMES::Rectangle collisionZone () const;

	virtual void initialize ();
	virtual void updatePositions ();
	/** To init the orchild from a orchild counter. */
	void actualizeFrom (SabreWulfWorld::OrchildCounter* ct);

	/** @see parent. */
	virtual void whenCollisionWith (QGAMES::Entity* e);

	private:
	void setOrchildAspect ();

	private:
	/** The counter the orchild is representing. */
	SabreWulfWorld::OrchildCounter* _counter;
};

#endif
  
// End of the file
/*@}*/
