#include "Map.h"

Map::Map()
{
	std::string border = "###########################################################";
	std::string space = "#                                                         #";
	_map.push_back(border);
	for (int i = 0; i < 25; i++)
		_map.push_back(space);
	_map.push_back(border);

	std::string start = "#               PRESS ENTER to START GAME                 #";
	std::string quit = "#                        Q to QUIT                        #";
	_gameMenuMap.push_back(border);
	for (int i = 0; i < 25; i++)
	{
		if (i == 10)
		{
			_gameMenuMap.push_back(start);
			_gameMenuMap.push_back(quit);
			continue;
		}
		_gameMenuMap.push_back(space);
	}
	_gameMenuMap.push_back(border);

	std::string over = "#                        GAME OVER                        #";
	std::string restart = "#                   Press R to restart                    #";
	_gameOverMap.push_back(border);
	for (int i = 0; i < 25; i++)
	{
		if (i == 10)
		{
			_gameOverMap.push_back(over);
			_gameOverMap.push_back(restart);
			continue;
		}
		_gameOverMap.push_back(space);
	}
	_gameOverMap.push_back(border);
}