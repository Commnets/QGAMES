#include <Common/sprite2d.hpp>
#include <Common/game.hpp>
#include <Common/screen.hpp>

// ---
QGAMES::Sprite2DOrthogonalFrameData::Sprite2DOrthogonalFrameData 
		(int w, int h, int xP, int yP, int dW, int dH, int xO, int yO, int aT /** Not used */)
	: Sprite2DFrameData (w, h, xP, yP, dW, dH, xO, yO, aT)
{ 
	_visualData = calculateVisualData (); /** The ones defined here last invoked */ 

	_Pr = Position ((bdata) (_xOffset + Rx ()), (bdata) _yOffset, (bdata) 0);

	_mainRectanglesAndPositions = calculateMainRectanglesAndPositions (); 
}

// ---
QGAMES::Sprite2DFrameData::MainRectanglesAndPositions 
	QGAMES::Sprite2DOrthogonalFrameData::calculateMainRectanglesAndPositions () const
{
	QGAMES::Position PrDown = Pr () + QGAMES::Vector (__BD 0, __BD Vh (), __BD 0);
	QGAMES::Rectangle baseZone (PrDown, PrDown + QGAMES::Vector (__BD Vw (), __BD 0, __BD 0), QGAMES::Vector::_yNormal);
	QGAMES::Box rBox (baseZone.pos1 (), QGAMES::Vector::_xNormal, QGAMES::Vector::_zNormal, 
		baseZone.width (), baseZone.height (), __BD Vh ());

	return (
		QGAMES::Sprite2DFrameData::MainRectanglesAndPositions
			(QGAMES::Position	(Pr () + 
				QGAMES::Vector (__BD (Vw () >> 1), __BD (Vh () >> 1), __BD 0)), // centralPosition
			 QGAMES::Position	(PrDown + 
				 QGAMES::Vector (__BD (Vw () >> 1), __BD 0, __BD 0)), // basePosition
			 QGAMES::Rectangle	(Pr (), 
				 Pr () + QGAMES::Vector (__BD Vw (), __BD Vh (), __BD 0), QGAMES::Vector::_zNormal), // roundingCentralZone
			 baseZone, // baseZone
			 QGAMES::Rectangle	(Pr (), 
				 Pr () + QGAMES::Vector (__BD Vw (), __BD 0, __BD 0), QGAMES::Vector::_yNormal), // coverZone
			 rBox // roundingBox
			));
}

// ---
QGAMES::Sprite2DFrameData::MainRectanglesAndPositions 
	QGAMES::Sprite2DAxonometricFrameData::calculateMainRectanglesAndPositions () const
{
	QGAMES::Position PrDown = Pr () + QGAMES::Vector (__BD 0, __BD 0, __BD Vh ());
	QGAMES::Rectangle baseZone (PrDown, PrDown + QGAMES::Vector (__BD Vw (), __BD Vd (), __BD 0), QGAMES::Vector::_zNormal);
	QGAMES::Position PrUpMiddle = Pr () + QGAMES::Vector (__BD 0, __BD (Vd () >> 1), __BD 0);
	QGAMES::Rectangle roundingCentralZone (PrUpMiddle, 
		PrUpMiddle + Vector (__BD Vw (), __BD 0, __BD Vh ()), QGAMES::Vector::_yNormal);

	return (
		QGAMES::Sprite2DFrameData::MainRectanglesAndPositions
			(roundingCentralZone.center (), // centerPosition
			 baseZone.center (), // basePosition
			 roundingCentralZone, // roundingCentralZone
			 baseZone, // baseZone
			 QGAMES::Rectangle (Pr (), 
				 Pr () + QGAMES::Vector (__BD Vw (), __BD Vd (), __BD 0), QGAMES::Vector::_zNormal), // coverZone
			 QGAMES::Box (baseZone, -__BD Vh () // roundingBox
			)));
}

// ---
QGAMES::Sprite2DIsometricFrameData::Sprite2DIsometricFrameData 
		(int w, int h, int xP, int yP, int dW, int dH, int xO, int yO, int aT /** Not used */)
	: Sprite2DAxonometricFrameData (w, h, xP, yP, dW, dH, xO, yO, aT)
{ 
	_visualData = calculateVisualData (); /** The ones defined here last invoked */ 

	_Pr = QGAMES::Position (__BD (((_xOffset + Rx ()) >> 1) + _yOffset),
							__BD (-((_xOffset + Rx ()) >> 1) + _yOffset), __BD 0);

	_mainRectanglesAndPositions = calculateMainRectanglesAndPositions (); 
}

