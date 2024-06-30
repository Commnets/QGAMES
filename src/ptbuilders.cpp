#include <Platform/ptbuilders.hpp>
#include <Platform/pttiles.hpp>
#include <Platform/ptlayers.hpp>
#include <Platform/ptmaps.hpp>

// ---
QGAMES::Entity* QGAMES::PlatformEntityBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	QGAMES::Entity* result = NULL;
	if (def._id >= __QGAMES_PTSOMETHINGTOEATBASEID__ &&// 
		def._id < (__QGAMES_PTSOMETHINGTOEATBASEID__ + __QGAMES_PTNUMBERSOMETHINGTOEAT__))
		result = createSomethingToEat (def);
	else 
	if (def._id >= __QGAMES_PTSOMETHINGTOCATCHBASEID__ && 
		def._id < (__QGAMES_PTSOMETHINGTOCATCHBASEID__ + __QGAMES_PTNUMBERSOMETHINGTOCATCH__))
		result = createSomethingToCatch (def);
	else
	if (def._id >= __QGAMES_PTBADGUYBASEID__ && 
		def._id < (__QGAMES_PTBADGUYBASEID__ + __QGAMES_PTNUMBERBADGUY__))
		result = createBadGuy (def);
	else
		result = QGAMES::AdvancedEntityBuilder::createEntity (def);

	assert (result); // It must be something...
	return (result);
}

// ---
void QGAMES::PlatformTMXMapBuilder::afterCreatingMap (QGAMES::Map* mp)
{
	QGAMES::PlatformMap* ptm = dynamic_cast <QGAMES::PlatformMap*> (mp);
	if (!ptm) // If it is not a platformap map, makes no sense to continue...
		return;

	// Add the parameters to the notification tiles...if any
	QGAMES::NotificationTileLayer* nL = ptm -> notificationLayer ();
	if (nL)
	{
		for (QGAMES::Tiles::const_iterator i = nL -> tiles ().begin (); i != nL -> tiles ().end (); i++)
		{
			QGAMES::NotificationTile* nT = dynamic_cast <QGAMES::NotificationTile*> (*i);
			if (nT)
				nT -> setParameters (ptm -> propertiesForPosition (nL -> tilePosition (nT)));
		}
	}

	// Add the parameters to the loction tiles...if any...
	QGAMES::LocationTileLayer* lL = ptm -> locationLayer ();
	if (lL)
	{
		for (QGAMES::Tiles::const_iterator i = lL -> tiles ().begin (); i != lL -> tiles ().end (); i++)
		{
			QGAMES::LocationTile* lT = dynamic_cast <QGAMES::LocationTile*> (*i);
			if (lT)
				lT -> setParameters (ptm -> propertiesForPosition (lL -> tilePosition (lT)));
		}
	}
}

