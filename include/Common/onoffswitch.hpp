/** \ingroup Game */
/** \ingroup Foundation */
/*@{*/

/**	
 *	@file	
 *	File: onoffswitch.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 27/03/2016 \n
 *	Description: Defines an on-off switch. \n
 *				 OnOff Switches are also important elements in the game. \n
 *				 Normally they are used to determinate when to launch an action as a consequence
 *				 of any other action or event within the element. \n
 *				 e.g. If an entity hits other one, the next time update method is executed. \n
 *				 could be interesting to increment the level of energy!. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ONOFFSWITCH__
#define __QGAMES_ONOFFSWITCH__

#include <Common/openvalue.hpp>
#include <map>
#include <vector>

namespace QGAMES
{
	class OnOffSwitch
	{
		public:
		/**
		 *	The on-off switch is built from:
		 *	@param id	The name of the switch.
		 *	@param iV	The initial value true or false. By default it is false.
		 */
		OnOffSwitch (int id, bool iV = false);
		virtual ~OnOffSwitch ()
							{ }

		/** To get the data as a set of openvalues. */
		virtual SetOfOpenValues asSetOfOpenValues () const;
		virtual void fromSetOfOpenValues (const SetOfOpenValues& oV);

		virtual void initialize ()
							{ _value = _initialValue; }
		/** This method is to know the status of the switch. */
		bool isOn () const
							{ return (_value); }
		/** This method is to set on off the switch. */
		void set (bool v)
							{ _value = v; }
		/** Change the value. */
		void change ()
							{ _value = !_value; }

		protected:
		int _id;
		bool _initialValue;

		// Implementation
		bool _value;
	};

	/** A group of on-off switches to be managed alltogether. */
	class OnOffSwitches
	{
		public:
		OnOffSwitches ()
			: _onOffSwitches ()
							{ }
		virtual ~OnOffSwitches ()
							{ delOnOffSwitches (); }

		/** To get the data as a set of openvalues. */
		virtual SetOfOpenValues asSetOfOpenValues () const;
		virtual void fromSetOfOpenValues (const SetOfOpenValues& oV);

		/** It invokes to the method createOnOffSwitches. \n 
			This method is invoked from the method reStartOnOffSwitches defined in the class Element (@see Element).
			So any time the onOffSwitches are restarted they are created again. */
		void initialize ();
		void reStartAll ();

		// To manage the individual switches
		int numberOfSwitches () const
							{ return ((int) _onOffSwitches.size ()); }
		bool existOnOffSwitch (int nS) const
							{ return (nS >= 0 && nS < (int) _onOffSwitches.size ()); }
		OnOffSwitch* onOffSwitch (int nC)
							{ return (_onOffSwitches [nC]); }
		const OnOffSwitch* onOffSwitch (int nC) const
							{ return (_onOffSwitches [nC]); }
		void addOnOffSwitch (OnOffSwitch* s);
		OnOffSwitch* operator [] (int nC)
							{ return (_onOffSwitches [nC]); }
		const OnOffSwitch* operator [] (int nC) const
							{ return (_onOffSwitches [nC]); }

		protected:
		void delOnOffSwitches ();
		virtual void createOnOffSwitches () = 0;

		protected:
		std::vector <OnOffSwitch*> _onOffSwitches;
	};

#define __DECLAREONOFFSWITCHES__(A) \
	class A : public QGAMES::OnOffSwitches { \
		public: \
		A () : QGAMES::OnOffSwitches () { } \
		virtual void createOnOffSwitches (); \
	};

#define __IMPLEMENTONOFFSWITCHES__(A) \
	void A::createOnOffSwitches ()
}

#endif
  
// End of the file
/*@}*/