#include <PacManLike/pcmaps.hpp>
#include <PacManLike/pcgame.hpp>

// ---
PACMAN::Map::Map (int c, const QGAMES::Layers& l, int w, int h, int d, int tW, int tH, int tD, 
		const QGAMES::MapProperties& p)
	: QGAMES::TiledMap (c, l, w, h, d, tW, tH, tD, p),
	  _maze (PACMAN::Maze::generateEmptyMaze (w, h)), // It will be assigned later, in a more complex formula...
	  _artistsLocationsLayer (nullptr),
	  _mazeLocationsLayer (nullptr),
	  _mazeZonesLayer (nullptr),
	  _directionsLayer (nullptr),
	  _mazeLayer (nullptr),
	  _backgroundLayer (nullptr),
	  _pacmanInitialPositions (), 
	  _monsterInitialPositions (), 
	  _monsterRunAwayPositions (),
	  _monsterExitingHomePosition (QGAMES::MazeModel::_noPosition),
	  _monsterReturningPositionAfterDieing (QGAMES::MazeModel::_noPosition),
	  _thingPositions (),
	  _mazeZones ()
{ 
	for (auto i : layers ())
	{
		assert (dynamic_cast <PACMAN::ArtistsLocationsLayer*> (i) != nullptr || 
				dynamic_cast <PACMAN::MazeLocationsLayer*> (i) != nullptr ||
				dynamic_cast <PACMAN::MazeZonesLayer*> (i) != nullptr ||
				dynamic_cast <PACMAN::DirectionsLayer*> (i) != nullptr || 
				dynamic_cast <PACMAN::MazeLayer*> (i) != nullptr ||
				dynamic_cast <PACMAN::BackgroundLayer*> (i) != nullptr);

		if (dynamic_cast <PACMAN::ArtistsLocationsLayer*> (i) != nullptr)
		{
			assert (_artistsLocationsLayer == nullptr); // Only one...
			_artistsLocationsLayer = dynamic_cast <PACMAN::ArtistsLocationsLayer*> (i);
		}
		else 
		if (dynamic_cast <PACMAN::MazeLocationsLayer*> (i) != nullptr)
		{
			assert (_mazeLocationsLayer == nullptr); // Only one...
			_mazeLocationsLayer = dynamic_cast <PACMAN::MazeLocationsLayer*> (i);
		}
		else 
		if (dynamic_cast <PACMAN::MazeZonesLayer*> (i) != nullptr)
		{
			assert (_mazeZonesLayer == nullptr); // Only one...
			_mazeZonesLayer = dynamic_cast <PACMAN::MazeZonesLayer*> (i);
		}
		else 
		if (dynamic_cast <PACMAN::DirectionsLayer*> (i) != nullptr)
		{
			assert (_directionsLayer == nullptr); // Only one...
			_directionsLayer = dynamic_cast <PACMAN::DirectionsLayer*> (i);
		}
		else 
		if (dynamic_cast <PACMAN::MazeLayer*> (i) != nullptr)
		{
			assert (_mazeLayer == nullptr); // Only one...
			_mazeLayer = dynamic_cast <PACMAN::MazeLayer*> (i);
		}
		else
		if (dynamic_cast <PACMAN::BackgroundLayer*> (i) != nullptr)
		{
			assert (_backgroundLayer == nullptr); // Only one...
			_backgroundLayer = dynamic_cast <PACMAN::BackgroundLayer*> (i);
		}
	}

	// ...and one of each...
	assert (_artistsLocationsLayer != nullptr && 
			_mazeLocationsLayer != nullptr &&
			_mazeZonesLayer != nullptr &&
			_directionsLayer != nullptr && 
			_mazeLayer != nullptr &&
			_backgroundLayer != nullptr); // There must be one of each minimum...

	// Generates the map...
	_maze = PACMAN::Maze::generateMazeFrom (_width, _height, this);
}

// ---
QGAMES::SetOfOpenValues PACMAN::Map::runtimeValues () const
{
	QGAMES::SetOfOpenValues result;

	// Layers have no status to keep in a pacman game...

	result.addOpenValue (0, QGAMES::OpenValue (ballsEatenStatus ()));

	return (result);
}

