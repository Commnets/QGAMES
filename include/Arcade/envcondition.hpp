/** \ingroup ArcadeGame */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: envconditions.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/01/2017 \n
 *	Description: Defines an environmental condition. \n
 *				 In an scene can happen many different environmental situations. \n
 *				 A earthquake can be one of them, or raining, or.... \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ENVCONDITION__
#define __QGAMES_ENVCONDITION__

#include <Common/element.hpp>
#include <Common/screen.hpp>
#include <Common/position.hpp>
#include <Arcade/artist.hpp>
#include <vector>

namespace QGAMES
{
	class Scene;
	class Entity;

	/** 
	 * An scene can be affected by some environmental condition. \n
	 * That condition can be "rain" i.e or "wind", or any other thing
	 * affecting all people in.
	 * The condition can affct the entities of a scene only when it starts or
	 * in every loop. Usually the first options is enough.
	 */
	class EnvironmentalCondition : public QGAMES::Element
	{
		public:
		/** The last attribute is to indicate whether the condition affects the artists
			only once (when initialized) or in everyLoop. By default once. */
		EnvironmentalCondition (int id, Scene* s, bool aOnce = true);
		virtual ~EnvironmentalCondition ()
							{ }

		/** To know the scene. */
		Scene* scene ()
							{ return (_scene); }

		/** To know whether affects only once or per loop. */
		bool affectOnlyOnce () const
							{ return (_affectsOnlyOnce); }

		/** @see parent. 
			To send events to whoever lisening this object.
			These event inidicate whether the condition starts or finishes. */
		virtual void initialize ();
		virtual void finalize ();

		// Parent doesn't define this method, and it is needed...
		virtual void updatePositions ()
							{ }

		/** The condition can affect the artists taking palce in a scene. \n
			By default this method invokes the method @see whenWheatherCondition in the artist. \n
			The method is usually invoked from the Scene. */
		virtual void affectArtists (const Entities& ets);
		virtual void stopAffectingArtists (const Entities& ets);

		protected:
		Scene* _scene;
		bool _affectsOnlyOnce;

	};

	// To simplify the definition of a set of conditions...
	typedef std::vector <EnvironmentalCondition*> EnvironmentalConditions;

	/** A set of environmental conditions. \n
		The class owns the conditions, so it deletes them when destroyed. 
		The individual conditions can be activated or desactivated. */
	class ComplexEnvironmentalCondition : public EnvironmentalCondition
	{
		public:
		ComplexEnvironmentalCondition (int id, QGAMES::Scene* s, 
			const EnvironmentalConditions& cds);
		/** @see parent. */
		virtual ~ComplexEnvironmentalCondition ()
							{ deleteConditions (); }

		// To manage the list of conditions....
		void setConditions (const EnvironmentalConditions& cds)
							{ deleteConditions (); _conditions = cds; }
		void addCondition (EnvironmentalCondition* cd);
		void activateCondition (EnvironmentalCondition* cd);
		void desactivateCondition (EnvironmentalCondition* cd);
		bool existsCondition (int id) const;
		EnvironmentalCondition* findCondition (int id);

		/** @see parent. */
		virtual void initialize ();
		virtual void inEveryLoop ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		virtual void finalize ();

		/** @see parent. */
		virtual void affectArtists (const Entities& ets);
		virtual void stopAffectingArtists (const Entities& ets);

		/** @see parent. */
		virtual void processEvent (const Event& evnt);

		protected:
		/** To test whether a condition is or not active. 
			if null or not exists, returns false. Otherwise returns the status. */
		bool isActive (EnvironmentalCondition* cd);

		private:
		// Implementation
		/** To delete the conditions. */
		void deleteConditions ();

		protected:
		/** The list of conditions. */
		EnvironmentalConditions _conditions;
		/** The status of the conditions. Initially all are active. */
		std::map <EnvironmentalCondition*, bool> _conditionStatus;		
	};

	/**
	  *	This class is to manage how the density of an environmental condition changes 
	  *	during the game cycles to become what it has been defined at construction time. \n 
	  *	This manager will be applied to those conditions requiring it. \n
	  *	Several environmental conditions require to draw things. \n
	  *	In many of them all things to draw don't appear out of the blue but bit by bit.
	  *	Rain is one example. It doesn't start to rain suddenly usually. Normally,
	  *	drops start to fall more and more. \n
	  *	This class is just to manage also this kind of situations. 
	  */
	class DensityFactorManager
	{
		public:
		// Constructrors & Destructor
		/** Default constructor. */
		DensityFactorManager ();
		/** 
		  *	To create the density manager.
		  *	It is needed:
		  *	@param d:  Target density.
		  *	@param iF: Initial density factor.
		  *	@param fF: Final density factor.
		  *	@param nT: The number of things to manage (rain drops?) 
		  *	@param i : The increment to go from the iF to the fF.
		  *	@param up: Does the manager goes from down to up o viceversa?
		  */
		DensityFactorManager (bdata d, bdata iF, bdata fF, int nT, bdata i = __BD 0.1, bool up = true);
		// No destructor needed...

		/** To know the current value of the density factor. \n
			It is not usually needed, but the method is here just in case. */
		bdata currentFactor () const
								{ return (_currentFactor); }
		/** To know which things must drawn in this specific cycle. */
		std::vector <bool> currentThingsStatus () const
								{ return (_thingsStatus); }

		/** Reverse the factor manager. */
		void reverse ();
		/** Adapt the factor manager to another smoothly */
		void adaptTo (bdata iF, bdata fF, QGAMES::bdata i = __BD 0.1, bool up = true);

		/** To initialize the factor manager back. */
		void initialize ();
		/** To update the factor in each interaction. */
		void update ();

		private:
		// Implementation
		/** Internal method to calculate the array of things to draw. */
		void adjustThingsStatus ();

		private:
		/** The target density. */
		bdata _targetDensity;
		/** The initial density factor. */
		bdata _initialFactor; 
		/** The final density factor. */
		bdata _finalFactor; 
		/** The incremental step to go from the initial factor to the final one. */
		bdata _incrementalFactor;
		// This incremental will go forward or backward depending on the final implementation
		/** Goes up or down? **/
		bool _goingUp; 
		/** Number of things being managed. */
		int _numberOfThings; 

		// Implementation
		bdata _currentFactor; /** Which is the current factor. */
		std::vector <bool> _thingsStatus; /** The status of the things. */
	};
}

#endif
  
// End of the file
/*@}*/
