#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

class Map
{
public:
	Map();

	std::vector<std::string>& GetMap() { return _map; }
	std::vector<std::string>& GetGameOverMap() { return _gameOverMap; }
	std::vector<std::string>& GetGameMenuMap() { return _gameMenuMap; }
	int GetWidth() const { return static_cast<int>(_map[0].size()); }
	int GetHeight() const { return static_cast<int>(_map.size()); }

private:
	std::vector<std::string> _map;
	std::vector<std::string> _gameOverMap;
	std::vector<std::string> _gameMenuMap;
};

#endif