// ---
void PACMAN::Map::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	assert (cfg.existOpenValue (0));

	setBallsEatenStatus (cfg.openValue (0).strValue ());
}

// ---
const QGAMES::MazeModel::PositionInMaze& PACMAN::Map::pacmanInitialPosition (int nP) const
{
	assert (_artistsLocationsLayer != nullptr);

	std::map <int, QGAMES::MazeModel::PositionInMaze>::const_iterator pP;
	if ((pP = _pacmanInitialPositions.find (nP)) != _pacmanInitialPositions.end ())
		return ((*pP).second);

	return (_pacmanInitialPositions [nP] = mapPositionToMazePosition (_artistsLocationsLayer -> pacmanInitialPosition (nP)));
}

// ---
const QGAMES::MazeModel::PositionInMaze& PACMAN::Map::monsterInitialPosition (int nM) const
{
	assert (_artistsLocationsLayer != nullptr);

	std::map <int, QGAMES::MazeModel::PositionInMaze>::const_iterator mP;
	if ((mP = _monsterInitialPositions.find (nM)) != _monsterInitialPositions.end ())
		return ((*mP).second);

	return (_monsterInitialPositions [nM] = mapPositionToMazePosition (_artistsLocationsLayer -> monsterInitialPosition (nM)));
}

// ---
const QGAMES::MazeModel::PositionInMaze& PACMAN::Map::monsterRunAwayPosition (int nM) const
{
	assert (_artistsLocationsLayer != nullptr);

	std::map <int, QGAMES::MazeModel::PositionInMaze>::const_iterator mP;
	if ((mP = _monsterRunAwayPositions.find (nM)) != _monsterRunAwayPositions.end ())
		return ((*mP).second);

	return (_monsterRunAwayPositions [nM] = mapPositionToMazePosition (_artistsLocationsLayer -> monsterRunAwayPosition (nM)));
}

// ---
const QGAMES::MazeModel::PositionInMaze& PACMAN::Map::monsterExitingHomePosition () const
{
	assert (_mazeLocationsLayer != nullptr);

	if (_monsterExitingHomePosition != QGAMES::MazeModel::_noPosition)
		return (_monsterExitingHomePosition);

	return (_monsterExitingHomePosition = mapPositionToMazePosition (_mazeLocationsLayer -> monsterExitingHomePosition ()));
}

// ---
const QGAMES::MazeModel::PositionInMaze& PACMAN::Map::monsterReturningPositionAfterDieing () const
{
	assert (_mazeLocationsLayer != nullptr);

	if (_monsterReturningPositionAfterDieing != QGAMES::MazeModel::_noPosition)
		return (_monsterReturningPositionAfterDieing);

	return (_monsterReturningPositionAfterDieing = 
		mapPositionToMazePosition (_mazeLocationsLayer -> monsterReturningPositionAfterDieing ()));
}

// ---
const QGAMES::MazeModel::PositionInMaze& PACMAN::Map::oneHomePosition () const
{
	return (maze ().positionsForZone (__PACMAN_MAZEMONSTERSHOMEZONE__).empty () 
		? QGAMES::MazeModel::_noPosition : maze ().positionsForZone (__PACMAN_MAZEMONSTERSHOMEZONE__)[0]);
}

// ---
const QGAMES::MazeModel::PositionInMaze& PACMAN::Map::thingPosition (int nT) const
{
	assert (_artistsLocationsLayer != nullptr);

	std::map <int, QGAMES::MazeModel::PositionInMaze>::const_iterator tP;
	if ((tP = _thingPositions.find (nT)) != _thingPositions.end ())
		return ((*tP).second);

	return (_thingPositions [nT] = mapPositionToMazePosition (_artistsLocationsLayer -> thingPosition (nT)));
}

// ---
QGAMES::MazeModel::PositionInMaze PACMAN::Map::randomMazePosition (const std::vector <int>& fZ) const
{
	return (maze ().randomPosition (fZ));
}

