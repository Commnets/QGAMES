#include <AdvancedArcade/admaps.hpp>

// ---
QGAMES::BackgroundMap::BackgroundMap (int id, const QGAMES::Layers& l, int w, int h, const QGAMES::MapProperties& p)
	: QGAMES::ObjectMap (id, l, w, h, 0, p)
{
	assert (l.size () == 1); // 1 and only 1...
	assert (dynamic_cast <QGAMES::BackgroundLayer*> (*_layers.begin ())); // It has to be a background layer...
}
