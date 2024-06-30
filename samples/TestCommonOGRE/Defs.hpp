/** \ingroup Example */
/*@{*/

/**	
 *	@file	
 *	File: main.cpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/01/2018 \n
 *	Description: A pure QGAMES game based in OGRE behind. Definition file.
 *	Versions: 1.0 Initial
 */

#ifndef __TESTCOMMONOGREDEFS__
#define __TESTCOMMONOGREDEFS__

#include <Arcade/arcadeinclude.hpp>
#include <CommonOGRE/ogreinclude.hpp>

// The entities of the test, and the form used behind...
#define __OGREHEADBASEENTITY__		1
#define __OGREHEADNUMBERENTITIESX__	5
#define __OGREHEADNUMBERENTITIESY__	18
#define __OGREHEADNUMBERENTITIES__	(__OGREHEADNUMBERENTITIESX__ * __OGREHEADNUMBERENTITIESY__)
#define __OGRESEPAMONGENTITIES__	90
#define __OGREHEADFORM__			1
#define __NINJAFORM__				2
#define __ROBOTFORM__				3
// The world, scene and map...
#define __OGREHEADWORLD__			1
#define __OGREHEADSCENE__			11
#define __OGREHEADMAP__				111
// The number of the introductory sound...
#define __OGREHEADINTROSOUND__		1

// The score object
#define __OGRESCOREOBJBASEENTITY__	100
#define __OGRENUMBERSCOREOBJECTS__	2
  
#endif

// End of the file
/*@}*/
