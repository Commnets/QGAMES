#include <Common/widgetbuilder.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/widgetsystem.hpp>
#include <Common/widget.hpp>
#include <Common/game.hpp>

// ---
QGAMES::GUISystemBuilder::GUISystemBuilder (const std::string& fDef, QGAMES::FormBuilder* fB)
	: _definitions (),
	  _externalEngine (NULL), // No external engine by default!
	  _formBuilder (fB), // It can't be null...
	  _definitionFile (fDef),
	  _basePath (QGAMES::onlyPath (fDef))
{
	assert (_formBuilder);

	if (_basePath == std::string (__NULL_STRING__)) 
		_basePath = std::string (".");

	readFile (__RL (_definitionFile));
}

// ---
QGAMES::GUISystem* QGAMES::GUISystemBuilder::system (int nS)
{
	__WLOGPLUS (std::string ("Loading GUI system: ") + std::to_string (nS));

	// Look for the definition...
	QGAMES::GUISystemBuilder::Definitions::const_iterator j = _definitions.find (nS);
	assert (j != _definitions.end ()); // it has to exist...
	QGAMES::GUISystemBuilder::GUISystemDefinition sysDef = (*j).second;

	// Wen a system is requested, it is sure that all internal references exists
	// But it doesn't mean all widget can be created and that the main widget refered is a composite one
	// So it it time to verify all those things before creating the final result...

	// Then try to create all widgets individually...
	std::map <int, QGAMES::Widget*> widgets;
	for (std::map <int, QGAMES::GUISystemBuilder::WidgetDefinition>::const_iterator i = sysDef._widgets.begin ();
			i != sysDef._widgets.end (); i++)
	{
		QGAMES::GUISystemBuilder::WidgetDefinition wDef = (*i).second;
		QGAMES::Widget* widget = createWidget (wDef);
		assert (widget); // Valid?
		widget -> setActive (wDef._active); // Whether active or not?
		QGAMES::CompositeWidget* cWidget = dynamic_cast <QGAMES::CompositeWidget*> (widget);
		assert ((!cWidget && wDef._widgets.size () == (int) 0) || (cWidget && wDef._widgets.size () != (int) 0)); // Is valid?

		// Now it is time to add the decorator for the widget...
		// First of all, create all the decorators attached to the widget
		QGAMES::Widget::ListOfDecorators decs;
		for (std::vector <QGAMES::GUISystemBuilder::DecoratorDefinition>::const_iterator j = wDef._decorators.begin ();
				j != wDef._decorators.end (); j++)
		{
			QGAMES::Widget::Decorator* dec = createDecorator ((*j));
			assert (dec); // Is valid?
			dec -> initializeFrom ((*j)._properties); // The decorator can be initialized...
			decs.push_back (dec);
		}

		// Then If there is more than 1 decorator, it is needed a composite one...
		QGAMES::Widget::Decorator* decToAdd = NULL;
		if (decs.size () > 0)
		{
			if (decs.size () > 1)
				decToAdd = new QGAMES::Widget::ComplexDecorator (decs);
			else
				decToAdd = *(decs.begin ()); // Otherwise just a simple one to add...
		}

		widget -> setDecorator (decToAdd); // Adds the decorator (if any)...
		widget -> setExternalEngine (externalEngine ()); // Adds a reference to the external engin if any...
		widget -> setPosition ((*i).second._position); // set its position 
		widgets [widget -> id ()] = widget; // ...and then adds it to the list of widgets part of the system...
	}

	// If after that there are errors, or the main widget declared is not a composite one,
	// all widgets will be destroyed and nothing will be returned...
	QGAMES::CompositeWidget* mainWidget = 
		dynamic_cast <QGAMES::CompositeWidget*> ((*(widgets.find (sysDef._mainWidget))).second);
	assert (mainWidget);
	assert (mainWidget -> isActive ()); // It has to be active obviously!...

	// It is time to create the system, if possible...
	beforeCreateElement (sysDef._id);
	QGAMES::GUISystem* result = createSystem (sysDef, mainWidget);
	assert (result);

	// Now everything can be completed, 
	// because the widget definition is almost perfect!...
	for (std::map <int, QGAMES::GUISystemBuilder::WidgetDefinition>::const_iterator i = sysDef._widgets.begin ();
			i != sysDef._widgets.end (); i++)
	{
		QGAMES::GUISystemBuilder::WidgetDefinition wDef = (*i).second;
		QGAMES::Widget* widget = (*(widgets.find ((*i).first))).second;
		for (std::vector <int>::const_iterator j = wDef._widgets.begin (); 
				j != wDef._widgets.end (); j++)
			((QGAMES::CompositeWidget*) widget) -> addWidget ((*(widgets.find ((*j)))).second); 
			// We are sure it is a composite...(tested above)
		widget -> initializeFrom (wDef._properties); // Initialize the widget from the properties...
	}

	// Initialize the system...
	result -> initialize ();

	// What the system has to do just after creating the graphical interface...
	afterCreateElement (result);
	
	// ...and returned to be used.
	return (result);
}

