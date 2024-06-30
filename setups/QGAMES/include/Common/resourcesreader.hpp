/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: game.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class to "translate" a resources name into another thing. \n
 *				 This type of classes are used mainly in the builders.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_RESOURCESREADER__
#define __QGAMES_RESOURCESREADER__

#include <map>
#include <string>

namespace QGAMES
{
	/** 
	  * To read the resources. \n
	  *	The resources are usually stored in files, but there can be many 
	  *	different implementations of that files. 
	  *	e.g. There can be a file per resource, this is the normal implementation. 
	  *	e.e. There can be a single file and the resources are a part of it or even in a zip file.
	  *	What it is true is that graphical libraries only know how to read the 
	  *	data from a file usually. So, if the resources are stored in a single file, 
	  *	it is needed to extract the resource required t a single file and then read it. 
	  */
	class ResourceReader
	{
		public:
		/** Constructor. Singleton pattern. */
		ResourceReader ();
		virtual ~ResourceReader ()
							{ }

		static ResourceReader* resourceReader ();

		/** Given the name of the resource,
			It returns the name of the implementation file. */
		virtual std::string implementationNameFor (const std::string& rN) = 0;

		private:
		static ResourceReader* _theResourceReader;
	};

	/** Standard resource reader. 
		It suposses all the resources are kept in a file each. */
	class StandardResourceReader : public ResourceReader
	{
		public:
		StandardResourceReader ()
			: ResourceReader ()
							{ }

		/** @see parent.  */
		virtual std::string implementationNameFor (const std::string& rN)
							{ return (rN); }
	};

	/** The resource reader when the information is in a zip file. \n 
		The internal structure of that files have to be like a zip 
		but the resource files have to be called ending in ".icf". */
	class InZipResourceReader : public ResourceReader
	{
		public:
		InZipResourceReader (const std::string& tL = std::string ("./Temp"));
		~InZipResourceReader ();

		const std::string& tempLocation () const
							{ return (_tempLocation); }
		/** To change the temp location. */
		void setTempLocation (const std::string& tL);

		/** @see parent. 
			First time a resource is requested, all zip files are uncompressed over a temporal structure. \n
			By defaut this temporal place is ".\temp" but can be change at construction time. */
		virtual std::string implementationNameFor (const std::string& rN);

		private:
		typedef std::map <std::string, std::string, std::less <std::string>> MapOfResources;
		MapOfResources _resources; // The list of the resources used...

		// Implementation
		std::string _tempLocation;
	};
}

#ifndef __RL
#define __RL(A)	QGAMES::ResourceReader::resourceReader () -> implementationNameFor (A)
#endif

#endif
  
// End of the file
/*@}*/
