#include "Stage5Scenes.hpp"
#include "Artists.hpp"

// ---
void BattleshipII::Stage5Scene::initialize ()
{
	setBackgroundMap (__BATTLESHIP_TRANSPARENTMAPID__); 

	setMap (__BATTLESHIP_COMPLEXSTARTSMOVINGBACKGROUNDMAP5ID__);

	BATTLESHIP::TimeLimitScene::initialize ();
}

// ---
void BattleshipII::Stage5Scene3::initialize ()
{
	setBackgroundMap (__BATTLESHIP_TRANSPARENTMAPID__); 

	setMap (__BATTLESHIP_COMPLEXSTARTSMOVINGBACKGROUNDMAP5ID__);

	BATTLESHIP::FocusingInOneElementScene::initialize ();

	_flyWithLife = NULL;

	_fliesWithShield = BattleshipII::Stage5Scene3::UFOSWithShield ();
	_numberBlocksShieldAdded = 0;

	// Get the blocks moving UFOS...
	_blocksWithUFOs = std::vector <BATTLESHIP::StdUFOSceneActionBlock*> ();
	for (auto i : actionBlocks ())
	{
		if (dynamic_cast <BATTLESHIP::StdSetUFOsSceneActionBlock*> (i) != nullptr)
		{
			for (auto j : ((BATTLESHIP::StdSetUFOsSceneActionBlock*) (i)) -> actionBlocks ())
				_blocksWithUFOs.push_back (dynamic_cast <BATTLESHIP::StdUFOSceneActionBlock*> (j));
		}
	}

	assert (std::count_if (_blocksWithUFOs.begin (), _blocksWithUFOs.end (), 
		[](BATTLESHIP::StdUFOSceneActionBlock* i) { return (i == nullptr); }) == 0); // No nulls allowed... 
}

// ---
void BattleshipII::Stage5Scene3::finalize ()
{
	BATTLESHIP::FocusingInOneElementScene::initialize ();

	if (existsActionBlock (_LIFEBLOCKNUMBER))
		removeActionBlock (_LIFEBLOCKNUMBER);

	for (int i = 0; i < _numberBlocksShieldAdded; i++)
		if (existsActionBlock (_SHIEDBASEBLOCKNUMBER + i))
			removeActionBlock (_SHIEDBASEBLOCKNUMBER + i);
}

// ---
void BattleshipII::Stage5Scene3::processEvent (const QGAMES::Event& evnt)
{
	BATTLESHIP::FocusingInOneElementScene::processEvent (evnt);

	// When an element appears in the scene...
	if (evnt.code () == __BATTLESHIP_SPACEELEMENTACTIONBLOCKELMNTMEMERGED__)
	{
		BATTLESHIP::StdUFOSceneActionBlock* ufoblk = 
			static_cast <BATTLESHIP::StdUFOSceneActionBlock*> (evnt.data ());
		assert (!ufoblk -> spaceElementsControlled ().empty ()); // Just in case! (it shouln't but who knows!)

		// The element to follow hasn't been assigned yet!
		if (focusedElement () == nullptr &&
			dynamic_cast <BattleshipII::MothershipUFO*> (ufoblk -> spaceElementsControlled ()[0]) != nullptr) 
			setFocusedElement ((_blockWithElementFocused = ufoblk) -> spaceElementsControlled ()[0]);

		// The ufo with the life hasn't been assigned yet, and the block isn't the one with the mothership!
		if (_flyWithLife == nullptr &&
			dynamic_cast <BattleshipII::MothershipUFO*> (ufoblk -> spaceElementsControlled ()[0]) == nullptr &&
			__1IN4PROBABLE__)
			_flyWithLife = dynamic_cast <BATTLESHIP::UFO*> (ufoblk -> spaceElementsControlled ()[0]);

		// The ufos with the shield haven't been assigned yet, and the block isn't the one with the mothership!
		if ((int) _fliesWithShield.size () <= 3 &&
			dynamic_cast <BattleshipII::MothershipUFO*> (ufoblk -> spaceElementsControlled ()[0]) == nullptr &&
			__1IN4PROBABLE__)
			_fliesWithShield.push_back (dynamic_cast <BATTLESHIP::UFO*> (ufoblk -> spaceElementsControlled ()[0]));
	}

	else
	// Now the case is when one element has been destroyed
	if (evnt.code () == __BATTLESHIP_SPACEELEMENTDESTROYED__ && // An element has been destroyed...
		evnt.data () != nullptr) // ..and it is not null (it shouldn't but who knows!!)
	{
		// The one destroyed is the one with the life!
		if (evnt.data () == _flyWithLife)
		{
			addActionBlock (new BATTLESHIP::StdLiveSceneActionBlock (_LIFEBLOCKNUMBER, 
				new BATTLESHIP::StdLiveSceneActionBlock::Properties 
					(__BD 0,								// Appears inmediatly
					 100,									// Always appear
					 false,									// No emerge
					 __BD 0,								// Moves inmediatly
					 false,									// Never regenerate 
					 false,									// Never rolls
					 static_cast <BATTLESHIP::UFO*> (evnt.data ()) -> 
						relativeCenterPositionInMovingZone (), // from the position of the killed ufo
					 static_cast <BATTLESHIP::UFO*> (evnt.data ()) -> 
						getMovingZone (),					// In that specific moving zone
					 __BD -1,								// Never finishes 
					 BATTLESHIP::LiveToCatch::_LIVETOCATCH, // It is a life 
					 0,										// Moves normaly. Not used! 
					 __BD 1)));								// Moves a little bit quicker...
		}

		else
		// The one destroyed is the one with a shield!
		if (std::find (_fliesWithShield.begin (), _fliesWithShield.end (), evnt.data ()) != _fliesWithShield.end ())
		{
			addActionBlock (new BATTLESHIP::StdShieldSceneActionBlock (_SHIEDBASEBLOCKNUMBER + (_numberBlocksShieldAdded++), 
				new BATTLESHIP::StdShieldSceneActionBlock::Properties 
					(__BD 0,								// Appears inmediatly
					 100,									// Always appear
					 false,									// No emerge
					 __BD 0,								// Moves inmediatly
					 false,									// Never regenerate 
					 false,									// Never rolls
					 static_cast <BATTLESHIP::UFO*> (evnt.data ()) -> 
						relativeCenterPositionInMovingZone (), // from the position of the killed ufo
					 static_cast <BATTLESHIP::UFO*> (evnt.data ()) -> 
						getMovingZone (),					// In that specific moving zone
					 __BD -1,								// Never finishes 
					 BATTLESHIP::ShieldToCatch::_SHIELDTOCATCH, // It is a shield
					 0,										// Moves normaly. Not used! 
					 __BD 1)));								// Moves a little bit quicker...
		}

	}
}
