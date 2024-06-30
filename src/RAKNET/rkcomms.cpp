#include <RAKNET/rkcomms.hpp>
#include <BitStream.h>

// ---
RAKNETCommunicationChannelImplementation::RAKNETCommunicationChannelImplementation 
	(const QGAMES::IPCommunicationChannelImplementation::Properties& dt)
		: QGAMES::IPCommunicationChannelImplementation (dt),
		  _peer (NULL),
		  _channelOpened (false)
{
	_peer = RakNet::RakPeerInterface::GetInstance ();

	assert (_peer); // Just in case...
}

// ---
RAKNETCommunicationChannelImplementation::~RAKNETCommunicationChannelImplementation ()
{
	if (isChannelOpened ()) closeChannel ();

	RakNet::RakPeerInterface::DestroyInstance (_peer);

	_peer = NULL;
	
	_channelOpened = false;
}
	
// ---
int RAKNETCommunicationChannelImplementation::openChannel ()
{
	if (_channelOpened)
		return (0); // The channel has already been opened, so it is needn't open it back...

	// By default the error is the most likey thing...(figer crossed)
	int result = -1;

	if (_properties._isServer)
	{
		RakNet::SocketDescriptor sd (_properties._channelAddress._port, 0);
		if (_peer -> Startup (_properties._maxNumberIncomingConnections, &sd, 0) == RakNet::StartupResult::RAKNET_STARTED)
			result = 0; // Everything was ok, otherwise there was an error...
	}
	else
	{
		RakNet::SocketDescriptor sd;
		if (_peer -> Startup (1, &sd, 1) == RakNet::StartupResult::RAKNET_STARTED)
		{
			if (_peer -> Connect (_properties._channelAddress._ip.c_str (), _properties._channelAddress._port, NULL, NULL) == 
					RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED)
				result = 0; // Everything was ok, otherwise some type of mistake happens...
			// The channel is not considered still opened until the notification from the server is recived...
			// @see method receive string...
		}
	}

	return (result);
}

// ---
int RAKNETCommunicationChannelImplementation::closeChannel ()
{
	// Only if it's a clinet, the connection can be closed...
	if (_properties._isServer)
		return (-1); // ...the server can't be closed...
	// If it is a server, when the server is destroyed, all connections will be closed at the same time...

	_peer -> CloseConnection (_peer -> GetMyGUID (), true); // Informing about the disconnection...
	_channelOpened = false;

	return (-1);
}

// ---
int RAKNETCommunicationChannelImplementation::sendString 
	(const QGAMES::IPCommunicationChannelTarget& chD, const std::string& str)
{
	// The packages only can be sent when it a client
	if (_properties._isServer)
		return (-1); // ...and indicates that the string can't be sent

	// If it is client, but the channel is not still openen, nothing could be sent
	if (!_channelOpened)
		return (-1);

	int result = 0;

	// Use a BitStream to write a custom user message...
	// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
	RakNet::BitStream bsOut;
	bsOut.Write ((RakNet::MessageID) _MESSAGEID);
	bsOut.Write (str.c_str ());
	RakNet::SystemAddress sA (chD._ip.c_str (), chD._port);
	_peer -> Send (&bsOut, IMMEDIATE_PRIORITY, UNRELIABLE_SEQUENCED, 0, sA, false);
	// UNREALIABLE_SEQUENCE means to use UDP

	return (-1);
}

// ---
int RAKNETCommunicationChannelImplementation::receiveString (std::string& str)
{
	// Both clients and servers receive in RakNet packages...
	// The difference is the type of packages that one and other can receive...

	int result = -1;

	RakNet::Packet* packet = NULL;
	while (packet = _peer -> Receive ())
	{
		switch (packet -> data [0]) // The first char has the id of the mesaage...
		{
			case DefaultMessageIDTypes::ID_REMOTE_DISCONNECTION_NOTIFICATION:
				// A client has been disconnected...
				break;

			case DefaultMessageIDTypes::ID_REMOTE_CONNECTION_LOST:
				// A client has lost the connection, what is similar by the way to the previous one...
				break;

			case DefaultMessageIDTypes::ID_REMOTE_NEW_INCOMING_CONNECTION:
				// A new customer has connected...
				break;

			case DefaultMessageIDTypes::ID_CONNECTION_REQUEST_ACCEPTED:
				// A new request of connection has been acepted...
				_channelOpened = true;
				break;

			case DefaultMessageIDTypes::ID_NEW_INCOMING_CONNECTION:
				// A new request connection is incoming...
				break;

			case DefaultMessageIDTypes::ID_NO_FREE_INCOMING_CONNECTIONS:
				// The server can't acept more connections because it reached the max...
				break;

			case DefaultMessageIDTypes::ID_CONNECTION_LOST:
				// The connection has been lost 
				// with either the clint or with the server, depending on which one received the communication)
				break;
				
			case _MESSAGEID: // Our message...
				{
					RakNet::RakString rs;
					RakNet::BitStream bsIn (packet -> data, packet -> length, false);
					bsIn.IgnoreBytes (sizeof (RakNet::MessageID));
					bsIn.Read (rs);
					str = rs.C_String (); // The string to be returned is then set...
					result = 0;
				}

				break;
			
			default:
				// This message has not been idenfitied at all...
				// So in debug mode, stops!
				assert (0);
		}
	}

	return (result);
}
