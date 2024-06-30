/** \ingroup BoardGame */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: boardbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 12/11/2015 \n
 *	Description: Defines the class to build boards.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_BOARDBUILDER__
#define __QGAMES_BOARDBUILDER__

#include <Common/elementbuilder.hpp>
#include <Board/board.hpp>
#include <Board/definitions.hpp>
#include <tinyxml.h>
#include <map>
#include <string>

namespace QGAMES
{
	/** The class to instantiate boards. \n
		The boards can have very different definitions. \n
		So the builder can have different adds on specilized in creating different 
		types of boards. \n */
	class BoardBuilder : public ElementBuilder
	{
		public:
		/** The structure defining the board, its id, 
			its type and the file where it is defined in detail. */
		struct BoardDefinition 
		{
			BoardDefinition ()
				: _id (-1), _type (-1), _fileDefinition (__NULL_STRING__)
							{ }

			int _id;
			int _type;
			std::string _fileDefinition;
		};

		/** An addson is a piece of code specialized in creating certain types
			of boards. A Builder can have many of them. There can be several 
			adds on able to create different types of boards, but only the 
			first one is useful. */
		class AddsOn
		{
			public:
			AddsOn ()
							{ }

			virtual bool canCreateBoard (int t) const = 0;
			virtual Board* createBoard (const BoardDefinition& def,
				const std::string& bP) = 0;
		};

		// A typedef to simplify the access to the addsOn elements. */
		typedef std::list <AddsOn*> AddsOnList;

		BoardBuilder (const std::string& fDef);
		virtual ~BoardBuilder ();

		// To manage addsOn
		/** To add a adds on */
		void addAddsOn (AddsOn *aO);

		/** Once the definition file is read, it is possible to load all boards
			usually before the game starts. This is done to improve the performamce later on. */
		void preLoad ();
		Board* board (int id);

		protected:
		typedef std::map <int, BoardDefinition> Definitions;

		Definitions _definitions;
		Boards _boards;
		AddsOnList _addsOn;

		// Implementation...
		std::string _definitionFile;
		std::string _basePath;

		private:
		BoardDefinition readBoardDefinition (TiXmlElement* e);
	};

	/** 
	  *	This class is to create the very basic board. \n
	  *	An standard board is made up of resources, squares, and definitions to connect the boards. \n
	  *	The structure of the board is as follows: \n
	  *	<Board> \n
	  *		<Resources> \n
	  *			<Resource id="X"> \n
	  *				<Data id="XXXX" value="xx"/> \n
	  *				... \n
	  *			</Resource> \n
	  *			... \n
	  *		</Resources> \n
	  *		<Squares> \n
	  *			<Square id="X"> \n
	  *				<Datas> \n
	  *					<Data id="XXXX" value="xx"/> \n
	  *					... \n
	  *				<Datas/> \n
	  *				<Resources> \n
	  *					<Resource id="X"> \n
	  *						<Data id="XXXX" value="xx"/> \n
	  *						... \n
	  *					</Resource> \n
	  *				</Resources> \n
	  *			</Square> \n
	  *			... \n
	  *		</Squares> \n
	  *		<SquareConnections> \n
	  *			<Connection from="XX" in="X" to="XX"/> \n
	  *			... \n
	  *		</SquareConnections> \n
	  *	</Board>
	  */
	class BasicBoardAddsOn : public BoardBuilder::AddsOn
	{
		public:
		BasicBoardAddsOn ()
			: BoardBuilder::AddsOn () 
							{ }

		virtual bool canCreateBoard (int t) const
							{ return (t == __QGAMES__BOARDSIMPLETYPE__); }
		virtual Board* createBoard (const BoardBuilder::BoardDefinition& def,
			const std::string& bP);

		protected:
		// To overload with every specific application...
		// These methods are to create instances of the components of any board...
		virtual Board* createBoardInstance (int id, const BoardSquares& sqs, 
			const BoardResources& rscs) = 0; 
		virtual BoardSquare* createSquareInstance (int id, 
			const BoardResources& rscs,
			const std::map <std::string, std::string> pts) = 0;
		virtual BoardResource* createResourceInstance (int id, int grp, 
			const std::map <std::string, std::string> pts) = 0;

		private:
		// Internal structures to simplify how the board is created...
		// The structures keep the how the elements are kept into the definition file
		// The resources:
		struct ResourceDefinition 
		{
			int _id;
			int _group;
			std::map <std::string, std::string> _properties;
		};

		// The squares:
		struct SquareDefinition
		{
			int _id;
			std::map <int, ResourceDefinition> _resources;
			std::map <std::string, std::string> _properties;
		};

		// The connections among squares:
		struct SquareConnectionDefinition
		{
			int _from;
			int _in;
			int _to;
		};

		// The methods to read the different parts of the board definition...
		std::map <std::string, std::string> readProperties (TiXmlElement* e);
		std::map <int, ResourceDefinition> readResources (TiXmlElement* e);
		std::map <int, SquareDefinition> readSquares (TiXmlElement* e);
		std::list <SquareConnectionDefinition> readConnections (TiXmlElement* e);

		// To create the basic elements...
		BoardResources createBoardResources (const std::map <int, ResourceDefinition>& rD);
		BoardSquares createBoardSquares (const std::map <int, SquareDefinition>& sD);
		void setConnections (const BoardSquares& bS, const std::list <SquareConnectionDefinition>& sCD);
	};
}

#endif
  
// End of the file
/*@}*/