/** \ingroup Game */
/** \ingroup Foundation */
/*@{*/

/**	
 *	@file	
 *	File: loopcounter.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a basic class to count loops. It is used in the Game class (exec method) mainly.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_LOOPCOUNTER__
#define __QGAMES_LOOPCOUNTER__

#include <Common/notifierobserver.hpp>
#include <list>

namespace QGAMES
{
	class LoopCounter;
	typedef std::list <LoopCounter*> LoopCounters;

	/**
	 *	A loop counter is a class initially thought to count loops. \n
	 *	The class is defined by the number of loop to count, and whether
	 *	the behaviour is or not cylic. \n
	 *	A simple method call count increments the number of cycles. \n
	 *	When the number of loops defined is reached, then an event is thrown
	 *	and an action (received at construction time) is also executed. \n
	 *	It is possible to link the loop counters to the main loop of the game exec
	 *	method. Doing so, the game class is accountable of managing these objects.
	 *	Otherwise you will. \n
	 *	The counter can be stopped or restarted with specific methods. \n
	 */
	class LoopCounter : public Notifier
	{
		public:
		class Action
		{
			public:
			/** This is the method invoked when the loop counter finishes a cycle.
				The behaviour has to be redefined. */
			virtual void doAction () = 0;
		};

		LoopCounter (int id, int bn, bool c = true, 
				Action* a = NULL)
			: _id (id),
			  _blocks (bn),
			  _cyclic (c),
			  _action (a),
			  _stop (false), // The initial state is running...
			  _counter (0) 
							{ }
		~LoopCounter ()
							{ delete (_action); _action = NULL; }

		int id () const
							{ return (_id); }
		void stop ()
							{ _stop = true; }
		void run ()
							{ _stop = false; }
		bool isStopped () const
							{ return (_stop); }

		void initialize (int blk, bool c)
							{ _blocks = blk, _cyclic = c; _counter = 0; }

		void count ();

		protected:
		int _id;
		int _blocks;
		bool _cyclic;
		Action* _action;
		bool _stop;

		// Implementation
		int _counter;
	};
}

#endif
  
// End of the file
/*@}*/
