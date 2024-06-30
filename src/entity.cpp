#include <Common/entity.hpp>
#include <Common/screen.hpp>
#include <Common/game.hpp>
#include <algorithm>
#include <sstream>

// ---
const std::string QGAMES::Entity::Animation::parameter (const std::string& prmName) const
{
	QGAMES::Entity::Animation::Parameters::const_iterator i;
	return (((i = _parameters.find (prmName)) == _parameters.end ()) 
		? std::string (__NULL_STRING__) : (*i).second);
}

// ---
void QGAMES::Entity::Animation::setParameter (const std::string& prmName, const std::string& val)
{
	QGAMES::Entity::Animation::Parameters::iterator i = _parameters.find (prmName);
	if (i != _parameters.end ())
		(*i).second = val;
	else
		assert (0); // No possible to insert what not exist...
}

// ---
bool QGAMES::Entity::Animation::existParameter (const std::string& prmName) const
{
	QGAMES::Entity::Animation::Parameters::const_iterator i;
	return ((i = _parameters.find (prmName)) != _parameters.end ()); 
}

// ---
QGAMES::Entity::FormAnimation::~FormAnimation ()
{
	delete (_counterAspect);
	_counterAspect = NULL;
	delete (_counterToChange);
	_counterToChange = NULL;
}

// ---
QGAMES::SetOfOpenValues QGAMES::Entity::FormAnimation::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Entity::Animation::asSetOfOpenValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, 
		QGAMES::OpenValue (_counterAspect ? _counterAspect -> value () : __MININT__));
	result.addOpenValue (lE + 2, 
		QGAMES::OpenValue (_counterToChange ? _counterToChange -> value () : __MININT__));

	return (result);
}

// ---
void QGAMES::Entity::FormAnimation::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1));

	QGAMES::SetOfOpenValues cCfg = cfg;

	if (_counterToChange) 
		_counterToChange -> setValue (cCfg.openValue (lE).intValue ());
	cCfg.removeOpenValue (lE);

	if (_counterAspect)
		_counterAspect -> setValue (cCfg.openValue (lE - 1).intValue ());
	cCfg.removeOpenValue (lE - 1);

	QGAMES::Entity::Animation::fromSetOfOpenValues (cCfg);
}

// ---
void QGAMES::Entity::FormAnimation::initialize ()
{
	delete (_counterAspect);
	delete (_counterToChange);

	_beginning = true;
	_end = false;

	_counterAspect = new Counter (_id, finalAspectId (), initialAspectId (), true, cyclic ());
	_counterToChange = new Counter (_id + 1, fractionToChange (), 0, true, true);

	_entity -> setCurrentForm (formId ());
	_entity -> setCurrentAspect (initialAspectId ());
}

// ---
void QGAMES::Entity::FormAnimation::inheritStateFrom (QGAMES::Entity::Animation* a)
{
	QGAMES::Entity::FormAnimation* fA = dynamic_cast <QGAMES::Entity::FormAnimation*> (a);
	if (fA) 
	{
		_counterToChange -> setValue (fA -> _counterToChange -> value ()); // They could fail...take care
		_counterAspect -> setValue (fA -> _counterAspect -> value ());
	}
}

// ---
void QGAMES::Entity::FormAnimation::updatePositions ()
{
	if (_end || !_counterToChange -> isEnd ())
		return; // No time to change...

	_beginning = _counterAspect -> value () == initialAspectId (); // the first one...
	_end = _counterAspect -> isEnd () && !cyclic ();

	_entity -> setCurrentAspect (_counterAspect -> value ());
	onAspect (_counterAspect -> value () - _counterAspect -> initialValue ());
}

// ---
void QGAMES::Entity::State::initialize (bool cA)
{ 
	assert (_entity);

	// Is it needed to set back the animation?
	if (!cA || _entity -> currentAnimation () == NULL ||
		(cA && _entity -> currentAnimation () != NULL && _entity -> currentAnimation () -> id () != _animationId)) 
		_entity -> setCurrentAnimation (_animationId);

	// Is it neded to set back the movement?
	if (!cA || _entity -> currentMovement () == NULL ||
		(cA && _entity -> currentMovement () != NULL && _entity -> currentMovement () -> id () != _movementId))
	{
		_entity -> setCurrentMovement (_movementId); 

		QGAMES::Character* chr = dynamic_cast <QGAMES::Character*> (_entity);
		if (chr != NULL)
		{
			std::map <std::string, std::string>::const_iterator i;
			QGAMES::Vector dr = QGAMES::Vector::_cero;
			QGAMES::Vector acl = QGAMES::Vector::_cero;

			if ((i = _parameters.find (std::string (__QGAMES_ENTITYSTATEPRMDIRECTION__))) != _parameters.end ())
			{
				std::vector <QGAMES::bdata> dt = QGAMES::getElementsFromAsBDATA ((*i).second, ',', 3);
				dr = QGAMES::Vector (dt [0], dt [1], dt [2]);
			}

			if ((i = _parameters.find (std::string (__QGAMES_ENTITYSTATEPRMACCELERATION__))) != _parameters.end ())
			{
				std::vector <QGAMES::bdata> dt = QGAMES::getElementsFromAsBDATA ((*i).second, ',', 3);
				acl = QGAMES::Vector (dt [0], dt [1], dt [2]);
			}

			chr -> setMove (dr, acl);
		}
	}
}

