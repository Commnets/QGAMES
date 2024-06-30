/** \addtogroup SabreWulf */
/*@{*/

#ifndef __WORLDS_HPP__
#define __WORLDS_HPP__

#include "Events.hpp"
#include "Defs.hpp"
#include <Arcade/arcadeinclude.hpp>

class Sabreman;
class Orchild;
class SabreWulfGuardian;

/** This class describes the world where sabrewulf happens. */
class SabreWulfWorld : public QGAMES::World
{
	public:
	/** An structure to know the status of every orchild in
		the game. */
	struct OrchildCounter
	{
		OrchildCounter ()
			: _mazePlace (0), 
			 _position (QGAMES::Position::_cero),
			 _percentaje (0),
			 _secondsToGrow (0), _secondsBig (0),
			 _color (0),
			 _growing (false),
			 _growingDirection (1),
			 _counter (0)
							{ }

		/** Maze place where the orchild is. */
		int _mazePlace; 
		/** Screen position where the orchild grows. */
		QGAMES::Position _position; 
		/** What is the percentaje of growth th orchild has (from 0 to 100) */
		int _percentaje; 
		/** Number of seconds for the orchild to grow. */
		int _secondsToGrow; 
		/** Number of seconds. */
		int _secondsBig; 
		/** The type of the orchild (determinated by its color) */
		int _color;

		// Implementation
		bool _growing;
		int _growingDirection;
		int _counter;
	};

	/** To manage a vector of orchild counters. */
	typedef std::vector <OrchildCounter*> OrchildCounters;

	/** An structure to know the status of every object in
		the game. */
	struct ObjectCounter
	{
		ObjectCounter ()
			: _mazePlace (0), 
			 _position (QGAMES::Position::_cero),
			 _type (0),
			 _caught (false)
							{ }

		/** Maze place where the object is. */
		int _mazePlace; 
		/** Screen position where the object is. */
		QGAMES::Position _position; 
		/** The type of the object (almost determinated by its color) */
		int _type;

		// Implementation
		bool _caught; // Has been already caught or not... (is it visble or not?)
	};

	/** To manage a vector of object counters. */
	typedef std::vector <ObjectCounter*> ObjectCounters;
	/** To manage a vector of vector of object counters. 
		This will be used per maze place. */
	typedef std::vector <ObjectCounters> VectorOfObjectCounters;

	// Constructors & Destructors
	SabreWulfWorld (const QGAMES::Scenes& s, const QGAMES::WorldProperties& p);
	/** Clear the things attributes. */
	~SabreWulfWorld ()
							{ clearThings (); }

	/** To know the maze place where the player is in. */
	int mazePlaceNumber () const
							{ return (_mazePlaceNumber); }
	/** To change the maze place where the player is in. */
	void setMazePlaceNumber (int r);

	/** To know the percetage of the game played. */
	int percentagePlayed () const;

	/** To move the player. 
		The acceleration is not taken into account. */
	void moveCharacter (const QGAMES::Vector& d);
	/** Kill al nasties.
		This is something that happens usually when sabreman dies. */
	void killNasties ();

	virtual void initialize ();
	/** To initialize the orchilds, the food, the bad guys... */
	void initializeThings ();
	virtual void updatePositions ();
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
	/** To clear the things.
		A very internal method just called by the initialize method. */
	void clearThings ();
	/**	When an entity (sabreman or other) is in the limit of a maze place
		this method calculates the new position and the new maze place number.
		If the entity is not in the limit, then the current position and
		maze place are returned. */
	void inLimitNewPlaceFor (QGAMES::Entity* e, const QGAMES::Position& rZ, 
		QGAMES::Vector& tM, int& tA); 

	private:
	/** The number of the maze place where the player is now in. */
	int _mazePlaceNumber; // Initially it is 0;
	/** The main character. */
	Sabreman* _mainCharacter;
	/** Where all orchilds are. */
	OrchildCounters _orchilds;
	/** The orchild of the room (the object is reused) */
	Orchild* _orchild;
	/** Where the objects are. */
	VectorOfObjectCounters _objects;
	/** The guardian. (When it is needed. There is only one guardian in the game) */
	SabreWulfGuardian* _guardian;
	/** A vector to indicate which maze places have been already visited.
		It is used to calculate the precenaje of the game played. */
	std::vector <bool> _visitedMazePlaces;
	/** Counter to show the fire. */
	int _counterToFire;

	// Define a set of important buoys..
	/** Define a buoy to change the maze place. */
	class ToChangeMazePlaceBuoy : public QGAMES::Buoy
	{
		public:
		ToChangeMazePlaceBuoy ()
			: QGAMES::Buoy (__SABREWULFTOCHANGEROOMBUOYID__, __BD 0)
							{ }

		void setData (SabreWulfChangeMazePlaceEventData* dt)
							{ _data = dt; }

		/** The data is destroyed when treated. */
		virtual void* treatFor (QGAMES::Element* e);

		private:
		SabreWulfChangeMazePlaceEventData* _data;
	};

	/** Define a buoy to exit the maze. */
	class ToExitMazeBuoy : public QGAMES::Buoy
	{
		public:
		ToExitMazeBuoy ()
			: QGAMES::Buoy (__SABREWULFTOEXITMAZEBUOYID__, __BD 0)
							{ }

		virtual void* treatFor (QGAMES::Element* e);
	};

	/** Define a buoy to change the place where an inmortal is. */
	class InmortalToChangeMazePlaceBuoy : public QGAMES::Buoy
	{
		public:
		InmortalToChangeMazePlaceBuoy ()
			: QGAMES::Buoy (__INMORTALTOCHANGEROOMBUOYID__, __BD 0)
							{ }

		void setData (InmortalChangeMazePlaceEventData* dt)
							{ _data = dt; }

		/** The data is destroyed when treated. */
		virtual void* treatFor (QGAMES::Element* e);

		private:
		InmortalChangeMazePlaceEventData* _data;
	};
};

#endif
  
// End of the file
/*@}*/