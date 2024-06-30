/** \ingroup Game */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: movementbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class to build up movements. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_MOVEMENTBUILDER__
#define __QGAMES_MOVEMENTBUILDER__

#include <Common/movement.hpp>
#include <map>
#include <vector>
#include <string>

class TiXmlElement;

namespace QGAMES
{
	/** To create movements.
		The builder owns the movements it creates. */
	class MovementBuilder
	{
		public:
		/** 
		  *	The structure of the movements file definition is as follows: \n
		  *	Take care, it could contain include directives. \n
		  * Indivisual movements and set of them can be defined. \n
		  *	<?xml version="1.0"?> \n
		  *	<Movements> \n
		  *		<Include file="CCCC"/> \n
		  *		<Movement id="X"> \n
		  *			<Variable id="Y" value="YYY"/> \n
		  *			... \n
		  *		</Movement> \n
		  *		<SetOfMovements fromId="X" number="Y"> \n
		  *			<Variable id="Y" value="YYY"/> \n
		  *			... \n
		  *		</SetOfMovements> \n
		  *		... \n
		  *	</Movements> 
		  */			
		MovementBuilder (const std::string& fDef);
		/** The movement builder owns the movements created from it. \n
			When the builder is destroyed all movements created from it are also removed. */
		virtual ~MovementBuilder ();

		/** To get the number of definitions included in the definition file. */
		int numberResources () const;
		/** To get the id of every resource to be loaded. */
		std::vector <int> resourcesId () const;

		/** When the movement builder is created the definitions are loaded. \n
			It is also possible lo load the movements using this method. \n
			This normally happens before the game starts for performace reasons. */
		void preLoad ();
		Movement* movement (int nm);

		protected:
		struct MovementDefinition
		{
			typedef enum { _SIMPLE, _SET } Type;

			MovementDefinition ()
				: _type (_SIMPLE),
				  _id (-1),
				  _number (0),
				  _variables ()
							{ }

			Type _type;
			int _id;
			int _number;
			std::map <int, double> _variables;
		};

		/** To create the specific movement. 
			It has to be redefined per game. */
		virtual Movement* createMovement (const MovementDefinition& def) = 0;
		
		private:
		void readFile (const std::string& fDef);
		MovementDefinition readMovementDefinition (TiXmlElement* m);
		MovementDefinition readSetOfMovementsDefinition (TiXmlElement* m);
		/** To create a movement definition for a movement defined within a group. */
		MovementDefinition groupDefinitionFor (int nm);

		protected:
		typedef std::map <int, MovementDefinition> Definitions;

		Definitions _definitions;
		Definitions _setDefinitions;
		Movements _movements;

		std::string _definitionFile;
		std::string _basePath;
	};
}

#endif
  
// End of the file
/*@}*/
