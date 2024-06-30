#include <Common/entitybuilder.hpp>
#include <Common/formbuilder.hpp>
#include <Common/movementbuilder.hpp>
#include <Common/definitions.hpp>
#include <Common/genalgorithms.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/game.hpp>
#include <tinyxml.h>

// ---
QGAMES::EntityBuilder::EntityBuilder (const std::string& eDef, 
		QGAMES::FormBuilder* fB, QGAMES::MovementBuilder* mB)
	: _definitions (),
	  _setDefinitions (),
	  _entities (),
	  _formBuilder (fB),
	  _movementBuilder (mB),
	  _definitionFile (eDef),
	  _basePath (QGAMES::onlyPath (eDef))
{
	assert (_formBuilder);
	assert (_movementBuilder);

	if (_basePath == std::string (__NULL_STRING__)) 
		_basePath = std::string (".");

	readFile (__RL (_definitionFile));
	// Because there can be "include" lines, this method can be called recursevely...
	// At this point the definitionFile variable is storing the name of file to read,
	// and the list of definitions is empty...
}

// ---
QGAMES::EntityBuilder::~EntityBuilder ()
{
	QGAMES::Entities::const_iterator i;
	for (i = _entities.begin (); i != _entities.end (); i++)
		delete ((*i).second);
	_entities.clear ();
}

// ---
int QGAMES::EntityBuilder::numberResources () const
{
	int result = (int) _definitions.size ();
	for (QGAMES::EntityBuilder::Definitions::const_iterator i = _setDefinitions.begin ();
			i != _setDefinitions.end (); i++)
		result += (*i).second._number;
	return (result);
}

// ---
std::vector <int> QGAMES::EntityBuilder::resourcesId () const
{
	std::vector <int> result;
	for (QGAMES::EntityBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end (); i++)
		result.push_back ((*i).first);
	for (QGAMES::EntityBuilder::Definitions::const_iterator i = _setDefinitions.begin ();
			i != _setDefinitions.end (); i++)
		for (int j = 0; j < (*i).second._number; j++)
			result.push_back ((*i).second._id + j);
	return (result);
}

// ---
void QGAMES::EntityBuilder::preLoad ()
{
	// Load the entities...(and the composite ones)
	for (QGAMES::EntityBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end (); i++)
	{
		QGAMES::EntityBuilder::EntityDefinition def = (*i).second;
		if (_entities.find (def._id) == _entities.end ()) entity (def._id);
	}

	// Load the set of entities...
	for (QGAMES::EntityBuilder::Definitions::const_iterator i = _setDefinitions.begin ();
			i != _setDefinitions.end (); i++)
	{
		QGAMES::EntityBuilder::EntityDefinition def = (*i).second;
		for (int j = def._id; j < (def._id + def._number); j++)
			if (_entities.find (j) == _entities.end ()) entity (j);
	}
}

