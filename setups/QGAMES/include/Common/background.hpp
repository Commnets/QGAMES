/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: background.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 05/06/2016 \n
 *	Description: Defines the background for a game. \n
 *				 Any game can have a background. The background is always something so far away \n
 *				 from the plabe of the main game, and usually is represented in 2D.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_BACKGROUND__
#define __QGAMES_BACKGROUND__

#include <Common/object.hpp>
#include <Common/form.hpp>
#include <Common/position.hpp>

namespace QGAMES
{
	/** This class represents the background of a game. \n
		A game usually has a background. \n
		Sometimes the background moves, sometimes doesn't. The speed can be changed. \n
		The background can be made up of many copies one after other in the movement direction and roll when 
		the position is out of the visible screen. \n
		The background can be linked with other creating such a layer effect. */
	class Background : public Object
	{
		public:
		/** 
		 *	Constructor
		 *	@param id	The id of the background.
		 *	@param pos	The position of the background in the game / screen
		 *	@param f	The form (2D) the background is representing by.
		 *	@param n	The number of times this background is repeteated, one after other.
		 *	@param oP	The properties of the background. All of them
		 *				are comming from the definition file. */
		Background (int id, const Position& pos, Sprite2D* f,
			 int n = 1, const ObjectProperties& oP = ObjectProperties ());
		// No destructor needed...
		// The class is not the owner of the forms received
		// They must be created usng the form builder, who really owns them!

		int fadeGrade () const
							{ return (_fadeGrade); }
		void setFade (int fG);

		bool autoMovement () const
							{ return (_automaticMov); }
		const Vector& movementDirection () const
							{ return (_movDirection); }
		int movementSpeed () const
							{ return (_speed); }
		int movementPixels () const
							{ return (_pixels); }

		// To move the background
		/** 
		  *	To set the parameters to control the movement of the background. 
		  *	@param s	The number of steps (counted in everyLoop method) to wait before any movement.
		  *	@param p	The number of pixles to move once the other counter reaches the limit.
		  */
		void setMovementData (int s, int p);
		/** 
		  * To set whether the background moves alone or not. \n
		  * If it does, it actualizes its position in every loop. 
		  *	@param s	Does the background move alone?
		  * @param d	The direction of that automatic movement (if any).
		  * @param e	Is the movement linked with other background? */
		void setMove (bool s, const Vector& d = Vector (__BD 1, __BD 0, __BD 0), bool e = false);
		void move ();
		/** To move the background like a block,
			not taking into account the different speeds per layer, if any. \n
			The fix layers are maintained in their position. */
		void solidMove (const QGAMES::Vector& d);

		// Managing the linked background if any...
		Background* linkedBackground () 
							{ return (_linkedBackground); }
		int linkedBackgroundSpeed () const
							{ return (_linkedSpeed); }
		/** Link the background to other. \n
			It means that when the background moves drags the linked one.
			This movement happens as the number of steps reflected in the parameter. */
		void setLinkedBackground (int s, Background* b);

		bool roll () const
							{ return (_roll); }
		/** To roll or not. */
		void setRoll (bool r);

		/** Part of the data needed to move the background could be read
			from the properties receives as parameter. If they are not a default value is provided. */
		virtual void initialize ();
		virtual void inEveryLoop ();
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		virtual void finalize ();

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }

		// Variables to control how the background moves...
		static const int _COUNTERSPEED = 0;
		static const int _COUNTERLINKEDSPEED = 1;

		protected:
		/** A reference to the image.
			The class is not the owner of the background. 
			Notice that it is always a 2D image not a generic form. */
		Sprite2D* _background;
		/** Number of times the background is repeated in the direction
			of the movement always. */
		int _numberOfTimes;
		/** The fade grade of that backfround. 255 means fully visible. */
		int _fadeGrade;
		/** Does the background move automatically?. */
		bool _automaticMov;
		/** The direction of the movement. */
		Vector _movDirection;
		/** Number of steps per everyloop execution to move the background. */
		int _speed;
		/** Number of pixels the background moves when it is time. */
		int _pixels;
		/** A reference to the background linked if any.
			If there is no linked background the value of the variable is NULL. */
		Background* _linkedBackground;
		/** The number of pixels advanced before moving the linked background.
			This variable makes only sense when the previous one is not NULL. */
		int _linkedSpeed;
		/** To determinate whether the background rolls or not. */
		bool _roll;
	};
}

#endif
  
// End of the file
/*@}*/
