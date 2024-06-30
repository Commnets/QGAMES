/** \ingroup Game */
/** \ingroup Foundation */
/*@{*/

/**	
 *	@file	
 *	File: event.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class to represent an event. \n
 *				 The events are part of the things notified by notifiers. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_EVENT__
#define __QGAMES_EVENT__

#include <Common/openvalue.hpp>
#include <list>

namespace QGAMES
{
	class Event;
	typedef std::list <Event> Events;

	/**	
	  *	All notifications must be done using this class.
	  *	The event is not the owner of the data received. \n
	  *	Don't forget to delete the data (if necessary) when the 
	  *	event has been processed. \n
	  *	The event can also have a set of parameters (@see SetOfOpenValues). By default they are not needed...
	  *	The event is the element transmitied among notifiers and observers. 
	  */
	class Event
	{
		public:
		Event (int c, void* d, const SetOfOpenValues& v = SetOfOpenValues ())
			: _code (c), _data (d), _values (v)
							{ }
		virtual ~Event ()
							{ }

		int code () const
							{ return (_code); }
		void* data () const
							{ return (_data); }
		const SetOfOpenValues& values () const
							{ return (_values); }

		bool operator == (const Event& evnt)
							{ return (_code == evnt._code && _data == evnt._data && _values == evnt._values); }
		bool operator != (const Event& evnt)
							{ return (_code != evnt._code || _data != evnt._data || _values != evnt._values); }

		public:
		static Event _noEvent;

		private:
		int _code;
		void* _data;
		SetOfOpenValues _values;
	};

	/** Define a no event. */
	class NoEvent : public Event
	{
		public:
		NoEvent ()
			: Event (-1, NULL)
							{ }
	};
}

#endif
  
// End of the file
/*@}*/
