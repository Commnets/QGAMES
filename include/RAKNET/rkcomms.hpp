/** \ingroup RAKNET */
/** \ingroup Communications */
/*@{*/

/**	
 *	@file	
 *	File: rkcomms.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 14/10/2019 \n
 *	Description: Defines classes to manage the communications under raknet
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_RAKNETCOMMS__
#define __QGAMES_RAKNETCOMMS__

#include <Common/comms.hpp>
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>

/** Implementation of the communicastions using the RakNet library. 
	The protocol used for everything is UDP. */
class RAKNETCommunicationChannelImplementation : public QGAMES::IPCommunicationChannelImplementation
{
	public:
	// Code of the message sent throw the system...
	static const unsigned char _MESSAGEID = DefaultMessageIDTypes::ID_USER_PACKET_ENUM + 1;

	RAKNETCommunicationChannelImplementation 
		(const QGAMES::IPCommunicationChannelImplementation::Properties& dt);
	~RAKNETCommunicationChannelImplementation ();

	/** @see parent. */
	virtual int openChannel ();
	virtual bool isChannelOpened ()
							{ return (_channelOpened); }
	virtual int closeChannel ();
	virtual int sendString (const QGAMES::IPCommunicationChannelTarget& chD, const std::string& str);
	virtual int receiveString (std::string& str);

	protected:
	RakNet::RakPeerInterface* _peer;

	// Implementation
	bool _channelOpened;
};

#endif
  
// End of the file
/*@}*/