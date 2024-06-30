/** \ingroup Game */
/** \ingroup Foundation */
/*@{*/

/**	
 *	@file	
 *	File: counter.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 25/03/2016 \n
 *	Description: Defines a counter. \n
 *				 Counters are important managing elements in the game. \n
 *				 Normally they are used to determinate when to change the form of an sprite \n
 *				 or when to move forward an Entity i.e. \n
 *				 A Counter can be grouped into a map of counters.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_COUNTER__
#define __QGAMES_COUNTER__

#include <Common/openvalue.hpp>
#include <map>
#include <vector>

namespace QGAMES
{
	/** The counter. */
	class Counter
	{
		public:
		/**
		 *	The counter is built from:
		 *	@param id	The name of the counter.
		 *	@param lV	the value limit. 
		 *	@param iV	The initial value. By default it is 0.
		 *	@param u	Whether the counter goes up or down
		 *				if it is going up lV couldn't be less than the iV, and the opposite if going up!
		 *				By default all counters go up.
		 *	@param rI	Does the counter have to be reinitialized automatically?
		 *				By default it has to.
		 */
		Counter (int id, int lV, int iV = 0, bool u = true, bool rI = true);
		virtual ~Counter ()
							{ }

		/** To get the data as a set of openvalues. */
		virtual SetOfOpenValues asSetOfOpenValues () const;
		virtual void fromSetOfOpenValues (const SetOfOpenValues& oV);

		int id () const
							{ return (_id); }

		virtual void initialize ()
							{ _count = _initialValue; }
		/** Initialize the counter with other data than the ones passed at construction time. */
		virtual void initialize (int lV, int iV, bool u = true, bool rI = true);
		/** This method "adds one" to the counter and if it reaches the limits then returns true.
			Otherwise it'll return false. \n
			If the counter has been declared as "to be restarted", it will start again back. */
		bool isEnd ();
		/** To change the value. \n
			This is not a normal method to be used.
			The new value can not be out of the limit, otherwise crashes. */
		void setValue (int nV);

		// Accesing attributes
		/** To know the limit of the counter. */
		int limitValue () const
							{ return (_limitValue); }
		/** To know the initial value. */
		int initialValue () const
							{ return (_initialValue); }
		/** To know whether the counter is going up or not. */
		bool countingUp () const
							{ return (_countingUp); }
		/** To know whether the counter reinit automatically or not. */
		bool autoReinitialize () const
							{ return (_autoReinitialize); }
		/** To know the value of the counter at this specific moment. */
		int value () const
							{ return (_count); }

		protected:
		int _id;
		int _limitValue;
		int _initialValue;
		bool _countingUp;
		bool _autoReinitialize;

		// Implementation
		int _count;
	};

	/** A counter with a random limit. \n
		The value received at construction time is just the top. \n 
		The top must be a number bigger than 0, otherwise it will crashes! */
	class RandomCounter : public Counter
	{
		public:
		RandomCounter (int id, int lV, bool rI = true)
			: Counter (id, rand () % lV, 0, true, rI), // lV can't be 0...otherwise error!
			  _topOfRandomValue (lV)
							{ }

		virtual void initialize ()
							{ _limitValue = _topOfRandomValue; Counter::initialize (); }

		private:
		int _topOfRandomValue;
	};

	/** A group of counters to be managed alltogether. \n
		This is what Element class e.g. has as parameter. \n 
		The class owns the counters. */
	class Counters
	{
		public:
		/** Makes no sense to pass a list of counters in the constructor.
			The method initialize (@see) call createCounters (can't be overloaded). \n
			That method has to be overloaded to add (@see method addCounter) all counters needed. */
		Counters ()
			: _counters ()
							{ }
		virtual ~Counters ()
							{ delCounters (); }

		/** To get the data as a set of openvalues. */
		virtual SetOfOpenValues asSetOfOpenValues () const;
		virtual void fromSetOfOpenValues (const SetOfOpenValues& oV);

		/** It invokes to the method createCounters. \n 
			This method is invoked from the method reStartCounters defined in the class Element (@see Element).
			So any time the counters are restarted they are created again. */
		void initialize ();
		void reStartAll (); // Just initialize the counters but it doesn't create them again...

		// To manage the indivial counters
		int numberOfCounters () const
							{ return ((int) _counters.size ()); }
		bool existCounter (int nC) const
							{ return (nC >= 0 && nC < (int) _counters.size ()); }
		Counter* counter (int nC) // If nC bigger than the last, crashes...
							{ return (_counters [nC]); }
		const Counter* counter (int nC) const
							{ return (_counters [nC]); }
		void addCounter (Counter* c); // In the right order
		void replaceCounter (int nC, Counter* c); // counter number nC has to exit

		// Operators
		Counter* operator [] (int nC)
							{ return (_counters [nC]); }
		const Counter* operator [] (int nC) const
							{ return (_counters [nC]); }

		protected:
		void delCounters ();
		virtual void createCounters () = 0;

		protected:
		std::vector <Counter*> _counters;
	};

#define __DECLARECOUNTERS__(A) \
	class A : public QGAMES::Counters { \
		public: \
		A () : QGAMES::Counters () { } \
		virtual void createCounters (); \
	};

#define __IMPLEMENTCOUNTERS__(A) \
	void A::createCounters ()
}

#endif
  
// End of the file
/*@}*/
