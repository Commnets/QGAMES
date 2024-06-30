/** \ingroup DoomCastle */
/*@{*/

/**	
 *	@file	
 *	File: Builders.hpp \n
 *	Game: DoomCastle \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 03/05/2021 \n
 *	Description: Builders needed for DoomCastle \n
 *	Versions: 1.0 Initial
 */

#ifndef __DOOMCASTLE_BUILDERS__
#define __DOOMCASTLE_BUILDERS__

#include <Platform/ptinclude.hpp>

namespace DOOMCASTLE
{
	/** To create the entities of the DoomCastle. */
	class EntityBuilder final : public QGAMES::PlatformEntityBuilder
	{
		public:
		EntityBuilder (const std::string& eDef, QGAMES::FormBuilder* fB, QGAMES::MovementBuilder* mB)
			: QGAMES::PlatformEntityBuilder (eDef, fB, mB)
							{ }

		protected:
		/** @see parent. */
		virtual QGAMES::Entity* createEntity (const QGAMES::EntityBuilder::EntityDefinition& def) override;
		virtual QGAMES::CompositeEntity* createCompositeEntity (const QGAMES::EntityBuilder::EntityDefinition& def) override;

		/** @see parent. */
		virtual QGAMES::SomethingToEat* createSomethingToEat (const QGAMES::EntityBuilder::EntityDefinition& def) override;
		virtual QGAMES::SomethingToCatch* createSomethingToCatch (const QGAMES::EntityBuilder::EntityDefinition& def) override;
		virtual QGAMES::BadGuy* createBadGuy (const QGAMES::EntityBuilder::EntityDefinition& def) override;
	};

	/** To create the movements of the DoomCastle. */
	class MovementBuilder final : public QGAMES::AdvancedMovementBuilder
	{
		public:
		MovementBuilder (const std::string& fDef)
			: QGAMES::AdvancedMovementBuilder (fDef)
								{ }
	
		protected:
		/** @see parent. */
		virtual QGAMES::Movement* createMovement (const QGAMES::MovementBuilder::MovementDefinition& def) override;
	};

	/** The tiles used to referrer locations are now a little bit different. */
	struct ToKnowTileFrameInformation : public QGAMES::ToKnowTileFrameInformation
	{
		public:
		ToKnowTileFrameInformation ();

		private:
		static int _defaultTileFramesRepresentingSomethingToCatch [];
	};

	/** To use the previous. */
	class TMXMapBuilderAddsOn final : public QGAMES::PlatformTMXMapBuilder
	{
		public:
		TMXMapBuilderAddsOn (QGAMES::Sprite2DBuilder* sB)
			: QGAMES::PlatformTMXMapBuilder (sB)
							{ }

		protected:
		/** @see parent. */
		virtual QGAMES::ToKnowTileFrameInformation* createTileFrameInformationObject () override
							{ return (new ToKnowTileFrameInformation ()); }

		private:
	};

	/** To create the worlds and scenes of DoomCastle. */
	class WorldBuilder : public QGAMES::PlatformWorldBuilder
	{
		public:
		WorldBuilder (const std::string& eDef, QGAMES::MapBuilder* mB)
			: QGAMES::PlatformWorldBuilder (eDef, mB)
							{ }

		/** @see parent. */
		virtual QGAMES::World* createWorldObject (int no, const QGAMES::Scenes& s, 
			const QGAMES::WorldProperties& p);
		virtual QGAMES::Scene* createSceneObject (int ns, const QGAMES::Maps& m, 
			const QGAMES::Scene::Connections& cn, 
			const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL);
		virtual QGAMES::SceneActionBlock* createSceneActionBlockObject (int nAB, 
			const QGAMES::SceneActionBlockProperties& prps);
	};
}

#endif

// End of the file
/*@}*/
