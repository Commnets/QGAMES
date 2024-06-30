/** \ingroup Platform */
/** \ingroup Strcuture */
/*@{*/

/**	
 *	@file	
 *	File: pttiles.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 05/05/2018 \n
 *	Description: The tiles that are usually part of the platform game. \n
 *				  They don't add specific behaviours.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_PTTILES__
#define __QGAMES_PTTILES__

#include <Platform/ptdefinitions.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace QGAMES
{
	/** It represents something where characters can stand up on. */
	class BaseTile : public Tile
	{
		public:
		BaseTile (int id, Form* f, int nf, int t, const TileProperties& p = TileProperties ())
				: Tile (id, f, nf, t, p)
							{ }
	};

	/** It represents something that the characters can collision with (avoinding to advance). */
	class SolidTile : public Tile
	{
		public:
		SolidTile (int id, Form* f, int nf, int t, const TileProperties& p = TileProperties ())
				: Tile (id, f, nf, t, p)
							{ }
	};

	/** It represents something that the characters can use to go up & down. */
	class LadderTile : public Tile
	{
		public:
		LadderTile (int id, Form* f, int nf, int t, const TileProperties& p = TileProperties ())
				: Tile (id, f, nf, t, p)
							{ }
	};

	/** It represents something where the characters can "slidding" over. */
	class SliddingTile : public Tile
	{
		public:
		SliddingTile (int id, Form* f, int nf, int t, const TileProperties& p = TileProperties ())
				: Tile (id, f, nf, t, p)
							{ }
	};

	/** It represents something where the characters can "swin" in. */
	class LiquidTile : public Tile
	{
		public:
		LiquidTile (int id, Form* f, int nf, int t, const TileProperties& p = TileProperties ())
				: Tile (id, f, nf, t, p)
							{ }
	};

	/** It represents something that can kill a character. */
	class KillingTile : public Tile
	{
		public:
		KillingTile (int id, Form* f, int nf, int t, const TileProperties& p = TileProperties ())
				: Tile (id, f, nf, t, p)
							{ }
	};

	/** Just decoration. */
	class DecorationTile : public Tile
	{
		public:
		DecorationTile (int id, Form* f, int nf, int t, const TileProperties& p = TileProperties ())
				: Tile (id, f, nf, t, p)
							{ }
	};

	/** A tile with additional information. */
	class InformedTile : public Tile
	{
		public:
		InformedTile (int id, Form* f, int nf, int t, const TileProperties& p = TileProperties ())
				: Tile (id, f, nf, t, p),
				  _parameters (p)
						{ }
	
		/** To get & set the configuration values. */
		const SetOfOpenValues& parameters () const
							{ return (_parameters); }
		void setParameters (const SetOfOpenValues& oV)
							{ _parameters = oV; }

		protected:
		SetOfOpenValues _parameters;
	};

	/** A tile that notifies something when a character collisions with it. 
		When the tile is defined in the map, it can have several attributes inside. */
	class NotificationTile : public InformedTile
	{
		public:
		NotificationTile (int id, Form* f, int nf, int t, const TileProperties& p = TileProperties ())
				: InformedTile (id, f, nf, t, p)
							{ }
	};

	/** A tile that indicates the location of something. 
		When the tile is defined in the map, it can have several attributes associated. */
	class LocationTile : public InformedTile
	{
		public:
		LocationTile (int id, Form* f, int nf, int t, const TileProperties& p = TileProperties ())
				: InformedTile (id, f, nf, t, p)
							{ }
	};
}

#endif
  
// End of the file
/*@}*/