// ---
void PACMAN::Map::startBlinking (QGAMES::bdata bT, int nB)
{ 
	_mazeLayer -> startBlinkingMaze (bT, nB); 
}

// ---
bool PACMAN::Map::isBlinking () const
{ 
	return (_mazeLayer -> isBlinkingMaze ()); 
}

// ---
void PACMAN::Map::stopBlinking ()
{ 
	_mazeLayer -> stopBlinkingMaze (); 
}

// ---
int PACMAN::Map::maxNumberBallsToEat () const
{ 
	return (_mazeLayer -> maxNumberBallsToEat ()); 
}

// ---
int PACMAN::Map::numberBallsEaten () const
{ 
	return (_mazeLayer -> numberBallsEaten ()); 
}

// ---
std::string PACMAN::Map::ballsEatenStatus () const
{
	return (_mazeLayer -> ballsEatenStatus ()); 
}

// ---
void PACMAN::Map::setBallsEatenStatus (const std::string& st)
{
	_mazeLayer -> setBallsEatenStatus (st);
}

// ---
void PACMAN::Map::initialize ()
{
	QGAMES::TiledMap::initialize ();

	_mazeLayer -> setBallsEatenStatus (std::string (__NULL_STRING__)); // All points by default...
}

// ---
const std::map <int, QGAMES::Position>& PACMAN::ArtistsLocationsLayer::pacmanInitialPositions () const
{
	if (!_pacmanInitialPositions.empty ())
		return (_pacmanInitialPositions);

	const PACMAN::Game* g = dynamic_cast <const PACMAN::Game*> (game ());
	assert (g != nullptr);
	const PACMAN::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
	assert (mB != nullptr);

	if (mB -> pacmanHomeFrames ().empty ())
		return (_pacmanInitialPositions);

	_pacmanInitialPositions = { };
	for (auto i : tiles ())
	{
		if (dynamic_cast <QGAMES::NullTile*> (i) == nullptr)
		{
			std::vector <int>::const_iterator j = 
				std::find (mB -> pacmanHomeFrames ().begin (), mB -> pacmanHomeFrames ().end (), i -> numberFrame ());
			if (j != mB -> pacmanHomeFrames ().end ())
				_pacmanInitialPositions [((*j) - mB -> pacmanHomeFrames () [0])] = tilePosition (i);
		}
	}

	return (_pacmanInitialPositions);
}

// ---
const QGAMES::Position& PACMAN::ArtistsLocationsLayer::pacmanInitialPosition (int nP) const
{ 
	std::map <int, QGAMES::Position>::const_iterator i = pacmanInitialPositions ().find (nP);
	return ((i != pacmanInitialPositions ().end ()) ? (*i).second : QGAMES::Position::_noPoint);
}

// ---
const std::map <int, QGAMES::Position>& PACMAN::ArtistsLocationsLayer::monsterInitialPositions () const
{
	if (!_monsterInitialPositions.empty ())
		return (_monsterInitialPositions);

	const PACMAN::Game* g = dynamic_cast <const PACMAN::Game*> (game ());
	assert (g != nullptr);
	const PACMAN::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
	assert (mB != nullptr);

	if (mB -> monsterHomeFrames ().empty ())
		return (_monsterInitialPositions);

	_monsterInitialPositions = { };
	for (auto i : tiles ())
	{
		if (dynamic_cast <QGAMES::NullTile*> (i) == nullptr)
		{
			std::vector <int>::const_iterator j = 
				std::find (mB -> monsterHomeFrames ().begin (), mB -> monsterHomeFrames ().end (), i -> numberFrame ());
			if (j != mB -> monsterHomeFrames ().end ())
				_monsterInitialPositions [((*j) - mB -> monsterHomeFrames () [0])] = tilePosition (i);
		}
	}

	return (_monsterInitialPositions);
}

// ---
const QGAMES::Position& PACMAN::ArtistsLocationsLayer::monsterInitialPosition (int nM) const
{ 
	std::map <int, QGAMES::Position>::const_iterator i = monsterInitialPositions ().find (nM);
	return ((i != monsterInitialPositions ().end ()) ? (*i).second : QGAMES::Position::_noPoint);
}

