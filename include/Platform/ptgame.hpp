/** \ingroup Platform */
/** \ingroup Strcuture */
/*@{*/

/**	
 *	@file	
 *	File: ptgame.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 31/12/2018 \n
 *	Description: To define a platform game.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_PTGAME__
#define __QGAMES_PTGAME__

#include <Platform/ptdefinitions.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace QGAMES
{
	struct ToKnowTileFrameInformation;

	/** All platform games shoud inherit from this base class. */
	class PlatformGame : public AdvancedArcadeGame
	{
		public:
		/** Represent a potential element in the pocket of an artist.
			The main artist's pocket (whatever it is) is stored into the configuration data file.
			If it hasn't been defined, an empty pocket with just one null element will be kept only. 
			However take into account that any artist can have a pocket with things. */
		struct PocketElement
		{
			PocketElement ()
				: _id (-1), _type (-1), _values ()
								{ }
			PocketElement (int id, int tp, const QGAMES::SetOfOpenValues& oV = QGAMES::SetOfOpenValues ())
				: _id (id), _type (tp), _values (oV)
								{ }

			QGAMES::SetOfOpenValues likeSetOfOpenValues () const;

			friend std::ostream& operator << (std::ostream& oS, const PocketElement& tE);
			friend std::istream& operator >> (std::istream& iS, PocketElement& tE);

			int _id;
			int _type;
			QGAMES::SetOfOpenValues _values;
		};

		/** All pocket elements are grouped into a pocket. */
		struct Pocket
		{
			public:
			Pocket ()
				: _elements (3, PocketElement ()) // The usual size could be three...
								{ }

			Pocket (int nE)
				: _elements (nE, PocketElement ())
								{ assert (nE > 0); } // TThe size of the pocket has to be positive...

			void toIterateElements ()
								{ _elements [0] = toAddElement (PocketElement ()); }
			PocketElement toAddElement (const PocketElement& pE);
			const PocketElement& firstElement () const
								{ return (_elements [0]); }
			PocketElement& firstElement ()
								{ return (_elements [0]); }
			const PocketElement& lastElement () const
								{ return (_elements [_elements.size () - 1]); }
			PocketElement& lastElement ()
								{ return (_elements [_elements.size () - 1]); }
			std::vector <int> listOfElementsType () const;
			bool isEmpty () const;

			friend std::ostream& operator << (std::ostream& oS, const Pocket& tE);
			friend std::istream& operator >> (std::istream& iS, Pocket& tE);

			std::vector <PocketElement> _elements;
		};

		/** Defines an element in the game */
		struct ElementDefinition
		{
			ElementDefinition ()
				: _id (-1),
				  _type (-1),
				  _worldId (-1), _sceneId (-1),
				  _positions (), // Empty
				  _orientation (Vector::_cero),
				  _state (-1), // Means not needed...
				  _otherValues (SetOfOpenValues ())
							{ }
			ElementDefinition (int id, int tp, int wId, int sId, const Positions& pos, int st = -1,
					const SetOfOpenValues& oV = SetOfOpenValues ())
				: _id (id),
				  _type (tp),
				  _worldId (wId), _sceneId (sId),
				  _positions (pos),
				  _orientation (Vector::_cero),
				  _state (st),
				  _otherValues (oV)
							{ }

			friend std::ostream& operator << (std::ostream& oS, const ElementDefinition& tE);
			friend std::istream& operator >> (std::istream& iS, ElementDefinition& tE);

			/** IMPORTANT:
				If new ElementDefinition entities were necessary, 
				it would be needed to overload their own << and >> operators. */

			int _id;
			int _type;
			int _worldId;
			int _sceneId;
			Positions _positions;
			Vector _orientation;
			int _state; // Just if is needed (usually not)
			SetOfOpenValues _otherValues;
		};

		// To simplify the manipulation of lists of those...
		typedef std::map <int, ElementDefinition*> ElementsDefinition;
		typedef std::vector <ElementsDefinition> ElementsDefinitionPerPlayer;
		typedef std::map <int, bool> ElementsDefinitionLoadedOnce;
		typedef std::vector <ElementsDefinitionLoadedOnce> ElementsDefinitionLoadedOncePerPlayer;

		// By default there are three types of elements
		// Things to eat...
		typedef ElementDefinition ThingToEatDefinition;
		typedef ElementsDefinition ThingsToEatDefinition;
		typedef ElementsDefinitionPerPlayer ThingsToEatDefinitionPerPlayer;
		typedef ElementsDefinitionLoadedOnce ThingsToEatDefinitionLoadedOnce;
		typedef ElementsDefinitionLoadedOncePerPlayer ThingsToEatDefinitionLoadedOncePerPlayer;
		// Things to catch...
		typedef ElementDefinition ThingToCatchDefinition;
		typedef ElementsDefinition ThingsToCatchDefinition;
		typedef ElementsDefinitionPerPlayer ThingsToCatchDefinitionPerPlayer;
		typedef ElementsDefinitionLoadedOnce ThingsToCatchDefinitionLoadedOnce;
		typedef ElementsDefinitionLoadedOncePerPlayer ThingsToCatchDefinitionLoadedOncePerPlayer;
		// bad guys...
		typedef ElementDefinition BadGuyDefinition;
		typedef ElementsDefinition BadGuysDefinition;
		typedef ElementsDefinitionPerPlayer BadGuysDefinitionPerPlayer;
		typedef ElementsDefinitionLoadedOnce BadGuysDefinitionLoadedOnce;
		typedef ElementsDefinitionLoadedOncePerPlayer BadGuysDefinitionLoadedOncePerPlayer;
		// and exit doors...
		typedef ElementDefinition ExitDoorDefinition;
		typedef ElementsDefinition ExitDoorsDefinition;
		typedef ElementsDefinitionPerPlayer ExitDoorsDefinitionPerPlayer;
		typedef ElementsDefinitionLoadedOnce ExitDoorsDefinitionLoadedOnce;
		typedef ElementsDefinitionLoadedOncePerPlayer ExitDoorsDefinitionLoadedOncePerPlayer;

		/** The configuration adds more things. */
		class Conf : public AdvancedArcadeGame::Conf
		{
			public:
			Conf (int nP);
			/** The default copy constructor is enough. */
			~Conf ();

			/** @see parent. */
			virtual Configuration* clone () const
							{ Conf* result = new Conf (*this); 
							  result -> cloneImplementation (this); 
							  return (result); }

			/** The pocket if any. */
			const Pocket& pocket (int nP) const
							{ assert (nP > 0 && nP <= (int) _pocket.size ());
							  return (_pocket [nP - 1]); }
			void setPocket (int nP, const Pocket& p)
							{ assert (nP > 0 && nP <= (int) _pocket.size ());
							  _pocket [nP - 1] = p; }

			/** The get things to eat for a specific scene. */
			ThingsToEatDefinition thingsToEatFor (int nP, int nS) const;
			bool existThingToEatFor (int nP, int nS, int id) const;
			void removeThingToEatFor (int nP, int nS, int id)
							{ removeElementDefinitionFor (nP, nS, id, _thingsToEat, _eatInfoLoadedOnce); }
			void deleteThingsToEatFor (int nP, int nS)
							{ deleteElementsDefinitionFor (nP, nS, _thingsToEat, _eatInfoLoadedOnce); }
			void deleteThingsToEatFor (int nP)
							{ deleteElementsDefinitionFor (nP, _thingsToEat, _eatInfoLoadedOnce); }
			void initializeThingsToEatFor (int nP)
							{ deleteThingsToEatFor (nP); }
			virtual ThingToEatDefinition* createEmptyThingToEatDefinition () const
							{ return (new ThingToEatDefinition ()); }

			/** Same for things to catch. */
			ThingsToCatchDefinition thingsToCatchFor (int nP, int nS) const;
			bool existThingToCatchFor (int nP, int nS, int id) const;
			ThingToCatchDefinition* addThingToCatch (int nP, int nW, int nS, int id, int t, 
				const Positions& pos, int st, const SetOfOpenValues& sV = SetOfOpenValues ());
			void removeThingToCatchFor (int nP, int nS, int id)
							{ removeElementDefinitionFor (nP, nS, id, _thingsToCatch, _catchInfoLoadedOnce); }
			void deleteThingsToCatchFor (int nP, int nS)
							{ deleteElementsDefinitionFor (nP, nS, _thingsToCatch, _catchInfoLoadedOnce); }
			void deleteThingsToCatchFor (int nP)
							{ deleteElementsDefinitionFor (nP, _thingsToCatch, _catchInfoLoadedOnce); }
			void initializeThingsToCatchFor (int nP)
							{ deleteThingsToCatchFor (nP); }
			virtual ThingToCatchDefinition* createEmptyThingToCatchDefinition () const
							{ return (new ThingToCatchDefinition ()); }

			/** Same for bad guys. */
			BadGuysDefinition badGuysFor (int nP, int nS) const;
			bool existBadGuyFor (int nP, int nS, int id) const;
			void removeBadGuyFor (int nP, int nS, int id)
							{ removeElementDefinitionFor (nP, nS, id, _badGuys, _badGuysInfoLoadedOnce); }
			void deleteBadGuysFor (int nP, int nS)
							{ deleteElementsDefinitionFor (nP, nS, _badGuys, _badGuysInfoLoadedOnce); }
			void deleteBadGuysFor (int nP)
							{ deleteElementsDefinitionFor (nP, _badGuys, _badGuysInfoLoadedOnce); }
			void initializeBadGuysFor (int nP)
							{ deleteBadGuysFor (nP); }
			virtual BadGuyDefinition* createEmptyBadGuyDefinition () const
							{ return (new BadGuyDefinition ()); }

			/** To manipulate the state of the exit doors. */
			ExitDoorsDefinition exitDoorsFor (int nP, int nS) const;
			bool existExitDoorFor (int nP, int nS, int id) const;
			void removeExitDoorFor (int nP, int nS, int id)
							{ removeElementDefinitionFor (nP, nS, id, _exitDoors, _exitDoorsInfoLoadedOnce); }
			void deleteExitDoorsFor (int nP, int nS)
							{ deleteElementsDefinitionFor (nP, nS, _exitDoors, _exitDoorsInfoLoadedOnce); }
			void deleteExitDoorsFor (int nP)
							{ deleteElementsDefinitionFor (nP, _exitDoors, _exitDoorsInfoLoadedOnce); }
			void initializeExitDoorsFor (int nP)
							{ deleteExitDoorsFor (nP); }
			virtual ExitDoorDefinition* createEmptyDoorDefinition () const
							{ return (new ExitDoorDefinition ()); }

			/** To get the default values per scene. 
				The method creates new values each time it is invoked. */
			ThingsToEatDefinition defaultThingsToEatFor (int nS) const;
			ThingsToCatchDefinition defaultThingsToCatchFor (int nS) const;
			BadGuysDefinition defaultBadGuysFor (int nS) const;
			ExitDoorsDefinition defaultExitDoorsFor (int nS) const;

			protected:
			/** @see parent. */
			virtual void adjustToPlayers (int nP);

			/** @see parent. */
			virtual void cfgToStream (std::ostringstream& oS);
			virtual void cfgFromStream (std::istringstream& iS);

			/** @see parent. */
			virtual void cloneImplementation (const Configuration* dt);

			private:
			/** To reduce complexity, and reuse code. */
			void removeElementDefinitionFor (int nP, int nS, int id,
				ElementsDefinitionPerPlayer& lst, ElementsDefinitionLoadedOncePerPlayer& lstOnce);
			void deleteElementsDefinitionFor (int nP, int nS, 
				ElementsDefinitionPerPlayer& lst, ElementsDefinitionLoadedOncePerPlayer& lstOnce);
			void deleteElementsDefinitionFor (int nP, 
				ElementsDefinitionPerPlayer& lst, ElementsDefinitionLoadedOncePerPlayer& lstOnce);

			protected:
			// Data
			// General configuration
			/** The pocket of the main artist if any. */
			std::vector <Pocket> _pocket; 
			/** The things to eat. */
			mutable ThingsToEatDefinitionPerPlayer _thingsToEat;
			/** The things to catch. */
			mutable ThingsToCatchDefinitionPerPlayer _thingsToCatch;
			/** The bad guys. */
			mutable BadGuysDefinitionPerPlayer _badGuys;
			/** The exit doors. */
			mutable ExitDoorsDefinitionPerPlayer _exitDoors;

			private:
			// Implementation
			/** Not necessary all scene definitions have to be loaded from the early beginning.
				So it is needed a mechanism to know whether the info of a scene has or not been already loaded. */
			mutable ThingsToEatDefinitionLoadedOncePerPlayer _eatInfoLoadedOnce;
			mutable ThingsToCatchDefinitionLoadedOncePerPlayer _catchInfoLoadedOnce;
			mutable BadGuysDefinitionLoadedOncePerPlayer _badGuysInfoLoadedOnce;
			mutable ExitDoorsDefinitionLoadedOncePerPlayer _exitDoorsInfoLoadedOnce;
		};

		PlatformGame (GameImplementation* impl = NULL)
			: AdvancedArcadeGame (impl)
							{ }

		/** The things that the main artist has in the pocket, if any */
		const Pocket& pocket (int nP = -1) const
							{ return (((Conf*) configuration ()) -> 
								pocket ((nP == -1) ? currentPlayer () : nP)); }
		void setPocket (const Pocket& p, int nP = -1)
							{ ((Conf*) configuration ()) -> 
								setPocket ((nP == -1) ? currentPlayer () : nP, p); }

		/** To access to the attributes to eatable things */
		bool existThingToEatFor (int nS, int id, int nP = -1) const
							{ return (((Conf*) configuration ()) -> 
								existThingToEatFor ((nP == -1) ? currentPlayer () : nP, nS, id)); }
		void removeThingToEat (int nS, int id, int nP = -1)
							{ ((Conf*) configuration ()) -> 
								removeThingToEatFor ((nP == -1) ? currentPlayer () : nP, nS, id); }
		ThingsToEatDefinition thingsToEatFor (int nS, int nP = -1)
							{ return (((Conf*) configuration ()) -> 
								thingsToEatFor ((nP == -1) ? currentPlayer () : nP, nS)); }
		ThingsToEatDefinition defaultThingsToEatFor (int nS)
							{ return (((Conf*) configuration ()) -> defaultThingsToEatFor (nS)); }

		/** ...catchable things */
		bool existThingToCatchFor (int nS, int id, int nP = -1) const
							{ return (((Conf*) configuration ()) -> 
								existThingToCatchFor ((nP == -1) ? currentPlayer () : nP, nS, id)); }
		ThingToCatchDefinition* addThingToCatch (int nW, int nS, int id, int t, const Positions& pos, int st, 
			const SetOfOpenValues& sV, int nP = -1)
							{ return (((Conf*) configuration ()) -> 
								addThingToCatch ((nP == -1) ? currentPlayer () : nP, 
									nW, nS, id, t, pos, st, sV)); }
		void removeThingToCatch (int nS, int id, int nP = -1)
							{ ((Conf*) configuration ()) -> 
								removeThingToCatchFor ((nP == -1) ? currentPlayer () : nP, nS, id); }
		ThingsToCatchDefinition thingsToCatchFor (int nS, int nP = -1)
							{ return (((Conf*) configuration ()) -> 
								thingsToCatchFor ((nP == -1) ? currentPlayer () : nP, nS)); }
		ThingsToCatchDefinition defaultThingsToCatchFor (int nS)
							{ return (((Conf*) configuration ()) -> defaultThingsToCatchFor (nS)); }

		/** ...bad guys. */
		bool existBadGuyFor (int nS, int id, int nP = -1) const
							{ return (((Conf*) configuration ()) -> 
								existBadGuyFor ((nP == -1) ? currentPlayer () : nP, nS, id)); }
		void removeBadGuy (int nS, int id, int nP = -1)
							{ ((Conf*) configuration ()) -> 
								removeBadGuyFor ((nP == -1) ? currentPlayer () : nP, nS, id); }
		BadGuysDefinition badGuysFor (int nS, int nP = -1)
							{ return (((Conf*) configuration ()) -> 
								badGuysFor ((nP == -1) ? currentPlayer () : nP, nS)); }
		BadGuysDefinition defaultBadGuysFor (int nS)
							{ return (((Conf*) configuration ()) -> defaultBadGuysFor (nS)); }

		/** ...and exit doors. */
		bool existExitDoorFor (int nS, int id, int nP = -1) const
							{ return (((Conf*) configuration ()) -> 
								existExitDoorFor ((nP == -1) ? currentPlayer () : nP, nS, id)); }
		void removeExitDoor (int nS, int id, int nP = -1)
							{ ((Conf*) configuration ()) -> 
								removeExitDoorFor ((nP == -1) ? currentPlayer () : nP, nS, id); }
		ExitDoorsDefinition exitDoorsFor (int nS, int nP = -1)
							{ return (((Conf*) configuration ()) -> 
								exitDoorsFor ((nP == -1) ? currentPlayer () : nP, nS)); }
		ExitDoorsDefinition defaultExitDoorsFor (int nS)
							{ return (((Conf*) configuration ()) -> defaultExitDoorsFor (nS)); }

		// Implementation
		/** To get the object with the information necessary to know what frames represent things in 
			both location layer and notification layer. */
		const ToKnowTileFrameInformation* tileFrameInformation () const
							{ return ((const ToKnowTileFrameInformation*) 
								((PlatformGame*) this) -> tileFrameInformation ()); }
		ToKnowTileFrameInformation* tileFrameInformation ();

		protected:
		/** @see parent. */
		virtual Configuration* createConfiguration ()
								{ return (new Conf (1)); } // One player by default...
		virtual MapBuilder* createMapBuilder ();
	};
}

#endif

// End of the file
/*@}*/