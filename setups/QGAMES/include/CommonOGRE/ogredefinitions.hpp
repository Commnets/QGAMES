/** \ingroup OGRE */
/*@{*/

/**	
 *	@file	
 *	File: ogredefinitions.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 14/11/2015 \n
 *	Description: Defines ogre library definitions. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_OGREDEFINITIONS__
#define __QGAMES_OGREDEFINITIONS__

// The default gorup where things created are defined in
#define __OGREDEFAULTRESOURCESGROUP__		"General"

// In the OGRE forms builder
#define __OGREREFIDFORMATTR__				"red2DId"

// The name of the variable containing the name of the file with the resources
#define __OGRERESOURCESFILEVARIABLE__		"RESOURCESFILE"
#define __OGREDEFAULTRESOURCESFILE__		"resources.cfg"
// The name of the variable containing the name of the file with the plugins used by ogre
#define __OGREPLUGINSFILEVARIABLE__			"PLUGINSFILE"
#define __OGREDEFAULTPLUGINSFILE__			"plugins.cfg"
// The name of the variable containing the name of the file where to keep (if any) information of caches...
#define __OGRECACHEFILEVARIABLE__			"CACHEFILE"
#define __OGREDEFAULTCACHEFILE__			"cache.bin"
// The default background color used in very viewport created
#define __OGREDEFAULTVIEWPORTBACKGROUND__	Ogre::ColourValue (0, 0, 0)
// The name of the main scene manager in any game (it has to be at least one)
#define __OGREMAINSCENEMANAGERID__			"MAINSCENEMANAGER"
// The name of the main camera used in the game (if any)
#define __OGREMAINCAMERAID__				"MAINCAMERA"

// The prefix used by the system to name every node where an entity is attached
#define __OGREENTITYNODEPREFIX__			"ETY"
// The same but for an score object 3D
#define __OGRESCOREOBJNODEPREFIX__			"SCOBJ"

// Name of the attribute in the animation definition to define the name of the animation
#define __OGREANIMATIONNAMEVARIABLE__		"ANIMATION"
// Name of the attribute in the animation definition to indicare whether the animation is or not cyclic
#define __OGREANIMATIONLOOPVARIABLE__		"LOOP"
// Name of the attribute with the rational of the speed (the bigger the quicker)
#define __OGREANIMATIONLOOPSPEEDVARIABLE__	"SPEED"

// Name of the attribute in the scene definition to describe the color of an ambient light...
#define __OGREAMBIENTLIGHTVARIABLE__		"AMBIENTLIGHT"
// Name of the attribute in the scene definition to hold where the definition of a light (name and position)
#define __OGRELIGHTVARIABLE__				"LIGHT"
// Name of the attribute with a big background
#define __OGRESKYDOMEVARIABLE__				"SKYDOME"

// The name of the overlay layer where the score objects of the gamne will be drawn...
#define __OGREOVERLAYLAYENAME__				"ScoreObjects"

#endif
  
// End of the file
/*@}*/
