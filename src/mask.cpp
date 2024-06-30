#include <Common/mask.hpp>

// ---
QGAMES::Screen::Buffer* QGAMES::Mask::bufferOver (QGAMES::Screen* scr)
{
	assert (scr);

	if (_buffer)
		return (_buffer);

	int w = _width;
	if (w == -1) w = scr -> visualWidth ();
	int h = _height;
	if (h == -1) h = scr -> visualHeight ();

	_buffer = scr -> openBuffer (_blendingType, w, h);
	drawOver (scr); // Everything will be drawn on the buffer...
	_buffer -> close ();

	return (_buffer);
}

// ---
QGAMES::INMiddleCircleMask::Properties::Properties (const std::map <std::string, std::string>& prms)
	: _radix (0), _color (QGAMES::Color (255, 255, 255, 255))
{
	std::map <std::string, std::string>::const_iterator i;
	if ((i = prms.find (std::string (__QGAMES_CIRCLEMASKRADIXATTR__))) != prms.end ()) 
		_radix = std::stoi ((*i).second);
	if ((i = prms.find (std::string (__QGAMES_CIRCLEMASKCOLORATTR__))) != prms.end ())
		_color = QGAMES::Color ((*i).second);

	QGAMES::INMiddleCircleMask::Properties (_radix, _color);
}

// ---
QGAMES::Mask* QGAMES::INMiddleCircleMask::opposite ()
{ 
	return (new QGAMES::OUTMiddleCircleMask (_id, _blendingType, 
		QGAMES::OUTMiddleCircleMask::Properties (_properties._radix, _properties._color), _width, _height));
}

// ---
void QGAMES::INMiddleCircleMask::drawOver (QGAMES::Screen* scr)
{
	int w = _width;
	if (w == -1) w = scr -> visualWidth ();
	int h = _height;
	if (h == -1) h = scr -> visualHeight ();

	scr -> drawRectangle (
		QGAMES::Rectangle (QGAMES::Position (__BD 0, __BD 0, __BD 0), 
						   QGAMES::Position (__BD w, __BD h, __BD 0), 
						   QGAMES::Vector::_zNormal), 
		QGAMES::Color (0, 0, 0, 255), true);
	scr -> drawCircle (QGAMES::Position (__BD (w >> 1), __BD (h >> 1), __BD 0), QGAMES::Vector::_zNormal, 
		__BD _properties._radix, __BD _properties._radix, _properties._color, true);
}

// ---
QGAMES::OUTMiddleCircleMask::Properties::Properties (const std::map <std::string, std::string>& prms)
	: _radix (0), _color (QGAMES::Color (255, 255, 255, 255))
{
	std::map <std::string, std::string>::const_iterator i;
	if ((i = prms.find (std::string (__QGAMES_CIRCLEMASKRADIXATTR__))) != prms.end ()) 
		_radix = std::stoi ((*i).second);
	if ((i = prms.find (std::string (__QGAMES_CIRCLEMASKCOLORATTR__))) != prms.end ())
		_color = QGAMES::Color ((*i).second);

	QGAMES::OUTMiddleCircleMask::Properties (_radix, _color);
}

// ---
QGAMES::Mask* QGAMES::OUTMiddleCircleMask::opposite ()
{ 
	return (new QGAMES::INMiddleCircleMask (_id, _blendingType, 
		QGAMES::INMiddleCircleMask::Properties (_properties._radix, _properties._color), _width, _height)); 
}

// ---
void QGAMES::OUTMiddleCircleMask::drawOver (QGAMES::Screen* scr)
{
	int w = _width;
	if (w == -1) w = scr -> visualWidth ();
	int h = _height;
	if (h == -1) h = scr -> visualHeight ();

	scr -> drawRectangle (
		QGAMES::Rectangle (QGAMES::Position (__BD 0, __BD 0, __BD 0), 
						   QGAMES::Position (__BD w, __BD h, __BD 0), 
						   QGAMES::Vector::_zNormal), 
		_properties._color, true);
	scr -> drawCircle (QGAMES::Position (__BD (w >> 1), __BD (h >> 1), __BD 0), QGAMES::Vector::_zNormal, 
		__BD _properties._radix, __BD _properties._radix, QGAMES::Color (0, 0, 0, 255), true);
}

