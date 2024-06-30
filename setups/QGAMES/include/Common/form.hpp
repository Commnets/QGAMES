/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: form.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class to represent a Form. \n
 *				 The form can have many different frames. \n
 *				 A class to represent a specific frame of the form is also defined here. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_FORM__
#define __QGAMES_FORM__

#include <Common/position.hpp>
#include <Common/rectangle.hpp>
#include <Common/box.hpp>
#include <string>
#include <map>
#include <vector>

namespace QGAMES
{
	class Screen;
	class Form;
	class Frame;
	typedef std::map <int, Form*> Forms;
	typedef std::vector <Frame*> Frames;

	/** 
	  * A Form is the graphical representation of a part of the Game (e.g. entity moving). \n
	  *	A Form can usually be made up of several Frames (e.g. different positions of that entity moving). \n
	  *	So a form could be some kind of what in 2D games is call the "sprite sheet", but trying even to cover 3D world. \n
	  *	There are many ways to define a Form. 
	  *	The FormBuilder (@see FormBuilder) is the entity in charge of creating them. \n
	  * That definition has 2 important group of attributes to take into account specially when drawing:
	  * 1.- The size of every frame, that can be the same for all or one per frame. \n
	  * 2.- The offset of every frame, that can be the same for all or one per frame. \n
	  * Every form is defined also by a name and an id.
	  */
	class Form
	{
		public:
		/** A form is made up of several frames. \n
			It is defined by an id, a name, and the number of frames. 
			The constructor creates "the place" for the frames, but it doesn't instantiate them. */
		Form (int id, const std::string& n, int nf);
		/** When the form is deleted, all its frames are also deleted (if they were created). */
		virtual ~Form ();

		/** When a form-frame is requested first time (@see frame method), it is created an kept into memory
			to improve performace in the following acceses. \n
			It is possible to create all form's frames at once with this instruction 
			to improve overall game performance. \n
			This method is used e.g. from the form builder (@see FormBuilder), */
		void preLoad ();

		int id () const
							{ return (_id); }
		const std::string& name () const
							{ return (_name); }
		int numberFrames () const
							{ return (_numberFrames); }

		// The "sizes" of the form. These methods are heavily used in the rest of the game
		/** The width of the form (the full form) in pixels. */
		virtual int width () const = 0; 
		/** The height of the form (the full form) in pixels. */
		virtual int height () const = 0; 
		/** The depth of the form (the full form) in pixels. */
		virtual int depth () const = 0; 

		// The "sizes" of every frame. These methods are heavily used inthe rest of the game
		/** The generic width of a frame of the form in pixels. \n
			A generic size for all frames can be defined and also one specific per frame. */
		virtual int frameWidth () const = 0; 
		/** The generic height of a frame of the form in pixels. \n
			A generic size for all frames can be defined and also one specific per frame. */
		virtual int frameHeight () const = 0; 
		/** The generic depth of a frame of the form in pixels. \n
			A generic size for all frames can be defined and also one specific per frame. */
		virtual int frameDepth () const = 0; 
		/** To know the width of a specific frame. By default the generic one is returned. */
		virtual int frameWidthForFrame (int nF) const 
							{ return (frameWidth ()); }
		/** To know the height of a specific frame. By default the generic one is returned. */
		virtual int frameHeightForFrame (int nF) const 
							{ return (frameHeight ()); }
		/** To know the height of a specific frame. By default the generic one is returned. */
		virtual int frameDepthForFrame (int nF) const 
							{ return (frameDepth ()); }

		/** A vector to know all "sizes" at once. */
		Vector frameSizeVector () const
							{ return (Vector ((bdata) frameWidth (), (bdata) frameHeight (), (bdata) frameDepth ())); }
		/** The same but taken into accoun the specific dimensions of a frame. */
		Vector frameSizeVectorForFrame (int nF) const
							{ return (Vector ((bdata) frameWidthForFrame (nF), (bdata) 
								frameHeightForFrame (nF), (bdata) frameDepthForFrame (nF))); }

