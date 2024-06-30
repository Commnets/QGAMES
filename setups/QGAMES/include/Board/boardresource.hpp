/** \ingroup BoardGame */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: boardresource.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 12/11/2015 \n
 *	Description: Defines a board resource. \n
 *			A board resource is any element managed during the game by the game players. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_BOARDRESOURCE__
#define __QGAMES_BOARDRESOURCE__

#include <Common/element.hpp>
#include <map>

namespace QGAMES
{
	class BoardElement;
	class BoardResource;
	typedef std::map <int, BoardResource*> BoardResources;
	typedef std::map <std::string, std::string> BoardResourceProperties;
	class BoardSquare;

	/**
	 *	A board resource is any element used during the game. \n
	 *	A resource belongs to an element. \n
	 *	Remember an element can be from the board to the player.
	 *	When the resource is assigned to an element something happens to both
	 *	The old element, and the new element. i.e. Imaging the resource
	 *	is a "card of power". As a result of the game the card goes from one player
	 *	to another, then the old player looses energy and the new player gets that. \n
	 *	Additionally the resource can be located at an square. \n
	 *	And same, when the resource reaches an square something can happens in both
	 *	the square leaving and the square reaching. \n
	 *	The owner of the resource is a Board Element. When the Board Element is destroyed
	 *	their resources are destroyed as well. \n
	 *	Any resource has to have an state during the game. The resource owns the state
	 *	and wil be destroyed with it. \n
	 *	The resources can be grouped. The group a resource belongs to is passed
	 *	as parameter at construction time, and defined in the board definition files.
	 */
	class BoardResource : public Element
	{
		public:
		/** The state of the resource. 
			The resourceshave an state. This stae can change during the game
			and can be checked to change the configuration of the game. \n
			The basic stats has nothing. It is mandatory to inherit from this
			class and create a real value state of the resources of the game. \n
			There is virtual function in the resource to create the instance of the
			state that the resource will use during the game. */
		class State
		{
			public:
			State ()
							{ }
			virtual ~State ()
							{ }

			virtual void initialize ()
							{ }
		};

		BoardResource (int id, int grp, 
				const BoardResourceProperties& pts = BoardResourceProperties ())
			: Element (id),
			  _group (grp),
			  _properties (pts),
			  _state (NULL),
			  _active (false),
			  _boardElement (NULL),
			  _boardSquare (NULL)
							{ }

		virtual ~BoardResource ()
							{ delete (_state); } // The resources owns the state.

		/** @see parent. \n
			The information added is the one comming from the boardElement (setOfOpenValues),
			and the one comming from the boardSquare (setOfOpenValues). */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		int group () const
							{ return (_group); }

		// To manage the properties
		/** To get the properties. */
		const BoardResourceProperties& properties () const
							{ return (_properties); }
		/** TO verify whether a property exists or not. */
		bool existProperty (const std::string& n) const
							{ return (_properties.find (n) != _properties.end ()); }
		/** To get the value of a property. */
		const std::string& property (const std::string& n) const;
		/** To verify whether a set of properties match or not the values received. */
		bool matchProperties (const BoardResourceProperties& pts) const;

		State* state ()
							{ return ((_state == NULL) 
								? (_state = createState ()) : _state); }

		bool isActive () const
							{ return (_active); }
		void setActive (bool a)
							{ _active = a; }

		// To manage the element the resource is assigned.
		/** To get the element the resource is assigned. */
		BoardElement* boardElement ()
							{ return (_boardElement); }
		/** To change the board element the resource is assigned. \n
			The parameter can not be null. if it is fails. \n
			A method called whenDesassignResource is called for the old element,
			and whenAssignResource method is called for the new element. */
		void setBoardElement (BoardElement* elmnt);
		/** This method is only used at construction time for any board element. */
		void setBoardElementSimple (BoardElement* elmnt);

		// To manage the board where the resource is located.
		/** To know ehere the resource is located (if any).
			It could be null. Take care. */
		BoardSquare* boardSquare ()
							{ return (_boardSquare); }
		/** To set the square where the resource is assigned.
			It could be null, meaning the resource is not over any square. */
		void setBoardSquare (BoardSquare* bs);

		virtual void initialize ();

		protected:
		virtual State* createState () = 0;

		/** When the resource is assigned to an element, then something can be programmed. 
			By default it is nothing. */
		virtual void whenAssignedTo (BoardElement* e)
							{ }
		/** When the resource is desassigned to an element, then something can be programmed. 
			By default it is nothing. */
		virtual void whenDesassignedTo (BoardElement* e)
							{ }
		/** When the resource is assigned to a square, then something can be programmed. 
			By default it is nothing. */
		virtual void whenExitingSquare (BoardSquare* sq)
							{ }
		/** When the resource is deassigned to a square, then something can be programmed. 
			By default it is nothing. */
		virtual void whenEnteringSquare (BoardSquare* sq)
							{ }

		protected:
		int _group; // The group the resource belongs to
		BoardResourceProperties _properties; // The properties of the resource (optional)
		State* _state; // The current state of the resource (NULL by default)

		// Implementation...
		bool _active;
		BoardElement* _boardElement; // The board resource belongs to an element always...
		BoardSquare* _boardSquare; // The resource can belong to an element, but at the
								   // same time to be locatewd at a square
	};
}

#endif
  
// End of the file
/*@}*/