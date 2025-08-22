#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace C
{
	// game settings
	constexpr int MAP_WIDTH = 50;
	constexpr int MAP_HEIGHT = 25;
	constexpr double FPS = 60.0;
	constexpr double FRAME_TIME = 1.0 / FPS;

	// gameplay
	constexpr int SNAKE_DEF_LEN = 3;
	constexpr double MOVE_DELAY = 0.15;
	constexpr int HEAD = 0;
	constexpr int MARGIN = 5;

	// symbols
	constexpr char SNAKE_HEAD = 'S';
	constexpr char SNAKE_TAIL = 'T';
	constexpr char FOOD = 'F';
}

#endif
