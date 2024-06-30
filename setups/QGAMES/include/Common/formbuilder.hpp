/** \ingroup Game */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: formbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class to create forms from a file. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_FORMBUILDER__
#define __QGAMES_FORMBUILDER__

#include <Common/definitions.hpp>
#include <Common/form.hpp>
#include <tinyxml.h>
#include <string>
#include <map>
#include <vector>

namespace QGAMES
{
	/** To create forms from a file. */
	class FormBuilder
	{
		public:
		/** The name of the file has to be suplied. */
		FormBuilder (const std::string& fDef);
		/** The builder is accountable for any form got from it. \n
			When the builder is destroyed all froms created from it are also deleted. */
		virtual ~FormBuilder ();

		/** To get the number of definitions included in the definition file. */
		int numberResources () const
							{ return ((int) _definitions.size ()); }
		/** To get the id of every resource to be loaded. */
		std::vector <int> resourcesId () const;

		/** Once the form's definitions have been created at construction time,
			it is possible to load in memory all forms as well. Doing so, the
			performance of the game will improve. \n
			This method also preload the frames of every loaded form. \n
			It can also be overloaded. However it is not usual. */
		virtual void preLoad ();

		/** If the form requested has not been created yet when this method is invoked,
			it will be loaded into memory to speed up further accesses. \n
			It can be overloaded but it should be the usual thing to do. */
		virtual Form* form (int fId);

		/** To either get the id of a form (the first) 
			or to verify one at least exists using its source. */
		bool existsFormForFileDefinition (const std::string& f) const;
		int formIdForFileDefinition (const std::string& f) const;
		/** To either get the id of a form (the first) 
			or to verify one at least exists using its name. */
		bool existsFormForName (const std::string& n) const;
		int formIdForName (const std::string& n) const;

		protected:
		struct FormDefinition
		{
			FormDefinition ()
				: _id (-1),
				  _name (__NULL_STRING__),
				  _fileDefinition (__NULL_STRING__),
				  _numberFrames (0)
							{ }
			
			FormDefinition (int id, const std::string& n, const std::string& fD, int nF)
				: _id (id), 
				  _name (n),
				  _fileDefinition (fD),
				  _numberFrames (nF)
							{ }

			virtual ~FormDefinition ()
							{ }

			int _id;
			std::string _name;
			std::string _fileDefinition;
			int _numberFrames;
		};

		/** To be redefined likely per game, or at least per type of graphical system. */
		virtual Form* createForm (FormDefinition* def) = 0;

		protected:
		typedef std::map <int, FormDefinition*> Definitions;

		Definitions _definitions;
		Forms _forms;

		std::string _definitionFile;
		std::string _basePath;
	};
}

#include <Common/sprite2dbuilder.hpp>
#include <Common/form3dbuilder.hpp>

#endif
  
// End of the file
/*@}*/
