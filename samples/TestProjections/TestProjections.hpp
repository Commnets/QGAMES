/** \ingroup Example */
/*@{*/

/**	
 *	@file	
 *	File: main.cpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/01/2018 \n
 *	Description: A pure QGAMES game based in SDL. Definition file.
 *	Versions: 1.0 Initial
 */

#ifndef __TESTPROJECTIONSINCLUDE__
#define __TESTPROJECTIONSINCLUDE__

#include "stdafx.h"
#include "Defs.hpp"

namespace TestProjections
{
	// -------ScoreObjects -----------------------------------------------------------------------
	/** Framerate */
	class FramerateScoreObject : public QGAMES::ScoreObjectNumber
	{
		public:
		FramerateScoreObject ();

		/** @see parent. 
			It hasn't neither movements, nor animations, not states to copy. */
		virtual QGAMES::Entity* clone () const
							{ return (new FramerateScoreObject ()); }

		/** @see parent. */
		virtual void updatePositions ();

		protected:
		/** @see parent */
		virtual int formToDrawDigits ()
							{ return (10000); }
	};

	// -------Angles-----------------------------------------------------------------------
	/** Angles */
	class AnglesScoreObject : public QGAMES::ScoreObjectText
	{
		public:
		AnglesScoreObject ();

		/** @see parent. 
			It hasn't neither movements, nor animations, not states to copy. */
		virtual QGAMES::Entity* clone () const
							{ return (new AnglesScoreObject ()); }

		/** @see parent. */
		virtual void updatePositions ();

		protected:
		/** @see parent */
		virtual int formToDrawLetters ()
							{ return (10000); }
	};

	// -------The entities-------------------------------------------------------------------------
	/** To create basic entities. 
		Nothing is done. */
	class EntityBuilder : public QGAMES::EntityBuilder
	{
		public:
		EntityBuilder (const std::string& fDef, QGAMES::FormBuilder* fB, QGAMES::MovementBuilder* mB)
			: QGAMES::EntityBuilder (fDef, fB, mB)
								{ }
	
		private:
		virtual QGAMES::Entity* createEntity (const EntityDefinition& def)
								{ return (NULL); }
	};
	// --------------------------------------------------------------------------------

	// -------The movements-------------------------------------------------------------------------
	/** To create basic movements. 
		Nothing is done. */
	class MovementBuilder : public QGAMES::MovementBuilder
	{
		public:
		MovementBuilder (const std::string& fDef)
			: QGAMES::MovementBuilder (fDef)
								{ }
	
		private:
		virtual QGAMES::Movement* createMovement (const MovementDefinition& def)
								{ return (NULL); }
	};
	// --------------------------------------------------------------------------------

	// -------The input handler-------------------------------------------------------------------------
	/** The handler managing the input elements. */
	class InputHandlerBehaviour : public QGAMES::InputHandler::Behaviour
	{
		public:
		InputHandlerBehaviour ()
			: QGAMES::InputHandler::Behaviour ()
								{ }

		/** @see parent. */
		/** Because it is necessary to detect the key to exit the game. */
		virtual void onKeyboardEvent (QGAMES::KeyBoardEventData* dt);
	};
	// --------------------------------------------------------------------------------

	/** The demo game. */
	class Game : public QGAMES::Game
	{
		public:
		Game ();

		QGAMES::bdata zCoord () const
							{ return (_zCoord); }

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s);
		virtual void finalize ();

		/** @see parent. */
		virtual void addScoreObjects ();

		private:
		/** see parent. */
		virtual QGAMES::EntityBuilder* createEntityBuilder ()
								{ return (new EntityBuilder (parameter (__GAME_PROPERTYENTITIESFILE__), 
									formBuilder (), movementBuilder ())); }
		virtual QGAMES::MovementBuilder* createMovementBuilder () 
								{ return (new MovementBuilder (parameter (__GAME_PROPERTYMOVEMENTSFILE__))); }
		virtual QGAMES::InputHandler* createInputHandler ()
								{ return (implementation () -> createInputHandler (new InputHandlerBehaviour ())); }
		virtual QGAMES::Screens createScreens ()
								{ QGAMES::Screens result;
								  result.insert (QGAMES::Screens::value_type (__QGAMES_MAINSCREEN__, 
										implementation () -> createScreen 
											(std::string ("Test Projections"),
											QGAMES::Position::_cero, 800, 600, 100, 100)));
								  return (result); }

		private:
		static const QGAMES::Position _iPoint;
		static const int _separation;
		static const int _length;
		static const int _it;

		QGAMES::bdata _zCoord;
		QGAMES::bdata _cAngle;
		QGAMES::bdata _inc;

		int _counter;
	};
}

#endif
  
// End of the file
/*@}*/