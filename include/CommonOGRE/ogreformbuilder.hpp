/** \ingroup OGRE */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: ogreformbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 24/11/2014 \n
 *	Description: Defines a ogre implememtation of a form builder. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_OGREFORMBUILDER__
#define __QGAMES_OGREFORMBUILDER__

#include <Common/formbuilder.hpp>

/** To create OGRE 2D Forms. 
	This will be used inside next class. */
class OGRE2DFormBuilder : public QGAMES::Sprite2DBuilder
{
	public:
	OGRE2DFormBuilder (const std::string& fDef)
		: QGAMES::Sprite2DBuilder (fDef)
							{ }

	/** @see parent. */
	virtual QGAMES::Form* createForm (QGAMES::FormBuilder::FormDefinition* def);
};

/** A class to create OGRE Meshes and also 2D OGRE Forms used e.g. for other things. */
class OGREFormBuilder : public QGAMES::FormBuilder
{
	public:
	/**
	  * The builder receives the name of the file containing the 3D forms definition,
	  * and also the name of the file with 2D classical definitions. These ones will be
	  * created using SDL library.
	  * The structure of the definition file for OGRE 3D Forms should be as follow:
	  * <?xml version="1.0"?> \n
	  * <Include file="XXX"/> \n
	  * <Forms> \n
	  *		<Form id="X" name="XXX" file="XXXX"/> // it is a mesh really...
	  *		...
	  * </Forms>
	  */
	OGREFormBuilder (const std::string& fDef, 
		const std::string& fDef2D = std::string (__NULL_STRING__));
	~OGREFormBuilder (); // Just to destroy the 2D builder created above...(if any)
	
	/** @see parent. 
		In this case preload has to preload both 3D forms and also 2D. 
		The other method looks first for a 3D form and then for a 2D.
		So there must not be 3D and 2D forms with the same id. */
	virtual void preLoad ();
	virtual QGAMES::Form* form (int fId);

	protected:
	/** @see parent. 
		Creates an OGRE Mesh and assign it to a OGREForm. */
	virtual QGAMES::Form* createForm (QGAMES::FormBuilder::FormDefinition* def);

	private:
	/** So far no special attributes to define a mesh are needed. 
		A specific class is created just for growing purposes. */
	struct OGREMeshDefinition : public FormBuilder::FormDefinition
	{
		OGREMeshDefinition ()
			: FormBuilder::FormDefinition ()
						{ }
	};

	/** To read the file and the definitions per 3D form. */
	void readFile (const std::string& fDef);
	OGREMeshDefinition* readFormDefinition (TiXmlElement* e);

	private:
	/** A referece to the 2D internal builder. */
	OGRE2DFormBuilder* _2DFormBuilder;
};

#endif
  
// End of the file
/*@}*/
