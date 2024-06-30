#include <Common/notifierobserver.hpp>
#include <assert.h>

// ---
QGAMES::Notifier::~Notifier ()
{
	for (QGAMES::Observers::const_iterator i = _observers.begin ();
			i != _observers.end (); i++)
		(*i) -> remove (this); // CRITICAL...
}

// ---
void QGAMES::Notifier::registerObserver (QGAMES::Observer* o)
{
	assert (o);

	bool found = false;
	for (QGAMES::Observers::const_iterator i = _observers.begin (); 
			i != _observers.end () && !found; i++)
		if ((*i) == o)
			found = true;
	
	if (!found)
		_observers.push_back (o);
}

// ---
void QGAMES::Notifier::unRegisterObserver (QGAMES::Observer* o)
{
	assert (o);

	bool found = false;
	QGAMES::Observers::iterator i = _observers.begin ();
	while (i != _observers.end () && !found)
	{
		if ((*i) == o)
			found = true;
		else
			i++;
	}

	if (found)
		_observers.erase (i);
}

// ---
void QGAMES::Notifier::notify (const Event& e) 
{ 
	for (QGAMES::Observers::const_iterator i = _observers.begin (); 
			i != _observers.end (); i++)
		(*i) -> processEvent (e); 
}

// ---
void QGAMES::Notifier::notifyFrom (const Event& e, Notifier* n)
{
	for (QGAMES::Observers::const_iterator i = _observers.begin (); 
			i != _observers.end (); i++)
		(*i) -> processEvent (e, n); // The same than before but indicating who is sending the event!
}

// ---
QGAMES::Observer::~Observer ()
{
	for (QGAMES::Notifiers::const_iterator i = _notifiers.begin ();
			i != _notifiers.end (); i++)
		(*i) -> unRegisterObserver (this);
}

// ---
void QGAMES::Observer::observe (QGAMES::Notifier* n)
{ 
	assert (n);

	bool found = false;
	for (QGAMES::Notifiers::const_iterator i = _notifiers.begin (); 
			i != _notifiers.end () && !found; i++)
		if ((*i) == n)
			found = true;

	if (!found)
	{
		_notifiers.push_back (n);
		n -> registerObserver (this);
	}
}

// ---
bool QGAMES::Observer::isObserving (Notifier* n)
{
	assert (n);

	bool found = false;
	for (QGAMES::Notifiers::iterator i = _notifiers.begin (); i != _notifiers.end () && !found; i++)
		if ((*i) == n) found = true;
	return (found);
}

// ---
void QGAMES::Observer::unObserve (QGAMES::Notifier* n)
{ 
	assert (n);

	bool found = false;
	QGAMES::Notifiers::iterator i = _notifiers.begin ();
	while (i != _notifiers.end () && !found)
	{
		if ((*i) == n)
			found = true;
		else
			i++;
	}

	if (found)
	{
		_notifiers.erase (i);
		n -> unRegisterObserver (this);
	}
}

// ---
void QGAMES::Observer::unObserveAll ()
{
	QGAMES::Notifiers::iterator i = _notifiers.begin ();
	while (i != _notifiers.end ())
	{
		QGAMES::Notifier* n = (*i);
		_notifiers.erase (i);
		n -> unRegisterObserver (this);

		i = _notifiers.begin ();
	}

	_notifiers.clear (); // Just in case...
}

// ---
void QGAMES::Observer::remove (QGAMES::Notifier* n)
{
	assert (n);

	bool found = false;
	QGAMES::Notifiers::iterator i = _notifiers.begin ();
	while (i != _notifiers.end () && !found)
	{
		if ((*i) == n)
			found = true;
		else
			i++;
	}

	if (found)
		_notifiers.erase (i);
}
