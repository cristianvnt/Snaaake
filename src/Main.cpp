#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>

#pragma region DECLARATIONS
constexpr double TARGET_FRAME_TIME = 1.0 / 60.0;

struct Position;
class Map;
class Snake;

void UpdateGame(double dt);
void Render(Map& map);
void FPS(double dt);
double GetDeltaTime();

#pragma endregion

struct Position
{
	int x;
	int y;
};

class Map
{
private:
	std::vector<std::string> _map;

public:
	Map(const std::vector<std::string>& map) : _map(map) {}

	std::vector<std::string> GetMap()
	{
		return _map;
	}

	void ModifyMap(const Position& pos)
	{
		_map[pos.x][pos.y] = 'S';
	}
};

#pragma region SNAKE
class Snake
{
private:
	Position _pos;

public:
	Snake(const Position& pos) : _pos(pos) {}

	const Position GetPosition()
	{
		return _pos;
	}

	void SetPosition(const int& x, const int& y)
	{
		_pos.x = x;
		_pos.y = y;
	}

	friend std::ostream& Snake::operator<<(std::ostream& os, const Snake& s)
	{
		return os << "Snake - x: " << s._pos.x << ", y: " << s._pos.y;
	}

	void SpawnSnake(Map& map, const Position& pos)
	{
		map.ModifyMap(pos);
	}
};

#pragma endregion

#pragma region GAME DEFAULTS
void UpdateGame(double dt)
{
	std::this_thread::sleep_for(std::chrono::duration<double>(TARGET_FRAME_TIME));
}

void Render(Map& map)
{
	std::string buffer;
	buffer += "\033[2J\033[H";
	for (auto& line : map.GetMap())
		buffer += line + "\n";
	std::cout << buffer << std::flush;
}

#pragma endregion

#pragma region FPS & DELTA TIME
void FPS(double dt)
{
	static int frameCount = 0;
	static double timeAccumulated = 0.0;

	timeAccumulated += dt;
	frameCount++;

	if (timeAccumulated >= 0.1f)
	{
		std::cout << "\rFPS: " << static_cast<double>(frameCount) / timeAccumulated << std::flush;

		frameCount = 0;
		timeAccumulated = 0.0;
	}
}

double GetDeltaTime()
{
	static std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
	double dt = std::chrono::duration<double>(currentTime - lastTime).count();
	lastTime = currentTime;
	return dt;
}

#pragma endregion

int main()
{
	Map map{ {
			"#############################",
			"#                           #",
			"#                           #",
			"#                           #",
			"#                           #",
			"#                           #",
			"#                           #",
			"#                           #",
			"#                           #",
			"#                           #",
			"#                           #",
			"#                           #",
			"#                           #",
			"#                           #",
			"#                           #",
			"#                           #",
			"#                           #",
			"#                           #",
			"#                           #",
			"#############################"
		} };
	Position pos{ 2, 10 };
	Snake snake{ pos };
	snake.SpawnSnake(map, pos);

	while (true)
	{
		double dt = GetDeltaTime();

		UpdateGame(dt);

		Render(map);

		FPS(dt);
	}

}
