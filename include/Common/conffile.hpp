/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: conffile.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 23/07/2021 \n
 *	Description: Defines a class to read a configuration file. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_CONFFILE__
#define __QGAMES_CONFFILE__

#include <Common/definitions.hpp>
#include <map>

namespace QGAMES
{
	/** Very simple class to read the configuration file. */
	class ConfigurationFile
	{
		public:
		typedef std::map <std::string, std::string> Properties;

		ConfigurationFile (const std::string& fName = std::string (__GAME_CONFIGURATIONFILE__));

		const Properties& properties () const
							{ return (_properties); }
		bool existsProperty (const std::string& p) const
							{ return (_properties.find (p) != _properties.end ()); }
		const std::string& property (const std::string& p) const
							{ return ((*_properties.find (p)).second); }

		protected:
		Properties _properties;
	};
}

#endif
  
// End of the file
/*@}*/