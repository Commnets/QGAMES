/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: sprite2D.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 07/01/2021 \n
 *	Description: Defines a class to represent an sprite2D Form.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_SPRITE2DFORM__
#define __QGAMES_SPRITE2DFORM__

#include <Common/form.hpp>

namespace QGAMES
{
	/** Class to hold the info about a frame in a Sprite 2D. */
	class Sprite2DFrameData
	{
		public:
		/** To hold how the visual measures are represented in the frame. 
			Their values will depend on the type of the projection of the frame. */
		struct VisualData
		{
			VisualData () 
				: _Vw (-1), _Vd (-1), _Vh (-1), _Rx (-1)
							{ } // Meaning not valid...

			VisualData (int vw, int vd, int vh, int rx)
				: _Vw (vw), _Vd (vd), _Vh (vh), _Rx (rx)
							{ assert (_Vw >= 0 && _Vd >= 0 && _Vh >= 0 && _Rx >= 0); } // All them positive

			VisualData (const VisualData& vD) = default;

			VisualData& operator = (const VisualData& vD) = default;

			int Vw () const
							{ return (_Vw); }
			int Vd () const
							{ return (_Vd); }
			int Vh () const
							{ return (_Vh); }
			int Rx () const
							{ return (_Rx); }

			private:
			int _Vw, _Vd, _Vh, _Rx; // Will depend on the type of projection...
									// The data is directly accesed...
		};

		/** A structure to keep the main positions and zones of a frame. 
			Their values will also depend on the type of the projection of the frame. */
		struct MainRectanglesAndPositions
		{
			MainRectanglesAndPositions ()
				: _centerPosition (Position::_noPoint),
				  _basePosition (Position::_noPoint),
				  _roundingCentralZone (Rectangle::_noRectangle),
				  _baseZone (Rectangle::_noRectangle),
				  _coverZone (Rectangle::_noRectangle),
				  _roundingBox (Box::_noBox)
							{ } // Meaning not valid

			MainRectanglesAndPositions (const Position& cP, const Position& bP, 
				const Rectangle& rZ, const Rectangle& bZ,const Rectangle& cZ, const Box& rB)
				: _centerPosition (cP),
				  _basePosition (bP),
				  _roundingCentralZone (rZ),
				  _baseZone (bZ),
				  _coverZone (cZ),
				  _roundingBox (rB)
							{ }

			MainRectanglesAndPositions (const MainRectanglesAndPositions& mR) = default;

			MainRectanglesAndPositions& operator = (const MainRectanglesAndPositions& mR) = default;

			const Position& centerPosition () const
							{ return (_centerPosition); }
			const Position& basePosition () const
							{ return (_basePosition); }
			const Rectangle& roundingCentralZone () const
							{ return (_roundingCentralZone); }
			const Rectangle& baseZone () const
							{ return (_baseZone); }
			const Rectangle& coverZone () const
							{ return (_coverZone); }
			const Box& roundingBox () const
							{ return (_roundingBox); }

			private:
			Position _centerPosition;
			Position _basePosition;
			Rectangle _roundingCentralZone;
			Rectangle _baseZone;
			Rectangle _coverZone;
			Box _roundingBox;
		};

		Sprite2DFrameData () = delete; // Not needed at all...

		Sprite2DFrameData (int w, int h, int xP, int yP, int dW, int dH, int xO, int yO, int aT)
			: _width (w), _height (h),
			  _xPosition (xP), _yPosition (yP),
			  _definitionWidth (dW), _definitionHeight (dH),
			  _xOffset (xO), _yOffset (yO),
			  _adjustType (aT),
			  _visualData (), _mainRectanglesAndPositions (), _Pr (Position::_noPoint)
			 // The default values are not valid so,
			 // In the classes inheriting this one, visual data and main recatangles and positions (and in that order)
			 // have to be calculated before being used! Otherwise it won't work properly
			 // And then the value of _Pr need also to be adjusted!
							{ 
							  /** In this way: 
								  _visualData = calculateVisualData (); 
								  _mainRectanglesAndPositions = calculateMainRectanglesAndPositions (); 
								  _Pr = Position ((bdata) (_xOffset + Rx ()), (bdata) _yOffset, (bdata) 0);
								*/ 
							} 

