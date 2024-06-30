/** \ingroup BoardGame */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: boardelement.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 14/11/2015 \n
 *	Description: Defines a basic information for any board element. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_BOARDELEMENT__
#define __QGAMES_BOARDELEMENT__

#include <Common/element.hpp>
#include <Board/boardresource.hpp>

namespace QGAMES
{
	class BoardElement;
	typedef std::map <int, BoardElement*> BoardElements;
	typedef std::map <std::string, std::string> BoardElementProperties;

	/** 
	 *	A BoardElement is any part of the game. \n
	 *	It can be the board, the squares the board is made up of, 
	 *	and the game made up of many boards. \n
	 *	Any board element can have resources associated. When the element is created
	 *	the resources associated to it are observed. \n
	 *	Any BoardElement can have also properties to define it. \n
	 *	They are complete optional.
	 *	The board element owns their resources. \n
	 *	Those resources can change of owner during the game, but at the end
	 *	they will belong to someone. Then when the board s destroyed
	 *	the resources with it will be destroyed too.
	 */
	class BoardElement : public Element
	{
		public:
		// The resource can invoke assign and desassign methods, that are private...
		friend BoardResource;

		BoardElement (int id, const BoardResources& rscs, 
				const BoardElementProperties& pts = BoardElementProperties ());
		virtual ~BoardElement ();

		/** @see parent. \n
			The information added is the one comming from each 
			resource being part of this element (setOfOpenValues). */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		/** To know the type of board element. 
			By default every basic board elements has a definition,
			but it can be overloaded inhereting from it. */
		virtual int type () const = 0;

		// To manage the resources of the element
		/** To get the resources. */
		const BoardResources& resources () const
							{ return (_resources); }
		/** To get the first resource. */
		BoardResource* firstResource ();
		/** To verify whether any resource exists or not. */
		virtual bool existsAnyResource () const
							{ return (!_resources.empty ()); }
		/** To verify whether a resource with an specific id exists or not. 
			The method is declared as virtual, because depeding on the type of element,
			the search can be extended to other internal elements the element can point. */
		virtual bool existResource (int id) const
							{ return (_resources.find (id) != _resources.end ()); }
		/** To get a resource from its id. If the resource doesn't exist, 
			the program fails, at debugging time. \n
			The method is declared as virtual, because depeding on the type of element,
			the search can be extended to other internal elements the element can point. */
		virtual BoardResource* resource (int id);
		/** To get a resource matching specific properties.
			The method is declared as virtual, because depeding on the type of element,
			the search can be extended to other internal elements the element can point. */
		virtual BoardResource* resource (const BoardElementProperties& pts);

		// To manage the properties of the element
		/** To get the properties of an element. */
		const BoardElementProperties& properties () const
							{ return (_properties); }
		/** To verify whether a property exists or not.
			The name of the property is received as parameter. */
		bool existProperty (const std::string& n) const
							{ return (_properties.find (n) != _properties.end ()); }
		/** To get the value of a property. If the poroperty doesn't exist, 
			then the program fails at debuggin time minimum. */
		const std::string& property (const std::string& n) const;
		/** To verify if the resource matches or not a set of properties.
			If doesn't, then false is returned. True if it does. */
		bool matchProperties (const BoardElementProperties& pts) const;

		virtual void initialize ();
		virtual void inEveryLoop ();
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		virtual void finalize ();

		protected:
		// The assignation and desasignation of a resource can only be executed from
		// the resource. This is the reason these methods are all decalred as protected and 
		// the class BoardResource declared as friend.
		/** This method is invoked from the resource, when it is assigned
			to a new board element. \n
			This method will invokes to whenAssignResource. */
		void assignResource (BoardResource* rsce);
		/** This method is invoked from the resource, when it is desassigned
			from this element. \n
			The method will invoke internally to whenDesassignedResource. */
		void desassignResource (BoardResource* rsce);
		/** Executed when a resource is assigned to the element. \n
			It can be overloaded. By default it does nothing. */
		virtual void whenAssignResource (BoardResource* rsce) { }
		/** Executed when a resource is desassigned of this element. \n
			It can be overloaded, but by default it does nothing. */
		virtual void whenDesassignResource (BoardResource* rsce) { }

		protected:
		BoardResources _resources;
		BoardElementProperties _properties;
	};
}

#endif
  
// End of the file
/*@}*/
