/** \ingroup Game */
/** \ingroup Foundation */
/*@{*/

/**	
 *	@file	
 *	File: uniqueidgenerator.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 24/02/2021 \n
 *	Description: Define a class to generate unique ids. The mechanism can be extended. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_UNIQUEIDGENERATOR__
#define __QGAMES_UNIQUEIDGENERATOR__

namespace QGAMES
{
	class UniqueIdGenerator
	{
		public:
		/** The constructor should fix the base to generate ids.
			The destructor is defined just in case. */
		virtual ~UniqueIdGenerator ()
							{ }

		/** To get the next id. */
		virtual int nextId () const = 0;
	};

	/** A very simple way to generate a unique id:
		Just adding one to the last one generated, starting from a base. */
	class StandardUniqueIdGenerator final : public UniqueIdGenerator
	{
		public:
		StandardUniqueIdGenerator ()
			: _id ((int) time (NULL))
							{ }

		/** @see parent. */
		virtual int nextId () const override
							{ return (_id++); }

		protected:
		mutable int _id;
	};
}

#endif
