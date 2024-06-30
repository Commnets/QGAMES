/** \addtogroup Atic */
/*@{*/

#ifndef __MOVEMENTSBUILDER_HPP__
#define __MOVEMENTSBUILDER_HPP__

#include <Arcade/arcadeinclude.hpp>

class MovementsBuilder : public QGAMES::MovementBuilder
{
	public:
	MovementsBuilder (const std::string& fDef)
		: QGAMES::MovementBuilder (fDef)
							{ }
	
	private:
	QGAMES::Movement* createMovement (const QGAMES::MovementBuilder::MovementDefinition& def);
};

#endif
  
// End of the file
/*@}*/