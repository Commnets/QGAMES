/** \ingroup Game */
/** \ingroup Foundation */
/*@{*/

/**	
 *	@file	
 *	File: notifyobserver.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines both notify and observer elements. \n
 *				 A notifier is someone that notify events when something happens on it. \n
 *				 An observer is someone that read an event over a notifier an interprets it. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_NOTIFIEROBSERVER__
#define __QGAMES_NOTIFIEROBSERVER__

#include <Common/event.hpp>
#include <list>

namespace QGAMES
{
	class Observer;
	typedef std::list <Observer*> Observers;
	class Notifier;
	typedef std::list <Notifier*> Notifiers;

	class Notifier
	{
		public:
		Notifier ()
			: _observers ()
							{ }
		/** When a notifier dies it has to warn the observers that
			it is not longer valid. */
		virtual ~Notifier ();

		void registerObserver (Observer* o);
		void unRegisterObserver (Observer* o);
		const Observers& observers () const
							{ return (_observers); }

		virtual void notify (const Event& e);
		virtual void notifyFrom (const Event& e, Notifier* n);

		private:
		Observers _observers;
	};

	class Observer
	{
		public:
		friend Notifier;

		Observer ()
			: _notifiers ()
							{ }
		/** When an observer dies no notifier can longer notify an event to it. */
		virtual ~Observer ();

		void observe (Notifier* n);
		const Notifiers& notifiers () const
							{ return (_notifiers); }
		bool isObserving (Notifier* n);
		void unObserve (Notifier* n);
		void unObserveAll ();
		virtual void processEvent (const Event& e) = 0;
		/** By default this method doesn't take into account the sender. 
			This has been done just because back compatibility (written in old versions). */
		virtual void processEvent (const Event& e, Notifier* n)
							{ processEvent (e); }

		private:
		/** CRITICAL: This method is used only when the notifier is deleted. 
			This is why the notifier is declared friend of the observer. 
			It is like unObserve method but not invoking the unRegisterObserver method in 
			the notifier back. */
		void remove (Notifier* n);

		private:
		Notifiers _notifiers;
	};
}

#endif
  
// End of the file
/*@}*/
