/** \ingroup Game */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: entitybuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class to create entities. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ENTITYBUILDER__
#define __QGAMES_ENTITYBUILDER__

#include <Common/elementbuilder.hpp>
#include <Common/entity.hpp>
#include <vector>
#include <string>

class TiXmlElement;

namespace QGAMES
{
	class FormBuilder;
	class MovementBuilder;

	/**	This builder is to create Entites from a definition file. \n
		Any Element created by this builder is own by it. 
		So the programmer has not to take care of deleting the entities created using this class. 
		They will be destroyed when the builder is destroyed (@see Game destructor). */
	class EntityBuilder : public ElementBuilder
	{
		public:
		/** 
		  *	A definition file is received as parameter. \n
		  *	The structure of the file should be as follows: \n
		  *	Take care. It could contain include directives... \n
		  *	In the file there can be definitions for individual entities, 
		  *	set of entities and composite entities (group of entities). \n
		  *	A set of entities can have a variable set of forms and movements.
		  *	<?xml version="1.0"?> \n
		  *	<Entities> \n
		  *		<Include file="CCC"/> \n
		  *		<Entity id="X"> \n
		  *			<Forms> \n
		  *				<Form id="XX"/> \n
		  *				... \n
		  *			</Forms> \n
		  *			<Capacities> \n
		  *				<Capacity id="Y" value="YY"/> \n
		  *				... \n
		  *			</Capacities> \n
		  *			<Physics> \n
		  *				<Physic id="Z" value="ZZZ"/> \n
		  *				... \n
		  *			</Physics> \n
		  *			<Attributes> \n
		  *				<Attribute id="XX" value="YY"/> \n
		  *				... \n
		  *			</Attributes> \n
		  *			<Movements> \n
		  *				<Movement id="K"/> \n
		  *				... \n
		  *			</Movements> \n
		  *			<Animations> \n
		  *				<Animation id="XXX" type="YYY"/> \n
		  *				<Animation id="YYY" type="ZZZ"> \n
		  *					<Attribute id="LL" value="YY"/> \n
		  *					... \n
		  *				</Animation> \n
		  *				... \n
		  *			</Animations> \n
		  *			<States> \n
		  *				<State id="XXX" animation="YYY" movement="ZZZ"> \n
		  *					<Attributes> \n
		  *						<Attribute id="XXX" value="YYY"/> \n
		  *						... \n
		  *					</Attributes> \n
		  *				</State> \n
		  *				... \n
		  *			</States> \n
		  *		</Entity> \n
		  *		...  \n
		  *
		  *		<SetOfEntities fromId="X" number="Y"> \n
		  *			<Forms> \n
		  *				<Form id="XX"/> \n
		  *				... \n
		  *			</Forms> \n
		  *			<!-- FormsBase added to Forms list with the number of the entity --> \n
		  *			<FormsBase> \n
		  *				<Form id="XX"/> \n
		  *				... \n
		  *			</FormsBase> \n
		  *			<Capacities> \n
		  *				<Capacity id="Y" value="YY"/> \n
		  *				... \n
		  *			</Capacities> \n
		  *			<Physics> \n
		  *				<Physic id="Z" value="ZZZ"/> \n
		  *				...
		  *			</Physics> \n
		  *			<Attributes> \n
		  *				<Attribute id="XX" value="YY"/> \n
		  *				... \n
		  *			</Attributes> \n
		  *			<Movements> \n
		  *				<Movement id="K"/> \n
		  *				... \n
		  *			</Movements> \n
		  *			<!-- MovementsBase added to Movements with the number of the entity --> \n
		  *			<MovementsBase> \n
		  *				<Movement id="K"/> \n
		  *				... \n
		  *			</MovementsBase> \n
		  *			<Animations> \n
		  *				<Animation id="XXX" type="YYY"/> \n
		  *				<!-- When the animation is a FORM animation
		  *					 and Formsbase has been defined, the form varies among entities --> \n
		  *				<Animation id="YYY" type="ZZZ"> \n
		  *					<Attribute id="LL" value="YY"/> \n
		  *					... \n
		  *				</Animation> \n
		  *				... \n
		  *			</Animations> \n
		  *			<States> \n
		  *				<!-- When a Movementbase has been defined,
		  *					 the real movement varies among entities --> \n
		  *				<State id="XXX" animation="YYY" movement="ZZZ"> \n
		  *					<Attributes> \n
		  *						<Attribute id="XXX" value="YYY"/> \n
		  *						... \n
		  *					</Attributes> \n
		  *				</State> \n
		  *				<State id="XXX" animation="YYY" movementBase="ZZZ"> \n
		  *					<Attributes> \n
		  *						<Attribute id="XXX" value="YYY"/> \n
		  *						... \n
		  *					</Attributes> \n
		  *				</State> \n
		  *				... \n
		  *			</States> \n
		  *		</SetOfEntities> \n
		  *		...\n
		  *
		  *		<!-- A composite entity can refer either a set of entities o a single one,
		  *			 and can also defined its own capacities, attributes or physics and also
		  *			 forms ot movements like any other entity --> \n
		  *		<CompositeEntity id="XX"> \n
		  *			<RefEntity id="YY/> \n
		  *			... \n
		  *			<RefSetEntities fromId="XX" number="YY"/> \n
		  *			... \n
		  *			<Capacities> \n
		  *				<Capacity id="Y" value="YY"/> \n
		  *				... \n
		  *			</Capacities> \n
		  *			<Physics> \n
		  *				<Physic id="Z" value="ZZZ"/> \n
		  *				... \n
		  *			</Physics> \n
		  *			<Attributes> \n
		  *				<Attribute id="XX" value="YY"/> \n
		  *				... \n
		  *			</Attributes> \n
		  *		</CompositeEntity> \n
		  *		... \n
		  *	</Entities> 
		  */
		EntityBuilder (const std::string& eDef, FormBuilder* fB, MovementBuilder* mB);
		/** The builder is accountable for any entity loaded from it.
			When the builder is destroyed all the entities loaded throught it will
			be also deleted. */
		virtual ~EntityBuilder ();

