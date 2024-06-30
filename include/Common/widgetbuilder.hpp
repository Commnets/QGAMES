/** \ingroup Game */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: widget.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 28/12/2018\n
 *	Description: Defines a class to represent fix elements in the screen usually over everything. \n
 *	Versions: 1.0 Initial
 */


#ifndef __QGAMES_WIDGETBUILDER__
#define __QGAMES_WIDGETBUILDER__

#include <Common/widget.hpp>
#include <tinyxml.h>
#include <map>

namespace QGAMES
{
	class FormBuilder;

	/** 
	  *	A builder for GUI systems. \n
	  * The definition is as follows: \n
	  *	<GUISystems> \n
	  *		<GUISystem id="XXX" main="YYY"> \n
	  *			<Widget id="XXX" type="YYY" position="ZZZ"> \n
	  *				<Decorator type="XX"> \n
	  *					<Attributes> (they are optional) \n
	  *						<Attribute id="XXX" value="xxx"/> \n
	  *						... \n
	  *					</Attributes> \n
	  *				... \n
	  *				<RefWidget id="XXX"/> \n
	  *				... \n
	  *				<Attributes> \n
	  *					<Attribute id="XXX" value="XXXX"/> \n
	  *					... \n
	  *				</Attributes> \n
	  *			</Widget> \n
	  *			... \n
	  *		</GUISystem> \n
	  *		<GUISystem id="XXX" main="YYY"> \n
	  *			<Widget id="XXX" type="YYY" position="ZZZ"> \n
	  *				<Decorator type="XX"/> \n
	  *				... \n
	  *				<RefWidget id="XXX"/> \n
	  *				... \n
	  *				<Attributes> \n
	  *					<Attribute id="XXX" value="XXXX"/> \n
	  *					... \n
	  *				</Attributes> \n
	  *			</Widget> \n
	  *			... \n
	  *		</GUISystem> \n
	  *	</GUISystems> \n
	  */
	class GUISystemBuilder
	{
		public:
		GUISystemBuilder (const std::string& fDef, FormBuilder* fB);
		// Nothing to delete...

		/** To assign an external engine. */
		_ExEngine* externalEngine ()
							{ return (_externalEngine); }
		void setExternalEngine (_ExEngine* egn)
							{ _externalEngine = egn; }

		GUISystem* system (int nS); // Can return a null pointer, take care!

		protected:
		/* The definition of the decorator for a widget. */
		class DecoratorDefinition
		{
			public:
			DecoratorDefinition ()
				: _type (-1),
				  _properties ()
							{ }

			int _type;
			std::map <std::string, std::string> _properties;
		};

		/** Every widget is defined by its id, its type, its position in the window
			and a list of the widgets contained (if it is a composite widget). */
		struct WidgetDefinition
		{
			public:
			WidgetDefinition ()
				: _id (-1),
				  _type (-1),
				  _active (true),
				  _position (Position::_cero),
				  _widgets (),
				  _properties ()
							{ }

			int _id;
			int _type;
			Position _position;
			bool _active; 
			std::vector <DecoratorDefinition> _decorators;
			std::vector <int> _widgets;
			std::map <std::string, std::string> _properties;
		};

		/** Every GUI System is defined by its id and
			the number (id) of the main widget. \n
			By the way this main widget has to have widgets inside to become a "composite widget"
			that is what the GUI System needs (and check) when it is built. */
		struct GUISystemDefinition
		{
			public:
			GUISystemDefinition ()
				: _id (-1),
				  _mainWidget (-1),
				  _widgets ()
							{ }

			int _id;
			int _mainWidget;
			std::map <int, WidgetDefinition> _widgets; // A list with all the widgets defined inside...
		};

		virtual GUISystem* createSystem (const GUISystemDefinition& def, CompositeWidget* mW) = 0;
		virtual Widget* createWidget (const WidgetDefinition& def) = 0;
		virtual Widget::Decorator* createDecorator (const DecoratorDefinition& def) = 0;

		protected:
		/** This method is invoke previously to create a new system.
			The system is defined by its id. By default it does nothing. */
		virtual void beforeCreateElement (int id)
							{ }
		/** This method is used by any futher method responsable to create
			a new system, and its main responsability is to invoke the
			right engine's method to complete the new sytem. */
		virtual void afterCreateElement (GUISystem* sys)
							{ if (_externalEngine) _externalEngine -> whenCreateGUISystem (sys); }
	
		private:
		void readFile (const std::string& fDef);
		GUISystemDefinition readSystemDefinition (TiXmlElement* s);
		WidgetDefinition readWidgetDefinition (TiXmlElement* w);
		DecoratorDefinition readDecoratorDefinition (TiXmlElement* d);
		std::map <std::string, std::string> readProperties (TiXmlElement* p);

		protected:
		typedef std::map <int, GUISystemDefinition> Definitions;

		Definitions _definitions;
		_ExEngine* _externalEngine;
		FormBuilder* _formBuilder;

		std::string _definitionFile;
		std::string _basePath;
	};

	/** The standard GUI System builder.
		It is used by most of the implementations. */
	class StandardGUISystemBuilder : public GUISystemBuilder
	{
		public:
		StandardGUISystemBuilder (const std::string& fDef, FormBuilder* fB)
			: GUISystemBuilder (fDef, fB)
							{ }

		protected:
		virtual GUISystem* createSystem (const GUISystemDefinition& def, CompositeWidget* mW);
		virtual Widget* createWidget (const WidgetDefinition& def);
		virtual Widget::Decorator* createDecorator (const DecoratorDefinition& def);
	};
}

#endif
  
// End of the file
/*@}*/