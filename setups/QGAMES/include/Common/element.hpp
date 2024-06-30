/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: element.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines an element. \n
 *				 The element is the basic brick of any game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ELEMENT__
#define __QGAMES_ELEMENT__

#include <Common/notifierobserver.hpp>
#include <Common/position.hpp>
#include <Common/buoy.hpp>
#include <Common/counter.hpp>
#include <Common/onoffswitch.hpp>
#include <Common/openvalue.hpp>
#include <Common/_exengine.hpp>
#include <vector>

namespace QGAMES
{
	class Screen;
	class Game;
	class Element;
	typedef std::vector <Element*> Elements;
	
	/** 
	  *	An Element is something that is part of a game. \n
	  *	It has always a life-cycle consisting on: \n
	  *	1.- To be initialized, \n
	  *	2.- to do something in every game loop, \n
	  *	3.- and to be finalized. \n
	  *	Additionaly, and element can be drawn in a screen. \n
	  *	QGAMES library is disegned to manage the elements but they can also be managed externally (external engine). \n
	  *	Elements include references to counters, switches and buoys, really
	  *	usefull to switch off and on things in every loop or to manage conditions. \n
	  *	Review them at their appropiate include file. \n
	  *	The runtime status of an element can be represented by a set of values. \n 
	  *	The element can also be initialized (runtime) from a set of these values.
	  */
	class Element : public Notifier, public Observer
	{
		public:
		/** What the potential external engine could need. \n
			All data is stored in a map of strings. */
		class ExternalData
		{
			public:
			ExternalData ()
				: _cache ()
							{ }
			virtual ~ExternalData ()
							{ } // Just in case...

			std::string getFromCache (const std::string& vN) const
							{ std::map <std::string, std::string>::const_iterator i;
								return (((i = _cache.find (vN)) == _cache.end ()) ? std::string (__NULL_STRING__) : (*i).second); }
			bool storeInCache (const std::string& vN, const std::string& vV);
			void clearCache ()
							{ _cache.clear (); }
			
			private:
			std::map <std::string, std::string> _cache;
		};

		/** Any element has to have an id. \n
			It is highly recommended that this id is unique in the system. */
		Element (int id)
			: _id (id),
			  _isVisible (true), // Visible by default, but this attribute is no used here...
			  _buoys (),
			  _counters (NULL), 
			  _onOffSwitches (NULL), 
			  _externalEngine (NULL), // An external engine...if any (By default it doesn't exit)
			  _externalData (NULL) // Makes no sense if _externalEngine is NULL
							{ }
		/** The destructor is virtual, just in case. The element is the owner of its buoys. */
		virtual ~Element ();

		/** Every element is represented by an id.
			For further reasons, the id should be unique in the system per element. */
		int id () const
							{ return (_id); }

		// Managing the runtime configuration...
		/** In an element the default runtimeValues will be the status 
			of the switchs and the counters if any (setOfOpenValues), 
			and whether the element is or not visible (openValue). */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		/** To know whether the elements will or not be visible on the screen. \n
			The attribute has to be used later, when a draw method is finally implemented.
			But as this capacity is something common to all drawable elements, the attribute is defined here. */
		bool isVisible () const
							{ return (_isVisible); }
		virtual void setVisible (bool v)
							{ _isVisible = v;  }

		// To manage the external engine
		/** To set the external engine, if any. */
		_ExEngine* externalEngine () 
							{ return (_externalEngine); }
		void setExternalEngine (_ExEngine* egn)
							{ _externalEngine = egn; }
		/** Set additional data, if any. */
		ExternalData* externalData ()
							{ return (_externalData); }
		void setExternalData (ExternalData* eD)
							{ delete (_externalData); _externalData = eD; }

		/** To recover the game where the element is in. \n
			If this method is invoke and no game is in place in debug mode, the system stops. */
		Game* game ();
		const Game* game () const;

		/** This method should be used when some initialization for the object is needed. \n
			By default the method calls only the initialization for an external engine if any. */
		virtual void initialize ()
							{ if (_externalEngine) _externalEngine -> whenInitialize (this); }
		/** This method is called in every game loop. \n
			By default the buoys are treating, and the external engine invoke, if any.*/
		virtual void inEveryLoop ();
		/** An element can also be drawn in an screen. \n
			The method admits a position as a reference.
			By default that position is noPosition what should mean that the internal element's position
			has to be used instead. That internal position should then be calculated. \n
			They way it is done depends on the specific type of element. 
			The external engine is also invoke here. */
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint)
							{ if (_externalEngine) _externalEngine -> whenDrawOn (this, s, p); }
		/** Sometimes a finalization of the element is needed.
			e.g: If the element reserves some memory, this could be the right place to delete 
			it, before destroying the object. \n
			The external engine is invoke also here. */
		virtual void finalize ()
							{ if (_externalEngine) _externalEngine -> whenFinalize (this); }