// ---
int QGAMES::PlatformTMXMapBuilder::tileTypeInForm (const std::string& frm, int nf)
{
	int result = -1; // Unknow...so far

	if (frm.find (std::string (__QGAMES_PTTILEFORMBASECONTAINS__)) != std::string::npos) 
		result = (std::find (_baseExcludedTileSet.begin (), _baseExcludedTileSet.end (), nf) == 
					_baseExcludedTileSet.end ()) ? __QGAMES_PTTBASETILETYPE__ : __QGAMES_PTDECORATIONTILETYPE__;
	else if (frm.find (std::string (__QGAMES_PTTILEFORMSOLIDCONTAINS__)) != std::string::npos) 
		result = (std::find (_solidExcludedTileSet.begin (), _solidExcludedTileSet.end (), nf) == 
					_solidExcludedTileSet.end ()) ? __QGAMES_PTSOLIDTILETYPE__ : __QGAMES_PTDECORATIONTILETYPE__;
	else if (frm.find (std::string (__QGAMES_PTTILEFORMLADDERCONTAINS__)) != std::string::npos) 
		result = (std::find (_ladderExcludedTileSet.begin (), _ladderExcludedTileSet.end (), nf) == 
					_ladderExcludedTileSet.end ()) ? __QGAMES_PTLADDERTILETYPE__ : __QGAMES_PTDECORATIONTILETYPE__;
	else if (frm.find (std::string (__QGAMES_PTTILEFORMSLIDDINGCONTAINS__)) != std::string::npos)
		result = (std::find (_sliddingExcludedTileSet.begin (), _sliddingExcludedTileSet.end (), nf) == 
					_sliddingExcludedTileSet.end ()) ? __QGAMES_PTSLIDDINGTILETYPE__ : __QGAMES_PTDECORATIONTILETYPE__;
	else if (frm.find (std::string (__QGAMES_PTTILEFORMLIQUIDCONTAINS__)) != std::string::npos)
		result = (std::find (_liquidExcludedTileSet.begin (), _liquidExcludedTileSet.end (), nf) == 
					_liquidExcludedTileSet.end ()) ? __QGAMES_PTLIQUIDTILETYPE__ : __QGAMES_PTDECORATIONTILETYPE__;
	else if (frm.find (std::string (__QGAMES_PTTILEFORMKILLINGCONTAINS__)) != std::string::npos) 
		result = (std::find (_killingExcludedTileSet.begin (), _killingExcludedTileSet.end (), nf) == 
					_killingExcludedTileSet.end ()) ? __QGAMES_PTKILLINGTILETYPE__ : __QGAMES_PTDECORATIONTILETYPE__;
	else if (frm.find (std::string (__QGAMES_PTTILEFORMDECORATIONCONTAINS__)) != std::string::npos) 
		result = __QGAMES_PTDECORATIONTILETYPE__;
	else if (frm.find (std::string (__QGAMES_PTTILEFORMLOCATIONCONTAINS__)) != std::string::npos) 
		result = (std::find (_locationExcludedTileSet.begin (), _locationExcludedTileSet.end (), nf) == 
					_locationExcludedTileSet.end ())? __QGAMES_PTLOCATIONTILETYPE__ : __QGAMES_PTDECORATIONTILETYPE__;
	else if (frm.find (std::string (__QGAMES_PTTILEFORMNOTIFICATIONCONTAINS__)) != std::string::npos) 
		result = (std::find (_notificationExcludedTileSet.begin (), _notificationExcludedTileSet.end (), nf) == 
					_notificationExcludedTileSet.end ()) ? __QGAMES_PTNOTIFICATIONTILETYPE__ : __QGAMES_PTDECORATIONTILETYPE__;
	else 
		result = __QGAMES_PTDECORATIONTILETYPE__; // By default it is decoration...

	return (result);
}

// ---
int QGAMES::PlatformTMXMapBuilder::layerType (const std::string& lName)
{
	int result = -1; // Unknow...so far

	if (lName.find (std::string (__QGAMES_PTTILEFORMBASECONTAINS__)) != std::string::npos) 
		result = __QGAMES_PTTBASETILETYPE__;
	else if (lName.find (std::string (__QGAMES_PTTILEFORMSOLIDCONTAINS__)) != std::string::npos) 
		result = __QGAMES_PTSOLIDTILETYPE__;
	else if (lName.find (std::string (__QGAMES_PTTILEFORMLADDERCONTAINS__)) != std::string::npos) 
		result = __QGAMES_PTLADDERTILETYPE__;
	else if (lName.find (std::string (__QGAMES_PTTILEFORMSLIDDINGCONTAINS__)) != std::string::npos) 
		result = __QGAMES_PTSLIDDINGTILETYPE__;
	else if (lName.find (std::string (__QGAMES_PTTILEFORMLIQUIDCONTAINS__)) != std::string::npos) 
		result = __QGAMES_PTLIQUIDTILETYPE__;
	else if (lName.find (std::string (__QGAMES_PTTILEFORMKILLINGCONTAINS__)) != std::string::npos) 
		result = __QGAMES_PTKILLINGTILETYPE__;
	else if (lName.find (std::string (__QGAMES_PTTILEFORMDECORATIONCONTAINS__)) != std::string::npos) 
		result = __QGAMES_PTDECORATIONTILETYPE__;
	else if (lName.find (std::string (__QGAMES_PTTILEFORMNOTIFICATIONCONTAINS__)) != std::string::npos) 
		result = __QGAMES_PTNOTIFICATIONTILETYPE__;
	else if (lName.find (std::string (__QGAMES_PTTILEFORMLOCATIONCONTAINS__)) != std::string::npos) 
		result = __QGAMES_PTLOCATIONTILETYPE__;
	else if (lName.find (std::string (__QGAMES_PTZONEOBJECTLAYERCONTAINS__)) != std::string::npos) 
		result = __QGAMES_PTZONEOBJECTLAYERTYPE__;
	else
		result = __QGAMES_PTDECORATIONTILETYPE__; // By default it is decoration...

	return (result);
}

