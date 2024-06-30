/** \ingroup OGRE */
/** \ingroup Screen */
/*@{*/

/**	
 *	@file	
 *	File: ogremovementcontroller.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/11/2017 \n
 *	Description: Defines a class to control the movements of a camenra in games. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_OGRECAMERACONTROLLER__
#define __QGAMES_OGRECAMERACONTROLLER__

#include <Common/screen.hpp>
#include <Common/inputhandler.hpp>
#include <OgreHeaderPrefix.h>
#include <Ogre.h>
#include <OgreHeaderSuffix.h>
#include <limits>

/** Wrapper for managging the internal camera of ogre. */
class OGRECameraWrapper : public QGAMES::Camera::ExternalLink
{
	public:
	OGRECameraWrapper (Ogre::Camera* oC)
		: _ogreCamera (oC)
						{ }
	// The wrapper doesn own the ogre camera...
	// So it is not deleted. No destructor needed!

	// Managing the camera connected...
	/** Swaps the camera on our camera man for another camera. */
	virtual void setCamera (Ogre::Camera* cam)
							{ _ogreCamera = cam; }
	virtual Ogre::Camera* getCamera ()
							{ return (_ogreCamera); }

	/** @see parent. */
	virtual void setPosition (const QGAMES::Position& pos)
							{ _ogreCamera -> setPosition (Ogre::Vector3 
								(Ogre::Real (pos.posX ()), Ogre::Real (pos.posY ()), Ogre::Real (pos.posZ ()))); }
	virtual void setOrientation (const QGAMES::Vector& or, const QGAMES::Vector& hv)
							{ _ogreCamera -> setDirection (Ogre::Vector3
								(Ogre::Real (or.posX ()), Ogre::Real (or.posY ()), Ogre::Real (or.posZ ()))); }

protected:
	Ogre::Camera* _ogreCamera;
};

/** An OGRE camera with movement. 
	The method update has to be redefined to take into account the time between two frames. 
	Anycase the external link has to be set before doing anything, otherwise nothing will hapen in the real screen. */
class OGREFreeMovementCamera : public QGAMES::FreeMovementCamera
{
	public:
	OGREFreeMovementCamera (const std::string& n, Ogre::Camera* oC, 
			const Keys& k = Keys (), QGAMES::bdata tS = __BD 150, QGAMES::bdata aR = __BD 0.15f)
		: QGAMES::FreeMovementCamera (n, k, tS, aR),
		  _ogreCamera (oC)
							{ }

	/** To get the camera. */
	virtual Ogre::Camera* getCamera ()
							{ return (_ogreCamera); }
	
	/** @see parent. */
	virtual void updatePositions (); // The camera moves taking into account the speed of actualization of the frames...

	protected:
	Ogre::Camera* _ogreCamera;
};

#endif