		/** The elements can observe other parts of the game. \n
			If something happens on those elements being observed,
			an event will be received here. The default treatment is to do nothing,
			but the user can overload it to do whatever he wishes. \n
			The processEvent method is called sync during the notification of an event. 
			The notify method (@see notifier) iterates over the list of observers executing
			this method in each observer (Element is also an observer). 
			IMPORTANT NOTE: So if this method changed the list of observers, a crash would be generated. \n 
			Use the buoys to "note" the event and then process it layer during the inEveryLoop method (if any). */
		virtual void processEvent (const Event& e)
							{ }

		protected:
		// Implementation
		// The buoys...
		/** To manage the buoys.
			@see processEvent method details for further information. */
		void addBuoy (int id, Buoy* b); // Adds, the buoy doesn't have to exist before...
		void addBuoys (Buoys b); // Adds buoys to the list...
		void setBuoys (Buoys b); // Deletes the previous list and a new one is set
		void setBuoy (int id, Buoy* b); // Replace one buoy, the buoy has to exist...
		void activeBuoy (int id, bool a);
		Buoy* buoy (int id);
		const Buoy* buoy (int id) const;
		const Buoys& buoys () const
							{ return (_buoys); }
		Buoys& buoys ()
							{ return (_buoys); }
		/** To delete all buoys created. \n
			It is something internal only. Never used outside. */
		void deleteBuoys ();

		// Implementation to access just flow control elements...
		// Counters...
		/** To set the counters. 
			If anything exists before it is deleted. */
		void setCounters (Counters* c);
		/** To get a reference to the counters.
			If they are NULL, the method createCounters (@see below) is invoked. */
		Counters* counters ();
		const Counters* counters () const
							{ return (_counters); } // This one doesn't create the counters object (it has to exist before)
		/** To create the counters first time. \n
			By default, and to be compatible with all code already created. it returns NULL. \n
			@see macro __IMPLEMENTCOUNTERS__ defined in counter.hpp. */
		virtual Counters* createCounters ()
							{ return (NULL); }
		/** To access a counter. \n
			Due to counters are to control internal states only, it is a protected method. */
		Counter* counter (int nC)
							{ return (counters () -> counter (nC)); }
		const Counter* counter (int nC) const
							{ return (counters () -> counter (nC)); }
		/** To restart all counters. */
		void reStartAllCounters ()
							{ counters () -> reStartAll (); }

		// Switches
		/** To set the on-off switches. */
		void setOnOffSwitches (OnOffSwitches* s);
		/** To get a reference to the switches.
			If they are NULL, the method createOnOffSwitches (@see below) is invoked. */
		OnOffSwitches* onOffSwitches ();
		const OnOffSwitches* onOffSwitches () const
							{ return (_onOffSwitches); } // This ones doesn't create the switches object (it has to exist before)
		/** To create the on-off switches first time. \n
			By default, and to be compatible with all code already created. it returns NULL.
			@see macro __IMPLEMENTSWITCHES__ defined in onoffswitch.hpp. */
		virtual OnOffSwitches* createOnOffSwitches ()
							{ return (NULL); }
		/** To access an on-off switch. \n
			Due to switches are to control internal states only, it is a protected method. */
		OnOffSwitch* onOffSwitch (int nC)
							{ return (onOffSwitches () -> onOffSwitch (nC)); }
		const OnOffSwitch* onOffSwitch (int nC) const
							{ return (onOffSwitches () -> onOffSwitch (nC)); }
		/** To restart all on-off switches. */
		void reStartAllOnOffSwitches ()
							{ onOffSwitches () -> reStartAll (); }

		protected:
		int _id;
		bool _isVisible;
		// Very internal...
		Buoys _buoys;
		Counters* _counters;
		OnOffSwitches* _onOffSwitches;
		_ExEngine* _externalEngine;
		ExternalData* _externalData;
	};
}

#endif
  
// End of the file
/*@}*/