		/** To get the number of definitions included in the definition file. */
		int numberResources () const;
		/** To get the id of every resource to be loaded. \n
			The set of entities are considered individually (per component). */
		std::vector <int> resourcesId () const;

		/** Once the entities' definition file has been read, the entities can be "preload"
			It means the entity objects will be in memory waiting for they to be used. */
		void preLoad ();

		/** To verify whether an exntity exists or not. */
		bool existsEntity (int ne)
							{ return (_definitions.find (ne) != _definitions.end ()); }
		/** If an entity is not in memory when it is requested, it would be created 
			using the definition structure read from the definition file at 
			construction time. If the definition doen't exist, then NULL is returned. */
		Entity* entity (int ne);
		/** Just to load a set of entities. */
		Entities entities (const std::list <int> ne);

		protected:
		struct AnimationDefinition
		{
			AnimationDefinition ()
				: _id (-1), _type (-1), _parameters ()
							{ }

			int _id;
			int _type;
			std::map <std::string, std::string> _parameters;
		};

		struct StateDefinition
		{
			StateDefinition ()
				: _id (-1), _animationId (-1), _movementId (-1), _movementBaseId (-1), _parameters ()
							{ }

			int _id;
			int _animationId;
			int _movementId;
			int _movementBaseId;
			std::map <std::string, std::string> _parameters;
		};

		struct EntityDefinition
		{
			typedef enum { _SIMPLE, _SET, _COMPOSITE } Type;

			EntityDefinition ()
				: _type (_SIMPLE),
				  _id (-1),
				  _number (0),
				  _forms (),
				  _formsBase (),
				  _capacities (),
				  _physics (),
				  _parameters (),
				  _movements (),
				  _movementsBase (),
				  _animations (),
				  _states (),
				  _entities ()
							{ }

			Type _type;
			int _id;
			int _number;
			std::list <int> _forms;
			std::list <int> _formsBase;
			std::map <int, bool> _capacities;
			std::map <int, double> _physics;
			std::map <std::string, std::string> _parameters;
			std::list <int> _movements;
			std::list <int> _movementsBase;
			std::list <AnimationDefinition> _animations;
			std::list <StateDefinition> _states;
			std::list <int> _entities; // Make only sense when the the entity is a composite one...
		};

		/** Something to be likely redefined per game. */
		virtual Entity* createEntity (const EntityDefinition& def) = 0;
		/** A basic composite entity is created. \n
			The entities being part of it will be created and added later by the builder. \n
			So this method has only to create the composite entity itself. */
		virtual CompositeEntity* createCompositeEntity (const EntityDefinition& def) 
							{ return (new CompositeEntity (def._id, Entities (), 
								Entity::Data (Position::_cero, Vector::_cero, 
								def._capacities, def._physics, def._parameters, Movements ()))); } 
		/** 
		  * The basic animations are created. \n
		  *	def._type == -1 means QGAMES::Entity::NoAnimation. \n
		  *	def._type === 0 means QGAMES::Entity::FormAnimation and \n
		  *	def._type == other meand QGAMES::Entity::EmptyAnimation (can be useful e.g. when external engine is defined. 
		  */
		virtual Entity::Animation* createAnimation (const AnimationDefinition& def);
		/** The basic State (QGAMES::Entity::State) is created. */
		virtual Entity::State* createState (const StateDefinition& def); 

		private:
		void readFile (const std::string& fDef);
		// Invoked from readFile...(very internal and just to structure the code)
		EntityDefinition readEntityDefinition (TiXmlElement* e);
		EntityDefinition readSetOfEntitiesDefinition (TiXmlElement* e);
		EntityDefinition readCompositeEntityDefinition (TiXmlElement* e);
		std::list <int> readFormsGroup (TiXmlElement* g);
		std::map <int, bool> readCapacitiesGroup (TiXmlElement* g);
		std::map <int, double> readPhysicsGroup (TiXmlElement* g);
		std::list <int> readMovementsGroup (TiXmlElement* g);
		std::list <AnimationDefinition> readAnimationsGroup (TiXmlElement* g);
		std::list <StateDefinition> readStatesGroup (TiXmlElement* g);
		std::map <std::string, std::string> readAttributesGroup (TiXmlElement* g);
		/** To create a entity definition for a entity defined within a group. */
		EntityDefinition groupDefinitionFor (int ne);

		protected:
		typedef std::map <int, EntityDefinition> Definitions;

		Definitions _definitions;
		Definitions _setDefinitions;
		Entities _entities;
		FormBuilder* _formBuilder;
		MovementBuilder* _movementBuilder;

		std::string _definitionFile;
		std::string _basePath;
	};
}

#endif
  
// End of the file
/*@}*/
