#ifndef MAP_H
#define MAP_H

#include <array>
#include <string_view>

class Map
{
public:
	static constexpr int WIDTH = 50;
	static constexpr int HEIGHT = 25;

	static constexpr std::array<std::string_view, HEIGHT> MAP = 
	{
		"##################################################",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"##################################################"
	};

	static constexpr std::array<std::string_view, HEIGHT> GAME_OVER_MAP =
	{
		"##################################################",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                   GAME OVER                    #",
		"#               Press R to Restart               #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"##################################################"
	};

	static constexpr std::array<std::string_view, HEIGHT> GAME_MENU_MAP =
	{
		"##################################################",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                   START GAME                   #",
		"#            Press ENTER to START GAME           #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"#                                                #",
		"##################################################"
	};
};

#endif
