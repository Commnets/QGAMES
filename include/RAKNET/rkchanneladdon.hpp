/** \ingroup RAKNET */
/** \ingroup Communications */
/*@{*/

/**	
 *	@file	
 *	File: rkchanneladdon.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 23/10/2019 \n
 *	Description: Defines an addson to create a communication channel based on RAKNET
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_RAKNETCOMMSCHANNELADDSON__
#define __QGAMES_RAKNETCOMMSCHANNELADDSON__

#include <Common/commsbuilder.hpp>
#include <RAKNET/rkdefinitions.hpp>

class RAKNETCommunicationChannelBuilderAddsOn : public QGAMES::CommunicationChannelBuilder::AddsOn
{
	public:
	/** @see parent. */
	virtual bool canCreateType (const std::string& t)
							{ return (t == std::string (__QGAMES_COMMSCHANNELRAKNET__)); }
	virtual QGAMES::CommunicationChannel* createChannelType (const std::string& t, 
		const std::map <std::string, std::string>& p = std::map <std::string, std::string> ());
};

#endif
  
// End of the file
/*@}*/