		Sprite2DFrameData (const Sprite2DFrameData& fD) = default;

		Sprite2DFrameData& operator = (const Sprite2DFrameData& fD) = default;

		/** To know the type of projection of the frame. 
			It has to be overloaded in other classes inheriting from this one. */
		virtual int projectionType () const = 0;

		// Info about the frame
		int width () const
							{ return (_width); }
		int height () const
							{ return (_height); }
		int xPosition () const
							{ return (_xPosition); }
		int yPosition () const
							{ return (_yPosition); }
		int definitionWidth () const
							{ return (_definitionWidth); }
		int definitionHeight () const
							{ return (_definitionHeight); }
		int xOffset () const
							{ return (_xOffset); }
		int yOffset () const
							{ return (_yOffset); }
		int adjustType () const
							{ return (_adjustType); }

		// The visual appearance of the frame...
		int Vw () const
							{ return (_visualData.Vw ()); }
		int Vd () const
							{ return (_visualData.Vd ()); }
		int Vh () const
							{ return (_visualData.Vh ()); }
		int Rx () const
							{ return (_visualData.Rx ()); }

		// Main position within the frame...
		const Position& centerPosition () const
							{ return (_mainRectanglesAndPositions.centerPosition ()); }
		const Position& basePosition () const
							{ return (_mainRectanglesAndPositions.basePosition ()); }
		const Rectangle& roundingCentralZone () const
							{ return (_mainRectanglesAndPositions.roundingCentralZone ()); }
		const Rectangle& coverZone () const
							{ return (_mainRectanglesAndPositions.coverZone ()); }
		const Rectangle& baseZone () const
							{ return (_mainRectanglesAndPositions.baseZone ()); }
		const Box& roundingBox () const
							{ return (_mainRectanglesAndPositions.roundingBox ()); }

		protected:
		/** It has to be overloaded attenging the type of frame.
			That's it: It not the same when the projection is orthogonal, siometric, or cavalier. */
		virtual VisualData calculateVisualData () const = 0;
		virtual MainRectanglesAndPositions calculateMainRectanglesAndPositions () const = 0;

		// Implementation
		// This point represents the up left corner of the graphical visualization of the frame
		const Position& Pr () const
							{ return (_Pr); }

		protected:
		int _width, _height;
		int _xPosition, _yPosition;
		int _definitionWidth, _definitionHeight;
		int _xOffset, _yOffset;
		int _adjustType; // Type of adjustment, used in some type of projections...

		// Implementation
		VisualData _visualData;
		MainRectanglesAndPositions _mainRectanglesAndPositions;
		Position _Pr;
	};

	// To simplify the management of this type of elements...
	using ListOfSprite2DFrameData = std::vector <Sprite2DFrameData*>;

	/** Data for an orthogonal frame. */
	class Sprite2DOrthogonalFrameData : public Sprite2DFrameData
	{
		public:
		Sprite2DOrthogonalFrameData (int w, int h, int xP, int yP, int dW, int dH, int xO, int yO, int aT /** Not used */);

		/** @see parent. */
		virtual int projectionType () const override final
							{ return (0); } // Meaning ORTHOGONAL

		protected:
		/** @see parent. 
			the adjustType is not taken into account. */
		virtual VisualData calculateVisualData () const final override
							{ return (VisualData (width (), 0 /** Plain*/, height (), 
								0 /** Rx from the corner */)); }
		virtual MainRectanglesAndPositions calculateMainRectanglesAndPositions () const final override;
	};

	/** For any axonometric projection (ISOMETRIC; CAVALIER,...) */
	class Sprite2DAxonometricFrameData : public Sprite2DFrameData
	{
		public:
		Sprite2DAxonometricFrameData (int w, int h, int xP, int yP, int dW, int dH, int xO, int yO, int aT)
			: Sprite2DFrameData (w, h, xP, yP, dW, dH, xO, yO, aT)
							{ }

		protected:
		/** @see parent. 
			In the axonmetric views the main points and rectangles are calculated in the same way. */
		virtual MainRectanglesAndPositions calculateMainRectanglesAndPositions () const final override;
	};

