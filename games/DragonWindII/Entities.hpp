/** \ingroup DWTheRevege */
/*@{*/

/**	
 *	@file	
 *	File: Entities.hpp \n
 *	Game: DragonwindII (The Revenge) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 31/03/2020 \n
 *	Description: Entities defined in Dragonwind The Revenge. \n
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWINDTHEREVENGE_ENTITIES__
#define __DRAGONWINDTHEREVENGE_ENTITIES__

#include <DragonwindLike/dwinclude.hpp>

namespace DragonTheRevenge
{
	/** Some little modifications about how the thing is created from description. */
	class Thing : public DRAGONWIND::Thing
	{
		public:
		Thing (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::PlatformArtist::Data& d = QGAMES::PlatformArtist::Data ())
			: DRAGONWIND::Thing (cId, f, d)
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent. */
		virtual int aspectAssociatedToThingType (int t);
		virtual int thingTypeAssociatedToAspect (int a);
	};

	/** Some modifications over the type of shootings. */
	class Shooting : public DRAGONWIND::Shooting
	{
		public:
		Shooting (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::PlatformArtist::Data& d = QGAMES::PlatformArtist::Data ())
			: DRAGONWIND::Shooting (cId, f, d)
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		protected:
		/** @see parent. */
		virtual DRAGONWIND::Shooting::StatesId statesIdForType (int t);
		virtual DRAGONWIND::Shooting::Behaviour* behaviourForType (int t);

		#define __DRAGONWIND_FIREBALLSHOOTINGSTATESID__ \
			DRAGONWIND::Shooting::StatesId (40, 41, 42, 43, 44, 45, -1, -1, -1, -1, 50, __BD 1.2, true)
	};


	/** The bad guys of the game. */
	class BadGuy : public DRAGONWIND::BadGuy
	{
		public:
		// Just a couple of defines to simplify the definition of the very bad guys defined here...
		#define __DRAGONWIND_DINOSTATESID__ \
			DRAGONWIND::BadGuy::StatesId (80, 90, 81, 91, 82, 92, 83, 93, 84, 94, 85, 95, 86, 96)
		#define __DRAGONWIND_MALEZOMBIESTATESID__ \
			DRAGONWIND::BadGuy::StatesId (100, 110, 101, 111, 102, 112, 103, 113, 104, 114, 105, 115, 106, 116)
		#define __DRAGONWIND_FEMALEZOMBIESTATESID__ \
			DRAGONWIND::BadGuy::StatesId (120, 130, 121, 131, 122, 132, 123, 133, 124, 134, 125, 135, 126, 136)
		#define __DRAGONWIND_KONGSTATESID__ \
			DRAGONWIND::BadGuy::StatesId (140, 150, 141, 151, 142, 152, 143, 153, 144, 154, 145, 155, 146, 156)
		#define __DRAGONWIND_ORCUSSTATESID__ \
			DRAGONWIND::BadGuy::StatesId (160, 170, 161, 171, 162, 172, 163, 173, 164, 174, 165, 175, 166, 176)
		#define __DRAGONWIND_BOARSTATESID__ \
			DRAGONWIND::BadGuy::StatesId (180, 190, 181, 191, 182, 192, 183, 193, 184, 194, 185, 195, 186, 196)

		BadGuy (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::PlatformArtist::Data& d = QGAMES::PlatformArtist::Data ())
			: DRAGONWIND::BadGuy (cId, f, d)
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		protected:
		/** @see parent. */
		virtual StatesId statesIdForType (int t);
	};

	/** Specific flock of vultures. */
	class FlockOfVultures : public DRAGONWIND::FlockOfMonsters
	{
		public:
		FlockOfVultures (int cId, const QGAMES::Entities& eties, const QGAMES::Entity::Data& dt = QGAMES::Entity::Data ())
			: DRAGONWIND::FlockOfMonsters (cId, eties, Properties (dt._parameters), dt)
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent 
			The initialization of the sound is specific. */
		virtual void initialize ();
		virtual void finalize ();
	};

	/** Specific flock of Wasps. */
	class FlockOfWasps : public DRAGONWIND::FlockOfMonsters
	{
		public:
		FlockOfWasps (int cId, const QGAMES::Entities& eties, const QGAMES::Entity::Data& dt = QGAMES::Entity::Data ())
			: DRAGONWIND::FlockOfMonsters (cId, eties, Properties (dt._parameters), dt)
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent 
			The initialization of the sound is specific. */
		virtual void initialize ();
		virtual void finalize ();
	};

	/** Specific flock of bats. */
	class FlockOfBats : public DRAGONWIND::FlockOfMonsters
	{
		public:
		FlockOfBats (int cId, const QGAMES::Entities& eties, const QGAMES::Entity::Data& dt = QGAMES::Entity::Data ())
			: DRAGONWIND::FlockOfMonsters (cId, eties, Properties (dt._parameters), dt)
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent 
			The initialization of the sound is specific. */
		virtual void initialize ();
		virtual void finalize ();
	};
}

#endif

// End of the file
/*@}*/