// ---
void QGAMES::Entity::State::finalize ()
{
	assert (_entity);
	
	_entity -> currentAnimation () -> finalize ();
	_entity -> currentMovement () -> finalize ();
}

// ---
bool QGAMES::Entity::Data::operator == (const QGAMES::Entity::Data& dt) const
{ 
	return (_position == dt._position &&
			_orientation == dt._orientation &&
			std::equal (_capacities.begin (), _capacities.end (), dt._capacities.begin ()) &&
			std::equal (_physics.begin (), _physics.end (), dt._physics.begin ()) &&
			std::equal (_movements.begin (), _movements.end (), dt._movements.begin ()) &&
			std::equal (_parameters.begin (), _parameters.end (), dt._parameters.begin ())); 
}

// ---
QGAMES::Entity::Entity (int eId, const Forms& f, const Data& d) 
	: Element (eId),
	  _forms (f),
	  _data (d),
	  _animations (), // No animations initially...
	  _states (), // No states initially...
	  _currentForm (NULL),
	  _currentMovement (NULL),
	  _currentAnimation (NULL),
	  _currentState (NULL),
	  _currentAspect (0),
	  _alphaLevel (255), // By default is the maximum...
	  _notifyWhenChangePosition (false), // By default there is no notification
	  _maxVisualLength (__MININT__), _maxVisualHeight (__MININT__), _maxVisualDepth (__MININT__)
{ 
	QGAMES::Buoys lB;
	lB.insert (QGAMES::Buoys::value_type 
		(__QGAMES_CHANGESTATEBUOY__, new QGAMES::Entity::ChangeStateBuoy));
	setBuoys (lB); // Take care in another constrctor later, this variable could be deleted...
}

// ---
QGAMES::Entity::~Entity ()
{
	// The entity owns the animations...
	deleteAnimations ();
	// The entity owns the states...
	deleteStates ();
}

// ---
QGAMES::Entity::Animations QGAMES::Entity::cloneAnimations () const
{
	QGAMES::Entity::Animations result;
	for (QGAMES::Entity::Animations::const_iterator i = _animations.begin (); 
			i != _animations.end (); i++)
		result [(*i).first] = (*i).second -> clone ();
	return (result);
}

// ---
QGAMES::Entity::States QGAMES::Entity::cloneStates () const
{
	QGAMES::Entity::States result;
	for (QGAMES::Entity::States::const_iterator i = _states.begin ();
			i != _states.end (); i++)
		result [(*i).first] = (*i).second -> clone ();
	return (result);
}

// ---
QGAMES::SetOfOpenValues QGAMES::Entity::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Element::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	// The id of the movement, __MININT__ if nothing
	result.addOpenValue (lE + 1, 
		QGAMES::OpenValue (_currentMovement ? _currentMovement -> id () : __MININT__));

	// The information of the movement...
	result.addSetOfOpenValues (lNE + 1, 
		_currentMovement ? _currentMovement -> runtimeValues () 
			: QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESENTITYMOVTYPE__)));

	// The id of the animation, __MININT__ if nothing
	result.addOpenValue (lE + 2, 
		QGAMES::OpenValue (_currentAnimation ? _currentAnimation -> id () : __MININT__));

	// The data of the animation. Empty if no animation
	result.addSetOfOpenValues (lNE + 2, _currentAnimation 
		? _currentAnimation -> asSetOfOpenValues () 
		: QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESANIMATIONTYPE__)));

	// The id of state. __MININT__ if none...
	result.addOpenValue (lE + 3, 
		QGAMES::OpenValue (_currentState ? _currentState -> id () : __MININT__));

	// The data of the state. Empty if no state
	result.addSetOfOpenValues (lNE + 3, _currentState 
		? _currentState -> asSetOfOpenValues () 
		: QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESSTATETYPE__)));

	// The position...
	std::stringstream sS; sS << _data._position;
	result.addOpenValue (lE + 4, QGAMES::OpenValue (sS.str ()));
	// and the orientation
	sS.str (std::string (__NULL_STRING__)); sS << _data._orientation;
	result.addOpenValue (lE + 5, QGAMES::OpenValue (sS.str ()));

	return (result);
}