// ---
QGAMES::Sprite2DFrameData::VisualData QGAMES::Sprite2DIsometricFrameData::calculateVisualData () const
{
	QGAMES::Sprite2DFrameData::VisualData result;

	int vh = height () - (width () >> 1);
	if (vh < 0)
	{
		__WLOGPLUS (std::string ("Isometric frame not well defined (") + std::to_string (width ()) + std::string ("x") +
			std::to_string (height ()) + std::string (")"));
		vh = 0; // It is not possible. The minimum visual height should be 0
	}

	switch (adjustType ())
	{
		case 0: // Very long (in the y axis)
			result = QGAMES::Sprite2DFrameData::VisualData 
				(0 /** Vw */, 
				 width () /** Vd */, 
				 vh /** Vh */, 
				 width () /** Rx */);
			break;

		case 1: // Squares (usual)
			result = QGAMES::Sprite2DFrameData::VisualData 
				(width () - (width () >> 1) /** Vw */, 
				 width () >> 1 /** Vd */, 
				 vh /** Vh */, 
				 width () >> 1 /** Rx */);
			break;

		case 2: // Wide (in the x axis)
			result = QGAMES::Sprite2DFrameData::VisualData 
				(width () - (width () >> 2) /** Vw */, 
				 width () >> 2 /** Vd */, 
				 vh /** Vh */, 
				 width () >> 2 /** Rx */);
			break;

		case 3: // Very wide (in the x axis)
			result = QGAMES::Sprite2DFrameData::VisualData 
				(width () - (width () >> 3) /** Vw */, 
				 width () >> 3 /** Vd */, 
				 vh /** Vh */, 
				 width () >> 3 /** Rx */);
			break;

		default:
			assert (0); // No more values are allowed...
	}

	return (result);
}

// ---
QGAMES::Sprite2DCavalierFrameData::Sprite2DCavalierFrameData 
		(int w, int h, int xP, int yP, int dW, int dH, int xO, int yO, int aT /** Not used */)
	: Sprite2DAxonometricFrameData (w, h, xP, yP, dW, dH, xO, yO, aT)
{ 
	_visualData = calculateVisualData (); /** The ones defined here last invoked */ 

	_Pr = QGAMES::Position (__BD ((_xOffset + Rx ()) + (_yOffset << 1)), 
							__BD (_yOffset << 1), __BD 0);

	_mainRectanglesAndPositions = calculateMainRectanglesAndPositions (); 
}

// ---
QGAMES::Sprite2DFrameData::VisualData QGAMES::Sprite2DCavalierFrameData::calculateVisualData () const
{
	QGAMES::Sprite2DFrameData::VisualData result;

	int vh = height () - (width () >> (projectionType () + 1));
	if (vh < 0)
	{
		__WLOGPLUS (std::string ("Cavalier frame not well defined (") + std::to_string (width ()) + std::string ("x") +
			std::to_string (height ()) + std::string (")"));
		vh = 0; // It is not possible. The minimum visual height should be 0...
	}

	switch (adjustType ())
	{
		case 0: // Long (int Y axis)
			result = QGAMES::Sprite2DFrameData::VisualData 
				(width () >> 2 /** Vw */, 
				 width () /** Vd */, 
				 vh /** Vh */, 
				 width () - (width () >> 2) /** Rx */);
			break;

		case 1: // Squared (usual)
			result = QGAMES::Sprite2DFrameData::VisualData 
				(width () - (width () >> 1) /** Vw */, 
				 width () >> 1 /** Vd */, 
				 vh /** Vh */, 
				 width () >> 1 /** Rx */);
			break;

		case 2: // Wide (int the x axis)
			result = QGAMES::Sprite2DFrameData::VisualData 
				(width () - (width () >> 2) /** Vw */, 
				 width () >> 2 /** Vd */, 
				 vh /** Vh */, 
				 width () >> 2 /** Rx */);
			break;

		case 3: // Very wide (int the x axis)
			result = QGAMES::Sprite2DFrameData::VisualData 
				(width () - (width () >> 3) /** Vw */, 
				 width () >> 3 /** Vd */, 
				 vh /** Vh */, 
				 width () >> 3 /** Rx */);
			break;

		default:
			assert (0); // No more values are allowed...
	}

	return (result);
}

