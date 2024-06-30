/** \addtogroup SabreWulf */
/*@{*/

#ifndef __SABREWULF_HPP__
#define __SABREWULF_HPP__

#include <Arcade/arcadeinclude.hpp>

/** Sabrewulf original data is converted into
	information that can be managed by the application.
	This class is used to keep that information
	translated. */
class SabreWulfMaze
{
	public:
	static SabreWulfMaze _data;

	SabreWulfMaze ();

	void initialize ();

	public:
	/** Structure to keep a position in the map. */
	struct Coordinate
	{
		Coordinate ()
			: _x (0), _y (0)
							{ }
		Coordinate (int x, int y)
			: _x (x), _y (y)
							{ }

		/** To convert a coordinate
			from the original position to the position in the game.
			This is done scaling the position. */
		void scale ();

		int _x, _y;
	};

	/** Typedef to simplify a list of coordinates. */
	typedef std::vector <Coordinate> Coordinates;
	/** A vector of a vector of coordinates. */
	typedef std::vector <Coordinates> VectorOfCoordinates;

	/** Structure to keep one of the forms any part of the map is made up of. */
	struct MazeForm
	{
		MazeForm ()
			: _form (0), _position ()
							{ }

		int _form;
		Coordinate _position;
	};

	/** Typedef to reflect alist of parts of a map in the maze. */
	typedef std::vector <MazeForm> MazeForms;

	/** Structure to identify a type of map place.
		Every part of the map belongs to a type. 
		There are 48 types of map places in the game.
		Each of 256 maze places belongs to a type. */
	struct TypeOfMazePlace
	{
		TypeOfMazePlace ()
			: _forms ()
							{ }

		int _type; // The code...from 0 to 47 in sabrewulf
		MazeForms _forms;
	};

	/** A map with all the types of maze places. */
	typedef std::vector <TypeOfMazePlace> TypeOfMazePlaces;
	/** A vector with the type any part of the map. */
	typedef std::vector <int> MazePlaces; // Points to a type of map place... 
										  // The position in the vector is the room, 
										  // ...and the content is the type...
	/** A vector of a set of maze places. */
	typedef std::vector <MazePlaces> VectorOfMazePlaces;

	/** An structure to describe the information defining any entity the player 
		can interact with in the screen.
		Some of the entities are inmortal, it is they can not die,
		Others, on the other hand, can die. Others can be picked up. */
	struct SpriteStructure
	{
		SpriteStructure ()
							{ }

		int _type; 
		// The type of the sprite...
		// The inmortals are:
		// 0x10	Sabreman, It is not taking into account!
		// 0x30 Rhino running to the left,
		// 0x32 Rhino running to the right,
		// 0x3C	Spearman,
		// 0x00 Fire,
		// 0x50 Wolf,
		// 0xA0 Hippo,
		// 0x94 Guardian... It is not taking intop account
		int _location;
		Coordinate _position;
	};

	// The maze...
	/** There will be 48 type of maze places. */
	TypeOfMazePlaces _typeOfMazePlaces;
	/** There will be 256 maze places. */
	MazePlaces _mazePlaces;
	/** The initial selection window is rounded with grass.
		This can be represented with another type of maze. */
	TypeOfMazePlace _grassPlace;

	// The coordinates
	/** The potential positions of any object in the maze. 
		@see OriginalData to know the meaning of any potential position per room. */
	VectorOfCoordinates _objectCoordinates;
	/** The different possibilities to choose an object. */
	std::vector <int> _objectTypes;
	/** The potential positions of the orchilds in the different rooms or maze places. */
	Coordinates _orchildCoordinates;
	/** The different possibilities to choose an orchilds. */
	std::vector <int> _orchildTypes;
	/** The different possibilities to choose a naste sprite. */
	std::vector <int> _nastieTypes;
	/** The different possibilities for inmortals. */
	std::vector <SpriteStructure> _inmortalTypes;

	// The amulets
	/** The potential places where the amulet can appear. 
		It is a vector of eight possibilities. Every possibility includes
		the number of the four maze places where a piece of the amulet will be.
		The possibility has to be randomically choosen at the beginning of the game. */
	VectorOfMazePlaces _amuletMazePlaces;

	private:
	// To init every part of the maze...
	/** To init the maze. */
	void initializeMaze ();
	/** To init where the objects are. */
	void initializeObjects ();
	/** To init where the orchild are. */
	void initializeOrchild ();
	/** To init where the main monsters start. */
	void initializeMonsters ();
	/** To init where the different parts of the amulet are. */
	void initializeAmulet ();
};

#endif
  
// End of the file
/*@}*/