// ---
QGAMES::INCircleMask::Properties::Properties (const std::map <std::string, std::string>& prms)
	: _radix (0), _position (__BD 0.5, __BD 0.5, __BD 0.5), _color (QGAMES::Color (255, 255, 255, 255))
{
	std::map <std::string, std::string>::const_iterator i;
	if ((i = prms.find (std::string (__QGAMES_CIRCLEMASKRADIXATTR__))) != prms.end ()) 
		_radix = std::stoi ((*i).second);
	if ((i = prms.find (std::string (__QGAMES_CIRCLEMASKPOSATTR__))) != prms.end ())
		_position = QGAMES::Position ((*i).second);
	if ((i = prms.find (std::string (__QGAMES_CIRCLEMASKCOLORATTR__))) != prms.end ())
		_color = QGAMES::Color ((*i).second);

	QGAMES::INCircleMask::Properties (_radix, _position, _color);
}

// ---
QGAMES::Mask* QGAMES::INCircleMask::opposite ()
{
	return (new QGAMES::OUTCircleMask (_id, _blendingType, 
		QGAMES::OUTCircleMask::Properties 
			(_properties._radix, _properties._position, _properties._color), _width, _height));
}

// ---
void QGAMES::INCircleMask::drawOver (QGAMES::Screen* scr)
{
	int w = _width;
	if (w == -1) w = scr -> visualWidth ();
	int h = _height;
	if (h == -1) h = scr -> visualHeight ();

	scr -> drawRectangle (
		QGAMES::Rectangle (QGAMES::Position (__BD 0, __BD 0, __BD 0), 
						   QGAMES::Position (__BD w, __BD h, __BD 0), 
						   QGAMES::Vector::_zNormal), 
		QGAMES::Color (0, 0, 0, 255), true);
	scr -> drawCircle (QGAMES::Position (__BD w * _properties._position.posX (), 
										 __BD h * _properties._position.posY (), __BD 0), 
		QGAMES::Vector::_zNormal, __BD _properties._radix, __BD _properties._radix, _properties._color, true);
}

// ---
QGAMES::OUTCircleMask::Properties::Properties (const std::map <std::string, std::string>& prms)
	: _radix (0), _position (__BD 0.5, __BD 0.5, __BD 0.5), _color (QGAMES::Color (255, 255, 255, 255))
{
	std::map <std::string, std::string>::const_iterator i;
	if ((i = prms.find (std::string (__QGAMES_CIRCLEMASKRADIXATTR__))) != prms.end ()) 
		_radix = std::stoi ((*i).second);
	if ((i = prms.find (std::string (__QGAMES_CIRCLEMASKPOSATTR__))) != prms.end ())
		_position = QGAMES::Position ((*i).second);
	if ((i = prms.find (std::string (__QGAMES_CIRCLEMASKCOLORATTR__))) != prms.end ())
		_color = QGAMES::Color ((*i).second);

	QGAMES::OUTCircleMask::Properties (_radix, _position, _color);
}

// ---
QGAMES::Mask* QGAMES::OUTCircleMask::opposite ()
{
	return (new QGAMES::INCircleMask (_id, _blendingType, 
		QGAMES::INCircleMask::Properties 
			(_properties._radix, _properties._position, _properties._color), _width, _height));
}

