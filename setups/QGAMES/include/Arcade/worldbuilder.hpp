/** \ingroup ArcadeGame */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: worldbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class to build up worlds. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_WORLDBUILDER__
#define __QGAMES_WORLDBUILDER__

#include <Common/elementbuilder.hpp>
#include <Arcade/world.hpp>
#include <Arcade/mapbuilder.hpp>
#include <tinyxml.h>
#include <map>
#include <vector>
#include <string>

namespace QGAMES
{
	class WorldBuilder : public ElementBuilder
	{
		public:
		/** 
		  *	A world definition file should has the following structure: \n
		  *	The world is made up of several scenes, and attributes. \n
		  *	Each scene is made up of attributes, and action blocks, and it is reperesented by one o many maps,
		  *	and it is connected with other scenes thought conexions. \n
		  *	The maps can have many different forms. \n
		  *	A map builder is needed as parameter to create a world builder. \n
		  *	<?xml version="1.0"?> \n
		  *	<Worlds> \n
		  *		<World id="X"> \n
		  *			<Scenes> \n
		  *				<Scene id="XX"> \n 
		  *					<Actions> \n
		  *						<Action id="X" active="YES|NO""> \n
		  *							<Attributes> \n
		  *								<Attribute id="X" value="XX"/> \n
		  *								... \n
		  *							</Attributes> \n
		  *							... \n
		  *						<Action> \n
		  *						... \n
		  *					</Actions> \n
		  *					<Attributes> \n
		  *						<Attribute id="X" value="XX"/> \n
		  *						... \n
		  *					</Attributes> \n
		  *					<Conexions> \n
		  *						<Conexion id="X" with="XX" at="XX" (optional, id of the point)/> \n
		  *						... \n
		  *					</Conexions> \n
		  *					<Maps> \n
		  *						<Map id="X" file="XX"/> \n 
		  *						... \n
		  *					</Maps> \n
		  *					<Entities> \n
		  *						<Entity id="X" layer="XX"/> \n
		  *						... \n
		  *					</Entities> \n
		  *				</Scene> \n
		  *			</Scenes> \n
		  *			<SetOfScenes baseId="XX" number="YYY>
		  *			... // The maps will be the base...
		  *			</SetOfScenes>
		  *			<Attributes> \n
		  *				<Attribute id="X" value="X"/> \n
		  *				... \n
		  *			</Attributes> \n
		  *			... \n
		  *		</World> \n
		  *	</Worlds> \n
		  */
		WorldBuilder (const std::string& eDef, MapBuilder* mB);
		/** The builder owns any map got from it. \n
			When the builder is destroyed all worlds loaded from it are also destroyed. */
		virtual ~WorldBuilder ();

		/** To get the number of definitions included in the definition file. */
		int numberResources () const
							{ return ((int) _definitions.size ()); }
		/** To get the id of every resource to be loaded. */
		std::vector <int> resourcesId () const;

		/** Once the world builder has been created, and the world definitions
			are loaded, it it possible to create the worlds usually before the
			game starts. This is done to improve the game performance. */
		void preLoad ();

		/** To load a world. 
			It the world has al ready been created then its reference is returned, otherwise
			the builder tries to create it from the definition loaded at construction time. */
		World* world (int nw);

		protected:
		struct SceneConexion
		{
			SceneConexion ()
				: _id (-1), 
				  _toScene (-1), 
				  _toPoint (-1)
							{ }

			int _id;
			int _toScene;
			int _toPoint;
		};

		struct SceneBlockDefinition
		{
			SceneBlockDefinition ()
				: _id (-1),
				  _active (true),
				  _variables ()
							{ }

			int _id;
			bool _active;
			std::map <std::string, std::string> _variables;
		};

		struct SceneDefinition
		{
			typedef enum { _SIMPLE, _SET } Type;

			SceneDefinition ()
				: _id (-1),
				  _type (_SIMPLE),
				  _number (1),
				  _maps (),
				  _entitiesInMaps (),
				  _sceneBlocks (),
				  _variables () 
							{ }

			int _id;
			Type _type;
			int _number; // There could be a set of them...
			std::map <int, std::string> _maps; // In that casee, thes maps are the base if (not the definition)
			std::map <std::string, std::vector <int>> _entitiesInMaps;
			std::map <int, SceneConexion> _conexions; // When a set of scenes are defined, the conecctions makes no sense...
			std::map <int, SceneBlockDefinition> _sceneBlocks;
			std::map <std::string, std::string> _variables;
		};
 
		struct WorldDefinition
		{
			WorldDefinition ()
				: _id (-1),
				  _scenes (),
				  _setOfScenes (),
				  _variables () 
							{ }

			int _id;
			std::map <int, SceneDefinition> _scenes;
			std::map <int, SceneDefinition> _setOfScenes;
			std::map <std::string, std::string> _variables;
		};

		protected:
		virtual World* createWorldObject (int no, const Scenes& s, const WorldProperties& p) = 0;
		virtual Scene* createSceneObject (int ns, const Maps& m, const Scene::Connections& cn, 
			const SceneProperties& p, const EntitiesPerLayer& ePL) = 0;
		virtual SceneActionBlock* createSceneActionBlockObject (int nAB, const SceneActionBlockProperties& prps)
							{ return (NULL); } // Usually there is no action block in the scenes...

		private:
		void readFile (const std::string& nF);
		WorldDefinition readWorldDefinition (TiXmlElement* e);
		std::map <int, SceneDefinition> readScenesDefinition (TiXmlElement* e);
		SceneDefinition readSceneDefinition (TiXmlElement* e);
		std::map <int, SceneDefinition> readSetOfScenesDefinition (TiXmlElement* e);
		SceneDefinition readTemplateSceneDefinition (TiXmlElement* e);
		std::map <int, SceneBlockDefinition> readScenesBlockDefinition (TiXmlElement* e);
		SceneBlockDefinition readSceneBlockDefinition (TiXmlElement* e);
		std::map <int, SceneConexion> readConexions (TiXmlElement* e);
		std::map <int, std::string> readMaps (TiXmlElement* e);
		std::map <std::string, std::vector <int>> readEntitiesInMaps (TiXmlElement* e);
		std::map <std::string, std::string> readVariables (TiXmlElement* e);
		World* createWorld (const WorldDefinition& def);
		Scene* createScene (const SceneDefinition& def);
		SceneActionBlock* createSceneActionBlock (const SceneBlockDefinition& def);
		void addSceneConnections (Scene* s, const Scenes& scns, SceneDefinition& def);
		int worldIdForScene (int scnId);

		private:
		typedef std::map <int, WorldDefinition> Definitions;

		Definitions _definitions;
		Worlds _worlds;
		MapBuilder* _mapBuilder;

		std::string _definitionFile;
		std::string _basePath;
	};
}

#endif
  
// End of the file
/*@}*/
