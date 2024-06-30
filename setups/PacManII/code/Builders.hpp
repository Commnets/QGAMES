/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Builders.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 22/01/2022 \n
 *	Description: The specific builders for this pacman game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_BUILDERS__
#define __PACMANII_BUILDERS__

#include <PacManLike/pcinclude.hpp>

namespace PacManII
{
	class MovementBuilder final : public PACMAN::MovementBuilder
	{
		public:
		MovementBuilder () = delete;

		MovementBuilder (const std::string& fDef)
			: PACMAN::MovementBuilder (fDef) 
								{ }

		MovementBuilder (const MovementBuilder&) = delete;

		MovementBuilder& operator = (const MovementBuilder&) = delete;

		protected:
		virtual QGAMES::Movement* createMovement 
			(const QGAMES::MovementBuilder::MovementDefinition& def) override;
	};

	class EntityBuilder final : public PACMAN::EntityBuilder
	{
		public:
		EntityBuilder () = delete;

		EntityBuilder (const std::string& eDef, QGAMES::FormBuilder* fB, QGAMES::MovementBuilder* mB)
			: PACMAN::EntityBuilder (eDef, fB, mB)
								{ }

		EntityBuilder (const EntityBuilder&) = delete;

		EntityBuilder& operator = (const EntityBuilder&) = delete;

		protected:
		virtual QGAMES::Entity* createEntity (const QGAMES::EntityBuilder::EntityDefinition& def) override;
	};

	class WorldBuilder final : public PACMAN::WorldBuilder
	{
		public: 
		WorldBuilder () = delete;
			
		WorldBuilder (const std::string& eDef, QGAMES::MapBuilder* mB)
			: PACMAN::WorldBuilder (eDef, mB)
							{ }

		WorldBuilder (const WorldBuilder&) = delete;

		WorldBuilder& operator = (const WorldBuilder&) = delete;

		protected:
		virtual QGAMES::Scene* createSceneObject (int ns, const QGAMES::Maps& m, 
			const QGAMES::Scene::Connections& cn, 
			const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL) override;
		virtual QGAMES::SceneActionBlock* createSceneActionBlockObject (int nAB, 
			const QGAMES::SceneActionBlockProperties& prps) override;
	};
}

#endif
  
// End of the file
/*@}*/