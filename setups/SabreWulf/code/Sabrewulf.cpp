#include "Sabrewulf.hpp"
#include "OriginalData.hpp"
#include "Defs.hpp"

SabreWulfMaze SabreWulfMaze::_data = SabreWulfMaze ();

// ---
SabreWulfMaze::SabreWulfMaze ()
	: _typeOfMazePlaces (),
	  _mazePlaces (),
	  _grassPlace (),
	  _objectCoordinates (),
	  _objectTypes (),
	  _orchildCoordinates (),
	  _orchildTypes (),
	  _nastieTypes ()
{
	// Nothing else to do
	// The object has to be initialized
}

// ---
void SabreWulfMaze::initialize ()
{
	initializeMaze ();
	initializeObjects ();
	initializeOrchild ();
	initializeMonsters ();
	initializeAmulet ();
}

// ---
void SabreWulfMaze::initializeMaze ()
{
	// First of all inits the different types of maze places...
	// The number of types are __NUMBEROFMAZEPLACETYPES__ (48)
	_typeOfMazePlaces.clear ();
	_typeOfMazePlaces.resize (__NUMBEROFMAZEPLACETYPES__);
	for (int i = 0; i < __NUMBEROFMAZEPLACETYPES__; i++)
	{
		TypeOfMazePlace tMP;
		tMP._type = i; // The type is sequential...

		// The type of map place, is made of different map forms...
		// The number of forms depends on the type of map place
		// The definition of a map place finishes when the word read is equal to 0x0000
		// The first word read is the type of the form
		// The second word is the position x (byte 1) and the position y (byte 2)
		MazeForms mFrms;
		int j = (OriginalData::_mazeDefinitionData [i] - OriginalData::_mazeCompositionAddress) >> 1;
		while (OriginalData::_mapCompositionData [j] != 0x0000)
		{
			Coordinate cd (OriginalData::_mapCompositionData [j + 1] % 0x100,
				OriginalData::_mapCompositionData [j + 1] / 0x100);
			cd.scale (); // The original position is scaled... 

			MazeForm mF;
			mF._form = OriginalData::_mapCompositionData [j]; 
			// The number of the form is just the same than the original one...
			// Take this into account when read the forms file!
			mF._position = cd;

			mFrms.push_back (mF);

			// Every form of the maze is two words long always...
			j += 2; 
		}

		// Adds the forms the type is made up of...
		tMP._forms = mFrms;
		// Adds the type to the list of them...
		_typeOfMazePlaces [i] = tMP;
	}

	// Now the maze itself...
	// The map is a list of __NUMBEROFMAZEPLACES__ (256) rooms
	// Every room belongs to a type. This type is always
	// One of the elements set up in the previous paragraph!!
	_mazePlaces.clear ();
	_mazePlaces.resize (__NUMBEROFMAZEPLACES__);
	for (int k = 0; k < __NUMBEROFMAZEPLACES__; k ++)
		_mazePlaces [k] = OriginalData::_mazeLayoutTypeData [k];

	// Special places...
	// Now it is time to build the original selection room
	int k = 0;
	_grassPlace._type = __NUMBEROFMAZEPLACETYPES__; // At the end...
	while (OriginalData::_grassBorderCompositionData [k] != 0x0000)
	{
		Coordinate cd (OriginalData::_grassBorderCompositionData [k + 1] % 0x100,
			OriginalData::_grassBorderCompositionData [k + 1] / 0x100);
		cd.scale (); // The original position is scaled... 

		MazeForm mF;
		mF._form = OriginalData::_grassBorderCompositionData [k]; 
		mF._position = cd;
		_grassPlace._forms.push_back (mF);
		k += 2; 
	}
}

// ---
void SabreWulfMaze::initializeObjects ()
{
	_objectCoordinates.clear ();
	_objectCoordinates.resize (__NUMBEROFMAZEPLACETYPES__);
	for (int i = 0; i < __NUMBEROFMAZEPLACETYPES__; i++)
		_objectCoordinates [i].resize (__NUMBEROFRANDPOSITIONSPEROBJ__); 
	for (int i = 0; i < __NUMBEROFMAZEPLACETYPES__; i++)
	{
		for (int j = 0; j < __NUMBEROFRANDPOSITIONSPEROBJ__; j++)
		{
			Coordinate cd (OriginalData::_objectPositionsData [i][j << 1],
				OriginalData::_objectPositionsData [i][(j << 1) + 1]);
			cd.scale ();
			_objectCoordinates [i][j] = cd;
		}
	}

	// The type of the object...
	_objectTypes.clear ();
	_objectTypes.resize (__NUMBERPOSSIBILITIESOBJECT__);
	for (int i = 0; i < __NUMBERPOSSIBILITIESOBJECT__; i++)
		_objectTypes [i] = i; // It is a sequential number....
}