// ---
void QGAMES::Entity::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1) && 
			cfg.existOpenValue (lE - 2) && cfg.existOpenValue (lE - 3) && cfg.existOpenValue (lE - 4) &&
			cfg.existSetOfOpenValues (lNE) && cfg.existSetOfOpenValues (lNE - 1) && cfg.existSetOfOpenValues (lNE - 2));

	QGAMES::SetOfOpenValues cCfg = cfg;

	setOrientation (cCfg.openValue (lE).strValue ());
	cCfg.removeOpenValue (lE);
	setPosition (cCfg.openValue (lE - 1).strValue ());
	cCfg.removeOpenValue (lE - 1); // Just treated...

	// If __MININT__ no state will be set... 
	// With this instruction, the movement is also initialized (if it not null), but with default parameters!
	// The attributes of the movement will be set back later...
	setCurrentState (cCfg.openValue (lE - 2).intValue ()); 
	cCfg.removeOpenValue (lE - 2); // Just treated...

	// ...and then now the specific (if any) attributes of the state are set
	QGAMES::SetOfOpenValues cSVV = cCfg.setOfOpenValues (lNE);
	if (_currentState && cSVV.numberValues () != 0)
		_currentState -> fromSetOfOpenValues (cSVV); 
	cCfg.removeSetOfOpenValues (lNE);

	// If the state has been already set, the animation will have been set also, so 
	// The first next instruction could be redundant...
	// If __MININT__ no animation will be set either...
	setCurrentAnimation (cCfg.openValue (lE - 3).intValue ());
	cCfg.removeOpenValue (lE - 3);

	// ...and then now their specific attributes are set
	QGAMES::SetOfOpenValues cAVV = cCfg.setOfOpenValues (lNE - 1);
	if (_currentAnimation && cAVV.numberValues () != 0)
		_currentAnimation -> fromSetOfOpenValues (cCfg.setOfOpenValues (lNE - 1));
	cCfg.removeSetOfOpenValues (lNE - 1);

	// The if of the movement is got...
	setCurrentMovement (cCfg.openValue (lE - 4).intValue ());
	cCfg.removeOpenValue (lE - 4);

	// Finally the attributes of the movement are set...
	QGAMES::SetOfOpenValues cMVV = cCfg.setOfOpenValues (lNE - 2);
	assert (cMVV.name () == std::string (__QGAMES_RUNTIMEVALUESENTITYMOVTYPE__)); // Makes sense...
	if (_currentMovement && cMVV.numberValues () != 0)
		_currentMovement -> initializeRuntimeValuesFrom (cMVV); 
	cCfg.removeSetOfOpenValues (lNE - 2); 

	QGAMES::Element::initializeRuntimeValuesFrom (cCfg);
}

// ---
QGAMES::Position QGAMES::Entity::centerPosition () const
{ 
	assert (_currentForm);

	return (_data._position + _currentForm -> centerPosition (_currentAspect)); 
}

// ---
QGAMES::Position QGAMES::Entity::basePosition () const
{ 
	assert (_currentForm);

	return (_data._position + _currentForm -> basePosition (_currentAspect)); 
}

// ---
QGAMES::Rectangle QGAMES::Entity::roundingFormZone () const
{
	assert (_currentForm);

	return (_currentForm -> roundingCentralZone (_currentAspect) + _data._position);
}

// ---
QGAMES::Rectangle QGAMES::Entity::coverZone () const
{ 
	assert (_currentForm);

	return (_data._position + _currentForm -> coverZone (_currentAspect)); 
}

// ---
QGAMES::Rectangle QGAMES::Entity::baseZone () const
{ 
	assert (_currentForm);

	return (_data._position + _currentForm -> baseZone (_currentAspect)); 
}

// ---
QGAMES::Box QGAMES::Entity::roundingBox () const
{
	assert (_currentForm);

	return (_data._position + _currentForm -> roundingBox (_currentAspect));
}

// ---
int QGAMES::Entity::visualLength () const
{
	assert (_currentForm);

	return (_currentForm -> visualLength (_currentAspect));
}

// ---
int QGAMES::Entity::visualHeight () const
{
	assert (_currentForm);

	return (_currentForm -> visualHeight (_currentAspect));
}

// ---
int QGAMES::Entity::visualDepth () const
{
	assert (_currentForm);

	return (_currentForm -> visualDepth (_currentAspect)); 
}

// ---
void QGAMES::Entity::maxVisualDimensions (int& vL, int& vH, int& vD) const
{
	if (_maxVisualLength == __MININT__) // Any of them. All are calculate dat the same time...
	{
		for (QGAMES::Forms::const_iterator i = _forms.begin (); i != _forms.end (); i++)
		{
			QGAMES::Form* frm = (*i).second;
			for (int j = 0; j < frm -> numberFrames (); j++)
			{
				int cVL = frm -> visualLength (j);
				if (cVL > _maxVisualLength) _maxVisualLength = cVL;
				int cVH = frm -> visualHeight (j);
				if (cVH > _maxVisualHeight) _maxVisualHeight = cVH;
				int cVD = frm -> visualDepth (j);
				if (cVD > _maxVisualDepth) _maxVisualDepth = cVD;
			}
		}
	}

	vL = _maxVisualLength; vH = _maxVisualHeight; vD = _maxVisualDepth; 
}