// ---
QGAMES::Entity* QGAMES::EntityBuilder::entity (int ne)
{
	QGAMES::Entities::const_iterator i = _entities.find (ne);
	if (i != _entities.end ())
		return ((*i).second);

	__WLOGPLUS (std::string ("Loading entity: ") + std::to_string (ne));
	
	beforeCreateElement (ne);

	QGAMES::Entity* result = NULL;
	QGAMES::EntityBuilder::Definitions::const_iterator j = _definitions.find (ne);
	QGAMES::EntityBuilder::EntityDefinition resultDef =
		(j == _definitions.end ()) ? groupDefinitionFor (ne) : (*j).second; // If it is a set a simple definition is returned
	assert (resultDef._id != -1); // Just in case, because the result of the groupDefinitionFor could be -1
	result = (resultDef._type == QGAMES::EntityBuilder::EntityDefinition::_COMPOSITE) 
		?  createCompositeEntity (resultDef) : createEntity (resultDef);
	// createMovement is an user's method, ant it could fail...
	assert (result);
	result -> setExternalEngine (externalEngine ()); // Adds the xternal engine if any...

	// Add the forms...(if it makes sense. e.g. in a composite entity it makes no sense at all)
	QGAMES::Forms forms;
	std::list <int>::const_iterator k;
	for (k = resultDef._forms.begin (); k != resultDef._forms.end (); k++)
		forms.insert (QGAMES::Forms::value_type ((*k), _formBuilder -> form ((*k)))); // Will fail if the forms id doesn't exist
	result -> setForms (forms);

	// ...and the movements...(same, only if it makes sense)
	QGAMES::Movements movements;
	std::list <int>::const_iterator l;
	for (l = resultDef._movements.begin (); l != resultDef._movements.end (); l++)
		movements.insert (QGAMES::Movements::value_type ((*l), 
			_movementBuilder -> movement ((*l)))); // Will fail if the movement id doesn't exist...
	result -> setMovements (movements);

	// ...and the animations (same, only if it makes sense)
	QGAMES::Entity::Animations animations;
	std::list <QGAMES::EntityBuilder::AnimationDefinition>::const_iterator m;
	for (m = resultDef._animations.begin (); m != resultDef._animations.end (); m++)
	{
		QGAMES::Entity::Animation* an = createAnimation ((*m));
		assert (an); // The animation has to be well created...
		animations.insert (QGAMES::Entity::Animations::value_type ((*m)._id, an));
	}

	result -> setAnimations (animations);

	// ...and the states... (some, only if it makes sense)
	QGAMES::Entity::States states;
	std::list <QGAMES::EntityBuilder::StateDefinition>::const_iterator s;
	for (s = resultDef._states.begin (); s != resultDef._states.end (); s++)
	{
		QGAMES::Entity::State* st = createState ((*s));
		assert (st); // The state has to be well created...
		states.insert (QGAMES::Entity::States::value_type ((*s)._id, st));
	}

	result -> setStates (states);

	// ...and of the entity is a composite one, then the entities are also added, but just a copy
	// beacause the original ones could also be loaded...
	if (resultDef._type == QGAMES::EntityBuilder::EntityDefinition::_COMPOSITE)
	{
		assert (dynamic_cast <QGAMES::CompositeEntity*> (result)); // // JUst to be sure...
		QGAMES::Entities eties = entities (resultDef._entities);
		QGAMES::Entities etiesCopy;
		for (QGAMES::Entities::const_iterator i = eties.begin (); i != eties.end (); i++)
			etiesCopy [(*i).first] = (*i).second -> clone ();
		((QGAMES::CompositeEntity*) result) -> setEntities (etiesCopy);
	}

	// Adds the entity to the list of those...
	_entities.insert (QGAMES::Entities::value_type (ne, result));
	
	afterCreateElement (result);

	return (result);
}

// ---
QGAMES::Entities QGAMES::EntityBuilder::entities (const std::list <int> ne)
{
	QGAMES::Entities result;
	for (std::list <int>::const_iterator i = ne.begin (); i != ne.end (); i++)
		result [(*i)] = entity ((*i)); // Adds the entities...
	return (result);
}

// ---
QGAMES::Entity::Animation* QGAMES::EntityBuilder::createAnimation 
	(const QGAMES::EntityBuilder::AnimationDefinition& def)
{ 
	QGAMES::Entity::Animation* result = NULL;

	switch (def._type)
	{
		case __QGAMES_ENTITYANIMATIONNOANTYPE__:
			result = new QGAMES::Entity::NoAnimation (def._id);
			break;

		case __QGAMES_ENTITYANIMATIONFORMANTYPE__:
			result = new QGAMES::Entity::FormAnimation (def._id, def._parameters);
			break;

		case __QGAMES_ENTITYANIMATIONEMPTYANTYPE__:
			result = new QGAMES::Entity::EmptyAnimation (def._id, def._parameters);
			break;

		default:
			assert (0); // It shouldn't be here...
	}

	return (result);
} 

// ---
QGAMES::Entity::State* QGAMES::EntityBuilder::createState (const StateDefinition& def)
{
	return (new QGAMES::Entity::State (def._id, def._animationId, def._movementId, def._parameters));
}

