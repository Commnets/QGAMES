/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: PacMan.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: Definition of the main artist = pacman. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_PACMAN__
#define __PACMANII_PACMAN__

#include <PacManLike/pcinclude.hpp>

namespace PacManII
{
	/** Our main artist can collision with othr things in this version of pacman. */
	class PacMan final : public PACMAN::PacMan
	{
		public:
		PacMan (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: PACMAN::PacMan (cId, f, d),
			  _shield (false)
							{ }

		virtual QGAMES::Entity* clone () const override
							{ return (new PacMan (id (), forms (), data ())); }

		/** To know and set the shield. */
		void setShield (bool s);
		bool shield () const
							{ return (_shield); }

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void drawOn (QGAMES::Screen* scr, const QGAMES::Position& p = QGAMES::Position::_noPoint) override;
		virtual void finalize () override;

		virtual void whenCollisionWith (QGAMES::Entity* e) override;

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters () override
							{ return (new Counters); }

		protected:
		bool _shield;

		// The counters and the switches...
		static const int _COUNTERTOFADE = 0;
		static const int _COUNTERFADEPOSITION = 1;
		static const int _FADE [20];
	};
}

#endif
  
// End of the file
/*@}*/