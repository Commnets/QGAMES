#include <Common/gamecommsaddon.hpp>
#include <sstream>

// ---
QGAMES::GameCommsStateTypeAddOn::Properties::Properties 
		(QGAMES::CommunicationChannelImplementation* c, const QGAMES::CommunicationChannelTarget& t,
		 const std::vector <int>& eT, const std::vector <int>& eR, bdata s,
		 const std::string& sE, const std::string eE)
	: _channel (c),
	  _target (t),
	  _seconds (s),
	  _startingElement (sE), _endingElement (eE)
{ 
	assert (_channel);
	assert (_seconds > (bdata) 0);
	assert (_startingElement != _endingElement);
}

// ---
void QGAMES::GameCommsStateTypeAddOn::initialize ()
{
	// The communication channel object is created now
	// In this momento the channel itself (whatever it is) will be created!!
	_comms = new QGAMES::CommunicationChannel (_properties._channel);
	// The _comms object becomes the owner of the implementation
}

// ---
void QGAMES::GameCommsStateTypeAddOn::inEveryLoop ()
{
	if (++_loopCounter < (game () ->framesPerSecond () * 60))
		return; // Nothing to do so far...

	_loopCounter = 0;

	// Send the status of the main elements
	// Every element is separated from the rest using the startingElement and the endingElement strings
	std::stringstream oS;
	std::vector <int> eToTransmit = elementsToTransmit ();
	for (int i = 0; i < (int) eToTransmit.size (); i++)
		oS << _properties._startingElement // Every element starts by a code...
			<< game () -> entity (eToTransmit [i]) -> runtimeValues () 
			<< _properties._endingElement // And ends by another...
			<< std::endl;

	_comms -> send (_properties._target, oS.str ()); // Send the situation...

	// Receive the status of the rest...if any...
	QGAMES::SetOfOpenValues oV;
	std::vector <int> eToRead = elementsToRead ();
	_comms -> receive (); 
	std::string str = _comms -> lastElementReceived ();
	int j = 0;
	while (str != std::string (__NULL_STRING__))
	{
		// TODO
		// To get the OV from the string...

		if (j != (int) eToRead.size ())
		{
			game () -> entity (eToRead [j]) -> initializeRuntimeValuesFrom (oV);

			j++;
		}
	}
}

// ---
void QGAMES::GameCommsStateTypeAddOn::finalize ()
{
	delete (_comms);
	// The implementation is deleted when the object is destroyed...

	_comms = NULL;
}