// ---
void QGAMES::Entity::setCurrentMovement (int nM)
{
	// __MININT__ means no movement!

	if (nM == __MININT__)
	{
		if (_currentMovement)
		{
			_currentMovement -> finalize ();
			unObserve (_currentMovement);
		}

		_currentMovement = NULL;
	}
	else
	{
		QGAMES::Movements::const_iterator i = _data._movements.find (nM);
		assert (i != _data._movements.end ()); // The movement has to exist...

		if (_currentMovement)
		{
			_currentMovement -> finalize ();
			unObserve (_currentMovement);
		}

		QGAMES::Movement* lastMovement = _currentMovement;
		_currentMovement = (*i).second;
		observe (_currentMovement); // To see its notifications...
		_currentMovement -> initializeFrom (lastMovement); 
		// Usually the initialization is from the previous movement to inherit physical properties...
	}
}

// ---
void QGAMES::Entity::setCurrentForm (int nf)
{
	QGAMES::Forms::const_iterator i = _forms.find (nf);
	assert (i != _forms.end ()); // It doesn't exist...

	_currentForm = (*i).second;
	_currentAspect = 0; // By default...
}

// ---
void QGAMES::Entity::addAnimation (QGAMES::Entity::Animation* a)
{
	assert (a);

	a -> setEntity (this); // Adjust the entity...
	_animations [a -> id ()] = a;
}

// ---
bool QGAMES::Entity::existAnimation (int id)
{
	return (_animations.find (id) != _animations.end ());
}

// ---
QGAMES::Entity::Animation* QGAMES::Entity::animation (int id)
{
	QGAMES::Entity::Animations::const_iterator i = _animations.find (id);
	assert (i != _animations.end ());

	return ((*i).second);
}

// ---
const QGAMES::Entity::Animation* QGAMES::Entity::animation (int id) const
{
	QGAMES::Entity::Animations::const_iterator i = _animations.find (id);
	assert (i != _animations.end ());

	return ((*i).second);
}

// ---
void QGAMES::Entity::setCurrentAnimation (int nA)
{
	// __MININT__ means no animation at all...

	if (nA == __MININT__)
		_currentAnimation = NULL;
	else
	{
		QGAMES::Entity::Animations::const_iterator i = _animations.find (nA);
		assert (i != _animations.end ()); // The animation has to exist...

		_currentAnimation = (*i).second;
		_currentAnimation -> setEntity (this);
		_currentAnimation -> initialize ();
	}
}

// ---
void QGAMES::Entity::addState (QGAMES::Entity::State* st)
{
	assert (st);

	st -> setEntity (this); // Adjust the entity...
	_states [st -> id ()] = st;
}

// ---
bool QGAMES::Entity::existState (int id)
{
	return (_states.find (id) != _states.end ());
}

// ---
QGAMES::Entity::State* QGAMES::Entity::state (int id)
{
	QGAMES::Entity::States::const_iterator i = _states.find (id);
	assert (i != _states.end ());

	return ((*i).second);
}

// ---
const QGAMES::Entity::State* QGAMES::Entity::state (int id) const
{
	QGAMES::Entity::States::const_iterator i = _states.find (id);
	assert (i != _states.end ());

	return ((*i).second);
}

// ---
void QGAMES::Entity::setCurrentState (int st, bool cA)
{
	// __MININT__ means no animation at all...

	if (st == __MININT__)
		_currentState = NULL;
	else
	{
		QGAMES::Entity::States::const_iterator i = _states.find (st);
		assert (i != _states.end ()); // The state has to exist...

		_currentState = (*i).second;
		_currentState -> setEntity (this);
		_currentState -> initialize (cA); // Should animation be kept? (cA == true)
	}
}

// ---
void QGAMES::Entity::setCurrentStateDeferred (int st, bool cA)
{
	QGAMES::Entity::ChangeStateBuoy* b =
		(QGAMES::Entity::ChangeStateBuoy*) buoy (__QGAMES_CHANGESTATEBUOY__);
	b -> setTargetState (st);
	b -> setContinueAnimation (cA);
	b -> active (true); 
}

// ---
QGAMES::Rectangle QGAMES::Entity::collisionZone () const
{
	assert (_currentForm);

	return (_data._position + _currentForm -> roundingCentralZone (_currentAspect));
}

// ---
bool QGAMES::Entity::hasCollisionWith (QGAMES::Entity* e) const
{
	assert (e);

	// It is necessary that 2 rectangles rounding the figure intersects...
	// This is valid for any dimension!
	return (coverZone ().toBase ().intersectsWith (e -> coverZone ().toBase ()) && 
			collisionZone ().toBase ().intersectsWith (e -> collisionZone ().toBase ()));
}

// ---
bool QGAMES::Entity::isNearOf (QGAMES::Entity* e, QGAMES::bdata d) const
{
	assert (e);

	// To go quicker and to avoid negative numbers...
	return ((centerPosition () - e -> centerPosition ()).module2 () < (d * d));
}

// ---
void QGAMES::Entity::updatePositions ()
{
	// Moves the entity if an animation has been defined...
	if (_currentAnimation != NULL)
	{
		if (_currentAnimation -> end ())
			setCurrentAnimation (_currentAnimation -> link ());
		else
			_currentAnimation -> updatePositions ();
	}

	// Actualize the state if any...
	if (_currentState != NULL)
		_currentState -> updatePositions ();

	if (_externalEngine) 
		_externalEngine -> whenUpdatePositions (this); 
}

