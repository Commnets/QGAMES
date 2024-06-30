#include <Common/comms.hpp>
#include <Common/genalgorithms.hpp>
#include <Common/game.hpp>
#include <sstream>
#include <assert.h>
#include <ctime>
#include <iomanip>
#include <colorwin.hpp>

// ---
QGAMES::CommunicationChannel::CommunicationChannel (QGAMES::CommunicationChannelImplementation* imp, int bSize)
	: _implementation (imp),
	  _maxNumberElementsInTheBuffer (bSize),
	  _buffer (),
	  _currentElementInTheBuffer (-1) // Empty by default...
{ 
	assert (_implementation);
	assert (_maxNumberElementsInTheBuffer > 1); // It has to be positive and greater than 1...

	_buffer = std::vector <std::string> (_maxNumberElementsInTheBuffer, std::string (__NULL_STRING__));

	_implementation -> openChannel ();

	assert (_implementation -> isChannelOpened ());
}

// ---
QGAMES::CommunicationChannel::~CommunicationChannel ()
{
	if (_implementation -> isChannelOpened ())
		_implementation -> closeChannel ();

	delete (_implementation);
	_implementation = NULL;
}

// ---
void QGAMES::CommunicationChannel::setImplementation (QGAMES::CommunicationChannelImplementation* i)
{
	assert (i); // It can't be null at all...

	delete (_implementation); // Deletes the previous one...
	_implementation = i;
}

// ---
void QGAMES::CommunicationChannel::setMaxNumberElementsInTheBuffer (int bSize)
{
	assert (bSize > 1); // It has to be positive and greater than 1...

	_maxNumberElementsInTheBuffer = bSize;
	_buffer = std::vector <std::string> (_maxNumberElementsInTheBuffer, std::string (__NULL_STRING__));
	_currentElementInTheBuffer = -1; // Starts back...
}

// ---
const std::string& QGAMES::CommunicationChannel::elementReceived (int nElmnt) const
{
	assert (nElmnt < (int) _buffer.size ());

	return (_buffer [nElmnt]);
}

// ---
int QGAMES::CommunicationChannel::send (const QGAMES::CommunicationChannelTarget& to, const std::string& str)
{
	if (!_implementation -> isChannelOpened ())
		return (-1); // It won't be possible to send until it is opened...
	
	return (_implementation -> sendString (to, str)); 
}

// ---
int QGAMES::CommunicationChannel::send (const QGAMES::CommunicationChannelTarget& to, const QGAMES::SetOfOpenValues& oV)
{
	if (!_implementation -> isChannelOpened ())
		return (-1);

	std::ostringstream oS;
	oS << oV; // It is converted to something "plane" and send throught the communication channel...
	return (_implementation -> sendString (to, oS.str ())); 
	// If it was needed to eliminate (e.g) characters from the string because it wasn't possible to transmit them, 
	// It would have to be done in the sendString method...
	// Same when the content should be hashed e.g.
}

// ---
int QGAMES::CommunicationChannel::receive ()
{
	if (!_implementation -> isChannelOpened ())
		return (-1); // It can't progress

	std::string str;
	if (_implementation -> receiveString (str) == -1)
		return (-1); // It hasn't been possible to receive anything...

	// If there is no string to deal with..it worked right but there is nothing else to do...
	if (str == std::string (__NULL_STRING__))
		return (0);

	// Keeps the element in the buffer...
	if (_currentElementInTheBuffer == _maxNumberElementsInTheBuffer)
		_currentElementInTheBuffer = -1; // Starts back...
	_buffer [++_currentElementInTheBuffer] = str;

	return (0); // Everything ok...
}

// ---
int QGAMES::LogChannelImplementation::sendString (const std::string& str)
{
	// Includes time?
	std::string tIS (__NULL_STRING__);
	if (_properties._keepTime)
	{
		std::time_t tt = std::time (NULL);
		char buffer [256]; ctime_s (buffer, 256, &tt);
		std::string ttS (buffer); // ctime_s denerates and /n at the end that has to be avoided...
		tIS = std::string ("[") + ttS.substr (0, ttS.length () - 1) + std::string ("]");
	}

	// Max length?
	std::string fS = tIS + str;
	if (_properties._maxLength != -1 && (int) fS.length () > _properties._maxLength)
		fS = fS.substr (0, _properties._maxLength);
	
	// Print it out..
	_target._stream << fS << std::endl; 
	
	return (0); // Everything ok
}

