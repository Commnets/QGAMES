/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: buoy.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 21/01/2015 \n
 *	Description: Defines an buoy. \n
 *				 The buoys are element than can be managed by any Element,
 *				 to mark situations. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_BUOY__
#define __QGAMES_BUOY__

#include <Common/definitions.hpp>
#include <map>

namespace QGAMES
{
	class Buoy;
	typedef std::map <int, Buoy*> Buoys;
	class Element;

	/** 
	 *	A Buoy is a class to mark a situation. \n
	 *	When something happens in an Element (this class is strongly linked to that other one)
	 *	the situation can be marked and treated later on during the everyLoop method.
	 */
	class Buoy
	{
		public:
		Buoy (int id, bdata v)
			: _id (id),
			  _value (v),
			  _active (false)
							{ }

		bool isActive () const
							{ return (_active); }
		/** To change the situation of the Buoy. */
		void active (bool a)
							{ _active = a; }
		int id () const
							{ return (_id); }
		bdata value () const
							{ return (_value); }
		void setValue (bdata v)
							{ _value = v; } // It is not the normal thing, but its value can be changed!!

		/** What to do with the buoy when active. */
		virtual void* treatFor (Element* e) = 0;

		protected:
		int _id;
		bdata _value;
		bool _active;
	};

	/** A bouy specific to notify something. */
	class NotificationBuoy : public Buoy
	{
		public:
		/** In this case the value is always an int, 
			and it is also the event id used in the notification. */
		NotificationBuoy (int id, bdata v) 
			: Buoy (id, v)
							{ }

		/** @see parent. */
		virtual void* treatFor (Element* e);
	};
}

#endif
  
// End of the file
/*@}*/