// ---
const std::map <int, QGAMES::Position>& PACMAN::ArtistsLocationsLayer::monsterRunAwayPositions () const
{
	if (!_monsterRunAwayPositions.empty ())
		return (_monsterRunAwayPositions);

	const PACMAN::Game* g = dynamic_cast <const PACMAN::Game*> (game ());
	assert (g != nullptr);
	const PACMAN::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
	assert (mB != nullptr);

	if (mB -> monsterRunAwayFrames ().empty ())
		return (_monsterRunAwayPositions);

	_monsterRunAwayPositions = { };
	for (auto i : tiles ())
	{
		if (dynamic_cast <QGAMES::NullTile*> (i) == nullptr)
		{
			std::vector <int>::const_iterator j = 
				std::find (mB -> monsterRunAwayFrames ().begin (), mB -> monsterRunAwayFrames ().end (), i -> numberFrame ());
			if (j != mB -> monsterRunAwayFrames ().end ())
				_monsterRunAwayPositions [((*j) - mB -> monsterRunAwayFrames () [0])] = tilePosition (i);
		}
	}

	return (_monsterRunAwayPositions);
}

// ---
const QGAMES::Position& PACMAN::ArtistsLocationsLayer::monsterRunAwayPosition (int nM) const
{ 
	std::map <int, QGAMES::Position>::const_iterator i = monsterRunAwayPositions ().find (nM);
	return ((i != monsterRunAwayPositions ().end ()) ? (*i).second : QGAMES::Position::_noPoint);
}

// ---
const std::map <int, QGAMES::Position>& PACMAN::ArtistsLocationsLayer::thingPositions () const
{
	if (!_thingPositions.empty ())
		return (_thingPositions);

	const PACMAN::Game* g = dynamic_cast <const PACMAN::Game*> (game ());
	assert (g != nullptr);
	const PACMAN::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
	assert (mB != nullptr);

	if (mB -> thingPositionFrames ().empty ())
		return (_thingPositions);

	_thingPositions = { };
	for (auto i : tiles ())
	{
		if (dynamic_cast <QGAMES::NullTile*> (i) == nullptr)
		{
			std::vector <int>::const_iterator j = 
				std::find (mB -> thingPositionFrames ().begin (), mB -> thingPositionFrames ().end (), i -> numberFrame ());
			if (j != mB -> thingPositionFrames ().end ())
				_thingPositions [((*j) - mB -> thingPositionFrames () [0])] = tilePosition (i);
		}
	}

	return (_thingPositions);
}

// ---
const QGAMES::Position& PACMAN::ArtistsLocationsLayer::thingPosition (int nT) const
{
	std::map <int, QGAMES::Position>::const_iterator i = thingPositions ().find (nT);
	return ((i != thingPositions ().end ()) ? (*i).second : QGAMES::Position::_noPoint);
}

// ---
const QGAMES::Position& PACMAN::MazeLocationsLayer::monsterExitingHomePosition () const
{
	if (_monsterExitingHomePosition != QGAMES::Position::_noPoint)
		return (_monsterExitingHomePosition);

	const PACMAN::Game* g = dynamic_cast <const PACMAN::Game*> (game ());
	assert (g != nullptr);
	const PACMAN::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
	assert (mB != nullptr);

	for (QGAMES::Tiles::const_iterator i = tiles ().begin (); 
		i != tiles ().end () && _monsterExitingHomePosition == QGAMES::Position::_noPoint; i++)
		if ((*i) -> numberFrame () == mB -> monsterExitingHomeFrame ())
			_monsterExitingHomePosition = tilePosition ((*i));

	return (_monsterExitingHomePosition);
}

