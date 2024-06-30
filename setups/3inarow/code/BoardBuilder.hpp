/** \addtogroup 3InARow */
/*@{*/

#ifndef __BOARDBUILDER_HPP__
#define __BOARDBUILDER_HPP__

#include <Board/boardinclude.hpp>

/** To create the board Tic Tac Toe. */
class BoardBuilderAddsOn : public QGAMES::BasicBoardAddsOn
{
	public:
	BoardBuilderAddsOn ()
		: QGAMES::BasicBoardAddsOn () 
								{ }

	protected:
	virtual QGAMES::Board* createBoardInstance (int id, const QGAMES::BoardSquares& sqs, 
		const QGAMES::BoardResources& rscs); 
	virtual QGAMES::BoardSquare* createSquareInstance (int id, 
		const QGAMES::BoardResources& rscs,
		const std::map <std::string, std::string> pts);
	virtual QGAMES::BoardResource* createResourceInstance (int id, int grp, 
		const std::map <std::string, std::string> pts);
};

#endif
  
// End of the file
/*@}*/