// ---
void QGAMES::Entity::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	_currentForm -> frame (_currentAspect) -> 
		drawOn (s, (p == QGAMES::Position::_noPoint) ? _data._position : p, _alphaLevel);

	QGAMES::Element::drawOn (s, p);
}

// ---
void QGAMES::Entity::setAlphaLevel (int aL)
{
	_alphaLevel = aL;
	if (_alphaLevel < 0) _alphaLevel = 0;
	if (_alphaLevel > 255) _alphaLevel = 255;
}

// ---
void QGAMES::Entity::deleteAnimations ()
{
	for (QGAMES::Entity::Animations::const_iterator i = _animations.begin (); 
			i != _animations.end (); i++)
		delete ((*i).second); 
	_animations.clear ();
}

// ---
void QGAMES::Entity::deleteStates ()
{
	for (QGAMES::Entity::States::const_iterator i = _states.begin (); 
			i != _states.end (); i++)
		delete ((*i).second); 
	_states.clear ();
}

// ---
void* QGAMES::Entity::ChangeStateBuoy::treatFor (QGAMES::Element* e)
{
	assert (e);

	(dynamic_cast <QGAMES::Entity*> (e)) -> setCurrentState (_targetState, _continueAnimation);
	return (this);
}

// ---
QGAMES::SetOfOpenValues QGAMES::Character::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Entity::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	// The direction...
	std::stringstream sS; sS << _direction;
	result.addOpenValue (lE + 1, QGAMES::OpenValue (sS.str ()));
	// The acceleration...
	sS.str (std::string (__NULL_STRING__)); sS << _acceleration;
	result.addOpenValue (lE + 2, QGAMES::OpenValue (sS.str ()));

	// The steps control monitor id...
	result.addOpenValue (lE + 3, 
		QGAMES::OpenValue (_stepsControlMonitor ? _stepsControlMonitor -> id () : __MININT__));

	// ...the information of the steps control monitor (if any, or empty if not)
	result.addSetOfOpenValues (lNE + 1,
		_stepsControlMonitor ? _stepsControlMonitor -> runtimeValues () 
			: QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESCHRMONITORTYPE__)));

	return (result);
}

// ---
void QGAMES::Character::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1) && cfg.existOpenValue (lE - 2) &&
			cfg.existSetOfOpenValues (lNE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	int mId = cCfg.openValue (lE).intValue ();
	if (mId != __MININT__)
		addControlStepsMonitor (game () -> characterMonitorBuilder () -> monitorFor (mId, this)); 
		// The monitor is initialized with this instruction...
		// The must be a builder for the game, otherwise this instruction will crash!
	cCfg.removeOpenValue (lE); // Just treated...

	// The character monitor might have been added after the runtime information was got...
	QGAMES::SetOfOpenValues mSS = cCfg.setOfOpenValues (lNE);
	assert (mSS.name () == std::string (__QGAMES_RUNTIMEVALUESCHRMONITORTYPE__)); // Just to check the structure is going ok...
	if (stepsMonitor () && mSS.numberValues () != 0) // With something...
		stepsMonitor () -> initializeRuntimeValuesFrom (mSS);
		// ...and with this other one its status will be changed (monitor)...
	cCfg.removeSetOfOpenValues (lNE); // Just treated...
	
	_acceleration = QGAMES::Vector (cCfg.openValue (lE - 1).strValue ());
	cCfg.removeOpenValue (lE - 1); // Just treated...

	_direction = QGAMES::Vector (cCfg.openValue (lE - 2).strValue ());
	cCfg.removeOpenValue (lE - 2); // Just treated...

	QGAMES::Entity::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::Character::addControlStepsMonitor (QGAMES::CharacterControlStepsMonitor* s)
{
	if (_stepsControlMonitor != NULL)
	{
		unObserve (_stepsControlMonitor);
		delete (_stepsControlMonitor);
	}

	_stepsControlMonitor = s;
	if (_stepsControlMonitor != NULL)
	{
		observe (_stepsControlMonitor);
		_stepsControlMonitor -> initialize (); // Inits the monitor...
	}
}

// ---
void QGAMES::Character::updatePositions ()
{
	QGAMES::Entity::updatePositions ();
	if (_currentMovement != NULL) 
		_currentMovement -> move (_direction, _acceleration, this);
	if (_stepsControlMonitor != NULL)
		_stepsControlMonitor -> run (); 
}

// ---
void QGAMES::BasicCharacter::initialize ()
{
	QGAMES::Character::initialize ();

	// Sets the first state by default...
	if (!_states.empty ())
		setCurrentState ((*_states.begin ()).second -> id ());
}

// ---
QGAMES::Entity* QGAMES::CompositeEntity::clone () const
{
	QGAMES::Entities eties;
	for (QGAMES::Entities::const_iterator i = _entities.begin (); i != _entities.end (); i++)
		eties [(*i).first] = (*i).second -> clone (); // To clone the composition, is is needed to clone everything...

	QGAMES::CompositeEntity* result = new QGAMES::CompositeEntity (_id, eties, _data);
	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());
	// To clone a composite it is needed to clone everything...

	return (result);
}