// ---
const QGAMES::Position& PACMAN::MazeLocationsLayer::monsterReturningPositionAfterDieing () const
{
	if (_monsterReturningPositionAfterDieing != QGAMES::Position::_noPoint)
		return (_monsterReturningPositionAfterDieing);

	const PACMAN::Game* g = dynamic_cast <const PACMAN::Game*> (game ());
	assert (g != nullptr);
	const PACMAN::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
	assert (mB != nullptr);

	for (QGAMES::Tiles::const_iterator i = tiles ().begin (); 
		i != tiles ().end () && _monsterReturningPositionAfterDieing == QGAMES::Position::_noPoint; i++)
		if ((*i) -> numberFrame () == mB -> monsterToComeBackFrame ())
			_monsterReturningPositionAfterDieing = tilePosition ((*i));

	return (_monsterReturningPositionAfterDieing);
}

// ---
PACMAN::MazeLayer::MazeLayer (int c, const std::string& n, const QGAMES::Tiles& t, QGAMES::Map* m, const QGAMES::LayerProperties& p)
	: QGAMES::TileLayer (c, n, t, m, QGAMES::TileLayer::_ORTHOGONAL, p, false)
{
#ifndef NDEBUG
	for (auto i : tiles ())
		assert (dynamic_cast <QGAMES::NullTile*> (i) != nullptr ||
				dynamic_cast <PACMAN::TileLimit*> (i) != nullptr ||
				dynamic_cast <PACMAN::TilePath*> (i) != nullptr);
#endif

	// Only the tiles type path are observed if they can be eaten...
	for (auto i : tiles ())
	{
		PACMAN::TilePath* tP = dynamic_cast <PACMAN::TilePath*> (i);
		if (tP != nullptr && tP -> canBeEaten ())
			observe (i); // The observation stops when the layer is destroyed, if any...
	}
}

// ---
void PACMAN::MazeLayer::BlinkingMaze::initialize ()
{
	_counterBlinkingTime.initialize ();
	_counterNumberBlinks.initialize ();
	_switchBlinkOn.initialize ();

	_active = true;

	// All must bright...
	for (auto i : (dynamic_cast <PACMAN::MazeLayer*> (_layer)) -> tiles ())
		setBright (i, true);

	_switchBlinkOn.set (true);
}

// ---
void PACMAN::MazeLayer::BlinkingMaze::updatePositions ()
{
	if (!_active)
		return;

	if (_counterBlinkingTime.isEnd ())
	{
		_switchBlinkOn.set (!_switchBlinkOn.isOn ());

		bool br = _switchBlinkOn.isOn ();
		for (auto i : (dynamic_cast <TileLayer*> (_layer)) -> tiles ())
			setBright (i, br);

		if (!_switchBlinkOn.isOn () && _counterNumberBlinks.isEnd ())
			_active = false; // No more blinks...
	}
}

// ---
void PACMAN::MazeLayer::BlinkingMaze::finalize ()
{
	_switchBlinkOn.set (false);

	for (auto i : (dynamic_cast <TileLayer*> (_layer)) -> tiles ())
		setBright (i, false);

	_active = false; // No more blinks...
}

// ---
void PACMAN::MazeLayer::BlinkingMazeLimit::setBright (QGAMES::Tile* t, bool b)
{
	PACMAN::TileLimit* tL = nullptr;
	if ((tL = dynamic_cast <PACMAN::TileLimit*> (t)) != nullptr)
		tL -> setBright (b);
}

// ---
void PACMAN::MazeLayer::BlinkingPowerBalls::setBright (QGAMES::Tile* t, bool b)
{
	PACMAN::TilePath* tL = dynamic_cast <PACMAN::TilePath*> (t);
	if (tL != nullptr && tL -> hasPower ())
	{
		// TODO
	}
}

// ---
void PACMAN::MazeLayer::startBlinkingMaze (QGAMES::bdata bT, int nB)
{
	setVisualEffect (new PACMAN::MazeLayer::BlinkingMazeLimit (this, bT, nB));
}

// ---
bool PACMAN::MazeLayer::isBlinkingMaze () const
{
	return ((dynamic_cast <const PACMAN::MazeLayer::BlinkingMazeLimit*> (visualEffect ()) != nullptr) 
		? (static_cast <const PACMAN::MazeLayer::BlinkingMazeLimit*> (visualEffect ())) -> isActive () : false);  
}