// ---
void QGAMES::GUISystemBuilder::readFile (const std::string& fDef)
{
	__WLOGPLUS (std::string ("Reading GUI systems definition file: ") + fDef);

	TiXmlDocument doc (fDef.c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the root element doesn't call "GUISystems" the system will work too...
	// Just to speed up theloading!

	for (TiXmlElement* defElement = rootElement -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		// The line can be an include line...
		// That line can be at any place in the movement definition file...
		if (strcmp (defElement -> Value (), __QGAMES_INCLUDENAME__) == 0)
		{
			std::string nF = defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__);
			readFile (__RL (nF));
		}
		else
		// The other type of line can be a simple movement line...
		if (strcmp (defElement -> Value (), __QGAMES_GUISYSTEMTAGNAME__) == 0)
		{
			QGAMES::GUISystemBuilder::GUISystemDefinition def = 
				readSystemDefinition (defElement);
			
			// After that the system definition can contain widgets with references that doesn't exist
			// It is time to make the cross check, before adding the gui system reference to the list
			// With any error the reference will not be added....
			bool allRefs = true;
			for (std::map <int, QGAMES::GUISystemBuilder::WidgetDefinition>::const_iterator i = def._widgets.begin ();
					i != def._widgets.end (); i++)
			{
				QGAMES::GUISystemBuilder::WidgetDefinition wDef = (*i).second;
				for (std::vector <int>::const_iterator j = wDef._widgets.begin (); j != wDef._widgets.end (); j++)
					if (def._widgets.find ((*j)) == def._widgets.end ())
						allRefs = false; // One reference is not well defined...tehn everything is wrong!
			}

			// If everything is ok, then it is added!, otherwise it isn't (takes later during the game)
			if (def._id != -1 && // Valid...
				def._widgets.find (def._mainWidget) != def._widgets.end () && // With the right main widget...
				allRefs) // ..and with all the refs well defined
				_definitions.insert (QGAMES::GUISystemBuilder::Definitions::value_type (def._id, def));
		}
	}
}

// ---
QGAMES::GUISystemBuilder::GUISystemDefinition QGAMES::GUISystemBuilder::readSystemDefinition (TiXmlElement* s)
{
	QGAMES::GUISystemBuilder::GUISystemDefinition result;

	s -> Attribute (__QGAMES_GUISYSTEMATTRID__, &result._id);
	s -> Attribute (__QGAMES_GUISYSTEMATTRMAINWIDGET__, &result._mainWidget);
	for (TiXmlElement* wElement = s -> FirstChildElement ();
		wElement != NULL; wElement = wElement -> NextSiblingElement ())
	{
		if (strcmp (wElement -> Value (), __QGAMES_WIDGETTAGNAME__) == 0)
		{
			QGAMES::GUISystemBuilder::WidgetDefinition wDef = 
				readWidgetDefinition (wElement);
			assert (wDef._id != -1); // Is valid?
			result._widgets [wDef._id] = wDef; // Adds a reference to the widget...
		}
	}

	return (result);
}

// ---
QGAMES::GUISystemBuilder::WidgetDefinition QGAMES::GUISystemBuilder::readWidgetDefinition (TiXmlElement* w)
{
	QGAMES::GUISystemBuilder::WidgetDefinition result;

	w -> Attribute (__QGAMES_WIDGETATTRID__, &result._id);
	w -> Attribute (__QGAMES_WIDGETATTRTYPE__, &result._type);

	std::string tStr (__YES_STRING__);
	if (w -> Attribute (__QGAMES_WIDGETATTRACTIVE__) != NULL) // It is not mandatory...
		tStr = w -> Attribute (__QGAMES_WIDGETATTRACTIVE__);
	result._active = (tStr == std::string (__YES_STRING__));

	std::string pos (__NULL_STRING__);
	if (w -> Attribute (__QGAMES_WIDGETATTRPOSITION__) != NULL)
		pos = w -> Attribute (__QGAMES_WIDGETATTRPOSITION__);
	std::vector <int> posV = QGAMES::getElementsFromAsInt (pos, ',', 3);
	result._position = QGAMES::Position (__BD posV [0], __BD posV [1], __BD posV [2]);

	for (TiXmlElement* wElement = w -> FirstChildElement ();
		wElement != NULL; wElement = wElement -> NextSiblingElement ())
	{
		if (strcmp (wElement -> Value (), __QGAMES_REFTOWIDGETTAGNAME__) == 0)
		{
			int refId = -1;
			wElement -> Attribute (__QGAMES_REFWIDGETATTRID__, &refId);
			assert (refId != -1); // Is valid?
			result._widgets.push_back (refId); // We don't still know whether the refId widget exists!
											   // It is something to test at construction time...
		}
		else
		if (strcmp (wElement -> Value (), __QGAMES_WIDGETATTRSTAGNAME__) == 0)
		{
			std::map <std::string, std::string> prp = readProperties (wElement);
			result._properties.insert (prp.begin (), prp.end ()); // Many properties blocks allowed...
		}
		else
		if (strcmp (wElement -> Value (), __QGAMES_WIDGETDECORATORTAGNAME__) == 0)
		{
			QGAMES::StandardGUISystemBuilder::DecoratorDefinition dDef = readDecoratorDefinition (wElement);
			assert (dDef._type != -1); // Is valid?
			result._decorators.push_back (dDef);
		}
	}

	return (result);
}

