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
	NONE,
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
	Position _pos = { 1, 1 };

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
	std::vector<std::string> _gameMenuMap;

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

		_gameMenuMap = {
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
			"#                  PRESS P to START GAME                  #",
			"#                        Q to QUIT                        #",
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
			"#                   Press R to restart                    #",
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

	std::vector<std::string> GetGameMenuMap()
	{
		return _gameMenuMap;
	}

	const int GetWidth() const
	{
		return static_cast<int>(_map[0].size());
	}

	const int GetHeight() const
	{
		return static_cast<int>(_map.size());
	}
};

#pragma endregion

#pragma region GAME
class Game
{
private:
	Map _map;
	GameState _gameState;
	Snake _snake;

	Direction _currentDirection;
	double _deltaTime;
	bool _isRunning = true;

public:
	explicit Game() : _snake{ RandomPosition() }, _currentDirection{ Direction::NONE }, _gameState{ GameState::PAUSE }
	{
		_deltaTime = GetDeltaTime();

		if (!CheckBounds(_snake.GetPosition()))
		{
			std::cerr << "Not gud position\n";
			_snake.SetPosition({ 2, 10 });
		}
	}

	bool CheckBounds(const Position& pos)
	{
		if (pos.x < 1 || pos.x > _map.GetWidth() - 2)
			return false;

		if (pos.y < 1 || pos.y > _map.GetHeight() - 2)
			return false;

		return true;
	}

	Position RandomPosition()
	{
		std::random_device dev;
		std::mt19937 rng(dev());

		std::uniform_int_distribution<int> distX(1, _map.GetWidth() - 2);
		std::uniform_int_distribution<int> distY(1, _map.GetHeight() - 2);

		return { distX(rng), distY(rng) };
	}

	bool IsOppositeDirection(Direction curr, Direction newDir)
	{
		return (curr == Direction::UP && newDir == Direction::DOWN ||
			curr == Direction::LEFT && newDir == Direction::RIGHT ||
			curr == Direction::DOWN && newDir == Direction::UP ||
			curr == Direction::RIGHT && newDir == Direction::LEFT);
	}

	void ProcessInput()
	{
		if (_kbhit())
		{
			char input = _getch();
			Direction newDirection = _currentDirection;

			switch (input)
			{
			case 'w':
				newDirection = Direction::UP;
				break;
			case 's':
				newDirection = Direction::DOWN;
				break;
			case 'a':
				newDirection = Direction::LEFT;
				break;
			case 'd':
				newDirection = Direction::RIGHT;
				break;
			case 'p':
				_gameState = (_gameState == GameState::PLAYING ? GameState::PAUSE : GameState::PLAYING);
				break;
			case 'q':
				_isRunning = false;
				break;
			case 'r':
				_gameState = GameState::RESTART;
				break;
			}

			if (!IsOppositeDirection(_currentDirection, newDirection))
				_currentDirection = newDirection;
		}
	}

	bool IsRunning() 
	{ 
		return _isRunning; 
	}

	bool Move(Direction dir)
	{
		if (!CheckBounds(_snake.GetPosition()))
		{
			_gameState = GameState::GAME_OVER;
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
	}

	void RestartGame()
	{
		_currentDirection = Direction::NONE;
		_snake.SetPosition(RandomPosition());
	}

	void UpdateGameplay(double dt)
	{
		static double moveTimer = 0.0;
		moveTimer += dt;

		if (moveTimer >= 0.15)
		{
			if (!Move(_currentDirection))
				_gameState = GameState::GAME_OVER;

			moveTimer = 0.0;
		}
	}

	void Update()
	{
		double dt = GetDeltaTime();
		
		switch (_gameState)
		{
		case GameState::PLAYING:
			UpdateGameplay(dt);
			break;
		case GameState::PAUSE:
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			break;
		case GameState::GAME_OVER:
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			break;
		case GameState::RESTART:
			RestartGame();
			_gameState = GameState::PAUSE;
			break;
		default:
			break;
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
		else if (_gameState == GameState::PAUSE)
		{
			for (auto& line : _map.GetGameMenuMap())
				buffer += line + '\n';
		}
		else
		{
			buffer += "Snake pos: (" + std::to_string(_snake.GetPosition().x)
			+ "," + std::to_string(_snake.GetPosition().y) + ")\n";

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