// ---
int QGAMES::ConsoleLogChannelImplementation::sendString (const std::string& str)
{
	static std::map <std::string, colorwin::CW_COLORS> colorValue ( 
		{ { std::string ("red"), colorwin::red },
		  { std::string ("yellow"), colorwin::yellow},
		  { std::string ("green"), colorwin::green },
		  { std::string ("cyan"), colorwin::cyan },
		  { std::string ("blue"), colorwin::blue },
		  { std::string ("magenta"), colorwin::magenta },
		  { std::string ("white"), colorwin::white },
		  { std::string ("grey"), colorwin::grey },
		  { std::string ("dark_red"), colorwin::dark_red },
		  { std::string ("dark_yellow"), colorwin::dark_yellow},
		  { std::string ("dark_green"), colorwin::dark_green},
		  { std::string ("dark_cyan"), colorwin::dark_cyan },
		  { std::string ("dark_blue"), colorwin::dark_blue },
		  { std::string ("dark_magenta"), colorwin::dark_magenta },
		  { std::string ("dark_grey"), colorwin::dark_grey },
		  { std::string ("back_red"), colorwin::back_red },
		  { std::string ("back_yellow"), colorwin::back_yellow},
		  { std::string ("back_green"), colorwin::back_green},
		  { std::string ("back_cyan"), colorwin::back_cyan },
		  { std::string ("back_blue"), colorwin::back_blue },
		  { std::string ("back_magenta"), colorwin::back_magenta },
		  { std::string ("back_grey"), colorwin::back_grey } });

	// Split the things to print in sections by color...
	size_t cII; size_t cIF;
	std::stack <std::string> nStrs ({ str });
	std::stack <colorwin::CW_COLORS> clrs ({ colorwin::grey });
	do
	{
		cII = cIF = -1; // start back to look for the color info...
		std::string nStr = nStrs.top (); nStrs.pop ();
		colorwin::CW_COLORS clr = clrs.top (); clrs.pop ();
		if ((cII = nStr.find ('|'), cII + 1) != std::string::npos &&
			(cIF = nStr.find ('|', cII + 1)) != std::string::npos)
		{
			std::string strL = nStr.substr (0, cII);
			std::string strR = nStr.substr (cIF + 1);
			std::map <std::string, colorwin::CW_COLORS>::const_iterator cVI = 
				colorValue.find (QGAMES::ltrim (QGAMES::trim (nStr.substr (cII + 1, cIF - cII - 1))));

			nStrs.push (strL); clrs.push (clr);
			nStrs.push (strR); clrs.push ((cVI == colorValue.end ()) ? colorwin::dark_grey : (*cVI).second); 
		}
		else
		{
			nStrs.push (nStr); clrs.push (clr);
		}
	} while (cII != std::string::npos);

	// Includes time?
	std::string tIS (__NULL_STRING__);
	if (_properties._keepTime)
	{
		std::time_t tt = std::time (NULL);
		char buffer [256]; ctime_s (buffer, 256, &tt);
		std::string ttS (buffer); // ctime_s denerates and /n at the end that has to be avoided...
		tIS = std::string ("[") + ttS.substr (0, ttS.length () - 1) + std::string ("]");
	}

	// Create the stacks contaning the info to be printed out...
	// That consists on revrsing the original one...
	std::stack <colorwin::CW_COLORS> clrsP;
	for (int i = (int) clrs.size (); i > 0; i--) { clrsP.push (clrs.top ()); clrs.pop (); }
	std::stack <std::string> nStrsP;
	for (int i = (int) nStrs.size (); i > 0; i--) {	nStrsP.push (nStrs.top ()); nStrs.pop (); }

	// Now it is time to print it out everything...
	std::string tStr (__NULL_STRING__);
	for(int i = (int) nStrsP.size (); i > 0; i--)
	{
		std::string nStr = nStrsP.top (); nStrsP.pop ();
		colorwin::CW_COLORS clr = clrsP.top (); clrsP.pop ();

		// Time to be included in the first print?
		if (i == 0 && tIS != std::string (__NULL_STRING__))
			nStr = tIS + nStr;

		// Does the number of things already printed out exceed the max length allowed?
		if (_properties._maxLength != -1 && (int) (tStr + nStr).length () > _properties._maxLength)
			nStr = nStr.substr (0, (_properties._maxLength - (tStr + nStr).length ()));

		// Print it out..
		_target._stream << colorwin::color (clr) << nStr; 
	}

	_target._stream << std::endl;

	return (0); // Everything ok
}

// ---
QGAMES::FileLogChannelImplementation::FileLogChannelImplementation (const std::string& nF)
	: LogChannelImplementation (StreamCommunicationChannelTarget 
				(std::string ("CONSOLE"), std::cout), LogChannelImplementation::Properties ()),
	  _file ()
{
	std::string dD = QGAMES::Game::game () -> parameter (__GAME_DATADIRPROPERTYNAME__);

	_file.open (dD + std::string (__PATH_SEPARATOR__) + nF);
	assert (_file);
	FileLogChannelImplementation (); // The deafult one...
}

// ---
QGAMES::IPCommunicationChannelTarget::IPCommunicationChannelTarget (const std::string& n, const std::string& ip, int p)
		: CommunicationChannelTarget (n),
		  _ip (ip), _port (p)
{
	// To be a valid Ip, the number of elements has to be four and all of them have to be between 1 and 255...
	std::vector <int> ipParts = QGAMES::getElementsFromAsInt (ip, ',');
	assert ((int) ipParts.size () == 4);
	for (int i = 0; i < 4; i++)
		assert (ipParts [i] > 0 && ipParts [i] <= 255);
}

// ---
QGAMES::IPCommunicationChannelImplementation::Properties::Properties
		(const QGAMES::IPCommunicationChannelTarget& chD, int iC,
		 const std::string& sB, const std::string& eB)
	: _isServer (true), 
	  _channelAddress (chD), 
	  _maxNumberIncomingConnections (iC),
	  _startBlockStr (sB), _endBlockStr (eB)
{ 
	// To be a server, the communications acceopted have to be more than 1...
	assert (_maxNumberIncomingConnections > 1); 
}

// ---
int QGAMES::IPCommunicationChannelImplementation::sendString 
	(const QGAMES::CommunicationChannelTarget& chD, const std::string& str)
{
	assert (dynamic_cast <const QGAMES::IPCommunicationChannelTarget*> (&chD)); // It has to be an IP...
	
	return (sendString (*(dynamic_cast <const QGAMES::IPCommunicationChannelTarget*> (&chD)), str));
}