// ---
QGAMES::Tile* QGAMES::PlatformTMXMapBuilder::createTile (int id, QGAMES::Form* form, int nf, 
	const QGAMES::TileProperties& p)
{
	QGAMES::Tile* result = NULL;

	int type = tileTypeInForm (form -> name (), nf); 
	switch (type)
	{
		case __QGAMES_PTTBASETILETYPE__:
			result = new QGAMES::BaseTile (id, form, nf, type, p);
			break;
		case __QGAMES_PTSOLIDTILETYPE__:
			result = new QGAMES::SolidTile (id, form, nf, type, p);
			break;
		case __QGAMES_PTLADDERTILETYPE__:
			result = new QGAMES::LadderTile (id, form, nf, type, p);
			break;
		case __QGAMES_PTSLIDDINGTILETYPE__:
			result = new QGAMES::SliddingTile (id, form, nf, type, p);
			break;
		case __QGAMES_PTLIQUIDTILETYPE__:
			result = new QGAMES::LiquidTile (id, form, nf, type, p);
			break;
		case __QGAMES_PTKILLINGTILETYPE__:
			result = new QGAMES::DecorationTile (id, form, nf, type, p);
			break;
		case __QGAMES_PTDECORATIONTILETYPE__:
			result = new QGAMES::DecorationTile (id, form, nf, type, p);
			break;
		case __QGAMES_PTLOCATIONTILETYPE__:
			result = new QGAMES::LocationTile (id, form, nf, type, p);
			break;
		case __QGAMES_PTNOTIFICATIONTILETYPE__:
			result = new QGAMES::NotificationTile (id, form, nf, type, p);
			break;

		default:
			assert (0); // It shouldn't be here...
			break;
	}

	return (result);
}

// ---
QGAMES::TileLayer* QGAMES::PlatformTMXMapBuilder::createTileLayer (int id, const std::string& n, 
	const QGAMES::Tiles& t, QGAMES::Map* m, QGAMES::TileLayer::VisualOrientation o, 
	const QGAMES::LayerProperties& p)
{
	QGAMES::TileLayer* result = NULL;

	int type = layerType (n); 
	switch (type)
	{
		case __QGAMES_PTTBASETILETYPE__:
			result = new QGAMES::PlatformTileLayer (id, n, t, m, o, p, false);
			break;
		case __QGAMES_PTSOLIDTILETYPE__:
			result = new QGAMES::SolidTileLayer (id, n, t, m, o, p, false);
			break;
		case __QGAMES_PTLADDERTILETYPE__:
			result = new QGAMES::LadderTileLayer (id, n, t, m, o, p, false);
			break;
		case __QGAMES_PTSLIDDINGTILETYPE__:
			result = new QGAMES::SliddingTileLayer (id, n, t, m, o, p, false);
			break;
		case __QGAMES_PTLIQUIDTILETYPE__:
			result = new QGAMES::LiquidTileLayer (id, n, t, m, o, p, false);
			break;
		case __QGAMES_PTKILLINGTILETYPE__:
			result = new QGAMES::KillingTileLayer (id, n, t, m, o, p, false);
			break;
		case __QGAMES_PTDECORATIONTILETYPE__:
			result = new QGAMES::DecorationTileLayer (id, n, t, m, o, p, false);
			break;
		case __QGAMES_PTLOCATIONTILETYPE__:
			result = new QGAMES::LocationTileLayer (id, n, t, m, o, p, false);
			break;
		case __QGAMES_PTNOTIFICATIONTILETYPE__:
			result = new QGAMES::NotificationTileLayer (id, n, t, m, o, p, false);
			break;

		default:
			assert (0); // It shouldn't be here...
	}

	return (result);
}

// ---
QGAMES::ObjectLayer* QGAMES::PlatformTMXMapBuilder::createObjectLayer (int id, const std::string& n, const QGAMES::Objects& o, 
	QGAMES::Map* m, const QGAMES::LayerProperties& oP)
{
	QGAMES::ObjectLayer* result = NULL;

	int type = layerType (n); 
	switch (type)
	{
		case __QGAMES_PTZONEOBJECTLAYERTYPE__:
			result = new QGAMES::ZoneObjectLayer (id, n, o, m, oP);
			break;

		default:
			assert (0); // It shouldn't be here...
	}

	return (result);
}

// ---
QGAMES::ImageLayer* QGAMES::PlatformTMXMapBuilder::createImageLayer (int id, const std::string& n, QGAMES::Image* i, 
	QGAMES::Map* m, const QGAMES::LayerProperties& oP)
{
	assert (0); // It shouldn't be here...
	return (NULL);
}

// ---
QGAMES::Map* QGAMES::PlatformTMXMapBuilder::createMapObject (int id, const QGAMES::Layers& l, int w, int h, int d, 
	int tW, int tH, int tD, const QGAMES::MapProperties& p)
{
	return (new QGAMES::PlatformMap (id, l, w, h, tW, tH, p)); // Always the same type...
}
