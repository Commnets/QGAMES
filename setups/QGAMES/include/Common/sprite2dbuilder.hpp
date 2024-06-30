/** \ingroup Game */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: sprite2dbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 08/02/2021 \n
 *	Description: Defines a class to create sprites 2d from a file. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_SPRITE2DBUILDER__
#define __QGAMES_SPRITE2DBUILDER__

#include <Common/formbuilder.hpp>

namespace QGAMES
{
	/** A builder for 2D forms.
		What defines a 2D form is not the same that in the case of a 3D one i.e. */
	class Sprite2DBuilder : public FormBuilder
	{
		public:
		/** 
		  * A definition file is received as parameter. \n
		  *	The structure of the file is as follows: \n
		  *	Take care, it could contain include directive... \n
		  *	The definition of the Frames can have two different possibilities: \n
		  *	One to define the width and the height of the image within the standard frame. \n
		  *	This is used when all the frames ocuppy the same size, but the image inside
		  *	has different dimensions. \n
		  *	The other one is to indicate where the frame is specifically located within the form
		  *	indicating both the x coordinate and the y coordinate, and its width and height. \n
		  *	The projection type parameter is optional. If nothing is defined then orthogonal is understood. \n
		  *	The next three parameters are also optional and determinates the way to draw a 2D form in 3D world: \n
		  *	vOffset is the distance from the left size of the sprite where the top point of the image is. \n
		  *	vLength is what visually the width of the image is. vDepth is the visual depth and makes no sense in orthogonal. \n
		  *	By default they are equal to -1 what means that the form will apply standard values. \n
		  *	<?xml version="1.0"?> \n
		  *	<Forms> \n
		  *		<Include file="CCCCC"/> \n
		  *		<Form id="X" name="XXX" file=".\XXXX" width="XX" height="XX" 
		  *			fSpacing="X" fMargin="X" fWidth="XX" fHeight="XX" aColor="XX" projectionType="0/1/2" 
		  *			vOffset="XX" vLength="XX" vDepth="XX" nFrames="X"/> \n
		  *			<Frame id="VV" width="XX" height="YY"/> \n
		  *			<Frame id="VV" fPosX="XX" fPosY="YY" fWidth="XX" fHeight="YY" width="XX" height="YY" xoffset="X" yoffset="Y"/> \n
		  *		... \n
		  *	</Forms> \n
		  *
		  * The data of every frame is kept in a special structure needed by (actually) Sprite2D (inheriting from Form)
		  * That structure could be different regarding the type of projection the frame emulates. \n
		  * So, it is needed a class to create every specific version (a factory builder).
		  * That factory builder is also passed as parameter. \n
		  * The factory is owned by the builder, so it has to be destoiyed at destruction time.
		  */
		Sprite2DBuilder (const std::string& fDef, 
			Sprite2DFrameDataFactory* fF = new Sprite2DFrameDataFactory /** The basic one by default is enough. */);

		virtual ~Sprite2DBuilder ()
							{ delete (_frameDataFactory); _frameDataFactory = NULL; }

		const Sprite2DFrameDataFactory* frameDataFactory () const
							{ return (_frameDataFactory); }
		Sprite2DFrameDataFactory* frameDataFactory ()
							{ return (_frameDataFactory); }

		/** It is possible to add a form to the builder defined from data no read from
			the initial definition file. (the Tmx AddsOn map builder uses it. */
		Form* formDefined (int id, const std::string& n, 
			int nf, const std::string& s, int w, int h, int sFW, int sFH, int spc, int mrg, int aC, int pT, int aT, 
			const ListOfSprite2DFrameData& fD, 
			const std::map <std::string, std::string>& prps = std::map <std::string, std::string> ());

		protected:
		/** A standard structure to define a 2D form. */
		struct SpriteDefinition : public FormBuilder::FormDefinition
		{
			public:
			/** A standard structure to define a frame within a 2D form. */
			struct FrameDefinition
			{
				FrameDefinition ()
					: _frameNumber (-1),
					  _width (0), _height (0),
					  _xPosition (0), _yPosition (0),
					  _definitionWidth (0), _definitionHeight (0),
					  _xOffset (0), _yOffset (0)
							{ }

				FrameDefinition (int fN, int fW, int fH, int xP, int yP, int dW, int dH, int xO, int yO)
					: _frameNumber (fN),
					  _width (fW), _height (fH),
					  _xPosition (xP), _yPosition (yP),
					  _definitionWidth (dW), _definitionHeight (dH),
					  _xOffset (xO), _yOffset (yO)
							{ }

				int _frameNumber;	  
				int _width, _height;
				int _xPosition, _yPosition;
				int _definitionWidth, _definitionHeight;
				int _xOffset, _yOffset;
			};

			typedef std::vector <FrameDefinition> FrameDefinitions;

			SpriteDefinition ()
				: FormBuilder::FormDefinition (),
				  _width (-1), _height (-1), _spacing (-1), _margin (-1),
				  _standardFrameWidth (-1), _standardFrameHeight (-1),
				  _alphaColor (255),
				  _projectionType (0), // The orthogonal by default
				  _adjustType (1), // The usual adjust type (valid for both cavalier and isometric) 
				  _frameDefinitions (), // No special definition per frame
				  _properties () // No properties by default...
							{ }

			SpriteDefinition (int id, const std::string& n, const std::string& fD, int nF,
					int w, int h, int fS, int fM, int fW, int fH, int aC, int pT, int aT, 
				const FrameDefinitions& frD, 
				const std::map <std::string, std::string>& prps = std::map <std::string, std::string> ())
				: FormBuilder::FormDefinition (id, n, fD, nF),
				  _width (w), _height (h), _spacing (fS), _margin (fM),
				  _standardFrameWidth (fW), _standardFrameHeight (fH),
				  _alphaColor (aC),
				  _projectionType (pT), 
				  _adjustType (aT),
				  _frameDefinitions (frD),
				  _properties (prps)
							{ }

			// Normal data...
			int _width, _height;
			int _spacing, _margin;
			int _standardFrameWidth, _standardFrameHeight;
			int _alphaColor;
			int _projectionType; // 0: ORTHONGONAL, 1: ISOMETRIC, 2: CAVALIER
			int _adjustType; // Within the projection type which type of adjust will be used!
			FrameDefinitions _frameDefinitions;
			std::map <std::string, std::string> _properties;
		};

		void readFile (const std::string& fDef);
		SpriteDefinition* readFormDefinition (TiXmlElement* e);

		// Implementation...
		/** The one receiving this info is accountable to delete it. */
		Sprite2DFormData* createSprite2DFormDataFromSpriteDefinition (SpriteDefinition* def);

		protected:
		// Implementation
		Sprite2DFrameDataFactory* _frameDataFactory; // It is own by the builder...
	};
}

#endif
  
// End of the file
/*@}*/
