#include "Builders.hpp"
#include "Pacman.hpp"
#include "Monsters.hpp"
#include "Shield.hpp"
#include "Scenes.hpp"
#include "SceneActionBlocks.hpp"
#include "Defs.hpp"

// ---
QGAMES::Movement* PacManII::MovementBuilder::createMovement 
	(const QGAMES::MovementBuilder::MovementDefinition& def)
{
	QGAMES::Movement* result = nullptr;

	// (Wormy) Monsters' chasing movements
	if (def._id >= __PACMANII_WORMYMAZEMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_WORMYMAZEMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__)) 
		result = new PACMAN::MazeMovement (def._id, def._variables);
	else
	// (Wormy) Monsters' at-home movements
	if (def._id >= __PACMANII_WORMYATHOMEMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_WORMYATHOMEMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	// Shield's staying movement
	if (def._id >= __PACMANII_SHIELDSTAYINGMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_SHIELDSTAYINGMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	// For th rest of the movements the system relies in the standard framework...
	else
		result = PACMAN::MovementBuilder::createMovement (def);

	return (result);
}

// ---
QGAMES::Entity* PacManII::EntityBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	QGAMES::Entity* result = nullptr;

	// The pacman used in this instance is a little but different
	// It can be shield a little bit!
	if (def._id >= __PACMAN_PACMANBASEENTITYID__ && 
		def._id < (__PACMAN_PACMANBASEENTITYID__ + __PACMAN_NUMBERENTITIES__)) 
			result = new PacManII::PacMan (def._id);
	else 
	// To create the new pacman monster for this version
	if (def._id >= __PACMANII_WORMYBASEENTITYID__ && 
		def._id < (__PACMANII_WORMYBASEENTITYID__ + __PACMAN_NUMBERENTITIES__)) 
			result = new PacManII::Wormy (def._id);
	else
	// To create th shields
	if (def._id >= __PACMANII_SHIELDBASEENTITYID__ &&
		def._id < (__PACMANII_SHIELDBASEENTITYID__ + __PACMAN_NUMBERENTITIES__))
			result = new PacManII::Shield (def._id);
	// For any other entity the default implementation can be usd...
	else 
		result = PACMAN::EntityBuilder::createEntity (def);

	return (result);
}

// ---
QGAMES::Scene* PacManII::WorldBuilder::createSceneObject (int ns, const QGAMES::Maps& m, 
	const QGAMES::Scene::Connections& cn, const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
{
	QGAMES::Scene* result = nullptr;

	if (ns == __PACMANII_BLOCKSCENES1__ ||
		ns == __PACMANII_BLOCKSCENES3MS1__ ||
		ns == __PACMANII_BLOCKSCENES3MS2__ ||
		ns == __PACMANII_BLOCKSCENES3MS3__ ||
		ns == __PACMANII_BLOCKSCENES3MS4__)
		result = new PacManII::BasicSceneWithAppearingMonsters (ns, m, cn, p, ePL); // With wormy...
	else
	if (ns == __PACMANII_BLOCKSCENES2MS1__ ||
		ns == __PACMANII_BLOCKSCENES2MS2__ ||
		ns == __PACMANII_BLOCKSCENES2MS3__ ||
		ns == __PACMANII_BLOCKSCENES2MS4__)
		result = new PACMAN::BasicScene (ns, m, cn, p, ePL); // Without wormy
	else
		result = PACMAN::WorldBuilder::createSceneObject (ns, m, cn, p, ePL); // The classical ones...

	return (result);
}

// ---
QGAMES::SceneActionBlock* PacManII::WorldBuilder::createSceneActionBlockObject (int nAB, 
	const QGAMES::SceneActionBlockProperties& prps)
{
	QGAMES::SceneActionBlock* result = nullptr;

	if (nAB >= __PACMANII_MONSTERAPPRANDMOVESCENEBLOCKBASEID__ && 
		nAB < (__PACMANII_MONSTERAPPRANDMOVESCENEBLOCKBASEID__ + __PACMANII_MONSTERAPPRANDMOVESCENEBLOCKNUMBER__))
		result = new PacManII::ElementToAppearSceneActionBlock
			(nAB, PacManII::ElementToAppearSceneActionBlock::Properties (prps));
	else
	if (nAB >= __PACMANII_WORMYSCENEBLOCKBASEID__ && 
		nAB < (__PACMANII_WORMYSCENEBLOCKBASEID__ + __PACMANII_WORMYSCENEBLOCKNUMBER__))
		result = new PacManII::ElementToAppearSceneActionBlock
			(nAB, PacManII::ElementToAppearSceneActionBlock::Properties (prps));
	else
		result = PACMAN::WorldBuilder::createSceneActionBlockObject (nAB, prps);

	return (result);
}