// ---
void SabreWulfMaze::initializeOrchild ()
{
	// Where the orchild is...
	_orchildCoordinates.clear ();
	_orchildCoordinates.resize (__NUMBEROFMAZEPLACETYPES__);
	for (int i = 0; i < __NUMBEROFMAZEPLACETYPES__; i++)
	{
		Coordinate cd (OriginalData::_orchildPositionsData [i][0],
			OriginalData::_orchildPositionsData [i][1]);
		cd.scale ();
		_orchildCoordinates [i] = cd;
	}

	// The type of the orchild...
	_orchildTypes.clear ();
	_orchildTypes.resize (__NUMBERPOSSIBILITIESORCHILD__);
	for (int i = 0; i < __NUMBERPOSSIBILITIESORCHILD__; i++)
	{
		_orchildTypes [i] = OriginalData::_orchildColours [i] - __BASEORCHILDCOLOR__; 
		if (_orchildTypes [i] >= 2) // The 0x44 doesn't exist...
			_orchildTypes [i]--;
	}
}

// ---
void SabreWulfMaze::initializeMonsters ()
{
	// The nasties...
	_nastieTypes.clear ();
	_nastieTypes.resize (__NUMBERPOSIBILITIESNATIE__);
	for (int i = 0; i < __NUMBERPOSIBILITIESNATIE__; i++)
		_nastieTypes [i] = (OriginalData::_nastySpriteTypes [i] - __BASENATIECOLOR__) >> 2;

	// The inmortals...
	// From the original data the first element is forgotten because it is sabreman
	// ...and the last one is also forgotten because it is the guardian
	// Both are elements considered in the game apart!
	int cI = 0;
	_inmortalTypes.clear ();
	_inmortalTypes.resize (__NUMBERPOSIBILITIESINMORTALS__);
	for (int i = 0; i < (__NUMBERPOSIBILITIESINMORTALS__ + 3); i++)
	{
		if (OriginalData::_defaultInmortalsData [i][0] != 0x10 &&
			OriginalData::_defaultInmortalsData [i][0] != 0x00 &&
			OriginalData::_defaultInmortalsData [i][0] != 0x94) 
		// One real inmortal...There should be three that are not real inmortals
		{
			SabreWulfMaze::SpriteStructure s;
			s._type = OriginalData::_defaultInmortalsData [i][0];
			s._location = OriginalData::_defaultInmortalsData [i][1];
			Coordinate cd (OriginalData::_defaultInmortalsData [i][3],
				OriginalData::_defaultInmortalsData [i][4]);
			cd.scale ();
			s._position = cd;
			_inmortalTypes [cI++] = s;
		}
	}
}

// ---
void SabreWulfMaze::initializeAmulet ()
{
	_amuletMazePlaces.clear ();
	_amuletMazePlaces.resize (__NUMBEROFAMULTERRANDPOS__);
	for (int i = 0; i < __NUMBEROFAMULTERRANDPOS__; i++)
		_amuletMazePlaces [i].resize (__NUMBEROFAMULETPIECES__);
	for (int i = 0; i < __NUMBEROFAMULTERRANDPOS__; i++)
	{
		for (int j = 0; j < __NUMBEROFAMULETPIECES__; j++)
			_amuletMazePlaces [i][j] = OriginalData::_amuletStartPositionData [i][j];
		std::sort (_amuletMazePlaces [i].begin (), 
			_amuletMazePlaces [i].begin () + __NUMBEROFAMULETPIECES__); // in order...
	}
}

// ---
void SabreWulfMaze::Coordinate::scale ()
{
	_x = (int) (_x * __XSCALE__) + __MAPORIGINX__;
	_y = (int) (_y * __YSCALE__) + __MAPORIGINY__;
}