// ---
QGAMES::GUISystemBuilder::DecoratorDefinition QGAMES::GUISystemBuilder::readDecoratorDefinition (TiXmlElement* d)
{
	QGAMES::GUISystemBuilder::DecoratorDefinition result;
	
	d -> Attribute (__QGAMES_WIDGETDECORATORATTRTYPE__, &result._type);
	for (TiXmlElement* dElement = d -> FirstChildElement ();
		dElement != NULL; dElement = dElement -> NextSiblingElement ())
	{
		if (strcmp (dElement -> Value (), __QGAMES_WIDGETDECORATORATTRSTAGNAME__) == 0)
		{
			std::map <std::string, std::string> prp = readProperties (dElement);
			result._properties.insert (prp.begin (), prp.end ()); // Many properties blocks allowed!
		}
	}

	return (result);
}

// ---
std::map <std::string, std::string> QGAMES::GUISystemBuilder::readProperties (TiXmlElement* p)
{
	std::map <std::string, std::string> result;
	for (TiXmlElement* prpElement = p -> FirstChildElement ();
		prpElement  != NULL; prpElement  = prpElement  -> NextSiblingElement ())
	{
		// If the element doesn't call "Attribute" the system still will work!
		// Just to speed up the loading...

		std::string id (__NULL_STRING__);
		if (prpElement -> Attribute (__QGAMES_WIDGETATTRSATTRID__) != NULL)
			id = prpElement -> Attribute (__QGAMES_WIDGETATTRSATTRID__);
		std::string value (__NULL_STRING__);
		if (prpElement -> Attribute (__QGAMES_WIDGETATTRSATTRVALUE__) != NULL)
			value = prpElement -> Attribute (__QGAMES_WIDGETATTRSATTRVALUE__);

		if (id != std::string (__NULL_STRING__))
			result [id] = value;
	}

	return (result);
}

// ---
QGAMES::GUISystem* QGAMES::StandardGUISystemBuilder::createSystem 
	(const QGAMES::GUISystemBuilder::GUISystemDefinition& def, QGAMES::CompositeWidget* mW)
{
	// The system by default is the one!...
	return (new QGAMES::GUISystem (mW));
}

// ---
QGAMES::Widget* QGAMES::StandardGUISystemBuilder::createWidget (const QGAMES::GUISystemBuilder::WidgetDefinition& def)
{
	QGAMES::Widget* result = NULL;

	if (def._type == __QGAMES_BASICCOMPOSITEWIDGETTYPE__)
		result = new QGAMES::CompositeWidget (def._id);
	else if (def._type == __QGAMES_LABELWIDGETTYPE__)
		result = new QGAMES::LabelWidget (def._id, NULL); // No form by default...
	else if (def._type == __QGAMES_INTROWIDGETTYPE__)
		result = new QGAMES::IntroWidget (def._id, NULL); // No form by default...
	else if (def._type == __QGAMES_ITERATINGLABELWIDGETTYPE__)
		result = new QGAMES::IteratingLabelWidget (def._id, NULL); // No form by default...
	else if (def._type == __QGAMES_ITERATINGLABELWIDGETKEYTYPE__)
		result = new QGAMES::KeyLabelWidget (def._id, NULL); // No form by default...

	return (result);
}

// ---
QGAMES::Widget::Decorator* QGAMES::StandardGUISystemBuilder::createDecorator 
	(const QGAMES::GUISystemBuilder::DecoratorDefinition& def)
{
	QGAMES::Widget::Decorator* result = NULL;
	if (def._type == __QGAMES_ROUNDINGDECORATORTYPE__)
		result = new QGAMES::Widget::RoundingDecorator ();
	return (result);
}
