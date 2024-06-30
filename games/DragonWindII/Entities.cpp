#include "Entities.hpp"
#include "Defs.hpp"

// ---
QGAMES::Entity* DragonTheRevenge::Thing::clone () const
{
	QGAMES::Entity* result = new DragonTheRevenge::Shooting (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
int DragonTheRevenge::Thing::aspectAssociatedToThingType (int t)
{
	return ((t >= 10 && t < 20) ? t - 10 : ((t >= 50 && t < 60) ? t - 40 : 5 /** Question by default. */));
}

// ---
int DragonTheRevenge::Thing::thingTypeAssociatedToAspect (int a)
{ 
	return ((a >= 0 && a < 10) ? a + 10 : ((a >= 10 && a < 20) ? a + 40 : 15 /** Question by default. */));
}

// ---
QGAMES::Entity* DragonTheRevenge::Shooting::clone () const
{
	QGAMES::Entity* result = new DragonTheRevenge::Shooting (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
DRAGONWIND::Shooting::StatesId DragonTheRevenge::Shooting::statesIdForType (int t)
{
	return ((t == __DRAGONWINDTHEREVENGE_BALLOFFIRETYPE__) 
		? __DRAGONWIND_FIREBALLSHOOTINGSTATESID__ : DRAGONWIND::Shooting::statesIdForType (t));
}

// ---
DRAGONWIND::Shooting::Behaviour* DragonTheRevenge::Shooting::behaviourForType (int t)
{
	return ((t == __DRAGONWINDTHEREVENGE_BALLOFFIRETYPE__) 
		? new DRAGONWIND::Shooting::JustFly (this) : DRAGONWIND::Shooting::behaviourForType (t));
}

// ---
QGAMES::Entity* DragonTheRevenge::BadGuy::clone () const
{
	QGAMES::Entity* result = new DragonTheRevenge::BadGuy (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
DRAGONWIND::BadGuy::StatesId DragonTheRevenge::BadGuy::statesIdForType (int t)
{
	DRAGONWIND::BadGuy::StatesId result;

	switch (t)
	{
		case __DRAGONWIND_DINOTYPEID__:
			result = __DRAGONWIND_DINOSTATESID__;
			break;
		
		case __DRAGONWIND_MALEZOMBIETYPEID__:
			result = __DRAGONWIND_MALEZOMBIESTATESID__;
			break;

		case __DRAGONWIND_FEMALEZOMBIETYPEID__:
			result = __DRAGONWIND_FEMALEZOMBIESTATESID__;
			break;

		case __DRAGONWIND_KONGTYPEID__:
			result = __DRAGONWIND_KONGSTATESID__;
			break;

		case __DRAGONWIND_ORCUSTYPEID__:
			result = __DRAGONWIND_ORCUSSTATESID__;
			break;

		case __DRAGONWIND_BOARTYPEID__:
			result = __DRAGONWIND_BOARSTATESID__;
			break;

		default:
			result = DRAGONWIND::BadGuy::statesIdForType (t);
			break;
	}

	return (result);
}

// ---
QGAMES::Entity* DragonTheRevenge::FlockOfVultures::clone () const
{
	QGAMES::Entities eties;
	for (QGAMES::Entities::const_iterator i = _entities.begin (); i != _entities.end (); i++)
		eties [(*i).first] = (*i).second -> clone (); 
	
	QGAMES::Entity* result = new DragonTheRevenge::FlockOfVultures (_id, eties, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void DragonTheRevenge::FlockOfVultures::initialize ()
{
	DRAGONWIND::FlockOfMonsters::initialize ();

	game () -> sound (__DRAGONWINDTHEREVENGE_VULTURESSOUNDID__) -> play (__DRAGONWIND_FLOCKSOUNDCHANNEL__);
}

// ---
void DragonTheRevenge::FlockOfVultures::finalize ()
{
	DRAGONWIND::FlockOfMonsters::finalize ();

	game () -> sound (__DRAGONWINDTHEREVENGE_VULTURESSOUNDID__) -> stop ();
}

// ---
QGAMES::Entity* DragonTheRevenge::FlockOfWasps::clone () const
{
	QGAMES::Entities eties;
	for (QGAMES::Entities::const_iterator i = _entities.begin (); i != _entities.end (); i++)
		eties [(*i).first] = (*i).second -> clone (); 
	
	QGAMES::Entity* result = new DragonTheRevenge::FlockOfWasps (_id, eties, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void DragonTheRevenge::FlockOfWasps::initialize ()
{
	DRAGONWIND::FlockOfMonsters::initialize ();

	game () -> sound (__DRAGONWINDTHEREVENGE_BEEWASPSSOUNDID__) -> play (__DRAGONWIND_FLOCKSOUNDCHANNEL__);
}

// ---
void DragonTheRevenge::FlockOfWasps::finalize ()
{
	DRAGONWIND::FlockOfMonsters::finalize ();

	game () -> sound (__DRAGONWINDTHEREVENGE_BEEWASPSSOUNDID__) -> stop ();
}

// ---
QGAMES::Entity* DragonTheRevenge::FlockOfBats::clone () const
{
	QGAMES::Entities eties;
	for (QGAMES::Entities::const_iterator i = _entities.begin (); i != _entities.end (); i++)
		eties [(*i).first] = (*i).second -> clone (); 
	
	QGAMES::Entity* result = new DragonTheRevenge::FlockOfBats (_id, eties, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void DragonTheRevenge::FlockOfBats::initialize ()
{
	DRAGONWIND::FlockOfMonsters::initialize ();

	game () -> sound (__DRAGONWINDTHEREVENGE_BATSSOUNDID__) -> play (__DRAGONWIND_FLOCKSOUNDCHANNEL__);
}

// ---
void DragonTheRevenge::FlockOfBats::finalize ()
{
	DRAGONWIND::FlockOfMonsters::finalize ();

	game () -> sound (__DRAGONWINDTHEREVENGE_BATSSOUNDID__) -> stop ();
}