// ---
void QGAMES::EntityBuilder::readFile (const std::string& fDef)
{
	__WLOGPLUS (std::string ("Reading entities definition file: ") + fDef);

	TiXmlDocument doc (fDef.c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the root element doesn't call "Entities" the code will work too!
	// This is done to speed up the loading...

	for (TiXmlElement* defElement = rootElement -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		// The line can be an include line...
		// That line can be at any place in the entity definition file...
		if (strcmp (defElement -> Value (), __QGAMES_INCLUDENAME__) == 0)
		{
			assert (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)); 
			readFile (__RL (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)));
		}
		else
		// The other type of line could be a simple entity line...
		if (strcmp (defElement -> Value (), __QGAMES_ENTITYTAGNAME__) == 0)
		{
			QGAMES::EntityBuilder::EntityDefinition def = readEntityDefinition (defElement);
			assert (def._id != -1); // Is it a valid entity?
			_definitions.insert (QGAMES::EntityBuilder::Definitions::value_type (def._id, def));
		}
		else
		// Or a set of entities also
		if (strcmp (defElement -> Value (), __QGAMES_SETOFENTITIESTAGNAME__) == 0)
		{
			QGAMES::EntityBuilder::EntityDefinition def = readSetOfEntitiesDefinition (defElement);
			assert (def._id != -1 && def._number != 0); // Is it a valid entity?
			_setDefinitions.insert (QGAMES::EntityBuilder::Definitions::value_type (def._id, def));
		}
		else
		// Or a composite entity even...
		if (strcmp (defElement -> Value (), __QGAMES_COMPOSITEENTITYTAGNAME__) == 0)
		{
			QGAMES::EntityBuilder::EntityDefinition def = readCompositeEntityDefinition (defElement);
			assert (def._id != -1); // Is it a valid entity?
			_definitions.insert (QGAMES::EntityBuilder::Definitions::value_type (def._id, def));
		}
	}
}

// ---
QGAMES::EntityBuilder::EntityDefinition QGAMES::EntityBuilder::readEntityDefinition (TiXmlElement* e)
{
	assert (e);

	QGAMES::EntityBuilder::EntityDefinition result;
	e -> Attribute (__QGAMES_ENTITYATTRID__, &result._id);
	for (TiXmlElement* groupElement = e -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYFORMSBLOCK__) == 0)
			result._forms = readFormsGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYCAPACITIESBLOCK__) == 0)
			result._capacities = readCapacitiesGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYPHYSICSBLOCK__) == 0)
			result._physics = readPhysicsGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYATTRSBLOCK__) == 0)
			result._parameters = readAttributesGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENITYMOVEMENTSBLOCK__) == 0)
			result._movements = readMovementsGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYANIMATIONSBLOCK__) == 0)
			result._animations = readAnimationsGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYSTATESBLOCK__) == 0)
			result._states = readStatesGroup (groupElement);

	}

	result._type = QGAMES::EntityBuilder::EntityDefinition::Type::_SIMPLE;

	return (result);
}

// ---
QGAMES::EntityBuilder::EntityDefinition QGAMES::EntityBuilder::readSetOfEntitiesDefinition (TiXmlElement* e)
{
	assert (e);

	QGAMES::EntityBuilder::EntityDefinition result;
	e -> Attribute (__QGAMES_SETOFENTITIESATTRID__, &result._id);
	e -> Attribute (__QGAMES_SETOFENTITIESATTRNUMBER__, &result._number);
	for (TiXmlElement* groupElement = e -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYFORMSBLOCK__) == 0)
			result._forms = readFormsGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_SETOFENTITIESFORMSBASEBLOCK__) == 0)
			result._formsBase = readFormsGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYCAPACITIESBLOCK__) == 0)
			result._capacities = readCapacitiesGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYPHYSICSBLOCK__) == 0)
			result._physics = readPhysicsGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYATTRSBLOCK__) == 0)
			result._parameters = readAttributesGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENITYMOVEMENTSBLOCK__) == 0)
			result._movements = readMovementsGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_SETOFENTITIESMOVSBASEBLOCK__) == 0)
			result._movementsBase = readMovementsGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYANIMATIONSBLOCK__) == 0)
			result._animations = readAnimationsGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYSTATESBLOCK__) == 0)
			result._states = readStatesGroup (groupElement);
	}

	result._type = QGAMES::EntityBuilder::EntityDefinition::Type::_SET;

	return (result);
}

