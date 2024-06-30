#include <Arcade/fixscreenonartist.hpp>
#include <Arcade/artist.hpp>
#include <Arcade/map.hpp>

// ---
QGAMES::ToFixScreenOnArtist::ToFixScreenOnArtist (QGAMES::Screen* s, 
	QGAMES::Artist* a, QGAMES::bdata pW, QGAMES::bdata pH, QGAMES::bdata pD)
	: ToFixScreen (s),
	  _artist (a),
	  _percentageX (pW), _percentageY (pH), _percentageZ (pD),
	  _considerXAxis (true), _considerYAxis (true), _considerZAxis (true),
	  _lastPosX (__MININT__), _lastPosY (__MININT__), _lastPosZ (__MININT__)
{
	if (pW <= 0 || pW > 1)
		_percentageX = 0.5; 
	if (pH <= 0 || pH > 1)
		_percentageY = 0.5; 
	if (pD <= 0 || pD > 1)
		_percentageZ = 0.5; 
	// If the percentaje is not valid, a default one is provided...
}

// ---
void QGAMES::ToFixScreenOnArtist::moveScreen ()
{
	if (!_considerXAxis && !_considerYAxis && !_considerZAxis)
		return; // If none of the axis has to be taken into account, nothing to do here...

	// The data to make the calculus
	assert (_artist); // The artist has to exist...
	QGAMES::Map* map = _artist -> map ();
	assert (map); // A valid map has to be on place...
	int sPosX = (int) _screen -> position ().posX ();
	int sPosY = (int) _screen -> position ().posY ();
	int sPosZ = (int) _screen -> position ().posZ ();
	int posX = (int) _artist -> position ().posX () /**- sPosX*/; // In the visual area... 
	int posY = (int) _artist -> position ().posY () /**- sPosY */;
	int posZ = (int) _artist -> position ().posZ () /**- sPosY */;
	int sWidth = _screen -> visualWidth (); // The visual area...
	int sHeight = _screen -> visualHeight ();
	int uWidth = (int) (__BD sWidth * _percentageX);
	int uHeight = (int) (__BD sHeight * _percentageY);
	int mWidth = map -> width (); // The maximum width of the map in pixels...
	int mHeight = map -> height (); // The maximum height of the map in pixels...
	int mDepth = map -> depth (); // The maximum depth of the map in pixels...

	// It is time to decide where the artist is moving to
	// because depending on that one limit or other will be applied...
	int xDirection = (_lastPosX == __MININT__) ? 0 
		: ((posX > _lastPosX) ? 1 : ((posX < _lastPosX) ? -1 : 0)); 
	int yDirection = (_lastPosY == __MININT__) ? 0 
		: ((posY > _lastPosY) ? 1 : ((posY < _lastPosY) ? -1 : 0));
	int zDirection = (_lastPosZ == __MININT__) ? 0
		: ((posZ - _lastPosZ) ? 1 : ((posZ < _lastPosZ) ? -1 : 0));

	// How many pixels to move?
	int incX = 0; int incY = 0; int incZ = 0;
	// ..in the x Axis?
	if (_considerXAxis)
	{
		if (_lastPosX == __MININT__) // _lastPosX == __MININT__ means first time...
		{
			if ((posX - sPosX) > (sWidth - uWidth))
			{
				incX = (posX - sPosX) - (sWidth - uWidth);
				if ((sPosX + incX + sWidth) > mWidth)
					incX = mWidth - sWidth - sPosX;
			}
		}
		else
		{
			if ((posX - sPosX) > (sWidth - uWidth) && xDirection == 1 && sPosX < (mWidth - sWidth))
				incX = (posX - sPosX) - (sWidth - uWidth);
			if ((posX - sPosX) < uWidth && xDirection == -1 && sPosX > 0)
				incX = (posX - sPosX) - uWidth;
		}
	}

	// ...and in the Y axis?
	if (_considerYAxis)
	{
		if (_lastPosY == __MININT__) // _lastPosY == __MININT__ means first time...
		{
			if ((posY - sPosY) > (sHeight - uHeight))
			{
				incY = (posY - sPosY) - (sHeight - uHeight);
				if ((sPosY + incY + sHeight) > mHeight)
					incY = mHeight - sHeight - sPosY;
			}
		}
		else
		{
			if ((posY - sPosY) > (sHeight - uHeight) && yDirection == 1 && sPosY < (mHeight - sHeight))
				incY = (posY -sPosY) - (sHeight - uHeight);
			if ((posY - sPosY) < uHeight && yDirection == -1 && sPosY > 0)
				incY = (posY - sPosY) - uHeight;
		}
	}

	// ...and in the Z axis?
	if (_considerZAxis)
	{
		if (_lastPosZ == __MININT__) // _lastPosZ == __MININT__ means first time...
		{
			if ((posZ - sPosZ) > 0)
			{
				incZ = posZ - sPosZ;
				if ((sPosZ + incZ) > mDepth)
					incZ = mDepth - sPosZ;
			}
		}
		else
		{
			if ((posZ - sPosZ) > 0 && zDirection == 1 && sPosZ < mDepth)
				incZ = posZ -sPosZ;
			if ((posZ - sPosZ) < 0 && zDirection == -1 && sPosZ > 0)
				incZ = posZ - sPosZ;
		}
	}

	// If the artist has to be set, then it is time to move it
	if (incX != 0 || incY != 0 || incZ != 0)
	{
		if (_lastPosX != __MININT__ &&
			(((sPosX + incX) > (mWidth - sWidth)) || ((sPosX + incX) < 0)) )incX = 0;
		if(_lastPosY != __MININT__ &&
			(((sPosY + incY) > (mHeight - sHeight)) || ((sPosY + incY) < 0))) incY = 0;
		if(_lastPosZ != __MININT__ &&
			(((sPosZ + incZ) > mDepth) || ((sPosZ + incZ) < 0))) incZ = 0;
		moveScreenPixels (incX, incY, incZ);
	}

	// Keep track of the last position...
	_lastPosX = posX;
	_lastPosY = posY;
	_lastPosZ = posZ;
}

// ---
void QGAMES::ToFixScreenOnArtist::moveScreenPixels (int x, int y, int z)
{
	_screen -> move (QGAMES::Position (__BD x, __BD y, __BD z));
}
