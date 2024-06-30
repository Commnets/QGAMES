#include "MapsBuilder.hpp"
#include "Maps.hpp"
#include "Defs.hpp"

// ---
QGAMES::Map* MapsBuilderAddsOn::createMap (int id, int w, int h, int d,
		const QGAMES::Layers& l, const QGAMES::MapProperties& pties)
{
	QGAMES::Map* result = (id == __SABREWULFMAP__) 
		? new SabreWulfMazePlace (id, l, w, h, pties)
		: QGAMES::ObjectMapBuilderAddsOn::createMap (id, w, h, d, l, pties);
	return (result);
}