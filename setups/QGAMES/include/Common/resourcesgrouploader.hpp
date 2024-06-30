/** \ingroup Game */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: resourcesloadergroup.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 17/01/2018 \n
 *	Description: Defines a class to load a group of resources (sounds, forms, movements, entities and objects). \n
 *				 That is all what can be loaded related with the common library. \n
 *				 This class is not used often. Its use is not mandatory.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_RESOURCESGROUPLOADER__
#define __QGAMES_RESOURCESGROUPLOADER__

#include <string>
#include <map>
#include <vector>
#include <tinyxml.h>

namespace QGAMES
{
	class FormBuilder;
	class SoundBuilder;
	class MovementBuilder;
	class EntityBuilder;
	class ObjectBuilder;

	/** 
	  *	The class to load resources. \n
	  *	The structure of the defintion file should be as follows: \n
	  *	<?xml version="1.0"?> \n
	  *	<Resources> \n
	  *		<Include file="CCC"/> \n
	  *		<ResourceGroup id="XX"> \n
	  *			<ResourceGroup id="RR"/> \n
	  *			... \n
	  *			<Sounds> \n
	  *				1 2 3 4 ... \n
	  *			</Sounds> \n
	  *			<Forms> \n
	  *				1 2 3 4 ... \n
	  *			</Forms> \n
	  *			<Movements> \n
	  *				1 2 3 4 ...
	  *			</Movements> \n
	  *			<Objects> \n
	  *				1 2 3 4 ... \n
	  *			</Objects> \n
	  *			<Entities> \n
	  *				1 2 3 4 ... \n
	  *			</Entities> \n
	  *			... \n
	  *		</ResourceGroup> \n
	  *		... \n
	  *	</Resources>
	  */
	class ResourcesGroupLoader
	{
		public:
		/** The resource loader is made up of little loaders,
			every one specialized in a type of content. */
		class TypeLoader
		{
			public:
			TypeLoader (const std::string& type)
				: _type (type)
							{ }

			/** which is the type of resource managed by this specific loader? */
			const std::string& type () const
							{ return (_type); }
			bool isForType (const std::string& tp) const
							{ return (tp == _type); }

			/** To load a list of resources.
				What to do depends on what this loader is specialized on. */
			virtual void load (const std::vector <int>& rscs) = 0;

			protected:
			std::string _type;
		};

		/** To load Forms. */
		class TypeFormLoader : public TypeLoader
		{
			public:
			TypeFormLoader (const std::string& type, FormBuilder* fBld);

			/** @see parent. */
			virtual void load (const std::vector <int>& rscs);

			private:
			FormBuilder* _formBuilder;
		};

		/** Standard loader for forms. */
		class StdTypeFormLoader : public TypeFormLoader
		{
			public:
			StdTypeFormLoader ();
		};

		/** To load sounds. */
		class TypeSoundLoader : public TypeLoader
		{
			public:
			TypeSoundLoader (const std::string& type, SoundBuilder* sBld);

			/** @see parent. */
			virtual void load (const std::vector <int>& rscs);

			private:
			SoundBuilder* _soundBuilder;
		};

		/** Standard loader for sounds. */
		class StdTypeSoundLoader : public TypeSoundLoader
		{
			public:
			StdTypeSoundLoader ();
		};

		/** To load movements. */
		class TypeMovementLoader : public TypeLoader
		{
			public:
			TypeMovementLoader (const std::string& type, MovementBuilder* mBld);

			/** @see parent. */
			virtual void load (const std::vector <int>& rscs);

			private:
			MovementBuilder* _movementBuilder;
		};

		/** Standard loader for movements. */
		class StdTypeMovementLoader : public TypeMovementLoader
		{
			public:
			StdTypeMovementLoader ();
		};

		/** To load entities. */
		class TypeEntityLoader : public TypeLoader
		{
			public:
			TypeEntityLoader (const std::string& type, EntityBuilder* eBld);

			/** @see parent. */
			virtual void load (const std::vector <int>& rscs);

			private:
			EntityBuilder* _entityBuilder;
		};

		/** Standard loader for entities. */
		class StdTypeEntityLoader : public TypeEntityLoader
		{
			public:
			StdTypeEntityLoader ();
		};

		/** To load objects. */
		class TypeObjectLoader : public TypeLoader
		{
			public:
			TypeObjectLoader (const std::string& type, ObjectBuilder* oBld);

			/** @see parent. */
			virtual void load (const std::vector <int>& rscs);

			private:
			ObjectBuilder* _objectBuilder;
		};

		/** Standard loader for objects. */
		class StdTypeObjectLoader : public TypeObjectLoader
		{
			public:
			StdTypeObjectLoader ();
		};

		/** To manage a list of specific loaders. 
			The first part of the map is the type of resource. */
		typedef std::map <std::string, TypeLoader*> TypeLoaders;

		ResourcesGroupLoader (const TypeLoaders& ldrs, const std::string& dF);
		~ResourcesGroupLoader (); // The resource loader owns the specific loaders...

		// To manage the specific loaders...
		void addLoader (TypeLoader* ld);
		bool existLoader (const std::string& tp) const
							{ return (_loaders.find (tp) != _loaders.end ()); }
		TypeLoader* loader (const std::string& tp);

		/** To load a group of resources. 
			It doesn't return anything, just loaded it. */
		void load (int rgrp);

		private:
		/** A structure to define a list of resources.
			What is needed is the type of resources and the list of them. */
		struct ResourceListDefinition
		{
			std::string _type;
			std::vector <int> _resources;
		};

		/** A structure to define a group of resources. 
			What is needed is just the number of the group and a set of resources. */
		struct ResourceGroupDefinition
		{
			ResourceGroupDefinition ()
				: _id (0), _references (), _resources ()
							{ }

			/** The id of the group. */
			int _id;
			/** References to other groups included in this one. */
			std::vector <int> _references; 
			/** The list of resources that this group consideres. */
			std::vector <ResourceListDefinition> _resources; 
		};

		void readFile (const std::string& dF);
		ResourceGroupDefinition readResourceGroupDefinition (TiXmlElement* e);

		protected:
		typedef std::map <int, ResourceGroupDefinition> Definitions;
		TypeLoaders _loaders;
		Definitions _definitions;
		std::string _definitionFile;
		std::string _basePath;
	};

	/** The common library resources group loader. */
	class CommonLibResourceGroupLoader : public ResourcesGroupLoader
	{
		public:
		CommonLibResourceGroupLoader (const std::string& dF);
	};
}

#endif

// End of the file
/*@}*/