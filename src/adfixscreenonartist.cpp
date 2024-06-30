#include <AdvancedArcade/adfixscreenonartist.hpp>
#include <AdvancedArcade/adscenes.hpp>

// ---
void QGAMES::ToFixScreenOnArtistInSceneWithBackground::moveScreenPixels (int x, int y, int z)
{
	if (x == 0 && y == 0 && z == 0)
		return;

	QGAMES::Vector dM (__BD x, __BD y, __BD z);

	// If the artist is on a scene with background, moves the background...
	// Take into account that the background has to be first of all stopped
	// The only way to move it is using this method...
	QGAMES::SceneWithBackground* bS = 
		dynamic_cast <QGAMES::SceneWithBackground*> (artist () -> map () -> scene ());
	if (bS)
	{
		if (_lD != dM)
		{
			bS -> backgroundMap () -> setMove (dM / 2);
			_lD = dM;
		}

		bS -> backgroundMap () -> solidMove (dM);
		bS -> backgroundMap () -> move ();
	}

	QGAMES::ToFixScreenOnArtist::moveScreenPixels (x, y, z);
}
