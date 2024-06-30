#include <Arcade/tile.hpp>
#include <Arcade/artist.hpp>
#include <Common/screen.hpp>

// ---
QGAMES::NullTile* QGAMES::NullTile::_nullTile = new QGAMES::NullTile (-1);

QGAMES::Tile::Tile (int id, QGAMES::Form* f, int nf, int t, const QGAMES::TileProperties& p)
	: Element (id),
	  _form (f),
	  _numberFrame (nf),
	  _fade (255), // By default. It has to be explicity changed!
	  _type (t),
	  _properties (),
	  _artists ()
{
	// Assert can't be done, because sometimes the tile is assigned to a form
	// after it's been created. This is tipycal in tmx maps i.e.
	// assert (f);
}

// ---
void QGAMES::Tile::setForm (QGAMES::Form* f, int nf)
{
	assert (f);

	_form = f; _numberFrame = nf;
}

// ---
void QGAMES::Tile::addArtistLinked (QGAMES::ArtistInATiledMap* a)
{
	assert (a);

	bool found = false;
	for (ArtistInATiledMapList::const_iterator i = _artists.begin ();
			i != _artists.end () && !found; i++)
		if ((*i) == a)
			found = true;
	if (!found)
		_artists.push_back (a);
}

// ---
void QGAMES::Tile::deleteArtistLiked (QGAMES::ArtistInATiledMap* a) 
{
	assert (a);

	bool found = false;
	ArtistInATiledMapList::const_iterator i = _artists.begin ();
	while (i != _artists.end () && !found)
		if ((*i) == a) found = true;
		else i++;
	if (found)
		_artists.erase (i);
}

// ---
void QGAMES::Tile::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	_form -> drawOn (s, _numberFrame, p, _fade);

	// It is not usual, but can happen there are entities linked to the tile...
	if (!_artists.empty ())
		for (ArtistInATiledMapList::const_iterator i = _artists.begin (); 
				i != _artists.end (); i++)
		(*i) -> drawOn (s, p); 
}
