#include <RAKNET/rkchanneladdon.hpp>
#include <RAKNET/rkcomms.hpp>
#include <Common/genalgorithms.hpp>

// ---
QGAMES::CommunicationChannel* RAKNETCommunicationChannelBuilderAddsOn::createChannelType (const std::string& t, 
	const std::map <std::string, std::string>& p)
{
	QGAMES::CommunicationChannel* result = NULL;

	if (t == std::string (__QGAMES_COMMSCHANNELRAKNET__))
	{
		std::map <std::string, std::string>::const_iterator i1 = p.find (std::string (__QGAMES_RKCHANNELSERVERATTR__));
		assert (i1 != p.end ()); // It has always to exist...it is mandatory!
		std::string serverS = (*i1).second; QGAMES::toUpper (serverS);
		bool server = (serverS == std::string (__YES_STRING__)); // No always unless a positive YES is said...

		std::string sBlock (__NULL_STRING__);
		std::map <std::string, std::string>::const_iterator i5 = 
			p.find (std::string (__QGAMES_RKCHANNELSTARTINGBLOCKDATTR__));
		if (i5 != p.end ()) 
			sBlock = (*i5).second;

		std::string eBlock (__NULL_STRING__);
		std::map <std::string, std::string>::const_iterator i6 = 
			p.find (std::string (__QGAMES_RKCHANNELENDINGBLOCKDATTR__));
		if (i6 != p.end ()) 
			eBlock = (*i6).second;

		if (server)
		{
			std::string ipAddress ("127.0.0.1"); 
			int port = -1; 
			int iConnects = 10; 

			std::map <std::string, std::string>::const_iterator i2 = p.find (std::string (__QGAMES_RKCHANNELIPATTR__));
			assert (i2 != p.end ());
			ipAddress = (*i2).second;

			std::map <std::string, std::string>::const_iterator i3 = p.find (std::string (__QGAMES_RKCHANNELPORTATTR__));
			assert (i3 != p.end ());
			port = std::atoi ((*i3).second.c_str ());
			assert (port > 0 && port < 65536); // The right range between...

			std::map <std::string, std::string>::const_iterator i4 = 
				p.find (std::string (__QGAMES_RKCHANNELMAXCONNECTIONSATTR__));
			if (i4 != p.end ())
			{
				iConnects = std::atoi ((*i4).second.c_str ());
				assert (iConnects > 0); // If defined it has to be posite...
			}

			result = new QGAMES::CommunicationChannel (new RAKNETCommunicationChannelImplementation 
					(QGAMES::IPCommunicationChannelImplementation::Properties 
						(QGAMES::IPCommunicationChannelTarget (std::string ("SERVER"), ipAddress, port), 
							iConnects, sBlock, eBlock))); // The properties for a server are quite different...
		}
		else
			result = new QGAMES::CommunicationChannel (
				new RAKNETCommunicationChannelImplementation 
					(QGAMES::IPCommunicationChannelImplementation::Properties (sBlock, eBlock)));

	}
	else
		assert (0);

	return (result);
}