// ---
QGAMES::SetOfOpenValues QGAMES::CompositeEntity::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Entity::runtimeValues ();

	int lNE = result.lastNestedOpenValueId ();

	QGAMES::SetOfOpenValues eSS (std::string (__QGAMES_RUNTIMEVALUESENTITIESTYPE__));
	for (QGAMES::Entities::const_iterator i = _entities.begin (); i != _entities.end (); i++)
		eSS.addSetOfOpenValues ((*i).second -> id (), (*i).second -> runtimeValues ());
	result.addSetOfOpenValues (lNE + 1, eSS); // It could be empty...

	return (result);
}

// ---
void QGAMES::CompositeEntity::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existSetOfOpenValues (lNE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	QGAMES::SetOfOpenValues eSS = cCfg.setOfOpenValues (lNE);
	assert (eSS.name () == std::string (__QGAMES_RUNTIMEVALUESENTITIESTYPE__)); // Just to check whether the structure is going ok...
	for (QGAMES::Entities::const_iterator i = _entities.begin (); i != _entities.end (); i++)
		if (eSS.existSetOfOpenValues ((*i).second -> id ())) // Only it it exists...
			(*i).second -> initializeRuntimeValuesFrom (eSS.setOfOpenValues ((*i).second -> id ()));
	cCfg.removeSetOfOpenValues (lNE); // Just treated...

	QGAMES::Entity::initializeRuntimeValuesFrom (cCfg);
}

// ---
QGAMES::Box QGAMES::CompositeEntity::roundingBox () const
{
	if (_entities.empty ())
		return (QGAMES::Box::_noBox);

	QGAMES::Entities::const_iterator i = _entities.begin ();
	QGAMES::Box result = (*i).second -> roundingBox ();
	for (; i != _entities.end (); i++)
		if ((*i).second -> isVisible ())
			result += (*i).second -> roundingBox ();

	return (result);
}

// ---
bool QGAMES::CompositeEntity::hasCollisionWith (QGAMES::Entity* e) const
{
	bool result = false;
	for (QGAMES::Entities::const_iterator i = _entities.begin (); 
			i != _entities.end () && !result; i++)
		result |= (*i).second -> isVisible () && (*i).second -> hasCollisionWith (e);
	return (result);
}

// ---
void QGAMES::CompositeEntity::initialize ()
{
	for (QGAMES::Entities::const_iterator i = _entities.begin (); 
			i != _entities.end (); i++)
		(*i).second -> initialize ();
	QGAMES::Entity::initialize ();
}

// ---
void QGAMES::CompositeEntity::inEveryLoop ()
{
	for (QGAMES::Entities::const_iterator i = _entities.begin (); 
			i != _entities.end (); i++)
		(*i).second -> inEveryLoop ();
	QGAMES::Entity::inEveryLoop ();
}

// ---
void QGAMES::CompositeEntity::updatePositions ()
{
	for (QGAMES::Entities::const_iterator i = _entities.begin (); 
			i != _entities.end (); i++)
		(*i).second -> updatePositions ();
	QGAMES::Entity::updatePositions ();
}

// ---
void QGAMES::CompositeEntity::drawOn (Screen* s, const QGAMES::Position& p)
{
	assert (s);

	for (QGAMES::Entities::const_iterator i = _entities.begin (); 
			i != _entities.end (); i++)
		(*i).second -> drawOn (s, 
			(p != QGAMES::Position::_noPoint) ? p + (*i).second -> position () : p);
	// A composite entity has to be drawn in each place unless a specific position is passed
	// that is taken as reference of the individual elements...
}

// ---
void QGAMES::CompositeEntity::finalize ()
{
	for (QGAMES::Entities::const_iterator i = _entities.begin (); 
			i != _entities.end (); i++)
		(*i).second -> finalize ();
	QGAMES::Entity::finalize ();
}

// ---
void QGAMES::CompositeEntity::setAlphaLevel (int aL)
{
	for (QGAMES::Entities::const_iterator i = _entities.begin (); 
			i != _entities.end (); i++)
		(*i).second -> setAlphaLevel (aL); 
	QGAMES::Entity::setAlphaLevel (aL);
}

// ---
void QGAMES::CompositeEntity::whenCollisionWith (QGAMES::Entity* e)
{
	assert (e);

	for (QGAMES::Entities::const_iterator i = _entities.begin (); 
			i != _entities.end (); i++)
		(*i).second -> whenCollisionWith (e);
}

// ---
void QGAMES::CompositeEntity::deleteEntities ()
{
	for (QGAMES::Entities::iterator i = _entities.begin ();
			i != _entities.end (); i++)
		delete (*i).second;
	_entities.clear ();
}

// ---
QGAMES::Positions QGAMES::CompositeEntity::getPointsFromVisibleEntities 
	(QGAMES::Position (QGAMES::Entity::*func)() const) const
{
	QGAMES::Positions result;
	for (QGAMES::Entities::const_iterator i = _entities.begin (); i != _entities.end (); i++)
		if ((*i).second -> isVisible ())
			result.push_back ((*((*i).second).*func)());

	return (result);
}

