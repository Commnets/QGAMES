#include <DragonwindLike/dwlayers.hpp>
#include <DragonwindLike/dwdefinitions.hpp>

// ---
bool DRAGONWIND::LiquidTileLayer::isPoison () const
{
	return (existsProperty (std::string (__DRAGONWIND_LIQUIDLAYERPOISONPRM__)) 
		? ((QGAMES::toUpper (layerProperty (std::string (__DRAGONWIND_LIQUIDLAYERPOISONPRM__))) == std::string (__YES_STRING__)) 
			? true : false)
		: false);
}
