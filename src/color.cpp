#include <Common/color.hpp>
#include <Common/definitions.hpp>

// ---
QGAMES::Color::Color (const std::string& c)
	: _red (0), _green (0), _blue (0), _alpha (255)
{
	std::string cR = c;
	int cN [4] = { 0,0,0,255 };
	int pos = 0;
	while (cR != std::string (__NULL_STRING__))
	{
		int pC = cR.find_first_of ('.');
		if (pC != -1)
		{
			std::string nS = cR.substr (0, pC - 1);
			cR = cR.substr (pC);
			cN [pos++] = atoi (nS.c_str ());
		}
		else
			cR = std::string (__NULL_STRING__);
	}

	_red = cN [0];
	_green = cN [1];
	_blue = cN [2];
	_alpha = cN [3];

	adjust ();
}

// ---
void QGAMES::Color::adjust ()
{ 
	if (_red <= 0) _red = 0; if (_red >= 255) _red = 255;
	if (_green <= 0) _green = 0; if (_green >= 255) _green = 255;
	if (_blue <= 0) _blue = 0; if (_blue >= 255) _blue = 255;
	if (_alpha <= 0) _alpha = 0; if (_alpha >= 255) _alpha = 255; 
}
