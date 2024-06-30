/**\ingroup AdArcadeGame */
/**\ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: adscoreobjects.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 14/08/2018 \n
 *	Description: Defines standard score objects to be used in the games. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ADSCOREOBJECTS__
#define __QGAMES_ADSCOREOBJECTS__

#include <AdvancedArcade/addefinitions.hpp>
#include <Arcade/arcadeinclude.hpp>

namespace QGAMES
{
	/** A way to represent the number of lives of a player. */
	class LivesScoreObject : public ScoreObject
	{
		public:
		/** 
		  * To define some of the properties to configure the way the lives are show.
		  * First of all, lives consumed can not be shown or shown with another figure.
		  * If the parameter _formDown is equal to -1, the lives consumed will not be shown.
		  * The lives can be shown from the left to the right or from the top to the bottom.
		  * The common circunstance is to show them from the left to the right by it can be changed.
		  * The form used to show the lives still in use can not be negative at all.
		  */
		struct Properties
		{
			typedef enum { _UPDOWN = 0, _LEFTRIGHT } Orientation;

			Properties ()
				: _formUp (__QGAMES_GREENBALLFORM__),
				  _formDown (__QGAMES_REDBALLFORM__),
				  _orientation (_LEFTRIGHT),
				  _pixelsSeparation (5) // 5 pixels of separation...
							{ }

			Properties (int fUp, int fDown = -1, Orientation o = _LEFTRIGHT, int pSep = 0)
				: _formUp (fUp),
				  _formDown (fDown),
				  _orientation (o),
				  _pixelsSeparation (pSep)
							{ assert (_formUp >= 0 && _formDown >= -1 && _pixelsSeparation >= 0); }

			int _formUp;
			int _formDown;
			Orientation _orientation;
			int _pixelsSeparation;
		};

		/** The maximum number of lives is the second parameter and it has to be greater than 0. */
		LivesScoreObject (int id, int nL, const Properties& prps = Properties (), const Position& p = Position::_cero);

		/** To get and set the number of lives still in use */
		int livesStill () const
								{ return (_livesStillInUse); }
		void setLivesStillInUse (int nL);

		/** To get and change the current properties. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; }

		/** @see parent. */
		virtual void initialize ();
		virtual void drawOn (QGAMES::Screen* s, 
			const QGAMES::Position& p = QGAMES::Position::_noPoint);

		protected:
		/** The maximum number of lives in use. */
		int _maxNumberOfLives;
		/** The properties used. */
		Properties _properties;

		// Implementation 
		/** How many are still in use now. */
		int _livesStillInUse;
	};

	/** A energy bar.
		How filled the bar is, is determinated by a number between 0 an 100. 
		Any form used to rerpesent the level of energy should be made up 2 frames only:
		The first representing the situation when there is energy at all,
		and the other one representing the situation with full of energy. */
	class EnergyBarScoreObject : public ScoreObject
	{
		public:
		/** 
		  *	Constructor.
		  *	@param id:	The id of the score object,
		  *	@param fU:	and the number of the form used to represent the bar. 
						This one has to exist.
		  * @param p:	Where to draw the energy bar, by default it is the top left corner.
		  */
		EnergyBarScoreObject (int id, int fU, const Position& p = Position::_cero);

		/** @see parent. 
			It doesn't have neither movements, nor animations, nor states to copy. */
		virtual Entity* clone () const
								{ return (new EnergyBarScoreObject (id (), _formUsed)); }

		/** To set the % of the energy consumed. */
		void setPercentage (int p);
		/** To know how % of the energy has been consumed. */
		int percentage () const
								{ return (_percentage); }

		/** @see parent. */
		virtual void initialize ();
		virtual void drawOn (QGAMES::Screen* s, 
			const QGAMES::Position& p = QGAMES::Position::_noPoint);

		protected:
		/** The number of the form used to represent the bar. */
		int _formUsed;

		// Implementation
		/** The percentrage of the energy that has been consumed. */
		int _percentage;
	};

	/** The standard bar is that...a bar of energy!. 
		The user cab parametrized the color of the bar (choosing among four colours. */
	class StandardEnergyBarScoreObject : public EnergyBarScoreObject
	{
		public:
		typedef enum { _BLUE = 0, _RED = 1, _GREEN = 2, _YELLOW = 3 } Color;

		StandardEnergyBarScoreObject (int id, const Position& p = Position::_cero, Color c = _BLUE)
			: EnergyBarScoreObject (id, __QGAMES_BLUEENERGYBARFORM__ + (int) c, p)
							{ }
	};
}

#endif
