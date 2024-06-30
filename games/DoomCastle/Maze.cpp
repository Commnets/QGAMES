#include "Maze.hpp"

// ---
DOOMCASTLE::MazeModel::MazeModel ()
	: QGAMES::MazeModel ()
{
	_mazeModel =
		{
			// Room 0,0,0
			{
				{ 
					{ 0, "exampl.tmx" }, 
					{ 1, "100" },
				},
				{
					{ 
						0, 
						{
							{
								{ 0, "1" },
								{ 1, "2" }
							},
							std::string ("East Doors")
						}
					},
					{
						1, 
						{
							{
								{ 0, "1" },
								{ 1, "2" }
							},
							std::string ("Noth Doors")
						}
					}
				},
				std::string ("N")
			}
		};
}
