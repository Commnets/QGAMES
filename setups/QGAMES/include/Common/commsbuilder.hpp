/** \ingroup Game */
/** \ingroup Communications */
/*@{*/

/**	
 *	@file	
 *	File: comms.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 22/10/2019 \n
 *	Description: Defines classes to create communication objects
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_COMMSBUILDER__
#define __QGAMES_COMMSBUILDER__

#include <Common/comms.hpp>
#include <string>

class TiXmlElement;

namespace QGAMES
{
	/** 
	  * The builder only creates instances of the communication channel. \n
	  * It never keeps a trace of the ones just created and it is not accountable for deleting them ever. \n
	  * The builder can be extended using addsons. Every addson is accountable for creating instances
	  * of an specific communication channel type. \n
	  * The builder owns the adds on and they will destroyed when it is so. 
	  * The communications are defined in a file. The structure of the file is:
	  *	<?xml version="1.0"?> \n
	  *	<Communications> \n
	  *		<Include file="CCC"/> \n
	  *		<Communication id="X" type="YY"> \n
	  *			<Attributes>
	  *				<Attribute id="X" value="Y"/>
	  *				...
	  *			</Attributes>
	  * </Communications>
	  */
	class CommunicationChannelBuilder
	{
		public:
		/* Different type of communications channels could be addedl literally later.
			So an add - on class is created to be specialized in specific types of communications. */
		class AddsOn
		{
			public:
			virtual ~AddsOn ()
							{ }

			/** To know whether a specific type of communication channel can or not be created. */
			virtual bool canCreateType (const std::string& t) = 0;
			/** To create this communication channel. */
			virtual CommunicationChannel* createChannelType (const std::string& t, 
				const std::map <std::string, std::string>& p = std::map <std::string, std::string> ()) = 0;
		};

		// To simplify add on list management...
		typedef std::vector <AddsOn*> AddsOnList;

		/** The standard addon is to create the very basic types of communication channels. 
			@see comms.hpp */
		class StandardAddsOn : public AddsOn
		{
			public:
			/** @see parent. */
			virtual bool canCreateType (const std::string& t);
			virtual CommunicationChannel* createChannelType (const std::string& t, 
				const std::map <std::string, std::string>& p = std::map <std::string, std::string> ());
		};

		CommunicationChannelBuilder (const std::string& fDef);
		~CommunicationChannelBuilder ();

		/** To add a new addson. 
			Take care not not to add something already added before. */
		void addAddsOn (AddsOn* aO);

		CommunicationChannel* createCommChannel (const std::string& t, 
			const std::map <std::string, std::string>& p = std::map <std::string, std::string> ());

		protected:
		struct CommsChannelDefinition
		{
			CommsChannelDefinition ()
				: _id (-1), _type (__NULL_STRING__), _parameters ()
							{ }

			int _id;
			std::string _type;
			std::map <std::string, std::string> _parameters;
		};

		typedef std::map <int, CommsChannelDefinition> Definitions;

		void readFile (const std::string& fDef);
		CommsChannelDefinition readCommsDefinition (TiXmlElement* elmnt);
		std::map <std::string, std::string> readAttributesGroup (TiXmlElement* g);

		protected:
		AddsOnList _addsOn;

		Definitions _definitions;

		std::string _definitionFile;
		std::string _basePath;
	};
}

#endif
  
// End of the file
/*@}*/
