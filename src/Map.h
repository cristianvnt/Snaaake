#ifndef MAP_H
#define MAP_H

#pragma once
#include <vector>
#include <string>

class Map
{
private:
	std::vector<std::string> _map;
	std::vector<std::string> _gameOverMap;
	std::vector<std::string> _gameMenuMap;

public:
	explicit Map();

	std::vector<std::string> GetMap() { return _map; }
	std::vector<std::string> GetGameOverMap() { return _gameOverMap; }
	std::vector<std::string> GetGameMenuMap() { return _gameMenuMap; }
	const int GetWidth() const { return static_cast<int>(_map[0].size()); }
	const int GetHeight() const { return static_cast<int>(_map.size()); }
};

#endif
