/** \ingroup Game */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: objectbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 19/02/2015 \n
 *	Description: Defines a class to build objects. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_OBJECTBUILDER__
#define __QGAMES_OBJECTBUILDER__

#include <Common/object.hpp>
#include <Common/background.hpp>
#include <Common/formbuilder.hpp>
#include <Common/elementbuilder.hpp>
#include <tinyxml.h>
#include <string>

namespace QGAMES
{
	class ObjectBuilder : public ElementBuilder
	{
		public:
		/** The object builder uses the form builder as something optional (to create image objects maybe),
		  *	as most of the object can be built without using the form builder,
		  *	except the background. So no background object can be built up
		  *	without having a reference to the form builder. \n
		  *	The structure of a object definition file is as follows: \n
		  *	<?xml version="1.0"?> \n
		  *	<Objects> \n
		  *		<Include file="CCC"/> \n
		  *		<Background id="X" form="Y"> \n
		  *			<Attributes> \n
		  *				<Attribute id="X" value="X"/> \n
		  *				...\n
		  *			</Attributes> \n
		  *		</Background> \n
		  *		... \n
		  *		<Image id="X" form="Y"> \n
		  *			<Attributes> \n
		  *				<Attribute id="X" value="X"/> \n
		  *				... \n
		  *			</Attributes> \n
		  *		</Image> \n
		  *		... \n
		  *		<Polyline id="X" color="R,G,B"> \n
		  *			<Point X="XX" Y="YY" Z="ZZ"/> \n
		  *			<Point X="XX" Y="YY" Z="ZZ"/> \n
		  *			... \n
		  *			<Attributes> \n
		  *				<Attribute id="X" value="X"/> \n
		  *				... \n
		  *			</Attributes> \n
		  *		</Polyline> \n
		  *		... \n
		  *		<Polygon id="X" color="R,G,B"/> \n
		  *			<Point X="XX" Y="YY" Z="ZZ"/> \n
		  *			<Point X="XX" Y="YY" Z="ZZ"/> \n
		  *			... \n
		  *			<Attributes> \n
		  *				<Attribute id="X" value="X"/> \n
		  *				... \n
		  *			</Attributes> \n
		  *		</Polygon> \n
		  *		... \n
		  *		<Ellipse id="X" vX="XX" vY="YY" vZ="ZZ" r1="XX" r2="YY" color="R,G,B"> \n
		  *			<Attributes> \n
		  *				<Attribute id="X" value="X"/> \n
		  *				... \n
		  *			</Attributes> \n
		  *		</Ellipse> \n
		  *		... \n
		  *		<Composite id="X" X="XX" y="YY" z="ZZ"> \n
		  *			<Element id="X"/> \n
		  *			<Element id="X"/> \n
		  *			... \n
		  *			<Attributes> \n
		  *				<Attribute id="X" value="X"/> \n
		  *				... \n
		  *			</Attributes> \n
		  *		</Composite> \n
		  *		... \n
		  *	</Objects> 
		  */
		ObjectBuilder (const std::string& mDef, FormBuilder* fB = NULL);
		/** The object builder owns all objects created from it.
			When the builder is destroyed all objects created with it are also destroyed. */
		virtual ~ObjectBuilder ();

		/** To load the definitions before they are used. */
		void preLoad ();
		/** To get the object. \n
			Take care with the use of this object. \n
			Don't delete because the builder owns the objects and they will be deleted 
			at the end of the game. */
		Object* object (int nO);

		protected:
		// Virtual methods to create the individual elements...
		// A default behaviour is given, but the final user can overload them with new objects or ways...
		// Al the objects created with this builder are located at the cero position...
		virtual GraphicalPolygon* createGraphycalPolygon (int id, 
			const Position& pos, const Positions& ptions, const QGAMES::Color& cl, 
			const ObjectProperties& pties);
		virtual GraphicalPolyline* createGraphycalPolyline (int id, 
			const Position& pos, const Positions& ptions, const QGAMES::Color& cl, 
			const ObjectProperties& pties);
		virtual Image* createImage (int id,
			const Position& pos, const std::string& fName, const ObjectProperties& pties);
		virtual Background* createBackground (int id,
			const Position& pos, Sprite2D* f, int nT, const ObjectProperties& pties);
		virtual Ellipse* createEllipse (int id,
			const Position& pos, const Vector& v, bdata r1, bdata r2, 
			const QGAMES::Color& cl, const ObjectProperties& pties);
		virtual CompositeObject* createComposite (int id, const Position& pos, 
			const Objects& objs, const ObjectProperties& pties);

		private:
		/** To read the file. \n
			it is virtual because new objects can be added later 
			(not usual, this is the reason for not to use adds-on like in GameState builder. */
		virtual void readFile (const std::string& fDef);

		// To read the individual elements componing a part of the file...
		GraphicalPolygon* readGraphicalPolygon (TiXmlElement* e);
		GraphicalPolyline* readGraphicalPolyline (TiXmlElement* e);
		Image* readImage (TiXmlElement* e);
		Background* readBackground (TiXmlElement* e);
		Ellipse* readEllipse (TiXmlElement* e);
		CompositeObject* readComposite (TiXmlElement* e);

		// Common parts in each element definition...
		std::map <std::string, std::string> readPropertiesSection (TiXmlElement* e);
		QGAMES::Position readPosition (TiXmlElement* e);

		protected:
		std::map <int, Object*> _objects;

		FormBuilder* _formBuilder;

		std::string _definitionFile;
		std::string _basePath;

		private:
		// Implementation
		bool _alreadyLoaded;
	};
}

#endif