// ---
QGAMES::Sprite2DFrameData* QGAMES::Sprite2DFrameDataFactory::createFrameDataForProjection 
	(int pT, /** type of projection. */ 
			int w, int h, int xP, int yP, int dW, int dH, int xO, int yO, int aT) const
{
	QGAMES::Sprite2DFrameData* result = NULL;

	switch (pT)
	{
		case 0: // Meaning ORTHOGONAL
			result = new QGAMES::Sprite2DOrthogonalFrameData (w, h, xP, yP, dW, dH, xO, yO, aT);
			break;

		case 1: // Meaning ISOMETRIC
			result = new QGAMES::Sprite2DIsometricFrameData (w, h, xP, yP, dW, dH, xO, yO, aT);
			break;

		case 2: // Meaning CAVALIER
			result = new QGAMES::Sprite2DCavalierFrameData (w, h, xP, yP, dW, dH, xO, yO, aT);
			break;

		default:
			assert (0); // It shouldn't be here...
	}

	return (result);
}

// ---
QGAMES::Sprite2DFormData::Sprite2DFormData 
		(int nF, int pT, int s, int m, int w, int h, int fw, int fh, int aC, int aT, QGAMES::Sprite2DFrameDataFactory* fF)
	: _numberFrames (nF),
	  _projectionType (pT),
	  _spacing (s), _margin (m),
	  _width (w), _height (h),
	  _standardFrameWidth (fw), _standardFrameHeight (fh),
	  _alphaColor (aC), _adjustType (aT),
	  _framesInfo (QGAMES::ListOfSprite2DFrameData ()), // Initially. It would be change later...
	  _basicFormData (true)
{
	assert (fF != NULL); // A factory for the frames is needed!
	assert ((_standardFrameWidth + _spacing) > 0);

	int wS = (_width - (_margin << 1) + _spacing) / (_standardFrameWidth + _spacing); // Number of elements per row...
	assert (wS > 0); // The number of elements per row should be 1 minimum otherwise, no sense to continue!

	_framesInfo.resize (_numberFrames);
	for (int i = 0; i < _numberFrames; i++)
		_framesInfo [i] = 
			fF -> createFrameDataForProjection (
				_projectionType, // It will determine the type of frame created actually!
				_standardFrameWidth, _standardFrameHeight, 
				(i % wS /** column */) * (_standardFrameWidth + _spacing) + _margin /** from the left */, 
				(i / wS /** row */) * (_standardFrameHeight + _spacing) + _margin /** from top */, 
				_standardFrameWidth, _standardFrameHeight, 0 /** no x offset */, 0 /** no y offset */, _adjustType);
}

// ---
QGAMES::Sprite2DFormData::Sprite2DFormData (int nF, int pT, int s, int m, int w, int h, int fw, int fh, int aC, int aT,
		const QGAMES::ListOfSprite2DFrameData& fDt) 
	: _numberFrames (nF),
	  _projectionType (pT),
	  _spacing (s), _margin (m),
	  _width (w), _height (h),
	  _standardFrameWidth (fw), _standardFrameHeight (fh),
	  _alphaColor (aC), _adjustType (aT),
	  _framesInfo (fDt),
	  _basicFormData (false) 
{ 
	assert (_numberFrames > 0); 

	assert ((int) fDt.size () == _numberFrames);
}

// ---
QGAMES::Sprite2DFormData::~Sprite2DFormData ()
{ 
	for (int i = 0; i < (int) _framesInfo.size (); delete (_framesInfo [i++]));
	_framesInfo.clear ();
}

// ---
QGAMES::Frame* QGAMES::Sprite2D::createFrame (int nF)
{ 
	return (new QGAMES::Sprite2DFrame (this, nF, formData () -> framesInfo ()[nF])); 
}

// ---
void QGAMES::Sprite2DFrame::drawOn (QGAMES::Screen* s, const QGAMES::Position& p, int a)
{
	assert (dynamic_cast <Sprite2D*> (_originalForm));

	// When we are speaking about a 2D sprite, the size and the offset can't be modified
	// The sprite is as it is...
	// So it makes no sense to include the offset as part of the coordinate passed as parameter to the screen
	// Makes more sense to consider the offset as a simple offset to the final drawing position calculated.
	s -> drawForm (p, _data -> xOffset (), _data -> yOffset (), 
			_originalForm, _data -> xPosition (), _data -> yPosition (), 
			_data -> definitionWidth (), _data -> definitionHeight (),
			(a == -1) ? ((Sprite2D*) _originalForm) -> formData () -> alphaColor () : a);
}