// ---
QGAMES::EntityBuilder::EntityDefinition QGAMES::EntityBuilder::readCompositeEntityDefinition (TiXmlElement* e)
{
	assert (e);

	QGAMES::EntityBuilder::EntityDefinition result;
	e -> Attribute (__QGAMES_COMPOSITEENTITYATTRID__, &result._id);
	for (TiXmlElement* groupElement = e -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __QGAMES_COMPOSITEENTYREFETYTAGNAME_) == 0)
		{
			int nE = -1;
			groupElement -> Attribute (__QGAMES_COMPOSITEENTYREFETYATTRID__, &nE);
			assert (nE != -1);
			result._entities.push_back (nE);
		}
		else
		if (strcmp (groupElement -> Value (), __QGAMES_COMPOSITEENTYREFSETYTAGNAME__) == 0)
		{
			int fE = -1; int nE = -1;
			groupElement -> Attribute (__QGAMES_COMPOSITEENTYREFSETYATTRFROM__, &fE);
			groupElement -> Attribute (__QGAMES_COMPOSITEENTYREFSETYATTRNUM__, &nE);
			assert (fE != -1 && nE != -1);
			for (int i = fE; i < (fE + nE); i++)
				result._entities.push_back (i);
		}
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYFORMSBLOCK__) == 0)
			result._forms = readFormsGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYCAPACITIESBLOCK__) == 0)
			result._capacities = readCapacitiesGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYPHYSICSBLOCK__) == 0)
			result._physics = readPhysicsGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYATTRSBLOCK__) == 0)
			result._parameters = readAttributesGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENITYMOVEMENTSBLOCK__) == 0)
			result._movements = readMovementsGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYANIMATIONSBLOCK__) == 0)
			result._animations = readAnimationsGroup (groupElement);
		else
		if (strcmp (groupElement -> Value (), __QGAMES_ENTITYSTATESBLOCK__) == 0)
			result._states = readStatesGroup (groupElement);
	}

	result._type = QGAMES::EntityBuilder::EntityDefinition::Type::_COMPOSITE;

	return (result);
}

// ---
std::list <int> QGAMES::EntityBuilder::readFormsGroup (TiXmlElement* g)
{
	assert (g);

	std::list <int> result;
	for (TiXmlElement* formElement = g -> FirstChildElement ();
		formElement != NULL; formElement = formElement -> NextSiblingElement ())
	{
		// If the element doesn't call "Form" the code will work too!
		// This is done to speed up the loading...
		
		int fId = -1;
		formElement -> Attribute (__QGAMES_ENTITYFORMSATTRID__, &fId);
		assert (fId != -1); // Is it a valid form?...
		result.push_back (fId);
	}

	return (result);
}

// ---
std::map <int, bool> QGAMES::EntityBuilder::readCapacitiesGroup (TiXmlElement* g)
{
	assert (g);

	std::map <int, bool> result;
	for (TiXmlElement* capacityElement = g -> FirstChildElement ();
		capacityElement != NULL; capacityElement = capacityElement -> NextSiblingElement ())
	{
		// If the element doesn't call "Capacity" the code will work too!
		// This is done to speed up the loading...
		
		int cId = -1; std::string onOff; bool on = false;
		capacityElement -> Attribute (__QGAMES_ENTITYCAPACITIESATTRID__, &cId);
		onOff = capacityElement -> Attribute (__QGAMES_ENTITYCAPACITIESATTRVALUE__);
		if (onOff == "true" || onOff == "TRUE") on = true;
		assert (cId != -1); // Is it a valid capacity definition?...
		result [cId] = on;
	}

	return (result);
}