// ---
void PACMAN::MazeLayer::stopBlinkingMaze ()
{
	setVisualEffect (nullptr);
}

// ---
int PACMAN::MazeLayer::maxNumberBallsToEat () const
{
	int result = 0;
	for (auto i : tiles ())
	{
		const PACMAN::TilePath* tl = dynamic_cast <const PACMAN::TilePath*> (i); 
		if (tl != nullptr && tl -> canBeEaten ())
			result++;
	}

	return (result);
}

// ---
int PACMAN::MazeLayer::numberBallsEaten () const
{
	int result = 0;
	for (auto i : tiles ())
	{
		const PACMAN::TilePath* tl = dynamic_cast <const PACMAN::TilePath*> (i); 
		if (tl != nullptr && tl -> alreadyEaten ())
			result++;
	}

	return (result);
}

// ---
std::string PACMAN::MazeLayer::ballsEatenStatus () const
{
	std::string result (__NULL_STRING__);

	for (auto i : tiles ())
	{
		const PACMAN::TilePath* tl = dynamic_cast <const PACMAN::TilePath*> (i);
		result += (tl == nullptr) 
			? std::string ("_") 
			: std::to_string (tl -> canBeEaten () ? (tl -> alreadyEaten () ? 0 : 1) : 2);
	}

	return (result);
}

// ---
void PACMAN::MazeLayer::setBallsEatenStatus (const std::string& st)
{
	if (st == std::string (__NULL_STRING__))
	{
		for (auto i : tiles ())
		{
			PACMAN::TilePath* tP = dynamic_cast <PACMAN::TilePath*> (i);
			if (tP != nullptr)
				tP -> eaten (false);
		}
	}
	else
	{
		int j = 0;
		for (auto i : tiles ())
		{
			if (st [j] == '_')
				// Just to be sure that the status requiered represents the map...
				assert (dynamic_cast <PACMAN::TilePath*> (i) == nullptr);
			else
			{
				PACMAN::TilePath* tP = dynamic_cast <PACMAN::TilePath*> (i);
				assert (tP != nullptr);
				if (tP -> canBeEaten ())
				{
					assert (st [j] != '2');
					tP -> eaten (st [j] == '1' ? false : true, false); // Do not notify...
				}
			}

			j++;
		}
	}
}

// ---
void PACMAN::MazeLayer::initialize ()
{
	QGAMES::TileLayer::initialize ();

	// The power balls have to blink from the eaarly beginning...
	setVisualEffect (new PACMAN::MazeLayer::BlinkingPowerBalls (this, __BD 0.15, -1));
}

// ---
void PACMAN::MazeLayer::finalize ()
{
	QGAMES::TileLayer::finalize ();

	setVisualEffect (nullptr); // Just in case...
}

// ---
void PACMAN::TileLimit::setBright (bool b)
{
	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr); // It can only be made within a PACMAN game, that it should be the case...
	PACMAN::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
	assert (mB != nullptr); // Just in case...
	// Reley in the builder to determine the equivalent frame...

	setForm (form (), 
		(b) ? mB -> brightFrameFor (numberFrame ()) : mB -> darkFrameFor (numberFrame ()));
}

// ---
bool PACMAN::TilePath::eaten (bool s, bool n)
{ 
	bool result = false;

	if (canBeEaten ())
	{
		PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
		assert (g != nullptr); // It can only be made within a PACMAN game, that it should be the case...
		PACMAN::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
		assert (mB != nullptr); // Just in case...

		if (s)
		{
			if (_type != PACMAN::TilePath::Type::_EMPTY)
			{
				_type = PACMAN::TilePath::Type::_EMPTY; // No more

				setForm (form (), mB -> frameForEmptyPath ());

				if (n)
					notify (QGAMES::Event (__PACMAN_BALLEATEN__, this));
			}
		}
		else
		{
			_type = _originalType;

			setForm (form (), _originalFrame);
		}

		result = true;
	}

	return (result);
}