// ---
void QGAMES::OUTCircleMask::drawOver (QGAMES::Screen* scr)
{
	int w = _width;
	if (w == -1) w = scr -> visualWidth ();
	int h = _height;
	if (h == -1) h = scr -> visualHeight ();

	scr -> drawRectangle (
		QGAMES::Rectangle (QGAMES::Position (__BD 0, __BD 0, __BD 0), 
						   QGAMES::Position (__BD w, __BD h, __BD 0), 
						   QGAMES::Vector::_zNormal), 
		_properties._color, true);
	scr -> drawCircle (QGAMES::Position (__BD w * _properties._position.posX (), 
										 __BD h * _properties._position.posY (), __BD 0), 
		QGAMES::Vector::_zNormal, __BD _properties._radix, __BD _properties._radix, QGAMES::Color (0, 0, 0, 255), true);
}

// ---
QGAMES::LeftSideMask::Properties::Properties (const std::map <std::string, std::string>& prms)
	: _distance (0), _color (QGAMES::Color (255, 255, 255, 255))
{
	std::map <std::string, std::string>::const_iterator i;
	if ((i = prms.find (std::string (__QGAMES_ZONEMASKLENGTHATTR__))) != prms.end ()) 
		_distance = std::stoi ((*i).second);
	if ((i = prms.find (std::string (__QGAMES_ZONEMASKCOLORATTR__))) != prms.end ())
		_color = QGAMES::Color ((*i).second);

	QGAMES::LeftSideMask::Properties (_distance, _color);
}

// ---
QGAMES::Mask* QGAMES::LeftSideMask::opposite ()
{ 
	return (new QGAMES::RightSideMask (_id, _blendingType, 
		QGAMES::RightSideMask::Properties (_properties._distance, _properties._color), _width, _height)); 
}

// ---
void QGAMES::LeftSideMask::drawOver (QGAMES::Screen* scr)
{
	int w = _width;
	if (w == -1) w = scr -> visualWidth ();
	int h = _height;
	if (h == -1) h = scr -> visualHeight ();

	scr -> drawRectangle (
		QGAMES::Rectangle (QGAMES::Position (__BD 0, __BD 0, __BD 0), 
						   QGAMES::Position (__BD w, __BD h, __BD 0), 
						   QGAMES::Vector::_zNormal), 
		QGAMES::Color (0, 0, 0, 255), true);
	scr -> drawRectangle (
		QGAMES::Rectangle (QGAMES::Position (__BD 0, __BD 0, __BD 0),
						   QGAMES::Position (__BD _properties._distance, __BD h, __BD 0),
						   QGAMES::Vector::_zNormal),
		_properties._color, true);
}

// ---
QGAMES::RightSideMask::Properties::Properties (const std::map <std::string, std::string>& prms)
	: _distance (0), _color (QGAMES::Color (255, 255, 255, 255))
{
	std::map <std::string, std::string>::const_iterator i;
	if ((i = prms.find (std::string (__QGAMES_ZONEMASKLENGTHATTR__))) != prms.end ()) 
		_distance = std::stoi ((*i).second);
	if ((i = prms.find (std::string (__QGAMES_ZONEMASKCOLORATTR__))) != prms.end ())
		_color = QGAMES::Color ((*i).second);

	QGAMES::RightSideMask::Properties (_distance, _color);
}

// ---
QGAMES::Mask* QGAMES::RightSideMask::opposite ()
{ 
	return (new QGAMES::LeftSideMask (_id, _blendingType, 
		QGAMES::LeftSideMask::Properties (_properties._distance, _properties._color), _width, _height)); 
}

// ---
void QGAMES::RightSideMask::drawOver (QGAMES::Screen* scr)
{
	int w = _width;
	if (w == -1) w = scr -> visualWidth ();
	int h = _height;
	if (h == -1) h = scr -> visualHeight ();

	scr -> drawRectangle (
		QGAMES::Rectangle (QGAMES::Position (__BD 0, __BD 0, __BD 0), 
						   QGAMES::Position (__BD w, __BD h, __BD 0), 
						   QGAMES::Vector::_zNormal), 
		QGAMES::Color (0, 0, 0, 255), true);
	scr -> drawRectangle (
		QGAMES::Rectangle (QGAMES::Position (__BD _properties._distance, __BD 0, __BD 0),
						   QGAMES::Position (__BD w, __BD h, __BD 0),
						   QGAMES::Vector::_zNormal),
		_properties._color, true);
}