// ---
std::map <int, double> QGAMES::EntityBuilder::readPhysicsGroup (TiXmlElement* g)
{
	assert (g);

	std::map <int, double> result;
	for (TiXmlElement* physicElement = g -> FirstChildElement ();
		physicElement != NULL; physicElement = physicElement -> NextSiblingElement ())
	{
		// If the element doesn't call "Physic" the code will work too!
		// This is done to speed up the loading...
		
		int pId = -1; double value = 0.0;
		physicElement -> Attribute (__QGAMES_ENTITYPHYSICSATTRID__, &pId);
		physicElement -> Attribute (__QGAMES_ENTITYPHYSICSATTRVALUE__, &value);
		assert (pId != -1); // Is it a valid physic definition?...
		result [pId] = value;
	}

	return (result);
}

// ---
std::list <int> QGAMES::EntityBuilder::readMovementsGroup (TiXmlElement* g)
{
	assert (g);

	std::list <int> result;
	for (TiXmlElement* movementElement = g -> FirstChildElement ();
		movementElement != NULL; movementElement = movementElement -> NextSiblingElement ())
	{
		// If the element doesn't call "Movement" the code will work too!
		// This is done to speed up the loading...
		
		int mId = -1;
		movementElement -> Attribute (__QGAMES_ENTITYMOVEMENTSATTRID__, &mId);
		assert (mId != -1); // Is it a valid movement?...
		result.push_back (mId);
	}

	return (result);
}

// ---
std::list <QGAMES::EntityBuilder::AnimationDefinition>
	QGAMES::EntityBuilder::readAnimationsGroup (TiXmlElement* g)
{
	assert (g);

	std::list <QGAMES::EntityBuilder::AnimationDefinition> result;
	for (TiXmlElement* aElement = g -> FirstChildElement ();
		aElement != NULL; aElement = aElement -> NextSiblingElement ())
	{
		int tp = 0; // Form by default...
		QGAMES::EntityBuilder::AnimationDefinition aDef;
		aElement -> Attribute (__QGAMES_ENTITYANIMATIONATTRID__, &aDef._id);
		assert (aDef._id != -1); // Is it valid?
		aElement -> Attribute (__QGAMES_ENTITYANIMATIONATTRTYPE__, &aDef._type);
		for (TiXmlElement* attrElement = aElement -> FirstChildElement ();
			attrElement != NULL; attrElement = attrElement -> NextSiblingElement ())
			if (strcmp (attrElement -> Value (), __QGAMES_ENTITYATTRSBLOCK__) == 0)
				aDef._parameters = readAttributesGroup (attrElement);
		result.push_back (aDef);
	}

	return (result);
}

// ---
std::list <QGAMES::EntityBuilder::StateDefinition> QGAMES::EntityBuilder::readStatesGroup (TiXmlElement* g)
{
	assert (g);

	std::list <QGAMES::EntityBuilder::StateDefinition> result;
	for (TiXmlElement* aElement = g -> FirstChildElement ();
		aElement != NULL; aElement = aElement -> NextSiblingElement ())
	{
		QGAMES::EntityBuilder::StateDefinition aDef;
		aElement -> Attribute (__QGAMES_ENTITYSTATEATTRID__, &aDef._id);
		aElement -> Attribute (__QGAMES_ENTITYSTATEATTRANIMATION__, &aDef._animationId);
		aElement -> Attribute (__QGAMES_ENTITYSTATEATTRMOVEMENT__, &aDef._movementId);
		aElement -> Attribute (__QGAMES_ENTITYSTATEATTRMOVEMENTBASE__, &aDef._movementBaseId);
		assert (aDef._id != -1 && aDef._animationId != -1 && 
				(aDef._movementId != -1 || aDef._movementBaseId != -1)); // Is it valid?
		for (TiXmlElement* attrElement = aElement -> FirstChildElement ();
			attrElement != NULL; attrElement = attrElement -> NextSiblingElement ())
			if (strcmp (attrElement -> Value (), __QGAMES_ENTITYATTRSBLOCK__) == 0)
				aDef._parameters = readAttributesGroup (attrElement);
		result.push_back (aDef);
	}

	return (result);
}

