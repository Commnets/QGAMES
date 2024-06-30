/** \ingroup OGRE */
/*@{*/

/**	
 *	@file	
 *	File: ogreinclude.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 11/12/2018 \n
 *	Description: Just a simple file to include all files used by QGAMES OGRE Library. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_OGREINCLUDE__
#define __QGAMES_OGREINCLUDE__

#ifdef _MSC_VER
#pragma warning(disable:4251) // To avoid the warning due to not using dllimport in Ogre's STL uses...
#endif
#include <CommonOGRE/ogredefinitions.hpp>
#include <CommonOGRE/ogreform.hpp>
#include <CommonOGRE/ogreformbuilder.hpp>
#include <CommonOGRE/ogregame.hpp>
#include <CommonOGRE/ogreinputhandler.hpp>
#include <CommonOGRE/ogremovementcontroller.hpp>
#include <CommonOGRE/ogrescreen.hpp>
#include <FMOD/fmodsound.hpp> // OGRE library uses FMOD sound librray to produce sounds...
#include <FMOD/fmodsoundbuilder.hpp>

#endif
  
// End of the file
/*@}*/

