/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Monsters.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: Definition of the monsters of the game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_MONSTERS__
#define __PACMANII_MONSTERS__

#include <PacManLike/pcinclude.hpp>

namespace PacManII
{
	/** Wormy leaves a trail when moving through the maze, and this trail is as dangerous as he is!. 
		However the farer the trail is from the monster the less dangerous it is! */
	class Wormy final : public PACMAN::StandardMonster
	{
		public:
		static const int _NUMBER = 4; // After Clyde!

		Wormy (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: StandardMonster (cId, _NUMBER, f, d),
			  _trailLength (12), // By default...
			  _trailPositions (12, QGAMES::Position::_noPoint)
							{ }

		virtual Entity* clone () const override
							{ return (new Wormy (id (), forms (), data ())); }

		void setTrailLength (int l);

		/** In this case to the right. */
		virtual QGAMES::Vector preferredDirectionAtStartingToMove () const override
							{ return (QGAMES::Vector (__BD 1, __BD 0, __BD 0)); }

		/** Apart of Pacman, Wormy has also Clyde as its reference. */
		virtual void setReferenceArtists (const std::vector <Artist*>& r) override;

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void drawOn (QGAMES::Screen* scr, const QGAMES::Position& p = QGAMES::Position::_noPoint) override;

		virtual bool hasCollisionWith (QGAMES::Entity* ety) const override;

		protected:
		/** Target position is in the middle of Clyde's and Pacman's when chasing, and home when don't */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;

		virtual QGAMES::Color trackingColor () const override
							{ return (QGAMES::Color (0x64, 0x00, 0x64, 0x80)); /** Purple. */ }

		// Implementation
		virtual QGAMES::MazeModel::PositionInMaze runAwayMazePosition () const override;

		private:
		// Implementation
		QGAMES::Rectangles trailRectangles () const;

		private:
		int _trailLength;

		// Implementation
		mutable QGAMES::Positions _trailPositions;
	};
}

#endif
  
// End of the file
/*@}*/