		// The offset information for frame
		/** To know the generic offset in the x axis of a frame definition (if any). */
		virtual int frameXOffset () const = 0; 
		/** To know the generic offset in the y axis of a frame definition (if any). */
		virtual int frameYOffset () const = 0;
		/** To know the generic offset in the z axis of a frame definition (if any). */
		virtual int frameZOffset () const = 0;
		/** To know the specific offset in the x axis of a frame definition (if any). 
			By default the generic one is returned. */
		virtual int frameXOffsetForFrame (int nF) const 
							{ return (frameXOffset ()); }
		/** To know the specific offset in the y axis of a frame definition (if any). 
			By default the generic one is returned. */
		virtual int frameYOffsetForFrame (int nF) const 
							{ return (frameYOffset ()); }
		/** To know the specific offset in the z axis of a frame definition (if any). 
			By default the generic one is returned. */
		virtual int frameZOffsetForFrame (int nF) const 
							{ return (frameZOffset ()); }

		/** A vector to know all offsets at once. */
		Vector frameOffsetVector () const
							{ return (Vector ((bdata) frameXOffset (), (bdata) frameYOffset (), (bdata) frameZOffset ())); }
		/** Same, but taking into account the specific frame information. */
		Vector frameOffsetVectorForFrame (int nF) const
							{ return (Vector ((bdata) frameXOffsetForFrame (nF), 
								(bdata) frameYOffsetForFrame (nF), (bdata) frameZOffsetForFrame (nF))); }

		// Important places in the form
		/** Methods to locate special zones of the form. \n
			The way these positions are calculated will depend on how the form is defined. \n
			These method are used from many other elements in the game, specially from entities (@see Entity definition). \n
			To understand what each method is consider a "cube" with the form inside. \n
			This first method is to know the "core" (usually the center) of that hypothetical cube. */
		virtual Position centerPosition (int nF) const = 0;
		/** Someplace (usually The center) in the base of the cube. */
		virtual Position basePosition (int nF) const = 0;
		/** A rectangle rounding the central zone. */
		virtual Rectangle roundingCentralZone (int nF) const = 0;
		/** A top rectangle of the cube. */
		virtual Rectangle coverZone (int nF) const = 0;
		/** The bottom rectangle of the cube. */
		virtual Rectangle baseZone (int nF) const = 0;
		/** A box rounding the full form. */
		virtual Box roundingBox (int nF) const = 0;

		// The "visual dimensions" of the form
		/** The "visualLength" in a 2D frame-form representing some also in 2D is obviouly the same than its width,
			but would it be also the same when it is representing something is a 2.5D (cavalier perspective e.g)? */
		virtual int visualLength (int nF) const
							{ return (frameWidthForFrame (nF)); }
		/** Similar with height */
		virtual int visualHeight (int nF) const
							{ return (frameHeightForFrame (nF)); }
		/** Similar with depth */
		virtual int visualDepth (int nF) const
							{ return (frameDepthForFrame (nF)); }

		/** To get an specific form's frame. \n
			The frame information (after it is created) is allocated in memory to speed up 
			following accesses to the same frame. */
		Frame* frame (int nf);

		/** The Form can be drawn in an screen. \n
			The frame to draw is mandatory. */
		virtual void drawOn (Screen* s, int nf, const Position& p = Position::_noPoint,
			int a = 255);

		protected:
		/** How to create a Frame from a Form will depend the way that From is implemented. \n 
			e.g. Under SDL framework will not the same than under OGRE. 
			One is a 2D engine, and the other ine is a 3D engine. */
		virtual Frame* createFrame (int nf) = 0;

		protected:
		int _id;
		std::string _name;
		int _numberFrames;
		Frames _frames; 
	};

	/** A Frame is a part of the Form. */
	class Frame
	{
		public:
		Frame (Form* f, int nf)
			: _originalForm (f),
			  _numberFrame (nf)
							{ assert (f); }
		virtual ~Frame ()
							{ }

		Form* originalForm ()
							{ return (_originalForm); }
		int numberFrame () const
							{ return (_numberFrame); }

		/** The frame can be drawn in a screen at an specific position. \n
			How to draw a Frame also will depend on the Form implementation. */
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint, int a = 255) = 0;

		protected:
		Form* _originalForm;
		int _numberFrame;
	};
}

#include <Common/sprite2d.hpp>
#include <Common/form3d.hpp>

#endif
  
// End of the file
/*@}*/
