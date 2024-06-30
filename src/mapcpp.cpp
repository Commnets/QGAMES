#include <Arcade/map.hpp>

QGAMES::Map::Map (int c, const Layers& l, int w, int h)
		: Element (c),
		  _layers (l),
		  _tiles (),
		  _objects (),
		  _images (),
		  _width (w),
		  _height (h)
{ 
}

QGAMES::Map::~Map ()
{
}

