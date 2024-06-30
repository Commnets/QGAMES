/** \ingroup Game */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: elementbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class base for any builder able to create elements. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ELEMENTBUILDER__
#define __QGAMES_ELEMENTBUILDER__

#include <Common/definitions.hpp>
#include <Common/_exengine.hpp>

namespace QGAMES
{
	class Element;

	/** This is the base class for any element builder. \n
		Its main responsability is to link with the external engine after
		creating a new element, if that external engine has been defined. \n
		The class doesn't own the external engine, just a reference to it. */
	class ElementBuilder
	{
		public:
		ElementBuilder () 
			: _externalEngine (NULL)
							{ }
		virtual ~ElementBuilder () 
							{ } // Just if it is needed later...

		/** To assign an external engine. */
		_ExEngine* externalEngine ()
							{ return (_externalEngine); }
		void setExternalEngine (_ExEngine* egn)
							{ _externalEngine = egn; }

		protected:
		/** This method is invoked previously to create a new element.
			The element is defined by its id. 
			By default it does nothing. */
		virtual void beforeCreateElement (int id)
							{ }
		/** This method is used by any futher method responsable to create
			a new element, and its main responsability is to invoke the
			right engine's method to complete the new element. */
		virtual void afterCreateElement (Element* elmnt)
							{ if (_externalEngine) _externalEngine -> whenCreateElement (elmnt); }

		protected:
		_ExEngine* _externalEngine;
	};
}

#endif
  
// End of the file
/*@}*/
