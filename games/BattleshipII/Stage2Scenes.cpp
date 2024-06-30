#include "Stage2Scenes.hpp"

// ---
void BattleshipII::Stage2Scene::initialize ()
{
	setBackgroundMap (__BATTLESHIP_TRANSPARENTMAPID__); 

	setMap (__BATTLESHIP_COMPLEXSTARTSMOVINGBACKGROUNDMAP2ID__);

	BATTLESHIP::SequentialActionBlocksScene::initialize ();
}

// ---
void BattleshipII::Stage2Scene3::initialize ()
{
	BattleshipII::Stage2Scene::initialize ();

	_flyWithLife = NULL;
}

// ---
void BattleshipII::Stage2Scene3::finalize ()
{
	BattleshipII::Stage2Scene::finalize ();

	if (existsActionBlock (_LIFEBLOCKNUMBER))
		removeActionBlock (_LIFEBLOCKNUMBER);
}

// ---
void BattleshipII::Stage2Scene3::processEvent (const QGAMES::Event& evnt)
{
	BattleshipII::Stage2Scene::processEvent (evnt);

	if (evnt.code () == __BATTLESHIP_SPACEELEMENTACTIONBLOCKELMNTMEMERGED__ && // A new element appears.. 
		_flyWithLife == NULL && // ...and no boby is still being follewed
		__1IN4PROBABLE__) // ..and 25% probable!
	{
		BATTLESHIP::StdUFOSceneActionBlock* aB = 
			static_cast <BATTLESHIP::StdUFOSceneActionBlock*> (evnt.data ());
		_flyWithLife = dynamic_cast <BATTLESHIP::UFO*> (aB -> spaceElementsControlled ()[0]);
		assert (_flyWithLife); // Just in case...
	}
	else
	if (evnt.code () == __BATTLESHIP_SPACEELEMENTDESTROYED__ && // An element has been destroyed...
		evnt.data () != NULL && // ..and it is not null (it shouldn't but who knows!!)
		evnt.data () == _flyWithLife) // ...and that element match the one being followed.
	{
		BATTLESHIP::UFO* ufo = static_cast <BATTLESHIP::UFO*> (evnt.data ());
		assert (ufo);

		addActionBlock (new BATTLESHIP::StdLiveSceneActionBlock (_LIFEBLOCKNUMBER, 
			new BATTLESHIP::StdLiveSceneActionBlock::Properties 
				(__BD 0,								// Appears inmediatly
				 100,									// Always appear
				 false,									// No emerge
				 __BD 0,								// Moves inmediatly
				 false,									// Never regenerate 
				 false,									// Never rolls
				 ufo -> relativeCenterPositionInMovingZone (), // from the position of the killed ufo
				 ufo -> getMovingZone (),				// In that specific moving zone
				 __BD -1,								// Never finishes 
				 BATTLESHIP::LiveToCatch::_LIVETOCATCH, // It is a life 
				 0,										// Moves normaly. Not used! 
				 __BD 1)));								// Moves a little bit quicker...
	}
}