// ---
void QGAMES::ScoreObject::initialize ()
{ 
	assert (!_forms.empty ()); // One form has to exist minimum...

	QGAMES::Entity::initialize ();
	_currentForm = (*_forms.begin ()).second;
	_currentAspect = 0; 
}

// ---
QGAMES::Entity* QGAMES::ScoreObjectDigit::clone () const
{
	QGAMES::Entity* result = new QGAMES::ScoreObjectDigit (_id);
	result -> initialize ();
	return (result);
}

// ---
QGAMES::ScoreObjectText::ScoreObjectText (int id, const std::string& t,
	const Forms& f, int fB)
	: QGAMES::CompositeEntity (id, QGAMES::Entities ()),
	  _text (t),
	  _maxTextLength (t.length ()),
	  _textDirection (QGAMES::ScoreObjectText::TextDirection::_HORIZONTALLTOR),
	  _space (0),
	  _frameBase (fB)
{
	fixInternalEntities ();
}

// ---
int QGAMES::ScoreObjectText::visualLength () const
{
	int result = 0; int max = 0;
	for (QGAMES::Entities::const_iterator i = _entities.begin ();
			i != _entities.end (); i++)
	{
		QGAMES::Entity* lT = (*i).second;
		int w = lT -> currentForm () -> frameWidthForFrame (lT -> currentAspect ());
		result += (w + _space);
		if (max < w) 
			max = w;
	}

	if (_textDirection == QGAMES::ScoreObjectText::_VERTICALDTOU ||
			_textDirection == QGAMES::ScoreObjectText::_VERTICALUTOD)
		result = max;

	return (result);
}

// ---
int QGAMES::ScoreObjectText::visualHeight () const
{
	int result = 0; int max = 0;
	for (QGAMES::Entities::const_iterator i = _entities.begin ();
			i != _entities.end (); i++)
	{
		QGAMES::Entity* lT = (*i).second;
		int w = lT -> currentForm () -> frameHeightForFrame (lT -> currentAspect ());
		result += (w + _space);
		if (max < w) 
			max = w;
	}

	if (_textDirection == QGAMES::ScoreObjectText::_HORIZONTALLTOR ||
			_textDirection == QGAMES::ScoreObjectText::_HORIZONTALRTOL)
		result = max;

	return (result);
}

// ---
void QGAMES::ScoreObjectText::setText (const std::string& t)
{
	_text = t;

	fixInternalEntities ();
}

// ---
void QGAMES::ScoreObjectText::setMaxTextLength (int mL)
{
	assert (mL > 0);

	_maxTextLength = mL;

	fixInternalEntities ();
}

// ---
void QGAMES::ScoreObjectText::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	int aPos = 0;
	QGAMES::Vector vDir = QGAMES::Vector::_cero;
	if (_textDirection == QGAMES::ScoreObjectText::_HORIZONTALRTOL)
		vDir = QGAMES::Vector (__BD -1, __BD 0, __BD 0);
	if (_textDirection == QGAMES::ScoreObjectText::_HORIZONTALLTOR)
		vDir = QGAMES::Vector (__BD 1, __BD 0, __BD 0);
	if (_textDirection == QGAMES::ScoreObjectText::_VERTICALUTOD)
		vDir = QGAMES::Vector (__BD 0, __BD 1, __BD 0);
	if (_textDirection == QGAMES::ScoreObjectText::_VERTICALDTOU)
		vDir = QGAMES::Vector (__BD 0, __BD -1, __BD 0);

	QGAMES::Position iPos = (p == QGAMES::Position::_noPoint) ? _data._position : p;
	for (QGAMES::Entities::const_iterator i = _entities.begin ();
			i != _entities.end (); i++)
	{
		QGAMES::Entity* lT = (*i).second;
		lT -> drawOn (s, (iPos + ((__BD aPos) * vDir)));
		aPos += (vDir.posX () != 0) 
			? lT -> currentForm () -> frameWidthForFrame (lT -> currentAspect ()) + _space
			: lT -> currentForm () -> frameHeightForFrame (lT -> currentAspect ()) + _space;
	}
}

// ---
void QGAMES::ScoreObjectText::fixInternalEntities ()
{
	if (_forms.size () == 0)
		return; // If there is no forms, there is nothing to draw with!...

	for (QGAMES::Entities::const_iterator i = _entities.begin ();
			i != _entities.end (); i++)
		delete ((*i).second);
	_entities.clear (); // Clears the previous content...

	if ((int) _text.length () <= _maxTextLength)
		_text = std::string (_maxTextLength, ' ').substr (0, _maxTextLength - _text.length ()) + _text;
	else
		_text = _text.substr (0, _maxTextLength);

	for (int j = 0; j < _maxTextLength; j++)
	{
		QGAMES::ScoreObjectDigit* d = new QGAMES::ScoreObjectDigit (_id + j + 1, _forms, _data); 
		d -> setCurrentForm (formToDrawLetters ()); 
		d -> setCurrentAspect (_text [j] - ' ' + _frameBase); // The aspect is the letter...
		d -> setAlphaLevel (_alphaLevel);
		_entities.insert (QGAMES::Entities::value_type (d -> id (), d));
	}
}

