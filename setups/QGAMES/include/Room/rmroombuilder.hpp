/** \ingroup Room */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: rmroom.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 18/03/2021 \n
 *	Description: To build a room. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_RMROOMBUILDER__
#define __QGAMES_RMROOMBUILDER__

#include <Room/rmdefinitions.hpp>
#include <Room/rmroom.hpp>
#include <Platform/ptinclude.hpp>

namespace QGAMES
{
	class RoomBuilder
	{
		public:
		/**
		  *	To define a room consists on the following lines:
		  * A room is merely made up of a set of decoration elements (in the room) 
		  * and a set of walls with maybe also decoration elements in them. \
		  * Every wall is made up of a set of bricks, the size of the wall is given en number of "bricks" per width, height, and depth. \n
		  * The meaning of "width", "height" and "depth" will depend on the type of projection used to describe the brick. \n
		  * Therefore, in a isometric brick type, "width" is the length in the x axis, "height" in the -z axis, and "depth" in the y axis. \n
		  * In the other hand, e,g, in a orthoginal brick type, "width" is the length also in the x axis, but "height" wiuld be in the y axis. \n
		  * The decoration elements have a position whether in the room or in the wall. \n
		  * That position is defined in terms of pixels in the case of the decoration elements of the room, and in terms of "bricks"
		  * in the case of a decoration element in the wall taking the base position of the first brick as the reference.
		  *	<?xml version="1.0"?> \n
		  * <Rooms>
		  *		<Include file="CCC"/> \n
		  *		<Room id="xxx" type="xxx"> \n
					<OverRoom id="xxx"/> <!-- To identifiy a previous room format -->
		  *			<Decoration> \n
		  *				<DecorationElement id="xxx", entityId="xxx" iform="xxx" iaspect="xxx" position="xx,yy,zz"/> \n
		  *				... \n
		  *			</Decoration> \n
		  *			<Walls> \n
		  *				<Wall id="xxx" 
		  *					brickEntityId="xx" brickIForm="xx" brickIAspect="yy" position="xx,yy,zz" 
		  *					bricksWidth="xx" bricksHeight="yy" bricksDepth="zz"> \n
		  *					<Hole position="x,y,z"/> <!-- To identify a hole -->
		  *					...
		  *					<Decoration> \n
		  *						<DecorationElement id="xxx", entityId="xxx" iform="xxx" iaspect="xxx" position="xx,yy,zz"/> \n
		  *						... \n
		  *					<Decoration> \n
		  *				</Wall> \n
		  *				... \n
		  *			</Walls> \n
		  *		</Room> \n
		  * </Rooms>
		  */
		RoomBuilder (const std::string& mDef, EntityBuilder* eB);
		/** The room builder doesn't own the rooms created. 
			The method is declared virtual and empty just in case it was needed. */
		virtual ~RoomBuilder ()
							{ }

		EntityBuilder* entityBuilder () 
							{ return (_entityBuilder); }
		const EntityBuilder* entityBuilder () const
							{ return (_entityBuilder); }

		/** To get the room.
			Every invocation to the method created always a new object. */
		Room* room (int nR);

		protected:
		struct DecorationElementDefinition
		{
			DecorationElementDefinition ()
				: _id (-1),
				  _entityId (-1), 
				  _position (Position::_cero)
							{ }

			DecorationElementDefinition (const DecorationElementDefinition& dE) = default;

			DecorationElementDefinition& operator = (const DecorationElementDefinition& dE) = default;

			int _id;
			int _entityId;
			Position _position;
		};

		using DecorationElementDefinitions = std::vector <DecorationElementDefinition>;
		using HoleDefinitions = std::vector <Position>;

		struct RoomWallDefinition
		{
			RoomWallDefinition ()
				: _id (-1),
				  _brickEntityId (-1), _brickInitialFormId (-1), _brickInitialAspectId (-1),
				  _position (Position::_cero),
				  _bricksWidth (-1), _bricksHeight (-1), _bricksDepth (-1),
				  _decoration (),
				  _holes ()
							{ }

			RoomWallDefinition (const RoomWallDefinition& rW) = default;

			RoomWallDefinition& operator = (const RoomWallDefinition& rW) = default;

			int _id;
			int _brickEntityId;
			int _brickInitialFormId;
			int _brickInitialAspectId;
			Position _position;
			int _bricksWidth, _bricksHeight, _bricksDepth;
			DecorationElementDefinitions _decoration;
			HoleDefinitions _holes;
		};

		using RoomWallDefinitions = std::vector <RoomWallDefinition>;

		struct RoomDefinition
		{
			RoomDefinition ()
				: _id (-1), _type (-1), _overRoom (), _decoration (), _roomWalls ()
							{ }

			RoomDefinition (const RoomDefinition& rD) = default;

			RoomDefinition& operator = (const RoomDefinition& rD) = default;

			int _id;
			int _type;
			std::list <int> _overRoom; // The room can be based in a list of others!
			DecorationElementDefinitions _decoration;
			RoomWallDefinitions _roomWalls;
		};

		using RoomDefinitions = std::map <int, RoomDefinition>;

		void readFile (const std::string& fDef);
		RoomDefinition readRoomDefinition (TiXmlElement* elmnt);
		RoomWallDefinitions readRoomWallDefinitions (TiXmlElement* elmnt);
		RoomWallDefinition readRoomWallDefinition (TiXmlElement* elmnt);
		DecorationElementDefinitions readDecorationElementDefinitions (TiXmlElement* elmnt);
		DecorationElementDefinition readDecorationElementDefinition (TiXmlElement* elmnt);
		Position readHolePosition (TiXmlElement* elmnt);

		/** The y can be overloaded later, but a basic definiton is provided. */
		virtual Room* createRoomObject (const RoomDefinition& def)
							{ return (new Room (def._id, def._type)); }
		virtual RoomWall* createRoomWallObject (const RoomWallDefinition& def)
							{ return (new RoomWall (def._id, def._brickEntityId, 
										def._position, def._bricksWidth, def._bricksHeight, def._bricksDepth)); }

		protected:
		std::string _definitionFile;
		std::string _basePath;
		RoomDefinitions _roomDefinitions;
		/** Needed to create the entities representing the decoration elements. */
		EntityBuilder* _entityBuilder;
	};
}

#endif
  
// End of the file
/*@}*/
