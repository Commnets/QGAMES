/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: dwsteps.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 11/07/2019 \n
 *	Description: Define speficif steps related to a draginwin like game.
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_STEPS__
#define __DRAGONWIND_STEPS__

#include <DragonwindLike/dwdefinitions.hpp>
#include <Platform/ptinclude.hpp>

namespace DRAGONWIND
{
	/** A step to control actions over a dragonwind character. 
		The actions take place for a time if it is defined. */
	class CharacterControlStepDragonAction : public QGAMES::CharacterControlStep
	{
		public:
		struct Properties
		{
			Properties ()
				: _type (0), // Meaning to stand still!!
				  _direction (0),
				  _backwards (false),
				  _impulse (1), // Negative means random up to that value
				  _seconds (-1) // -1, means eternal. Other negative value means random up to that value
							{ }

			Properties (int t, int d, bool bk, int i, QGAMES::bdata s, int p, const QGAMES::Rectangle& l)
				: _type (t),
				  _direction (d),
				  _backwards (bk),
				  _impulse (i),
				  _seconds (s),
				  _pixels (p),
				  _limits (l)
							{ }

			Properties (const std::map <std::string, std::string>& prps);

			// Attributes
			/** The type of step. */
			int _type;
			/** The direction the entity is looking to, if any, of the step. */
			int _direction;
			/** Whether the entity moves backwards or, usually, forwards. */
			bool _backwards;
			/** The impulse of the step, again if any. */
			int _impulse;
			/** And number of seconds that the step lasts. */
			QGAMES::bdata _seconds;
			/** A max number of pixels to move., if any. */
			int _pixels;
			/** The limits of the movement is come cases. */
			QGAMES::Rectangle _limits;
		};

		CharacterControlStepDragonAction (const Properties& prps = Properties ())
			: QGAMES::CharacterControlStep (__DRAGONWIND_CHRSTEPACTION__),
			  _properties (prps),
			  _counter (0), _timeLimit (false), _counterLimit (__MAXINT__),
			  _pixelsCounter (0), _pixelsLimit (false), _pixelsCounterLimit (__MAXINT__), 
			  _originalPosition (QGAMES::Position::_cero), _realBackwards (false)
							{ }

		/** @see parent. */
		virtual CharacterControlStep* clone () const
							{ return (new CharacterControlStepDragonAction (*this)); }

		/** @see parent. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		/** @see parent. */
		virtual void initializeOn (QGAMES::Character* a);
		virtual bool isEndOn (QGAMES::Character* a);
		virtual void updateOn (QGAMES::Character* a);
		virtual void executeOn (QGAMES::Character* a) { }

		protected:
		/** The properties of the step. */
		Properties _properties;

		// Implementation
		// Related with time in the state...
		int _counter;
		bool _timeLimit;
		int _counterLimit;
		// With movement of the character...
		int _pixelsCounter;
		bool _pixelsLimit;
		int _pixelsCounterLimit;
		QGAMES::Position _originalPosition;
		bool _realBackwards;
	};
}

#endif
  
// End of the file
/*@}*/
