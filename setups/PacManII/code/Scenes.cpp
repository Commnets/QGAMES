#include "Scenes.hpp"
#include "SceneActionBlocks.hpp"
#include "Monsters.hpp"
#include "Game.hpp"
#include "DataGame.hpp"
#include "Defs.hpp"

// ---
void PacManII::BasicSceneWithAppearingMonsters::initialize ()
{
	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr);
	const PacManII::DataGame::LevelDefinition* nLDef = 
		dynamic_cast <const PacManII::DataGame::LevelDefinition*> (&g -> levelDefinition (g -> level ())); 
	
	// This scene is only valid when the level includes the possibility of having addicional monsters,
	// and they have been defined either...
	if (nLDef == nullptr || (nLDef != nullptr && nLDef -> additionalMonsters ().empty ()))
		return;

	_additionalMonstersActionBlocks = { };

	// These block added ar transient
	// They will be removed at finalize method...
	for (int i = 0; 
		 i < (int) nLDef -> additionalMonsters ().size (); i++)
	{
		PacManII::DataGame::LevelDefinition::AdditionalMonster aM = nLDef -> additionalMonsters ()[i];

		PacManII::ElementToAppearSceneActionBlock* mABlkCtrl = 
			new PacManII::ElementToAppearSceneActionBlock (100 + (i << 1), 
				PacManII::ElementToAppearSceneActionBlock::Properties
					(aM.monsterId (), QGAMES::MazeModel::_noPosition, 
						aM.numberBallsEatenToAppear (), aM.maxSecondsToAppear (), 101 + (i << 1)));
		PacManII::WormyMonsterSceneActionBlock* mABlk = 
			new PacManII::WormyMonsterSceneActionBlock (101 + (i << 1), 
				PacManII::WormyMonsterSceneActionBlock::Properties
					(PACMAN::MonsterSceneActionBlock::Properties
						(false, aM.monsterId (), aM.monsterNumber (), aM.points ()), aM.trailLength ()));

		// First later in being finalized...
		_additionalMonstersActionBlocks.push_back (mABlk); 
		addActionBlock (mABlk, false); // Not initialized now...
		_additionalMonstersActionBlocks.push_back (mABlkCtrl); 
		// When this is finalized the element under control will be already removed from the scene 
		// This is taken into account in the control object...
		addActionBlock (mABlkCtrl, false); // Not initialized now...
	}

	PACMAN::BasicScene::initialize ();
}

// ---
void PacManII::BasicSceneWithAppearingMonsters::finalize ()
{
	for (auto i : _additionalMonstersActionBlocks)
		removeActionBlock (i);

	_additionalMonstersActionBlocks = { };

	PACMAN::BasicScene::finalize ();
}
