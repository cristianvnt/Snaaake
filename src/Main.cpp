#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <conio.h>
#include <random>

#pragma region DECLARATIONS
constexpr double TARGET_FRAME_TIME = 1.0 / 60.0;

struct Position;
class Snake;
class Map;

#pragma endregion

struct Position
{
	int x;
	int y;
};

enum class Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum class GameState
{
	PLAYING,
	PAUSE,
	GAME_OVER,
	RESTART
};

#pragma region SNAKE
class Snake
{
private:
	Position _pos;

public:
	explicit Snake(const Position& pos) : _pos{ pos } {};

	const Position GetPosition() const
	{
		return _pos;
	}

	void SetPosition(const Position& pos)
	{
		_pos.x = pos.x;
		_pos.y = pos.y;
	}

	friend std::ostream& Snake::operator<<(std::ostream& os, const Snake& s)
	{
		return os << "Snake - x: " << s._pos.x << ", y: " << s._pos.y;
	}

	void MoveUp()
	{
		_pos.y -= 1;
	}

	void MoveDown()
	{
		_pos.y += 1;
	}

	void MoveLeft()
	{
		_pos.x -= 1;
	}

	void MoveRight()
	{
		_pos.x += 1;
	}
};

#pragma endregion

#pragma region MAP
class Map
{
private:
	std::vector<std::string> _map;
	std::vector<std::string> _gameOverMap;

public:
	explicit Map()
	{
		_map = {
			"###########################################################",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"###########################################################"
		};

		_gameOverMap = {
			"###########################################################",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                        GAME OVER                        #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"#                                                         #",
			"###########################################################"
		};
	}

	std::vector<std::string> GetMap()
	{
		return _map;
	}

	std::vector<std::string> GetGameOverMap()
	{
		return _gameOverMap;
	}

	const int GetWidth() const
	{
		return _map[0].size();
	}

	const int GetHeight() const
	{
		return _map.size();
	}
};

#pragma endregion

#pragma region GAME
class Game
{
private:
	Direction _currentDirection;
	GameState _gameState;
	Snake _snake;
	Map _map;

	double _deltaTime;
	bool _isRunning = true;

public:
	explicit Game() : _snake{ { 5, 10 } }, _currentDirection{ Direction::UP }, _gameState{ GameState::PAUSE }
	{
		_deltaTime = GetDeltaTime();
	}

	bool CheckBounds(const Position& pos)
	{
		if (pos.x <= 0 || pos.x > _map.GetWidth() - 1)
			return false;

		if (pos.y <= 0 || pos.y > _map.GetHeight() - 1)
			return false;

		return true;
	}

	Position RandomPosition()
	{
		std::random_device dev;
		std::uniform_int_distribution<int> distX(1, _map.GetWidth() - 2);
		int x = distX(dev);

		std::uniform_int_distribution<int> distY(1, _map.GetHeight() - 2);
		int y = distY(dev);

		return { x, y };
	}

	void ProcessInput()
	{
		if (_kbhit())
		{
			char input = _getch();
			switch (input)
			{
			case 'w':
				_currentDirection = Direction::UP;
				break;
			case 's':
				_currentDirection = Direction::DOWN;
				break;
			case 'a':
				_currentDirection = Direction::LEFT;
				break;
			case 'd':
				_currentDirection = Direction::RIGHT;
				break;
			case 'p':
				_gameState = (_gameState == GameState::PLAYING ? GameState::PAUSE : GameState::PLAYING);
				break;
			case 'q':
				_isRunning = false;
				break;
			case 'r':
				_isRunning = true;
				break;
			}
		}
	}

	bool IsRunning() 
	{ 
		return _isRunning; 
	}

	bool Move(Direction dir, GameState gs)
	{
		if (gs == GameState::PAUSE)
			return true;

		if (!CheckBounds(_snake.GetPosition()))
		{
			_gameState = GameState::GAME_OVER;
			GameOver();
			return true;
		}

		switch (dir)
		{
		case Direction::UP:
			_snake.MoveUp();
			break;
		case Direction::DOWN:
			_snake.MoveDown();
			break;
		case Direction::LEFT:
			_snake.MoveLeft();
			break;
		case Direction::RIGHT:
			_snake.MoveRight();
			break;
		default:
			break;
		}

		return true;
	}

	void GameOver()
	{
		_gameState = GameState::GAME_OVER;
		_isRunning = false;
	}

	void Restart()
	{
		_snake.SetPosition({ 5, 10 });
		_gameState = GameState::PAUSE;
		_currentDirection = Direction::UP;
	}

	void Update()
	{
		double dt = GetDeltaTime();
		
		static double moveTimer = 0.0;
		moveTimer += dt;

		if (moveTimer >= 0.35)
		{
			if (!Move(_currentDirection, _gameState))
				_isRunning = false;

			moveTimer = 0.0;
		}
	}

	void Render()
	{
		std::string buffer;
		buffer += "\033[2J\033[H";
		if (_gameState == GameState::GAME_OVER)
		{
			for (auto& line : _map.GetGameOverMap())
				buffer += line + '\n';
		}
		else
		{
			for (int y = 0; y < _map.GetHeight(); y++)
			{
				for (int x = 0; x < _map.GetWidth(); x++)
				{
					if (y == _snake.GetPosition().y && x == _snake.GetPosition().x)
						buffer += 'S';
					else
						buffer += _map.GetMap()[y][x];
				}
				buffer += '\n';
			}
		}
		std::cout << buffer << std::flush;
	}

	double GetDeltaTime()
	{
		static std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
		std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
		double dt = std::chrono::duration<double>(currentTime - lastTime).count();
		lastTime = currentTime;
		return dt;
	}

	const void FPS() const
	{
		static int frameCount = 0;
		static double timeAccumulated = 0.0;

		timeAccumulated += _deltaTime;
		frameCount++;

		if (timeAccumulated >= 0.1f)
		{
			std::cout << "\rFPS: " << static_cast<double>(frameCount) / timeAccumulated << std::flush;

			frameCount = 0;
			timeAccumulated = 0.0;
		}
	}

};

#pragma endregion

int main()
{
	Game game;

	while (game.IsRunning())
	{
		game.ProcessInput();

		game.Update();

		game.Render();

		game.FPS();
	}

}
