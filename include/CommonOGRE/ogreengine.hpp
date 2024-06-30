/** \ingroup OGRE */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: ogreengine.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 18/12/2017 \n
 *	Description: Defines a class to manage external actualizations of any element. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_OGREENGINE__
#define __QGAMES_OGREENGINE__

#include <Common/_exengine.hpp>
#include <Common/element.hpp>
#include <Common/widgetsystem.hpp>
#include <Common/form.hpp>
#include <Common/entity.hpp>
#include <Common/color.hpp>
#include <Common/position.hpp>
#include <OgreHeaderPrefix.h>
#include <Ogre.h>
#include <OgreHeaderSuffix.h>

class OGREGame;

/** An struct to represent any additional data for any element built. */
class OGREElementAdditionalData: public QGAMES::Element::ExternalData
{
	public:
	OGREElementAdditionalData ();

	// Attributes common to any element...
	Ogre::SceneManager* _ogreSceneManager;
	// Attributes for an entity...
	Ogre::SceneNode* _ogreSceneNode;
	Ogre::Entity* _ogreEntity;
	Ogre::AnimationState* _ogreAnimationState;
	QGAMES::Form* _lastForm;
	QGAMES::Entity::Animation* _lastAnimation;
	// Attributes for an scene...
	QGAMES::Color _ambientLight;
	QGAMES::Position _lightPosition;
};

/** The class to acatualize in the external system any element change. */
class OGREExternalEngine : public QGAMES::_ExEngine
{
	public:
	OGREExternalEngine (OGREGame* gI);

	/** @see parent. */
	virtual void whenCreateGUISystem (QGAMES::GUISystem* sys);
	virtual void whenDeleteGUISystem (QGAMES::GUISystem* sys);
	virtual void whenInitialize (QGAMES::GUISystem* sys);
	virtual void whenInEveryLoop (QGAMES::GUISystem* sys);
	virtual void whenDrawOn (QGAMES::GUISystem* sys, QGAMES::Screen* s, const QGAMES::Position& p);

	/** @see parent. */
	virtual void whenCreateElement (QGAMES::Element* elmnt);
	virtual void whenDeleteElement (QGAMES::Element* elmnt);
	virtual void whenInitialize (QGAMES::Element* elmnt);
	virtual void whenInEveryLoop (QGAMES::Element* elmnt);
	virtual void whenUpdatePositions (QGAMES::Element* elmnt);
	virtual void whenDrawOn (QGAMES::Element* elmnt, QGAMES::Screen* s, const QGAMES::Position& p);
	virtual void whenFinalize (QGAMES::Element* elmnt);

	protected:
	OGREGame* _ogreGame;
};


#endif
  
// End of the file
/*@}*/