/** \ingroup OGRE */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: ogreform.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 24/12/2014 \n
 *	Description: Defines a sdl implememtation of a form. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_OGREFORM__
#define __QGAMES_OGREFORM__

#include <Common/form.hpp>
#include <OgreHeaderPrefix.h>
#include <Ogre.h>
#include <OgreHeaderSuffix.h>

/** Represents an OGRE Mesh.
	It is a 3D pure entity. */
class OGREForm : public QGAMES::Form
{
	public:
	OGREForm (int id, const std::string& n, const Ogre::MeshPtr& mesh);
	// No additional destructor is neded...

	const Ogre::MeshPtr& mesh () const
							{ return (_mesh); }

	/** @see parent. */
	virtual int width () const;
	virtual int height () const; 
	virtual int depth () const;
	virtual int frameWidth () const { return (width ()); }
	virtual int frameHeight () const { return (height ()); }
	virtual int frameDepth () const	{ return (depth ()); }
	virtual int frameXOffset () const { return (0); } // Pixels are aligned...
	virtual int frameYOffset () const { return (0); }
	virtual int frameZOffset () const { return (0); }

	/** @see parent. */
	virtual QGAMES::Position centerPosition (int nF) const;
	virtual QGAMES::Position basePosition (int nF) const;
	virtual QGAMES::Rectangle roundingCentralZone (int nF) const;
	virtual QGAMES::Rectangle coverZone (int nF) const;
	virtual QGAMES::Rectangle baseZone (int nF) const;
	virtual QGAMES::Box roundingBox (int nF) const
							{ return (QGAMES::Box (baseZone (nF), __BD visualHeight (nF))); }

	protected:
	/** @see parent. */
	virtual QGAMES::Frame* createFrame (int nf)
							{ return (new Frame (this, nf)); }

	protected:
	/** A reference to the ogre mesh used in the form. */
	Ogre::MeshPtr _mesh;

	// Implementation
	/** The internal frame representing the mesh.
		A mesh can have animation but in the case of meshes it is not managed changing the frames visualizaed,
		But using configuration parameters of the entity when defined. 
		So the frame is neved drawn. */
	class Frame : public QGAMES::Frame
	{
		public:
		Frame (OGREForm* f, int nf)
			: QGAMES::Frame (f, nf)
							{ }

		/** @see parent. */
		virtual void drawOn (QGAMES::Screen* s, 
			const QGAMES::Position& p = QGAMES::Position::_noPoint, int a = 255)
							{ }
	};
};

/** But OGRE can also manage 2D Forms represented like a OGRE Image.
	Images can be drawn in both 2D world or 3D world.
	What it has to be clear is that 3D world (world, scenes, maps and entities in them) will
	be drawn first then the 2D forms whether with 3D aspect or 2D 
	depending on the camera projection type. */
class OGRE2DForm : public QGAMES::Sprite2D
{
	public:
	OGRE2DForm (int id, const std::string& n, int nf, const std::string& s, int w, int h, 
		QGAMES::Sprite2DFormData* fD, Ogre::Image* image);
	virtual ~OGRE2DForm ();

	Ogre::Image* image ()
							{ return (_image); }

	protected:
	Ogre::Image* _image;

	// It is not necessary to define the fram, 
	// because it is enought what Sprite2D defines by default
};

#endif
  
// End of the file
/*@}*/
