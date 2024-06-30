/** \addtogroup SabreWulf */
/*@{*/

#ifndef __MAPS_HPP__
#define __MAPS_HPP__

#include "Defs.hpp"
#include <Arcade/arcadeinclude.hpp>

class SabreWulfMazePlace : public QGAMES::ObjectMap
{
	public:
	SabreWulfMazePlace (int c, const QGAMES::Layers& l, int w, int h,
		const QGAMES::MapProperties& p = QGAMES::MapProperties ());

	// To manage the aspect of the map...
	/** To know the the maze place is being representing. */
	int mazePlaceNumber () const
							{ return (_mazePlaceNumber); }
	/** To set the aspect of the map, depending on the place in the maze */
	void setMazePlaceNumber (int mP);

	// The spaces...
	/** To return the limits of the maze for an specific maze place. */
	const std::vector <QGAMES::Rectangle>& limits (int mZ)
						{ return (calculateLimitsFor (mZ)); }
	/** To return the spaces where the entities can move through for an
		specific maze place. */
	const std::vector <QGAMES::Rectangle>& spaces (int mZ)
						{ return (calculateEmptySpacesFor (mZ)); }

	/** To know whether a position is or not in the middle
		of the vegetation, for the current maze place number. */
	bool isPositionFree (const QGAMES::Position& pos) const;
	/** Same but passing a maze number as parameter. */
	bool isPositionFree (const QGAMES::Position& pos, int mN);
	/** To know whether the position is or not in the limits
		of the map. It is used to determinate whether the 
		sabreman has to pass or not to other part. Based on the current maze place number. */
	bool isOutOfLimits (const QGAMES::Position& pos) const;
	/** To know ehether the position is or not in the exit 
		zone of the exit maze place. If the current maze place
		is not the exit one, then this method will return also false. */
	bool isInExitZone (const QGAMES::Position& pos) const;

	/** In this type of map there are no relevant layers. */
	virtual QGAMES::Layers relevantLayers ();

	virtual void initialize ();
	virtual void drawOn (QGAMES::Screen* s, 
		const QGAMES::Position& p = QGAMES::Position::_noPoint);

	/** The entities are ordered from the beginning.
		It is not necessary to order at all. */
	virtual QGAMES::Entities orderEntitiesToDraw (const QGAMES::Entities& e)
							{ return (e); }

	// ---
	// To activate or desactivate to draw the limits of the maze
	// This is an option just valid in debug mode...
	#ifndef NDEBUG
	void onOffDrawLimits ()
							{ _onOffDrawLimits = !_onOffDrawLimits; }
	#endif
	// ---

	private:
	// Implementation
	/** To calculate the zones where the vegetation is for a 
		concrete maze place number. */
	const std::vector <QGAMES::Rectangle>& calculateLimitsFor (int mP);
	/** To calculate the empty spaces of the map, where sabreman can move through,
		for a concrete maze place number. */
	const std::vector <QGAMES::Rectangle>& calculateEmptySpacesFor (int mP);

	private:
	/** The number of the maze place the player is now in. */
	int _mazePlaceNumber; // Initially it is __FIRSTMAZEPLACE__

	// Implementation
	/** The limits of the maze. */
	std::vector <QGAMES::Rectangle> _limits;
	/** The rectangles where the player can move in. */
	std::vector <QGAMES::Rectangle> _emptySpaces;
	/** A buffer with the limits for all the maze places. 
		Once a limit is calculated is stored into this vector, to 
		speed up next time the map is in the same form. */
	std::vector <std::vector <QGAMES::Rectangle>> _allLimits;
	/** A buffer with the empty spaces for all the maze places. 
		Once an empty space is calculated is stored here to
		speed up next time the map has the same form. */
	std::vector <std::vector <QGAMES::Rectangle>> _allEmptySpaces;
	/** The exit in the map when the maze place is the exiting one. */
	QGAMES::Rectangle _exitingZone;

	// ---
	// The variable to control when to draw or not the limits of the maze
	// It makes only sense in debug mode...
	#ifndef NDEBUG
	bool _onOffDrawLimits;
	#endif
	// ---
};

#endif
  
// End of the file
/*@}*/