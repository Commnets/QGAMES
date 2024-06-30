/** \ingroup Game */
/** \ingroup Communications */
/*@{*/

/**	
 *	@file	
 *	File: gamecommsaddon.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 13/10/2019 \n
 *	Description: Defines classes to create and addon to manage how to communicate game state.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_GAMECOMMSADDON__
#define __QGAMES_GAMECOMMSADDON__

#include <Common/game.hpp>
#include <Common/comms.hpp>

namespace QGAMES
{
	/** 
	 * A game add- on to communicate the situation of the game per loop. \n
	 * The add on needs the implementation of the communication channel to used,
	 * the time (in seconds) to actualize the status
	 * It is needed to define which is the id of the main element, and what are the id of the rest 
	 * of the elements to be actualized.
	 */
	class GameCommsStateTypeAddOn : public GameTypeAddOn
	{
		struct Properties
		{
			// There can't be a default constructor...

			Properties (CommunicationChannelImplementation* c, const CommunicationChannelTarget& t,
				const std::vector <int>& eT, const std::vector <int>& eR, bdata s,
				const std::string& sE = std::string ("@["), const std::string eE = std::string ("]@"));

			CommunicationChannelImplementation* _channel;
			CommunicationChannelTarget _target;
			bdata _seconds;
			std::string _startingElement;
			std::string _endingElement;
		};

		GameCommsStateTypeAddOn (const std::string& n, const Properties& prps, Game* g)
			: GameTypeAddOn (n, g), // This constructor verifies whether the game pointer is not null!!
			  _comms (NULL), // It will create when initialize is executed...
			  _properties (prps),
			  _loopCounter (0)
							{ }

		virtual ~GameCommsStateTypeAddOn ()
							{ delete (_comms); _comms = NULL; } // Just in case...

		/** To get the properties. */
		const Properties& properties () const
							{ return (_properties); }

		/** To get the elements to transmit and the elements to read. */
		virtual std::vector <int> elementsToTransmit () = 0;
		virtual std::vector <int> elementsToRead () = 0;

		/** @see parent. */
		virtual void initialize ();
		virtual void inEveryLoop ();
		virtual void updatePositions () { } // Only every loop...
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint) { } // Nothing is drawn with this add-on
		virtual void finalize ();

		protected:
		/** A reference to the communication channel. */
		CommunicationChannel* _comms;
		Properties _properties;

		// Implementation
		int _loopCounter;
	};
}

#endif
  
// End of the file
/*@}*/
