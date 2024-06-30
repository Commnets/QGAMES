/** \ingroup ArcadeGame */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: tile.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a tile. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_TILE__
#define __QGAMES_TILE__

#include <Common/element.hpp>
#include <Common/form.hpp>
#include <vector>

namespace QGAMES
{
	#define __QGAMES_TILEEMPTY__	0

	class Tile;
	typedef std::vector <Tile*> Tiles;
	typedef std::vector <Tiles> SetOfTiles;
	typedef std::map <std::string, std::string> TileProperties;
	class ArtistInATiledMap;
	typedef std::list <ArtistInATiledMap*> ArtistInATiledMapList; 
	// It is also defined at artist.cpp

	/** A map is made up of tiles. \n
		A tile is a little piece of the tiled map. \n
		The same tile (not usual) can be part of many maps. */
	class Tile : public Element
	{
		public:
		Tile (int id, Form* f, int nf, int t, const TileProperties& p = TileProperties ());

		Form* form ()
							{ return (_form); }
		void setForm (Form* f, int nf);
		int fade ()
							{ return (_fade); }
		void setFade (int f)
							{ _fade = f; }
		Frame* frame ()
							{ return (_form -> frame (_numberFrame)); }
		int numberFrame () const
							{ return (_numberFrame); }
		int type () const
							{ return (_type); }
		TileProperties properties () const 
							{ return (_properties); }
		std::string property (const std::string& p) const
							{ return ((_properties.find (p) == _properties.end ()) 
								? std::string (__NULL_STRING__) : (*_properties.find (p)).second); }
		bool isEmpty () const
							{ return (type () == __QGAMES_TILEEMPTY__); }

		/** 
		  *	In some situations, an artist is quite linked to a tile. \n
		  *	When the tile is drawn, the artist has to be also drawn. \n
		  *	This is something usual in isometric and 3D maps. \n
		  *	In that cases, makes no sense to drawn some characteres when the layer is drawn
		  *	but draw the layer and draw the characters when the tiles associated to the
		  *	layer are drawn. 
		  */
		const ArtistInATiledMapList& artistsLinked () const
							{ return (_artists); }
		void addArtistLinked (ArtistInATiledMap* a);
		void deleteArtistLiked (ArtistInATiledMap* a);

		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);

		protected:
		Form* _form;
		int _numberFrame;
		int _fade;
		int _type;
		TileProperties _properties;
		ArtistInATiledMapList _artists;
	};

	/** When there is nothing in a map position, a null tile is put. 
		This type of tile is used often. When a tile in a map is not defined
		because it has not e.g. an special behaviour, a null tile should be assigned
		To avoid a vaste use of memory, the null tile should be shared among maps. 
		The null tile does not do anything, and is not painted as well. */
	class NullTile : public Tile
	{
		public:
		static NullTile* _nullTile;

		NullTile (int id)
			: Tile (id, NULL, 0, __QGAMES_TILEEMPTY__, TileProperties ())
							{ setVisible (false); }

		/** By default a null tile shouldn't be drawn. */
		virtual void drawOn (Screen* s, 
			int x = __MININT__, int y = __MININT__)
							{ }
	};
}

#endif

// End of the file
/*@}*/