// ---
std::map <std::string, std::string> QGAMES::EntityBuilder::readAttributesGroup (TiXmlElement* g)
{
	assert (g);

	std::map <std::string, std::string> result;
	for (TiXmlElement* aElement = g -> FirstChildElement ();
		aElement != NULL; aElement = aElement -> NextSiblingElement ())
	{
		// If the element doesn't call "Attribute" the code will work too!
		// This is done to speed up the loading...
		
		std::string id, value;
		id = aElement -> Attribute (__QGAMES_ENTITYATTRATTRID__);
		value = aElement -> Attribute (__QGAMES_ENTITYATTRATTRVALUE__);
		assert (id != std::string (__NULL_STRING__)); // Is it a valid variable definition?...
		result.insert (std::map <std::string, std::string>::value_type (id, value));
	}

	return (result);
}

// ---
QGAMES::EntityBuilder::EntityDefinition QGAMES::EntityBuilder::groupDefinitionFor (int ne)
{
	QGAMES::EntityBuilder::EntityDefinition result;
	for (QGAMES::EntityBuilder::Definitions::const_iterator i = _setDefinitions.begin (); 
			i != _setDefinitions.end () && result._id == -1; i++) // Look for it in the set...
	{
		QGAMES::EntityBuilder::EntityDefinition def = (*i).second;
		if (ne >= def._id && ne < (def._id + def._number))
		{
			result._id = ne;
			result._type = QGAMES::EntityBuilder::EntityDefinition::Type::_SIMPLE;
			result._number = 0;
			result._forms = def._forms;
			result._formsBase = std::list <int> ();
			for (std::list <int>::const_iterator j = def._formsBase.begin (); 
					j != def._formsBase.end (); j++)
				result._forms.push_back ((*j) + ne - def._id);
			result._movements = def._movements;
			result._movementsBase = std::list <int> ();
			for (std::list <int>::const_iterator j = def._movementsBase.begin (); 
					j != def._movementsBase.end (); j++)
				result._movements.push_back ((*j) + ne - def._id);
			result._capacities = def._capacities;
			result._physics = def._physics;

			// In a set, the FORM animations have to take into account the formsBase (if it exists)
			result._animations = def._animations;
			if (!def._formsBase.empty ())
			{
				result._animations = std::list <QGAMES::EntityBuilder::AnimationDefinition> ();
				for (std::list <QGAMES::EntityBuilder::AnimationDefinition>::const_iterator j = def._animations.begin (); 
						j != def._animations.end (); j++)
				{
					QGAMES::EntityBuilder::AnimationDefinition aDef = (*j);
					if (aDef._type == 0 /** Form Animation. */ && 
						aDef._parameters.find (__QGAMES_ENTITYANATTRFORM__) != aDef._parameters.end () && 
						std::find (def._formsBase.begin (), def._formsBase.end (), 
							std::atoi (((*aDef._parameters.find (__QGAMES_ENTITYANATTRFORM__)).second).c_str ())) != def._formsBase.end ())
						aDef._parameters [__QGAMES_ENTITYANATTRFORM__] = 
							std::to_string (std::atoi (aDef._parameters [__QGAMES_ENTITYANATTRFORM__].c_str ()) + ne - def._id);
				}
			}

			// In a set, the STATES have to take into account the movementBase (it they exists)
			result._states = def._states;
			if (!def._movementsBase.empty ())
			{
				result._states = std::list <QGAMES::EntityBuilder::StateDefinition> ();
				for (std::list <QGAMES::EntityBuilder::StateDefinition>::const_iterator j = def._states.begin (); 
						j != def._states.end (); j++)
				{
					QGAMES::EntityBuilder::StateDefinition stDef = (*j);
					if (std::find (def._movementsBase.begin (), def._movementsBase.end (),
							stDef._movementBaseId) != def._movementsBase.end ())
						stDef._movementId = stDef._movementBaseId + ne - def._id;
					stDef._movementBaseId = -1; // No onger needed...
					result._states.push_back (stDef); 
				}
			}
		}
	}

	return (result);
}