	/** For an isometric frame. */
	class Sprite2DIsometricFrameData : public Sprite2DAxonometricFrameData
	{
		public:
		Sprite2DIsometricFrameData (int w, int h, int xP, int yP, int dW, int dH, int xO, int yO, int aT);

		/** @see parent. */
		virtual int projectionType () const override final
							{ return (1); } // Meaning ISOMETRIC

		protected:
		/** @see parent. */
		virtual VisualData calculateVisualData () const final override;
	};

	/** For a cavalier frame. */
	class Sprite2DCavalierFrameData : public Sprite2DAxonometricFrameData
	{
		public:
		Sprite2DCavalierFrameData (int w, int h, int xP, int yP, int dW, int dH, int xO, int yO, int aT);

		/** @see parent. */
		virtual int projectionType () const override final
							{ return (2); } // Meaning CAVALIER

		protected:
		/** @see parent. */
		virtual VisualData calculateVisualData () const final override;
	};

	/** A class to create the right Sprite2DFrameData from the type of projection only. */
	class Sprite2DFrameDataFactory
	{
		public:
		virtual ~Sprite2DFrameDataFactory () 
							{ } // Just if neccessary later...

		virtual Sprite2DFrameData* createFrameDataForProjection (int pT, /** type of projection. */ 
			int w, int h, int xP, int yP, int dW, int dH, int xO, int yO, int aT) const;
	};

	/** Class to hold the information about a 2D form and the frames in it. */
	class Sprite2DFormData
	{
		public:
		Sprite2DFormData () = delete;

		/** All frames are equaled defined in the supporting file, so only glocal info about the form is needed,
			and (onbiously) the builder for the frame data that takes into account the projection. */
		Sprite2DFormData (int nF, int pT, int s, int m, int w, int h, int fw, int fh, int aC, int aT,
					Sprite2DFrameDataFactory* fF);
		/** All frames can be different, so not only the general form info is needed but also the detail per frame. */
		Sprite2DFormData (int nF, int pT, int s, int m, int w, int h, int fw, int fh, int aC, int aT,
					const ListOfSprite2DFrameData& fDt); 

		~Sprite2DFormData (); /** The form data own frames' info */

		// To know wheter is basic or not...
		// That is: info about the frames has or not been defined!
		bool isBasic () const
							{ return (_basicFormData); }

		// Generic info
		int numberFrames () const
							{ return (_numberFrames); }
		int projectionType () const
							{ return (_projectionType); }
		int spacing () const
							{ return (_spacing); }
		int margin () const
							{ return (_margin); }
		int width () const
							{ return (_width); }
		int height () const
							{ return (_height); }
		int standardFrameWidth () const
							{ return (_standardFrameWidth); }
		int standardFrameHeight () const
							{ return (_standardFrameHeight); }
		int alphaColor () const
							{ return (_alphaColor); }
		int adjustType () const
							{ return (_adjustType); }
		const ListOfSprite2DFrameData& framesInfo () const
							{ return (_framesInfo); }

		protected:
		// Generic values...
		int _numberFrames; // The number of frame the form has...
		int _projectionType; // Type of projection used in the frame...
		int _spacing, _margin; /** The _spacing between two frame's information.
								   The _margin at the right of the form's definition. */
		int _width, _height; /** width and height of the form's information. */
		int _standardFrameWidth, _standardFrameHeight; /** Valid only unless it's been defined per frame. */
		int _alphaColor; /** The alpha color used to transparent the sprite information when drawn. */
		int _adjustType; // Within the projection type which adjust is required! It is translated to the frames info.
		ListOfSprite2DFrameData _framesInfo; // The information about the frames

		// Implementation
		bool _basicFormData; // When all elements are almost similar...
	};

	/** An Sprite2D is a plain Form (equivalent to a 2D Sprite Sheet). \n
		Usually the definition of the Form wil be in file. */
	class Sprite2D : public Form
	{
		public:
		Sprite2D (int id, const std::string& n, int nf, const std::string& s, int w, int h, Sprite2DFormData* fD)
			: Form (id, n, nf),
				_source (s),
				_width (w),
				_height (h),
				_formData(fD)
							{ assert (_formData != NULL); }

