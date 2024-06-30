/** \ingroup Game */
/** \ingroup Foundation */
/*@{*/

/**	
 *	@file	
 *	File: timer.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a timer to control the games. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_TIMER__
#define __QGAMES_TIMER__

#include <stdlib.h>
#include <time.h>

namespace QGAMES
{
	class Timer
	{
		public:
		/** There can be only one timer at the same time in the application.
			If you try to create two, the application crashes (in debug mode). \n
			There are many methods that can be modified, but it is mandatory to redefine actualMilliseconds. */
		Timer ();
		virtual ~Timer ()
							{ }

		static Timer* timer ();

		/** The timer is actualized in every game application loop.
			It is calculating the number of loops a second takes in every moment.
			This is the method to know the number of loops in an specific moment. */
		int loopsPerSecond () const
							{ return (_loopsPerSecond); }
		/** The number of mili seconds happened from the last call. */
		long elapsedMilliseconds () const
							{ return (actualMilliSeconds () - _lastMilliSeconds); }

		void start ()
							{ _loopsPerSecondTemp = 0; 
							  _lastMilliSeconds = 0; 
							  _alreadyCalculated = false; }

		/** Called from the exec method in class Game. */
		virtual void initialize ()
							{ }
		/** Called in every iteration of the main loop (@see exec method in Game class). 
			Here the _loopsPerSecond is actualized. \n */
		virtual void inEveryLoop ();
		/** Called from the exec method in class Game. */
		virtual void finalize ()
							{ }

		protected:
		virtual long actualMilliSeconds () const = 0;

		private:
		static Timer* _theTimer;

		int _loopsPerSecond;

		int _loopsPerSecondTemp;
		long _lastMilliSeconds;
		bool _alreadyCalculated;
	};

	/** The usual timer control based in the way
		the c counts the seconds. */
	class UsualTimer : public Timer
	{
		public:
		UsualTimer ()
			: Timer ()
								{ }

		private:
		long actualMilliSeconds () const
								{ return ((long) clock ()); }
	};
}

#endif

// End of the file
/*@}*/
