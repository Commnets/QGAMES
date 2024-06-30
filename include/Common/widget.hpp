/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: widget.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 28/12/2018\n
 *	Description: Defines a class to represent interaction elements in the screen. \n
 *				 This elements usually are part of a GUI interfaz where the player configures the game.\n
 *				 They behave graphically like ScoreObjet (@see them), but they user can interate with them instead. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_WIDGET__
#define __QGAMES_WIDGET__

#include <Common/element.hpp>
#include <Common/inputhandler.hpp>
#include <Common/rectangle.hpp>
#include <Common/color.hpp>
#include <vector>

namespace QGAMES
{
	class CompositeWidget;
	class Form;

	/** 
	  *	The class represents a widget,
	  *	which usually is an element that interacts with the player usually to allow to configure something in the game. \n 
	  *	When a widget is drawn it can be modified by a Decorator. \n
	  *	So, two similar widgets in terms os behaviour, could have different ways of being represented in the screen. \n
	  *	The widget defines general methods to determinate the behaviour when it is selected, loses or gain the focus. \n
	  *	Every widget is defined by an id and a type. The id has to be unique in the GUI system. 
	  * The widget are made to exist in a 2S screen.
	  * The widget can or not be active. When it is not, it can't be selected at all, but initially is drawn in 
	  * the way and colors than an active one with no focus is drawn.
	  */
	class Widget : public Element
	{
		public:
		/** The list of properties for both widgets and decorators. */
		typedef std::map <std::string, std::string> Properties;

		/** This class is used to complete the widget decorated when it is drawn. \n
			By default the decorator does nothing. It has to be overloaded. \n
			The Decorator can be cloned, and includes a method to be initialized from a list of parameters (a map). \n
			This method is usually invoked by a widget builder. */
		class Decorator
		{
			public:
			virtual ~Decorator ()
							{ }

			virtual void decorate (Screen* scr, const Position& p, Widget* wdt) = 0;
			virtual Decorator* clone () const = 0;

			/** To indicate when to draw the decorator.
				Before of after drawing the widget. By default it is drawn before. */
			virtual bool isBefore () { return (true); }

			/** To initialize the decoratror from a list of properties. 
				By default it does nothinbg. */
			virtual void initializeFrom (const Properties& prp)
							{ }
		};

		/** To simplify the manipulation of a list of widgets. */
		typedef std::vector <Decorator*> ListOfDecorators;

		/** A complex decorator can be made of multiple simple decorators. \n
			The order in the list can be critical. \n
			The complex decorator owns the internal ones. \n
			Very simple class. It allows to add decoratorr but not to delete them. */
		class ComplexDecorator : public Decorator
		{
			public:
			ComplexDecorator (const ListOfDecorators& lD = ListOfDecorators ())
				: Decorator (), _decorators (lD)
							{ }

			virtual ~ComplexDecorator ();

			virtual void decorate (Screen* scr, const Position& p, Widget* wdt);
			virtual Decorator* clone () const;

			void addDecorator (Decorator* d);
			const ListOfDecorators& decorators () const
							{ return (_decorators); }

			protected:
			ListOfDecorators _decorators; 
		};

		/** A very classic and basic Decorator, consisting on rounding the widget with a rectangle. */
		class RoundingDecorator : public Decorator
		{
			public:
			RoundingDecorator ()
				: Decorator (),
				  _colorBoxLimit (Color (255,255,255,255)), // Pure white...
				  _thickness (2), // 2 pixels thick
				  _colorInside (Color (255,255,255,255)), // Also pure blank
				  _separation (0)
							{ }

			RoundingDecorator (const Color& cl, int th, const Color& cI, int sp)
				: Decorator (),
				  _colorBoxLimit (cl),
				  _thickness (th),
				  _colorInside (cI),
				  _separation (sp)
							{ }

			/** @see parent. */
			virtual void decorate (Screen* scr, const Position& p, Widget* wdt);
			virtual Decorator* clone () const
							{ return (new RoundingDecorator (*this)); } // The default one is enough...

			virtual void initializeFrom (const Properties& prp);

			protected:
			/** The color of the box around the widget. */
			Color _colorBoxLimit;
			/** How thick the box around is. */
			int _thickness;
			/** The inside color of rectangle. */
			Color _colorInside;
			/** The margin from the widget. */
			int _separation;
		};

		/** Blinking decorator. */
		class BlinkingBackgroundDecorator : public Decorator
		{
			public:
			BlinkingBackgroundDecorator ()
				: Decorator (),
				  _backgroundColor (Color (255,255,255,255)),
				  _separation (0),
				  _initialFade (0), _finalFade (255), _fadeGrowth (5),
				  _counter (1),
				  _currentFade (0)
							{ }