		virtual ~Sprite2D () // It is the owner...
							{ delete (_formData); }

		/** @see parent. */
		virtual int width () const override
							{ return (formData () -> width ()); }
		virtual int height () const override
							{ return (formData () -> height ()); }
		virtual int depth () const override
							{ return (0);  } // Makes no sense in a plain sprite sheet definition

		virtual int frameWidth () const override
							{ return (formData () -> standardFrameWidth ()); } 
		virtual int frameWidthForFrame (int nF) const override
							{ return (formData () -> framesInfo ()[nF] -> width ()); } 
		virtual int frameHeight () const override
							{ return (formData () -> standardFrameHeight ()); } 
		virtual int frameHeightForFrame (int nF) const override
							{ return (formData () -> framesInfo ()[nF] -> height ()); } 
		virtual int frameDepth () const override
							{ return (0); } // Makes no sense in a plain sprite sheet definition
		virtual int frameDepthForFrame (int nF) const override
							{ return (0); } // Makes no sense in a plain sprite sheet definition

		virtual int frameXOffset () const override
							{ return (0); } // Makes no sense a generical frame offset in this type of definition (COMMTYDEF)
		virtual int frameXOffsetForFrame (int nF) const override
							{ return (formData () -> framesInfo ()[nF] -> xOffset ()); }
		virtual int frameYOffset () const override
							{ return (0); }  // Makes no sense a generical frame offset in this of definition (COMMTYDEF)
		virtual int frameYOffsetForFrame (int nF) const override
							{ return (formData () -> framesInfo ()[nF] -> yOffset ()); }
		virtual int frameZOffset () const override
							{ return (0); } // Makes no sense in a plain sprite sheet definition
		virtual int frameZOffsetForFrame (int nF) const override
							{ return (0); } // Makes no sense in a plain sprite sheet definition

		/** @see parent. 
			Note that all these method return a copy of the information finally kept. */
		virtual Position centerPosition (int nF) const override
							{ return (formData () -> framesInfo ()[nF] -> centerPosition ()); }
		virtual Position basePosition (int nF) const override
							{ return (formData () -> framesInfo ()[nF] -> basePosition ()); }
		virtual Rectangle roundingCentralZone (int nF) const override
							{ return (formData () -> framesInfo ()[nF] -> roundingCentralZone ()); }
		virtual Rectangle coverZone (int nF) const override
							{ return (formData () -> framesInfo ()[nF] -> coverZone ()); }
		virtual Rectangle baseZone (int nF) const override
							{ return (formData () -> framesInfo ()[nF] -> baseZone ()); }
		virtual Box roundingBox (int nF) const override
							{ return (formData () -> framesInfo ()[nF] -> roundingBox ()); }
		virtual int visualLength (int nF) const override
							{ return (formData () -> framesInfo ()[nF] -> Vw ()); }
		virtual int visualHeight (int nF) const override
							{ return (formData () -> framesInfo ()[nF] -> Vh ()); }
		virtual int visualDepth (int nF) const override
							{ return (formData () -> framesInfo ()[nF] -> Vd ()); }

		/** The original information. */
		const Sprite2DFormData* formData () const
							{ return (_formData); }
		Sprite2DFormData* formData ()
							{ return (_formData); }
		const std::string& source () const
							{ return (_source); }

		protected:
		/** To create the frames of the form. */
		virtual Frame* createFrame (int nF) override;

		protected:
		std::string _source;
		int _width, _height;
		Sprite2DFormData* _formData;
	};

	/** The frame of a Sprite2D. 
		The information received as parameter is not owned by this object. */
	class Sprite2DFrame : public Frame
	{
		public:
		Sprite2DFrame (Sprite2D* s, int nf, Sprite2DFrameData* fD)
			: Frame (s, nf),
			  _data (fD) // It is not owned by the frame but by the form (see Sprite2DForm destructor...
							{ assert (fD); } // Can't be null...'

		/** @see parent. */
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint, int a = 255) override;

		protected:
		Sprite2DFrameData* _data;
	};
}

#endif
  
// End of the file
/*@}*/
