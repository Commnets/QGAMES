/** \ingroup Platform */
/** \ingroup Strcuture */
/*@{*/

/**	
 *	@file	
 *	File: ptartists.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 06/04/2018 \n
 *	Description: The artists in a platform game go through the platforms.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_PTARTISTS__
#define __QGAMES_PTARTISTS__

#include <Platform/ptdefinitions.hpp>
#include <Platform/ptgame.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace QGAMES
{
	/** The artist designed to play typically in a platform game. 
		The artist hasn't a pocket defined. 
		It is something to include in the specific implementation of the game if any. */
	class PlatformArtist : public ArtistInATiledMap
	{
		public:
		PlatformArtist (int cId, const Forms& f = Forms (), const Data& d = Data ());

		/** @see parent. */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		// Related with the description...
		/** To set the specific form and behaviour from an external definition.
			The method invokes to setState who is really the one taking the hardest work. 
			The methods can be overloaded later. */
		virtual void setFromDefinition (PlatformGame::ElementDefinition* eD);
		virtual void setFromNoDefinition ();
		PlatformGame::ElementDefinition* definition ()
							{ return (_definition); }
		const PlatformGame::ElementDefinition* definition () const
							{ return (_definition); }

		/** To active or desactive to draw limits. */
		void setDrawLimits (bool dL)
							{ _drawLimits = dL; }

		/** @see parent. */
		virtual bool canMove (const Vector& d, const Vector& a);

		/** To know whether a potential next position is or not allowed for the entity. */
		virtual bool isPossiblePosInMap (const Position& v);

		// Related with the state in the platform...
		/** to know the current state inthe platform game. 
			-1 means undefined. */
		int platformState () const
							{ return (_platformState); }
		/** To set the state of the element in the platform game. 
			-1 means indefined and has to be managed as any one else. */
		virtual void setPlatformState (int id);
		/** To know whether a potential platform state is possible for this element. */
		virtual bool isPlatformStatePossible (int pS) const
							{ return (true); } // Always by default...
		/** The current state is stable if there is no other in front. */
		bool isPlatformStateStable () const
							{ return (!buoy (__QGAMES_PTCHANGESTATEDEFEREDBUOYID__) -> isActive ()); } 

		/** To know whether the platform artist can or not do some actions. */
		bool canClimbUp () const
							{ return (_canClimbUp); }
		bool canClimbDown () const
							{ return (_canClimbDown); }
		bool canClimb () const
							{ return (_canClimbUp || _canClimbDown); }
		bool canSwim () const
							{ return (_canSwim); }
		bool canExitWaterUp () const
							{ return (_canExitWaterUp); }
		bool canExitWaterDown () const
							{ return (_canExitWaterDown); }
		bool canExitWaterLeft () const
							{ return (_canExitWaterLeft); }
		bool canExitWaterRight () const
							{ return (_canExitWaterRight); }
		bool canExitWater () const
							{ return (_canExitWaterUp || _canExitWaterDown || _canExitWaterLeft || _canExitWaterRight); }

		// Related with the position in the base...
		/** The base position can be set for an specific coordinate.
			The pura base position based on the frame could change as the entity moves.
			So it can be fix to an specific value. 
			When the coordinate is __MINDATA__ the value is got from the original base position.
			Otherwise the value is fixed. */
		const Position& fixBasePosition () const
							{ return (_fixBasePosition); }
		void setFixBasePosition (const Position& fV = Position::_noPoint)
							{ _fixBasePosition = fV; }
		/** @see parent. */
		virtual Position basePosition () const
							{ Vector p = ArtistInATiledMap::basePosition ();
							  return (Vector (_fixBasePosition.posX () == __MINBDATA__ ? p.posX () : _fixBasePosition.posX (),
											  _fixBasePosition.posY () == __MINBDATA__ ? p.posY () : _fixBasePosition.posY (),
											  _fixBasePosition.posZ () == __MINBDATA__ ? p.posZ () : _fixBasePosition.posZ ())); }
		/** To know whether the artist is or not over a base taking into account the current position.
			That is checking if the low part of the artist is o not a base layer. */
		bool isOnABase ();

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);

		/** @see parent. */
		virtual void whenCollisionWith (const Tiles& t, ArtistInATiledMap::TilePosition tP, TileLayer* l = NULL);
		virtual void whenOnTile (const Tiles& t, TileLayer* l = NULL);
		virtual void whenOver (const Tiles& t, TileLayer* tL, int w, int h, int d);

		protected:
		/** @see parent
			Taking into account the size of the reticle occupied by the entity
			and the orientation that the entity has, the pieces to take into account to determinate whether
			a movement is or not possible are different. */
		virtual void relevantTilePositions (int w, int h, int d, 
			std::vector <int>& up, std::vector <int>& down, 
			std::vector <int>& right, std::vector <int>& left, 
			std::vector <int>& front, std::vector <int>& back);

		/** Very internal method to draw things around the entity. 
			The method can be activated or desactivated with an internal variable. */ 
		virtual void drawOnLimits (Screen* s, const Position& p = Position::_noPoint);

		/** Define a buoy to change the platform state of the entity within the inEveryLoop method. 
			In many ocassions an event is thorwn from an element supervised by this one (e.g. movements), and that 
			event implies to change the state and with it things like form, aspect o even the movement itself. 
			In that case event treatment iterator could crash, so it is necessay to change the state using a buoy. */
		class ToChangeStateBuoy : public Buoy
		{
			public:
			ToChangeStateBuoy ()
				: QGAMES::Buoy (__QGAMES_PTCHANGESTATEDEFEREDBUOYID__, (bdata) 0), 
				  _targetState (-1),
				  _adjustAfterSetState (false)
								{ /** Nothing else to do. */ }

			void setTargetState (int tSt)
								{ _targetState = tSt; }
			void setAdjustBaseAfterTargetSet (bool a)
								{ _adjustAfterSetState = a; }

			virtual void* treatFor (QGAMES::Element* e);

			private:
			int _targetState;
			bool _adjustAfterSetState;
		};

		/** To change the state in the everyloop method. */
		void setPlatformStateDeferred (int s, bool a = false);

		/** To know which is the main reference tile regarding the movement that the entity has right now. 
			Here the type of the state has to be take into account. 
			This method is used, e.eg in canMove. 
			It has to be redefined in further classes. */
		virtual Tiles referenceTilesNow (const SetOfTiles& rT, const Vector& dr, const Vector& a) = 0;
		/** To know the reference base tile if it exists. 
			Takes the tile under the base position identified, if any. 
			Otherwise it returns null. */
		virtual Tiles referenceBaseTiles (Layer* l = NULL);

		/** To know whether a tile is or not an obstacle for the entity.
			This method is used e.g. within the PTARTIST canMove impWorldBasedOnRoomslementation to determine
			whether the obstacle the artist is collision against is or not something able to stop it. 
			By default the types PTSOLID and PTBASE are obstacles, but it can be overloaded. 
			The result can also depend on context. */
		virtual bool isTileAnObstacle (Tile* t);

		/** To verify whether the artist will collision or not with a type of tile. */
		virtual bool collisioningWithTile (int tT, const Vector& d, const Vector& a);
		/** The same with a set of type of tiles. */
		virtual bool collisioningWithTile (const std::vector <int> tT, const Vector& d, const Vector& a);
		/** To know whether the artist is or not over a type of tile. */
		virtual bool overTile (int tT);
		/** The same with a list of types. */
		virtual bool overTile (const std::vector <int> tT);

		/** To know whether at least one tile at the right of other is not null. */
		Tile* tileNotNullAtTheRightOf (Tile* rT, const Tiles& t);
		/** The same but at the left. */
		Tile* tileNotNullAtTheLeftOf (Tile* rT, const Tiles& t);

		/** What to do when all layers have been reviewed in the method whenOnTile
			It has to de redefined depending on the character. 
			By default it does nothing. */
		virtual void afterAllLayersTestedInWhenOnTile () = 0;

		// Implementation
		/** To adjust the position of the entity to where the base is set. 
			Sometimes, after changing the state then new animation is less tall o taller than the previous
			and it could be necessary to adjust that position to where the base is to 
			produce and smooth movement. 
			The parameters it to indicate whether the adjustment will take place considering only the y axis or not. */
		void adjustPositionToBase (bool oY = false);
		/** To adjust the center of the platform artist to a center. */
		void adjustPositionToCenter (const Position& pos);
		/** To determinate which is the base position closer to one in the base layer if any. */
		Position roundedLayerPositionCloserTo (const Position& pos, 
			TileLayer* l, int nP, bool x, bool y, bool z = false);

		protected:
		/** A reference to the definition. */
		PlatformGame::ElementDefinition* _definition;
		/** The platform state. According with that state the artist will perform in the game. */
		int _platformState;

		// Implementation
		/** Managed in every loop to know whether the entity is or not on the edge of a base. 
			The potential values are:
			 1: Not stable but there is a stable base at the right.
			-1: Not stable but there is a stable base at the left.
			 2: Stable, but maybe not enough because there is a hole at the right
			-2: Stable, but maybe not enough because there is a hole at the left
		  */
		int _onTheEdge;
		/** To know which is the last layer base used. */
		Layer* _lastLayerBase;
		/** To know when the artist is already on a base layer. */
		Layer* _layerBase;
		/** The position of the tile of the layer under, if any. Otherwise it is cero. */
		Position _layerBasePosition;
		/** The tiles referenced in the base. */
		Tiles _layerBaseTiles;
		/** The list of layers being tested in updatePosition method.
			They are always initialized at the beginning of the method.
			It is specially useful in the method whenOnTile. 
			If there is no more layers to be tested, then it is time to take decisions. */
		Layers _layersToTest;
		/** To know if after checking all layers, there is something that the entity is collision with and able to kill her/him. */
		bool _tileKillingEntity;
		/** To know if after checking all layers, there is a solid tile behind my feet. */
		bool _tileSolid;
		/** To keep the detection of a collision whilst the entity is still moving to a direction. */
		std::vector <bool> _collisionAtDir;
		/** A flag that is set when the entity is in a position where climbing is possible.
			That is that the central tiles are ladder tiles. */
		bool _canClimbUp, _canClimbDown;
		/** The layer containing the ladder when canClimb is true. */
		Layer* _layerLadder;
		/** A flag that is set when the entity is in a position where swiming is possible.
			It means that the central tile of the entity is a water tile. */
		bool _canSwim;
		/* The layer containing the water when canSwim is possible. */
		Layer* _layerWater;
		/** When the entity can exit the water (is because the tiles of the top part are empty). */
		bool _canExitWaterUp, _canExitWaterDown, _canExitWaterLeft, _canExitWaterRight;

		private:
		/** To determinate whether the limits has or nbot to be drawn. 
			By default when the compilation mode is a debug mode, the limits will be shown. */
		bool _drawLimits;

		// Implementation
		/** The fix base. By default it is noPosition what means the position has to be calculated everyloop. */
		Position _fixBasePosition;
	};

	// Redefinition just to simplify further managing...
	/** Something to eat. */
	typedef PlatformArtist SomethingToEat;
	/** Somnething to catch. */
	typedef PlatformArtist SomethingToCatch;
	/** A bad guy. */
	typedef PlatformArtist BadGuy;
}

#endif
  
// End of the file
/*@}*/