			BlinkingBackgroundDecorator (const Color& bC, int sp, int iF, int fF, int fG)
				: Decorator (),
				  _backgroundColor (bC),
				  _separation (sp),
				  _initialFade (iF),
				  _finalFade (fF),
				  _fadeGrowth (fG),
				  _counter (1),
				  _currentFade (iF)
							{ assert (_initialFade <= _finalFade); }

			/** @see parent. */
			virtual void decorate (Screen* scr, const Position& p, Widget* wdt);
			virtual Decorator* clone () const
							{ return (new BlinkingBackgroundDecorator (*this)); } // The default one is enough...

			virtual void initializeFrom (const Properties& prp);

			protected:
			Color _backgroundColor;
			int _separation;
			int _initialFade, _finalFade, _fadeGrowth;

			// Implementation
			int _counter;
			int _currentFade;
		};

		friend CompositeWidget;

		Widget (int id, Decorator* d = NULL)
			: Element (id),
			  _parent (NULL),
			  _hasFocus (false),
			  _position (Position::_cero), // No position...
			  _decorator (NULL),
			  _active (true)
							{ }
		~Widget ()
							{ delete (_decorator); } // If any...

		/** Every widget has to have a type. */
		virtual int type () = 0;

		/** To know whether it is active or not. 
			When it is desactivated and has the focus, the behaviour could be unstable. */
		bool isActive () const
							{ return (_active); }
		void setActive (bool a)
							{ _active = a; }

		/** Link the widget...
			Take care changing the parent. It could be very dangerous,
			The whole system can become unestable or even crash. */
		CompositeWidget* parent () // Can be null...
							{ return (_parent); }
		void setParent (CompositeWidget* wdt)
							{ _parent = wdt; }

		/** Managing the focus. */
		bool hasFocus () const
							{ return (_hasFocus); }
		virtual void setFocus (bool f); // It has to be propagated to the parent

		/** Managing the position. */
		Position position () const;
		Position absolutePosition () const; // ...the position taking into account where the widget is located!
		void setPosition (const Position& pos)
							{ _position = pos; } // Relative...
		/** To know whether a position is or not in. */
		bool isInPosition (int posX, int posY)
							{ return (box ().hasIn (Position (__BD posX, __BD posY, __BD 0))); }
		/** Getting the size of the element. Depens on the element itself. */
		virtual int width () = 0;
		virtual int height () = 0;
		/** This method is virtual, because depending on the direction of the drawing, the final square
			can be at some point or at another. By default, the method suppossed right-down drawing. */
		virtual Rectangle box ()
							{ return (Rectangle (position (), position () + Vector (__BD width (), __BD height (), __BD 0))); } 
		
		/** Managing the decoration */
		Decorator* decorator () 
							{ return (_decorator); }
		void setDecorator (Decorator* d); // Deletes the previous one, just one only at the same time

		/** Every widget can be initialized from a list of parameters.
			This method is specially invoked inthe widget builder. 
			By default it does the same that the normal initialization. */
		virtual void initializeFrom (const Properties& p)
							{ initialize (); }

		/** @see parent. \n
			But invokes the decorator. The decorator is always behind the widget. \n
			This method invokes by default drawOnWhenHasFocus and drawOnWhenNoHasFocus, depending on
			whether the widget has or not the focus now. \n */
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);

		/** What to do when a key is pressed over this widget.
			This method is onvoke by a GUISystem object. 
			Nothing is done by default. 
			The char code of the key is also received as parameter. */
		virtual void whenOnKey (KeyCode kC, char c)
							{ }
		/** What to do when the widget is selected (normally, when the user clicks on it). \n
			By default it simply notifies the situation (__QGAMES_WIDGETSELECTEDEVENT__) */
		virtual void whenSelected ();

		protected:
		/** Very important methods. \n
			What to do when gets and loses focus. */
		virtual void whenGetFocus () = 0;
		virtual void whenLoseFocus () = 0;

		// Implementation
		// p is just the real position where to draw
		// Taking into account that the screen can move and will reduce the coordinates
		virtual void drawOnWhenHasFocus (Screen* s, const Position& p) = 0; 
		virtual void drawOnWhenNoHasFocus (Screen* s, const Position& p) = 0;

