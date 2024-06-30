/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: object.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines an object. \n
 *				 An object is an element with a position in the space. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_OBJECT__
#define __QGAMES_OBJECT__

#include <Common/element.hpp>
#include <Common/definitions.hpp>
#include <Common/color.hpp>
#include <map>
#include <list>

namespace QGAMES
{
	class Object;
	typedef std::list <Object*> Objects;
	typedef std::map <std::string, std::string> ObjectProperties;
	class Image;
	typedef std::list <Image*> Images;

	/** 
	  * An object is not an "alive" element but it has a position inthe space. \n
	  *	They are elements so the can be initilized, actualized in every loop, drawn, and finalized.
	  *	The objects can have different properties defining them. 
	  *	An obhect can be sed for decoration purposes e.g (the background is probably the most important object). 
	  */
	class Object : public Element
	{
		public:
		Object (int id, const Position& pos, 
			const ObjectProperties& oP = ObjectProperties ())
				: Element (id), 
				  _position (pos),
				  _fixPosition (false),
				  _properties (oP)
							{ }

		/** To control the fade.
			It's to be redefined at any subclass. */
		virtual void setFade (int fd) = 0;

		const Position& position () const
							{ return (_position); }
		void setPosition (const Position& pos)
							{ _position = pos; }
		/** Objects can have a fix position in the screen.
			This is by default with status objects but not with the rest .*/
		void setFixPosition (bool f)
							{ _fixPosition = f; }
		const ObjectProperties& properties () const
							{ return (_properties); }
		bool existObjProperty (const std::string& oP) const;
		const std::string& objectProperty (const std::string& oP) const;

		protected:
		Position _position;
		bool _fixPosition;
		ObjectProperties _properties;
	};

	/** A reference to the object. 
		The reference doesn't own the object.
		So when it is deleted the pointed object isn't. */
	class ObjectProxy : public Object
	{
		public:
		ObjectProxy (int id, const Position& pos, Object* obj);

		// To get a reference to the original object...
		Object* ptr () 
							{ return (_object); }
		const Object* ptr () const
							{ return (_object); }

		virtual void setFade (int fd)
							{ _object -> setFade (fd); }

		virtual void initialize ()
							{ _object -> initialize (); } // Referenced to the proxy object...
		virtual void inEveryLoop ()
							{ _object -> inEveryLoop (); }
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint)
							{ _object -> drawOn (s, _position + _object -> position ()); }
		virtual void finalize ()
							{ _object -> finalize (); }

		private:
		Object* _object;
	};

	/** An image. */
	class Image : public Object
	{
		public:
		Image (int id, const Position& pos, const std::string& f, 
				const ObjectProperties& oP = ObjectProperties ())
			: Object (id, pos, oP),
			  _image (f),
			  _fade (255)
							{ }

		virtual void setFade (int fd)
							{ _fade = fd; }

		const std::string& image () const
							{ return (_image); }

		/** @see parent. 
			Remember that _noPoint means to use the internal position insted. */
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);

		protected:
		std::string _image;

		// Implementation
		int _fade;
	};

	/** An ellipse. */
	class Ellipse : public Object
	{
		public:
		Ellipse (int id, const Position& c, const Vector& o, bdata a1, bdata a2, 
			const QGAMES::Color& cl = QGAMES::Color (0,0,0),
			const ObjectProperties& oP = ObjectProperties ())
			: Object (id, c, oP),
			  _orientation (o),
			  _axis1 (a1),
			  _axis2 (a2),
			  _color (cl)
							{ }

		virtual void setFade (int fd)
							{ _color = QGAMES::Color (_color.red (), _color.green (), _color.blue (), fd);  }

		const Vector& orientation () const
							{ return (_orientation); }
		bdata axis1 () const
							{ return (_axis1); }
		bdata axis2 () const
							{ return (_axis2); }
		const QGAMES::Color& color () const
							{ return (_color); }

		/** @see parent. */
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);

		protected:
		Vector _orientation;
		bdata _axis1;
		bdata _axis2;
		QGAMES::Color _color;
		
	};

	/** A list of points in the space. */
	class ListOfPositions : public Object
	{
		public:
		ListOfPositions (int id, const Position& pos, const Positions& p,
				const ObjectProperties& oP = ObjectProperties ())
			: Object (id, pos, oP),
			  _positions (p)
							{ }
		
		const Positions& positions () const
							{ return (_positions); }

		protected:
		Positions _positions;
	};

	/** The list of points is closed and fullfill with color. */
	class GraphicalPolygon : public ListOfPositions
	{
		public:
		GraphicalPolygon (int id, const Position& pos, const Positions& p,
				const QGAMES::Color& cl = QGAMES::Color (0,0,0),
				const ObjectProperties& oP = ObjectProperties ())
			: ListOfPositions (id, pos, p, oP),
			  _color (cl)
							{ }

		virtual void setFade (int fd)
							{ _color = QGAMES::Color (_color.red (), _color.green (), _color.blue (), fd);  }
		const QGAMES::Color& color () const
							{ return (_color); }

		/** @see parent. */
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);

		protected:
		QGAMES::Color _color;
	};

	/** The list of points are not closed. */
	class GraphicalPolyline : public ListOfPositions
	{
		public:
		GraphicalPolyline (int id, const Position& pos, const Positions& p,
				const QGAMES::Color& cl = QGAMES::Color (0,0,0),
				const ObjectProperties& oP = ObjectProperties ())
			: ListOfPositions (id, pos, p, oP),
			  _color (cl)
							{ }

		virtual void setFade (int fd)
							{ _color = QGAMES::Color (_color.red (), _color.green (), _color.blue (), fd);  }
		const QGAMES::Color& color () const
							{ return (_color); }

		/** @see parent. */
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);

		protected:
		QGAMES::Color _color;
	};

	/** A composite object is made up of many others. */
	class CompositeObject : public Object
	{
		public:
		CompositeObject (int id, const Position& pos, const Objects& objs,
			const ObjectProperties& oP = ObjectProperties ())
			: Object (id, pos, oP), 
			  _objects (objs)
							{ }

		virtual void setFade (int fd);

		const Objects& objects () const
							{ return (_objects); }

		/** @see parent. */
		virtual void initialize ();
		virtual void inEveryLoop ();
		/** 
		  *	To draw every object of the composition. \n
		  *	This method does not take into account the position received. \n
		  *	All internal objects are drawn using their internal position. \n
		  *	Makes no sense to take into account the position received because
		  *	all objects will then drawn in the same position. 
		  */
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		virtual void finalize ();

		protected:
		Objects _objects;
	};

	/** The background is also another object (@see background.hpp) 
		but as its definition is large it has been included in another file. */}

#endif
  
// End of the file
/*@}*/
