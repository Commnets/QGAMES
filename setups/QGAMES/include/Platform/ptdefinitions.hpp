/** \ingroup Platform */
/*@{*/

/**	
 *	@file	
 *	File: ptdefinitions.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 06/04/2018 \n
 *	Description: Defines platform library definitions. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_PTDEFINITIONS__
#define __QGAMES_PTDEFINITIONS__

// When this string is contained into the form the type is one an no other...
#define __QGAMES_PTTILEFORMBASECONTAINS__			"Base"
#define __QGAMES_PTTBASETILETYPE__					1
#define __QGAMES_PTTILEFORMSOLIDCONTAINS__			"Solid"
#define __QGAMES_PTSOLIDTILETYPE__					2
#define __QGAMES_PTTILEFORMLADDERCONTAINS__			"Ladder"
#define __QGAMES_PTLADDERTILETYPE__					3
#define __QGAMES_PTTILEFORMLIQUIDCONTAINS__			"Liquid"
#define __QGAMES_PTLIQUIDTILETYPE__					4
#define __QGAMES_PTTILEFORMSLIDDINGCONTAINS__		"Slidding"
#define __QGAMES_PTSLIDDINGTILETYPE__				5					
#define __QGAMES_PTTILEFORMKILLINGCONTAINS__		"Killing"
#define __QGAMES_PTKILLINGTILETYPE__				6
#define __QGAMES_PTTILEFORMDECORATIONCONTAINS__		"Decoration"
#define __QGAMES_PTDECORATIONTILETYPE__				7
#define __QGAMES_PTTILEFORMNOTIFICATIONCONTAINS__	"Notification"
#define __QGAMES_PTNOTIFICATIONTILETYPE__			8
#define __QGAMES_PTTILEFORMLOCATIONCONTAINS__		"Location"
#define __QGAMES_PTLOCATIONTILETYPE__				9
#define __QGAMES_PTZONEOBJECTLAYERCONTAINS__		"ObjectZone"
#define __QGAMES_PTZONEOBJECTLAYERTYPE__			10


// The ids of the elements should be among certain limits...
#define __QGAMES_PTSOMETHINGTOEATBASEID__			30000
#define __QGAMES_PTNUMBERSOMETHINGTOEAT__			1000
#define __QGAMES_PTSOMETHINGTOCATCHBASEID__			31000
#define __QGAMES_PTNUMBERSOMETHINGTOCATCH__			1000
#define __QGAMES_PTBADGUYBASEID__					32000
#define __QGAMES_PTNUMBERBADGUY__					1000

// The buoys used in the platform...
#define __QGAMES_PTCHANGESTATEDEFEREDBUOYID__		30000

// The parameters used to identify elements in the artist's pocket
#define __QGAMES_POCKETATTRIBUTEID__				0
#define __QGAMES_POCKETATTRIBUTETYPE__				1

#endif
  
// End of the file
/*@}*/