// ---
QGAMES::UpSideMask::Properties::Properties (const std::map <std::string, std::string>& prms)
	: _distance (0), _color (QGAMES::Color (255, 255, 255, 255))
{
	std::map <std::string, std::string>::const_iterator i;
	if ((i = prms.find (std::string (__QGAMES_ZONEMASKLENGTHATTR__))) != prms.end ()) 
		_distance = std::stoi ((*i).second);
	if ((i = prms.find (std::string (__QGAMES_ZONEMASKCOLORATTR__))) != prms.end ())
		_color = QGAMES::Color ((*i).second);

	QGAMES::UpSideMask::Properties (_distance, _color);
}

// ---
QGAMES::Mask* QGAMES::UpSideMask::opposite ()
{ 
	return (new QGAMES::DownSideMask (_id, _blendingType, 
		QGAMES::DownSideMask::Properties (_properties._distance, _properties._color), _width, _height)); 
}

// ---
void QGAMES::UpSideMask::drawOver (QGAMES::Screen* scr)
{
	int w = _width;
	if (w == -1) w = scr -> visualWidth ();
	int h = _height;
	if (h == -1) h = scr -> visualHeight ();

	scr -> drawRectangle (
		QGAMES::Rectangle (QGAMES::Position (__BD 0, __BD 0, __BD 0), 
						   QGAMES::Position (__BD w, __BD h, __BD 0), 
						   QGAMES::Vector::_zNormal), 
		QGAMES::Color (0, 0, 0, 255), true);
	scr -> drawRectangle (
		QGAMES::Rectangle (QGAMES::Position (__BD 0, __BD 0, __BD 0),
						   QGAMES::Position (__BD w, __BD _properties._distance, __BD 0),
						   QGAMES::Vector::_zNormal),
		_properties._color, true);
}

// ---
QGAMES::DownSideMask::Properties::Properties (const std::map <std::string, std::string>& prms)
	: _distance (0), _color (QGAMES::Color (255, 255, 255, 255))
{
	std::map <std::string, std::string>::const_iterator i;
	if ((i = prms.find (std::string (__QGAMES_ZONEMASKLENGTHATTR__))) != prms.end ()) 
		_distance = std::stoi ((*i).second);
	if ((i = prms.find (std::string (__QGAMES_ZONEMASKCOLORATTR__))) != prms.end ())
		_color = QGAMES::Color ((*i).second);

	QGAMES::DownSideMask::Properties (_distance, _color);
}

// ---
QGAMES::Mask* QGAMES::DownSideMask::opposite ()
{ 
	return (new QGAMES::UpSideMask (_id, _blendingType, 
		QGAMES::UpSideMask::Properties (_properties._distance, _properties._color), _width, _height)); 
}

// ---
void QGAMES::DownSideMask::drawOver (QGAMES::Screen* scr)
{
	int w = _width;
	if (w == -1) w = scr -> visualWidth ();
	int h = _height;
	if (h == -1) h = scr -> visualHeight ();

	scr -> drawRectangle (
		QGAMES::Rectangle (QGAMES::Position (__BD 0, __BD 0, __BD 0), 
						   QGAMES::Position (__BD w, __BD h, __BD 0), 
						   QGAMES::Vector::_zNormal), 
		QGAMES::Color (0, 0, 0, 255), true);
	scr -> drawRectangle (
		QGAMES::Rectangle (QGAMES::Position (__BD 0, __BD _properties._distance, __BD 0),
						   QGAMES::Position (__BD w, __BD h, __BD 0),
						   QGAMES::Vector::_zNormal),
		_properties._color, true);
}
