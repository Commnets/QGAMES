/** \addtogroup SabreWulf */
/*@{*/


#ifndef __EVENTS_HPP__
#define __EVENTS_HPP__

#include <Arcade/arcadeinclude.hpp>

class SabreWulfInmortal;

/** A simple structure to keep information
	to change the mazePlace. */
struct SabreWulfChangeMazePlaceEventData
{
	int _mazePlace;
	QGAMES::Position _position;
};

/** A simple structure to keep info
	to change the maze pkace where an inmortal is. */
struct InmortalChangeMazePlaceEventData : public SabreWulfChangeMazePlaceEventData
{
	SabreWulfInmortal* _inmortal;
};

#endif
  
// End of the file
/*@}*/
