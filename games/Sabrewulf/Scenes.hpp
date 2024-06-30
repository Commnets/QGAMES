/** \addtogroup SabreWulf */
/*@{*/

#ifndef __SCENES_HPP__
#define __SCENES_HPP__

#include <Arcade/arcadeinclude.hpp>

/** This class represents a sabrewulf scene.
	This class is where the entities are moving. */
class SabreWulfScene : public QGAMES::Scene
{
	public:
	struct Path
	{
		int _mazePlaceNumber;
		int _nextMazePlaceNumber;
		std::vector <QGAMES::Position> _positions;
	};

	// Constructors % Destructors
	SabreWulfScene (const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn,
		const QGAMES::SceneProperties& p = QGAMES::SceneProperties (),
		const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ());

	/** To get the number of maze place where the scene is now playing. */
	int mazePlaceNumber () const
							{ return (_mazePlaceNumber); }
	/** To change the number of the maze place where the game is taking place. */
	void setMazePlaceNumber (int r);
	/** To look for the first nastie not already moving in the scene. 
		If there is none the number returned is -1. */
	int firstNastieFree ();
	/** To get the number of nasties on the current maze place. */
	int numberOfNasties ()
							{ return (numberOfNasties (_mazePlaceNumber)); }
	/** The same but in a specific place. */
	int numberOfNasties (int nP);
	/** To get a position a random position of the scene. */
	QGAMES::Position randomFreePosition (int mP);
	/** To know the max position of a right location, taking as 
		parameter a location and an orientation. */
	QGAMES::Position limitPosition (int mn, const QGAMES::Position& pos, 
		const QGAMES::Vector& or);

	// Going through the maze!
	/** To determinate a random path just for a couple of maze places. 
		The paremeters are the initial maze place, the initial position, 
		the number of mazeplaces length the random path is, 
		and a potential target maze place number. -1 means random absolutly! 
		As the method is recursive, it can be invoked from inside, and 
		the number of the maze to avoid is other paremeter. */
	std::map <int, Path> randomPathFrom (int mZ, 
		const QGAMES::Position& p, int nP, int mE = -1, int tMP = -1);
	/** To get the opposite path of another one. 
		The position to leave the room where it is, added. */
	std::map <int, Path> reversePathFrom (int mZ, const QGAMES::Position& pos, int nP,
		const std::vector <QGAMES::Position>& pth, int cP);
	
	virtual void initialize ();
	virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_noPoint);
	virtual void processEvent (const QGAMES::Event& e);
	virtual void processEvent (const QGAMES::Event& e, QGAMES::Notifier* n);

	// ---
	// To activate or desactivate to draw the limits of the maze
	// This is an option just valid in debug mode...
	#ifndef NDEBUG
	void onOffDrawLimits ();
	#endif
	// ---

	private:
	// Implementation
	/** An structure to identify the different locations
		within the maze place, and where the entities can move in. */
	struct Location
	{
		Location ()
			: _id (-1), _zone (QGAMES::Rectangle::_noRectangle), 
			  _type (Location::_ISOLATE), _exitType (Location::_NONE)
							{ for (int i = 0; i < 4; _conexions [i++] = -1); }

		int _id;
		QGAMES::Rectangle _zone;
		enum Type { _ISOLATE = 0, _DECIDE = 1, _EXIT = 2 } _type;
		enum ExitType { _NONE = 0, _LEFTE = 1, _UPE = 2, _RIGHTE = 3, _DOWNE = 4 } _exitType;
		int _conexions [4];

		/** The center in pixels. */
		QGAMES::Position center () const;
		/** The exit position is calculated as the middle point in the line
			linking the two point that are pointing in the exit direction. */
		QGAMES::Position exitPosition () const;
		/** To get the center of the part of the zone, closest to
			a position received as parameter. */
		QGAMES::Position thirdPartClosestTo (const QGAMES::Position& p);
		/** To get the number of the next room, just when the
			location is an exit location. */
		int nextMazePlace (int mZ);
	}; 

	// To simplify the definition of a map of locations...
	typedef std::map <int, Location, std::less <int>> Locations;

	// Managing locations...
	/** To calculate the locations for the current maze place. */
	const Locations& calculateLocations (int mP);
	/** To determinate the id of the locations that include the
		point received as parameter. */
	std::vector <int> locationIdsFor (int mP, const QGAMES::Position& p);
	/** To get the first exit location in a maze place in a direction closest to a point. 
		If there is no that type of exit location in that maze place number,
		an empty location is returned. */
	Location firstExitLocation (int mP, Location::ExitType eT, const QGAMES::Position& p);
	/** For one exit location in a maze place, this method determinates
		the connected location in the next maze place.
		If there is no location conneceted the method retruns an empty location. 
		As there could be more than 1 exit type of a type, a point
		is received to determinate in that cases the exit location closest to that point. */
	Location connectedLocationFor (int mP, Location::ExitType eT, const QGAMES::Position& p); 
	/** Very internal method.
		It return 0x0000 for no joins, 0b1000 left, 0b0100 up, 0b0010 right, 0b0001 down
		and the combinations among them when several joins are detected. 
		This methos is used by calculateLocations method. */
	int joinPositionBetween (const Location& l1, const Location& l2) const;

	// Going through the maze!. Private methods used just in the scene...
	/** The path from one position to other within the same maze place. 
		Returns the list of points to go from one point to other. */
	std::vector <QGAMES::Position> pathFrom (int mZ, const QGAMES::Position& p1, 
		const QGAMES::Position& p2, std::vector <int> lE = std::vector <int> ());
	/** To determinate the path from position in a maze place number
		going to another maze place. The result could be an empty set. */
	std::vector <QGAMES::Position> conexionBetween (int mZ1, const QGAMES::Position& p, int mZ2);
	/** To determinate wheter a maze place has or not connection with other. */
	bool isThereConexionBetween (int mZ1, const QGAMES::Position& p, int mZ2)
							{ return (!conexionBetween (mZ1, p, mZ2).empty ()); }
	/** To determinate a ramdom path within a maze place. */
	Path localPathFrom (int mZ, const QGAMES::Position& p);

	private:
	/** The number of the maze place the player is now in. */
	int _mazePlaceNumber;
	/** The different location in the map place. */
	Locations _locations;

	// Implementation
	/** Once a list of locations has been calculated,
		is stored in this vector to avoid futher calculations and
		then speed up the game. */
	std::vector <Locations> _allLocations;
	/** A list of numbers to add to change the maze place. */
	std::vector <int> _add;
	std::vector <SabreWulfScene::Location::ExitType> _addTp;
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