// ---
QGAMES::ScoreObjectNumber::ScoreObjectNumber (int id, int n, 
				const Forms& f, int fB)
	: QGAMES::CompositeEntity (id, QGAMES::Entities ()),
	  _number (n),
	  _space (0),
	  _numberLength (-1),
	  _numberDirection (QGAMES::ScoreObjectNumber::NumberDirection::_HORIZONTALLTOR),
	  _frameBase (fB),
	  _ceroAtLeft ('0')
{
	std::stringstream a; a << n;
	_numberAsAText = a.str ();
	_numberLength = _numberAsAText.length ();

	fixInternalEntities ();
}

// ---
int QGAMES::ScoreObjectNumber::visualLength () const
{
	int result = 0; int max = 0;
	for (QGAMES::Entities::const_iterator i = _entities.begin ();
			i != _entities.end (); i++)
	{
		QGAMES::Entity* lT = (*i).second;
		int w = lT -> currentForm () -> frameWidthForFrame (lT -> currentAspect ());
		result += (w + _space);
		if (max < w) 
			max = w;
	}

	if (_numberDirection == QGAMES::ScoreObjectNumber::_VERTICALDTOU ||
			_numberDirection == QGAMES::ScoreObjectNumber::_VERTICALUTOD)
		result = max;

	return (result);
}

// ---
int QGAMES::ScoreObjectNumber::visualHeight () const
{
	int result = 0; int max = 0;
	for (QGAMES::Entities::const_iterator i = _entities.begin ();
			i != _entities.end (); i++)
	{
		QGAMES::Entity* lT = (*i).second;
		int w = lT -> currentForm () -> frameHeightForFrame (lT -> currentAspect ());
		result += (w + _space);
		if (max < w) 
			max = w;
	}

	if (_numberDirection == QGAMES::ScoreObjectNumber::_HORIZONTALLTOR ||
			_numberDirection == QGAMES::ScoreObjectNumber::_HORIZONTALRTOL)
		result = max;

	return (result);
}

// ---
void QGAMES::ScoreObjectNumber::setNumber (int n)
{ 
	_number = n;
	std::stringstream a; a << _number;
	_numberAsAText = a.str ();

	fixInternalEntities ();
}

// ---
void QGAMES::ScoreObjectNumber::setNumberLength (int nL)
{
	assert (nL > 0);

	_numberLength = nL;

	fixInternalEntities ();
}

// ---
void QGAMES::ScoreObjectNumber::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	int aPos = 0;
	QGAMES::Vector vDir = QGAMES::Vector::_cero;
	if (_numberDirection == QGAMES::ScoreObjectText::_HORIZONTALRTOL)
		vDir = QGAMES::Vector (__BD -1, __BD 0, __BD 0);
	if (_numberDirection == QGAMES::ScoreObjectText::_HORIZONTALLTOR)
		vDir = QGAMES::Vector (__BD 1, __BD 0, __BD 0);
	if (_numberDirection == QGAMES::ScoreObjectText::_VERTICALUTOD)
		vDir = QGAMES::Vector (__BD 0, __BD 1, __BD 0);
	if (_numberDirection == QGAMES::ScoreObjectText::_VERTICALDTOU)
		vDir = QGAMES::Vector (__BD 0, __BD -1, __BD 0);

	QGAMES::Position iPos = (p == QGAMES::Position::_noPoint) ? _data._position : p;
	for (QGAMES::Entities::const_iterator i = _entities.begin ();
			i != _entities.end (); i++)
	{
		QGAMES::Entity* n = (*i).second;
		n -> drawOn (s, (iPos + ((__BD aPos) * vDir)));
		aPos += (vDir.posX () != 0) 
			? n -> currentForm () -> frameWidthForFrame (n -> currentAspect ()) + _space
			: n -> currentForm () -> frameHeightForFrame (n -> currentAspect ()) + _space;
	}
}

// ---
void QGAMES::ScoreObjectNumber::fixInternalEntities ()
{
	if (_forms.size () == 0)
		return; // If there is no forms, there is nothing to draw with!...

	for (QGAMES::Entities::const_iterator i = _entities.begin ();
			i != _entities.end (); i++)
		delete ((*i).second);
	_entities.clear (); // Clears the previous content...

	if ((int) _numberAsAText.length () < (int) _numberLength)
		_numberAsAText = std::string (_numberLength, _ceroAtLeft).
			substr (0, _numberLength - _numberAsAText.length ()) + _numberAsAText;
	else
		_numberAsAText = _numberAsAText.substr (0, _numberLength);

	for (int j = 0; j < _numberLength; j++)
	{
		QGAMES::ScoreObjectDigit* d = new QGAMES::ScoreObjectDigit (_id + j + 1, _forms, _data); 
		d -> setCurrentForm (formToDrawDigits ());
		d -> setCurrentAspect (_numberAsAText [j] - '0' + _frameBase); // The aspect is the letter...
		_entities.insert (QGAMES::Entities::value_type (d -> id (), d));
	}
}
