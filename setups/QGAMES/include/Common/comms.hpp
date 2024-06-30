/** \ingroup Game */
/** \ingroup Communications */
/*@{*/

/**	
 *	@file	
 *	File: comms.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 05/10/2019 \n
 *	Description: Defines classes to manage the communications.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_COMMS__
#define __QGAMES_COMMS__

#include <Common/notifierobserver.hpp>
#include <Common/openvalue.hpp>
#include <iostream>
#include <fstream>

namespace QGAMES
{
	/** To identify the destination of any communication. \n
		The destination is initially an abstract object. \n
		The final implementation could be typically an address or a file. */
	struct CommunicationChannelTarget
	{
		CommunicationChannelTarget (const std::string& n)
			: _name (n)
							{ }

		const std::string& name () const
							{ return (_name); }

		virtual ~CommunicationChannelTarget () 
							{ } // Nothing is done bu default...

		std::string _name;
	};

	/** The way a communication channel is implemented.
		Remember that a communication channel can be used for many different pruposes, and in many ways. e.g.:
		To transmit the status of the game across the networks could be one of them, but
		to log the status of the game could be other,... */
	class CommunicationChannelImplementation
	{
		public:
		/** No constructor is defined by default. */

		/** Virtual destructor just in case. 
			By default nothing is done. */
		virtual ~CommunicationChannelImplementation () 
							{ }

		/** To know the behaviour of the channel.
			Whether the channel can send and receive data. */
		virtual bool canSend () const = 0;
		virtual bool canReceive () const = 0;

		// Manage the channel
		/** Open the channel. 
			It should return 0 when everything is right and -1 when there is an error. 
			The channel cann't be opened twice. It has to be controlled by the specific implementation. */
		virtual int openChannel () = 0;
		/** It should return true when the channeld is opened, and false in other case. */
		virtual bool isChannelOpened () = 0;
		/** Close the channel. 
			It should return 0 when everything is right and -1 when there is an error. */
		virtual int closeChannel () = 0;
		/** Send a string through the channel. 
			It should return 0 when everything is right and -1 when there is an error. 
			The channel has to be opened previouly, otherwise an error should be issued. 
			It has to be controlled by the specific implementation. */
		virtual int sendString (const CommunicationChannelTarget& chD, const std::string& str) = 0;
		/** Receive a string through the channel. 
			It should return 0 when everything is right and -1 when there is an error. 
			The channel has to be opened previously, otherwise a error should be issed. 
			It has to be controlled by the specific implementation. */
		virtual int receiveString (std::string& str) = 0;
	};

	/** Class to communicate and receive information structures. \n
		The communication channel delegates the final implementation in a CommunicationChannelImplementation (defined later). \n
		The class manages a buffer. This buffer is used any time something is received. \n
		The elements is the buffer are saved as strings always (is the most flexible structure type) */
	class CommunicationChannel
	{
		public:
		/** The CommunicationChannel is the owner of the implementation. \n
			When the constructor is executed, the channel is tried to be opened. \n
			If it fails, and the application is under debug mode, an exception will be thrown.
			The constructor also receives the size of the buffer to maintain. */
		CommunicationChannel (CommunicationChannelImplementation* imp, int bSize = 10);
		/** The implementation is deleted, and the channel closed. */
		virtual ~CommunicationChannel ();

		/** To access to the implementation. 
			Not often used. */
		CommunicationChannelImplementation* implementation ()
							{ return (_implementation); }
		const CommunicationChannelImplementation* implementation () const
							{ return (_implementation); }
		/** To change the implementation. 
			It could be dangerous. Take care when invoking this method. */
		void setImplementation (CommunicationChannelImplementation* i);

		/** To know the behaviour of the channel. */
		bool canSend () const
							{ return (_implementation -> canSend ()); }
		bool canReceive () const
							{ return (_implementation -> canReceive ()); }

		/** To know and change the max number of elements in the buffer. When it is used!
			Always this value is changed then the buffer is reseted (bear this in mind). */
		int maxNumberElementsInTheBuffer () const
							{ return (_maxNumberElementsInTheBuffer); }
		void setMaxNumberElementsInTheBuffer (int bSize);

		/** To get the last result kept into the communications buffer. */
		const std::string& lastElementReceived () const
							{ return (elementReceived (_currentElementInTheBuffer)); }
		const std::string& elementReceived (int nElmnt) const;

		/** To send an string directly. */
		int send (const CommunicationChannelTarget& to, const std::string& str);
		/** To send a set of openvalues. 
			0 is returned when everything is ok, and -1 in any other circunstance. */
		int send (const CommunicationChannelTarget& to, const SetOfOpenValues& oV);
		/** To receive a set of open values. 
			0 is returned when everything is ok, and -1 in any other circunstance. 
			If everything is ok the set of open values is stored int the list of those. */
		int receive ();

		protected:
		CommunicationChannelImplementation* _implementation;
		int _maxNumberElementsInTheBuffer;
		std::vector <std::string> _buffer;

		// Implementation
		int _currentElementInTheBuffer;
	};

	/** A very simple communication channel to do nothing. 
		It is used when it is wanted to avoid the communications. */
	class NothingChannelImplementation : public CommunicationChannelImplementation
	{
		public:
		NothingChannelImplementation ()
							{ }

		/** @see parent. */
		virtual bool canSend () const
							{ return (false); }
		virtual bool canReceive () const
							{ return (false); }

		/** @see parent. */
		virtual int openChannel () { return (0); }
		virtual bool isChannelOpened () { return (true); }
		virtual int closeChannel () { return (0); }
		virtual int sendString (const CommunicationChannelTarget& chD, const std::string& str) final
							{ return (-1); }
		virtual int receiveString (std::string& str) final { return (-1); } 
	};

	/** Any Stream destination is defined by a ostream object. 
		At the end this class is simply a wrapper over the standard c++ ostream class. */
	struct StreamCommunicationChannelTarget : public CommunicationChannelTarget
	{
		StreamCommunicationChannelTarget (const std::string& n, std::ostream& o)
			: CommunicationChannelTarget (n), 
			  _stream (o)
							{ }

		/** A reference to the stream used in the communication. */
		std::ostream& _stream;
	};

	/** A very useful, common (and usual) communication channel is a simple log. \n
		It can be simulated just if the communication channel is defined as a stream instead of a network cable e.g. 
		The log recveives the stream to write to, but it is not responsable to manage it. */
	class LogChannelImplementation : public CommunicationChannelImplementation
	{
		public:
		/** The properties of the log channel. 
			By default, other information (apart of the date time) can be save. */
		struct Properties
		{
			Properties ()
				: _keepTime (true),
				  _maxLength (-1) // No max length...
							{ }

			Properties (bool kT, int mL = -1)
				: _keepTime (kT),
				  _maxLength (mL)
							{ assert (_maxLength == -1 || _maxLength > 0); } 

			/** Des it have to keep the time of the event. True by default. */
			bool _keepTime;
			/** How long must the messahe be maximum? */
			int _maxLength;
		};

		LogChannelImplementation (const StreamCommunicationChannelTarget& t, const Properties& prps = Properties ())
			: _target (t),
			  _properties (prps)
							{ }

		/** @see parent. */
		virtual bool canSend () const
							{ return (true); }
		/** In a log everything is sent to the channel, nothing can be received. */
		virtual bool canReceive () const
							{ return (false); }

		/** To get and change the properties. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; }

		/** @see parent. */
		virtual int openChannel () { return (0); }
		virtual bool isChannelOpened () { return (true); }
		virtual int closeChannel () { return (0); }
		/** Not ethat the rest can't be overloaded any more. This is the only one. */
		virtual int sendString (const std::string& str);
		/** Notice that this method is now declared as private, waht means it can be longer overloaded. \n
			The one and only to be overloaded from now onwards is sendString (string) defined above. \n
			This is because the string is always sent to the target defined at construction time. 
			This method can't be overloaded anymore. */
		virtual int sendString (const CommunicationChannelTarget& chD, const std::string& str) final
							{ return (sendString (str)); }
		virtual int receiveString (std::string& str) { return (-1); } // It is not useful in a log at all...

		protected:
		StreamCommunicationChannelTarget _target;
		Properties _properties;
	};

	/** The way a very standard log is implemented. 
		The things will be printed out int the console of the system. */
	class ConsoleLogChannelImplementation : public LogChannelImplementation
	{
		public:
		ConsoleLogChannelImplementation ()
			: LogChannelImplementation (StreamCommunicationChannelTarget 
					(std::string ("CONSOLE"), std::cout), LogChannelImplementation::Properties ())
							{ }

		virtual int sendString (const std::string& str) override;
	};

	/** The way a log to a file is implemented. */
	class FileLogChannelImplementation : public LogChannelImplementation
	{
		public:
		FileLogChannelImplementation (const std::string& nF);

		private:
		FileLogChannelImplementation ()
			: LogChannelImplementation (StreamCommunicationChannelTarget
					(std::string ("FILE"), _file), LogChannelImplementation::Properties ())
							{ }

		std::ofstream _file;
	};

	/** Any IP destination is defined by an address and a port. */
	struct IPCommunicationChannelTarget : public CommunicationChannelTarget
	{
		/** The constructor verifies it is right ip address (IPV4.0) */
		IPCommunicationChannelTarget (const std::string& n, const std::string& ip, int p);

		std::string _ip;
		int _port;
	};

	/** When the communication channel is based on IP. \n
		The final implementation is not defined in this class, because the libraries finally used could be many. */
	class IPCommunicationChannelImplementation : public CommunicationChannelImplementation
	{
		public:
		struct Properties
		{
			/** A configuration data to define an client IP channel. */
			Properties (const std::string& sB = std::string (__NULL_STRING__), 
						const std::string& eB = std::string (__NULL_STRING__))
				: _isServer (false), 
				  _channelAddress (IPCommunicationChannelTarget 
						(std::string ("LOCAL"), std::string ("127.0.0.1"), 0)), // None data is used... 
				  _maxNumberIncomingConnections (1),
				  _startBlockStr (sB), _endBlockStr (eB)
							{ }

			/** 
			  * Same, but to define a server
			  * @param chD	The address (port included) where the commnication channel is running when it is a server
			  * @param iC	The max number of incoming connections in the case that the implementation defines a server
			  */
			Properties (const IPCommunicationChannelTarget& chD, int iC = 10,
				const std::string& sB = std::string (__NULL_STRING__), 
				const std::string& eB = std::string (__NULL_STRING__));

			/** Is this a server of not? */
			bool _isServer;
			/** Where the server is running, if it is so... */
			IPCommunicationChannelTarget _channelAddress;
			/** The max number of connections the serer can accept, if it is so... */
			int _maxNumberIncomingConnections;
			/** Which is the string to delimit the beginning of a block sent throw the communication channel. */
			std::string _startBlockStr;
			/** The same with the end. */
			std::string _endBlockStr;
		};

		/** Depending on the configuration data it will be a server or a client. */
		IPCommunicationChannelImplementation (const Properties& dt)
			: _properties (dt)
							{ }

		/** @see parent. */
		virtual bool canSend () const
							{ return (true); }
		virtual bool canReceive () const
							{ return (true); }

		/** This is the one to overload later. */
		virtual int sendString (const IPCommunicationChannelTarget& chD, const std::string& str) = 0;
		/** @see parent. 
			This method targets the one above, that is the real virtual one to be implemented later. \n
			This is because that one receives an IP Address. \n
			This method can't be overloaded any more. */
		virtual int sendString (const CommunicationChannelTarget& chD, const std::string& str) final;

		/** The method receiveString can only be valid whether it is a server in many implemetations. 
			Take this into account when implementing. */

		protected:
		Properties _properties;
	};
}

#endif
  
// End of the file
/*@}*/