		protected:
		/** Who is the parent, if any. \n
			The parent has to be always a CompositeWidget. */
		CompositeWidget* _parent;
		bool _hasFocus;
		Position _position; // The position is always a 2D position 
							// (we are talking about an interaction element on the screen)
		Decorator* _decorator;
		/** When the widget is not active it can't gain the focus. */
		bool _active; 
	};

	typedef std::vector <Widget*> ListOfWidgets;

	/** Defines a set of widgets. \n
		The position of the internal widgets is relative to the position of its container. 
		The composite doesn't take into account whether the intrenal widgets are active. */
	class CompositeWidget : public Widget
	{
		public:
		/** Just a simple iterator to iterate over the composite widget. \n
			It is a iterator forward. Going back is not possible with it. \n
			If the next element would be a CompositeWidget, the Iterator would iterate over it as well. */
		class Iterator
		{
			public:
			Iterator (CompositeWidget* cW);
			~Iterator ()
							{ delete (_internalIterator); }

			Widget* first ();
			Widget* next ();
			/** Move the iterator to this specific widget.
				Take into account that the widget can or not exist. */
			bool moveTo (Widget* wdt);
			bool isLast ()
							{ return (_currentWidget == NULL); }
			Widget* current ()
							{ return (_currentWidget); }

			private:
			CompositeWidget* _compositeWidget;
			const ListOfWidgets& _widgets;
			ListOfWidgets::const_iterator _iterator;
			Iterator* _internalIterator;
			Widget* _currentWidget;
		};

		CompositeWidget (int id, Decorator* d = NULL, const ListOfWidgets& wdts = ListOfWidgets (), int wF = -1)
				: Widget (id, d),
				  _widgets (wdts), 
				  _widgetFocused (wF)
							{ correctWidgetFocused (); }
		~CompositeWidget ();

		/** @see parent. */
		virtual int type ()
							{ return (__QGAMES_BASICCOMPOSITEWIDGETTYPE__); }

		/** @see parent. */
		virtual void setFocus (bool f);
		/** Move focus to next and previous internal widgets */
		virtual void moveFocusToNextWidget ();
		virtual void moveFocusToPreviousWidget ();

		/** @see parent. */
		virtual int width ();
		virtual int height ();

		// Managing the widgets...
		/** Adds a widget. 
			There can not be two widgets with the same id. */
		bool addWidget (Widget* wdt); 
		/** Returns null if it doesn't exist (takes care) */
		Widget* widget (int id);
		const Widget* widget (int id) const;
		const ListOfWidgets& widgets ()
							{ return (_widgets); }
		/** To get the first widget if any. It could be null. */
		Widget* firstWidget ();
		/** To get the last widget if any. It could be null. */
		Widget* lastWidget ();
		/** Get the widget at the specific position if any. 
			It returns the deepest one always. If none is at that position NULL is returned. */
		Widget* widgetAt (int px, int py);
		bool existsWidget (int id) const;
		/** It does nothing if it doesn't exist. */
		void removeWidget (int id);
		/** To get the focused widget. 
			It can be null if the widget has none inside. */
		Widget* focusedWidget ();
		/** Recursive if any. */
		Widget* lastFocusedWidget ();
		/** To set the focus in a specific widget.
			If the widget doesn't exist as part of the composite widget, nothing changes. */
		void setFocusedWidget (Widget* wdt);
		void setFocusedWidget (int id);

		/** @see parent. */
		virtual void whenSelected ();

		/** @see parent. */
		virtual void inEveryLoop ();
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		virtual void finalize ();

		protected:
		/** @see parent. */
		virtual void whenGetFocus ();
		virtual void whenLoseFocus ();
		virtual void drawOnWhenHasFocus (Screen* s, const Position& p);
		virtual void drawOnWhenNoHasFocus (Screen* s, const Position& p);

		private:
		// Implemetation
		void correctWidgetFocused ();

		protected:
		/** The list of widgets. */
		ListOfWidgets _widgets;
		/** The number of widget having the focus. -1 when none has it. */
		int _widgetFocused;
	};

	/** The most basic widget. \n
		It consists on a set (or even 1) of Sprites. \n
		Those sprites can be set from an string, from a number and directly indicating the aspect of the form. \n
		The label can be drawn in diferent directions. */
	class LabelWidget : public Widget
	{
		public:
		enum Direction
		{
			_HORIZONTAL_RIGHT = 0,
			_HORIZONTAL_LEFT,
			_VERTICAL_DOWN,
			_VERTICAL_UP
		};

		/** The configuration data. */
		class Configuration
		{
			public:
			Configuration ()
				: _chrBase (32), // Space
				  _alphaFocus (255), _alphaNoFocus (255),
				  _pixelsAmong (0), // All together...it can be negative...
				  _direction (_HORIZONTAL_RIGHT)
							{ }
			
			int _chrBase;
			int _alphaFocus, _alphaNoFocus;
			int _pixelsAmong;
			Direction _direction;
		};

		LabelWidget (int id, Form* frm, Decorator* d = NULL);

		/** @see parent. */
		virtual int type ()
							{ return (__QGAMES_LABELWIDGETTYPE__); }

		/** @see parent. */
		virtual int width ();
		virtual int height ();
		virtual Rectangle box ();

		/** Manage the configuration of the label widget. */
		const Configuration& configuration () const
							{ return (_configuration); }
		void setConfiguration (const Configuration& cfg)
							{ _configuration = cfg; }

		/** To set the label */
		void setLabel (Form* frm, int f);
		void setLabel (const std::string& txt);
		void setLabel (int n)
							{ setLabel (std::to_string (n)); }
		void setLabel (float f)
							{ setLabel (std::to_string (f)); }

		/** @see parent. */
		virtual void initializeFrom (const Properties& prp);

		protected:
		/** @see parent. */
		virtual void whenGetFocus ()
							{ /** Nothing special by default. */ }
		virtual void whenLoseFocus ()
							{ /** Nothing special by default. */ }
		virtual void drawOnWhenHasFocus (Screen* s, const Position& p)
							{ drawWithAlpha (s, p, _configuration._alphaFocus); }
		virtual void drawOnWhenNoHasFocus (Screen* s, const Position& p)
							{ drawWithAlpha (s, p, _configuration._alphaNoFocus); }

		private:
		/** All set method finally invoke this one,
			that is just to filter and keep the aspects to draw eventually. */ 
		void setLabel (const std::vector <int>& a);
		/** The drawing for focus and for not focus is quite similar. 
			The difference is in the intensity of the alpha. */
		void drawWithAlpha (Screen* s, const Position& p, int a);

		protected:
		/** The form used in the label. */
		Form* _form;
		/** The configuration. */
		Configuration _configuration;

		// Implementation
		/** The list of aspects used to draw the label. 
			Can include several. */
		std::vector <int> _aspects;
	};

	/** A widget what behaves like a label, with the difference
		that when user clicks on it, the content changes to other within a list of them. */
	class IteratingLabelWidget : public LabelWidget
	{
		public:
		/** A basic constructor. */
		IteratingLabelWidget (int id, Form* frm, Decorator* d = NULL)
			: LabelWidget (id, frm, d),
			  _stringValues (), _aspectValues (), _currentValue (0)
							{ _stringValues.push_back (std::string ("Not defined")); }

		IteratingLabelWidget (int id, Form* frm, 
			const std::vector <std::string>& vals, Decorator* d = NULL)
			: LabelWidget (id, frm, d),
			  _stringValues (vals), // Can't be empty!
			  _aspectValues (), // Empty menas not used...
			  _currentValue (0)
							{ setValues (vals); }

		IteratingLabelWidget (int id, Form* frm, 
			const std::vector <int>& vals, Decorator* d = NULL)
			: LabelWidget (id, frm, d), // Cant't be empty!
			  _stringValues (), // Empty means not used...
			  _aspectValues (vals),
			  _currentValue (0)
							{ setValues (vals); }

		/** @see parent. */
		virtual int type ()
							{ return (__QGAMES_ITERATINGLABELWIDGETTYPE__); }

		/** To change the list of the values being iterated in the label. */
		void setValues (const std::vector <std::string> vals);
		void setValues (const std::vector <int> vals);

		/** Which is the current value selected. */
		void setCurrentValue (int v);
		int currentValue () const
							{ return (_currentValue); }

		/** @see parent. */
		virtual void initializeFrom (const Properties& prp);

		/** @see parent. */
		virtual void whenSelected ();

		protected:
		/** The different values the iterating widget can have. */
		std::vector <std::string> _stringValues;
		std::vector <int> _aspectValues;

		// Implementation
		int _currentValue;
	};

	/** 
	  *	In this widget the movement to the next element in the list happens not only pressing the enter key,
	  *	but also pressing the key that matches with the first letter of the element we want to move to.
	  *	This widget is only valid for texts. 
	  */
	class KeyLabelWidget : public IteratingLabelWidget
	{
		public:
		KeyLabelWidget (int id, Form* frm, Decorator* d = NULL)
			: IteratingLabelWidget (id, frm, d)
							{ }

		KeyLabelWidget (int id, Form* frm, 
			const std::vector <std::string>& vals, Decorator* d = NULL)
			: IteratingLabelWidget (id, frm, d)
							{ }

		KeyLabelWidget (int id, Form* frm, 
			const std::vector <int>& vals, Decorator* d = NULL)
			: IteratingLabelWidget (id, frm, d) 
							{ }

		/** @see parent. */
		virtual int type ()
							{ return (__QGAMES_ITERATINGLABELWIDGETKEYTYPE__); }

		virtual void whenOnKey (KeyCode kC, char c);
	};

	/** A widget to admit inputs inside. \n 
		The widget has also a fix text (presentation text) 
		that can be in the left side of the introduction field or above. */
	class IntroWidget : public Widget
	{
		public:
		enum IntroLocation
		{
			_INTRO_RIGHT = 0,
			_INTRO_DOWN,
		};

		/** The configuration data. */
		class Configuration
		{
			public:
			Configuration ()
				: _chrBase (32), // Space
				  _alphaFocus (255), _alphaNoFocus (255),
				  _pixelsAmong (0), // All together...it can be negative...
				  _numberChars (10),
				  _mask (__NULL_STRING__),
				  _cursorColor (Color (255,255,255,255)), // Pure white...
				  _location (_INTRO_RIGHT)
							{ }
			
			int _chrBase;
			int _alphaFocus, _alphaNoFocus;
			int _pixelsAmong;
			int _numberChars; // Allowed in the introduction
			std::string _mask; // Mask for the introduction (empty for nothing)
			Color _cursorColor; // The color of the cursor
			IntroLocation _location;
		};

		IntroWidget (int id, Form* frm, Decorator* d = NULL);

		/** @see parent. */
		virtual int type ()
							{ return (__QGAMES_INTROWIDGETTYPE__); }

		/** @see parent. */
		virtual int width ();
		virtual int height ();

		/** Manage the configuration of the intro widget. */
		const Configuration& configuration () const
							{ return (_configuration); }
		void setConfiguration (const Configuration& cfg)
							{ _configuration = cfg; }

		/** To manage the intro */
		const std::string& text () const
							{ return (_text); }
		void setText (const std::string& txt);
		/** ...and the label associated... */
		void setLabel (Form* frm, int f);
		void setLabel (const std::string& txt);
		void setLabel (int n)
							{ setLabel (std::to_string (n)); }
		void setLabel (float f)
							{ setLabel (std::to_string (f)); }

		/** @see parent. */
		virtual void initializeFrom (const Properties& prp);

		/** @see parent. */
		virtual void whenOnKey (KeyCode kC, char c);

		protected:
		/** @see parent. */
		virtual void whenGetFocus ()
							{ /** Nothing by default. */ }
		virtual void whenLoseFocus ()
							{ /** Nothing by default. */ }
		virtual void drawOnWhenHasFocus (Screen* s, const Position& p)
							{ drawWithAlpha (s, p, _configuration._alphaFocus); }
		virtual void drawOnWhenNoHasFocus (Screen* s, const Position& p)
							{ drawWithAlpha (s, p, _configuration._alphaNoFocus); }

		/**
		  *	To verify whether a string is valid according to the mask. \n
		  *	The default behaviour just takes only into account letters and number, but
		  *	further behaviours can be defined. \n
		  *	For the default mask: \n
		  *	a means a letter. \n
		  *	9 means a number. \n
		  *	A means a letter and a symbol. \n
		  *	x means either a letter or a number. \n
		  *	X means either a letter, a number or a symbol.
		  */
		virtual bool isTextValid (const std::string& chr) const;

		private:
		/** All set methods finally invoke this one,
			that is just to filter and keep the aspects to draw eventually. */ 
		void setText (const std::vector <int>& a)
							{ _textAspects = clearAspects (a); }
		/** The same than above but with the aspects of the text. */
		void setLabel (const std::vector <int>& a)
							{ _labelAspects = clearAspects (a); }
		/** The drawing for focus and for not focus is quite similar. 
			The difference is in the intensity of the alpha. */
		void drawWithAlpha (Screen* s, const Position& p, int a);
		/** Clear the aspets. Eliminates those that can not be drawn. */
		std::vector <int> clearAspects (const std::vector <int>& a);

		protected:
		/** The form used in the widget for both text and label. */
		Form* _form;
		/** The configuration. */
		Configuration _configuration;
		/** The text manage by the widget. */
		std::string _text;

		// Implementation
		/** Th elists of aspects used to draw the content. */
		std::vector <int> _textAspects;
		/** The list of aspects used to draw the label. 
			Can include several. */
		std::vector <int> _labelAspects;
		/** Where the cursor is whilst typing. */
		int _cursorPosition;
	};
}

#endif
  
// End of the file
/*@}*/
