#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <conio.h>
#include <random>

#pragma region DECLARATIONS
constexpr double TARGET_FRAME_TIME = 1.0 / 60.0;
constexpr int HEAD = 0;

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
	std::vector<Position> _pos;

public:
	explicit Snake(const std::vector<Position>& pos) : _pos{ pos } {}

	Position GetHead()
	{
		return _pos.front();
	}

	Position GetTail()
	{
		return _pos.back();
	}

	void RemoveTail()
	{
		_pos.pop_back();
	}

	void ResetPositions()
	{
		if (!_pos.empty())
			_pos.clear();
	}

	const std::vector<Position> GetAllPositions() const
	{
		return _pos;
	}

	Position GetPosition(int p)
	{
		return _pos[p];
	}

	void SetAllPositions(const std::vector<Position>& pos)
	{
		_pos = pos;
	}

	void SetHeadPosition(const Position pos)
	{
		_pos[HEAD].x = pos.x;
		_pos[HEAD].y = pos.y;
	}

	void SetBodyPositions(const std::vector<Position>& pos)
	{
		for (auto& bp : pos)
			_pos.push_back(bp);
	}

	void SetPosition(int p, const Position pos)
	{
		_pos[p].x = pos.x;
		_pos[p].y = pos.y;
	}

	void Move()
	{
		for (size_t i = _pos.size() - 1; i > 0; i--)
			_pos[i] = _pos[i - 1];
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
	bool _isRunning = true;
	Position _oldTailPosition{};

public:
	explicit Game() : _snake{ {{2, 10}, {2, 11}} }, _currentDirection{ Direction::NONE }, _gameState{ GameState::PAUSE }
	{
		if (!CheckBounds(_snake.GetPosition(HEAD)))
		{
			std::cerr << "Not gud position\n";
			_snake.SetAllPositions({ {2, 10}, {2, 11} });
		}
	}

	Direction RandomDirection()
	{
		std::random_device dev;
		std::mt19937 rng(dev());

		std::uniform_int_distribution<int> distrib(static_cast<int>(Direction::UP), static_cast<int>(Direction::RIGHT));

		return static_cast<Direction>(distrib(rng));
	}

	bool CheckBounds(const Position& pos)
	{
		if (pos.x < 2 || pos.x > _map.GetWidth() - 3)
			return false;

		if (pos.y < 2 || pos.y > _map.GetHeight() - 3)
			return false;

		return true;
	}

	Position RandomPosition()
	{
		std::random_device dev;
		std::mt19937 rng(dev());

		std::uniform_int_distribution<int> distX(2, _map.GetWidth() - 3);
		std::uniform_int_distribution<int> distY(2, _map.GetHeight() - 3);

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

	void Move()
	{
		if (!CheckBounds(_snake.GetPosition(HEAD)))
		{
			_gameState = GameState::GAME_OVER;
			return;
		}

		_snake.Move();

		switch (_currentDirection)
		{
		case Direction::UP:
			_snake.SetPosition(HEAD, { _snake.GetPosition(HEAD).x, _snake.GetPosition(HEAD).y - 1 });
			break;
		case Direction::DOWN:
			_snake.SetPosition(HEAD, { _snake.GetPosition(HEAD).x, _snake.GetPosition(HEAD).y + 1 });
			break;
		case Direction::LEFT:
			_snake.SetPosition(HEAD, { _snake.GetPosition(HEAD).x - 1, _snake.GetPosition(HEAD).y });
			break;
		case Direction::RIGHT:
			_snake.SetPosition(HEAD, { _snake.GetPosition(HEAD).x + 1, _snake.GetPosition(HEAD).y });
			break;
		default:
			break;
		}

		if (_snake.GetAllPositions().size() > 2)
			_snake.RemoveTail();
	}

	void GameOver()
	{
		_gameState = GameState::GAME_OVER;
	}

	void RestartGame()
	{
		_gameState = GameState::PAUSE;
		_currentDirection = Direction::NONE;
		_snake.ResetPositions();

		Position headPos = RandomPosition();
		_snake.SetAllPositions({ headPos, {headPos.x, headPos.y + 1} });
	}

	void UpdateGameplay(double dt)
	{
		static double moveTimer = 0.0;
		moveTimer += dt;

		if (moveTimer >= 0.15)
		{
			_oldTailPosition = _snake.GetTail();
			Move();
			moveTimer = 0.0;
		}
	}

	void Update(double deltaTime)
	{
		switch (_gameState)
		{
		case GameState::PLAYING:
			return UpdateGameplay(deltaTime);
		case GameState::PAUSE:
		case GameState::GAME_OVER:
			return;
		case GameState::RESTART:
			return RestartGame();
		default:
			return;
		}
	}

	void PrintMap(const std::vector<std::string>& map, std::string& buffer)
	{
		for (auto& line : map)
			buffer += line + '\n';
	}

	void Render()
	{
		static bool initialDraw = true;
		if (_gameState == GameState::GAME_OVER)
		{
			std::string buffer;
			buffer += "\033[2J\033[H";
			PrintMap(_map.GetGameOverMap(), buffer);
			std::cout << buffer << std::flush;
			initialDraw = true;
			return;
		}

		if (_gameState == GameState::PAUSE)
		{
			std::string buffer;
			buffer += "\033[2J\033[H";
			PrintMap(_map.GetGameMenuMap(), buffer);
			std::cout << buffer << std::flush;
			initialDraw = true;
			return;
		}

		if (initialDraw)
		{
			std::string buffer;
			buffer += "\033[2J\033[H";
			PrintMap(_map.GetMap(), buffer);
			std::cout << buffer << std::flush;
			for (int i = 0; i < _snake.GetAllPositions().size(); i++)
				std::cout << "\033[" << _snake.GetPosition(i).y + 1 << ";" << _snake.GetPosition(i).x + 1 << "H" << (i == HEAD ? "S" : "T");
			initialDraw = false;
			std::cout << std::flush;
		}
		else
		{
			// erase old tail position
			if (_currentDirection != Direction::NONE)
				std::cout << "\033[" << _oldTailPosition.y + 1 << ";" << _oldTailPosition.x + 1 << "H" << " ";

			for (int i = 1; i < _snake.GetAllPositions().size(); i++)
				std::cout << "\033[" << _snake.GetPosition(1).y + 1 << ";" << _snake.GetPosition(1).x + 1 << "H" << "T";

			std::cout << "\033[" << _snake.GetPosition(HEAD).y + 1 << ";" << _snake.GetPosition(HEAD).x + 1 << "H" << "S";
			std::cout << std::flush;
		}

		// set cursor at the bottom to avoid text overwriting
		std::cout << "\033[" << _map.GetHeight() + 2 << ";1H";
	}
	
	void Run()
	{
		static auto lastTime = std::chrono::high_resolution_clock::now();
		static int frameCount = 0;
		static double timeAccumulated = 0.0;

		while (IsRunning())
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			double deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
			lastTime = currentTime;

			ProcessInput();
			Update(deltaTime);
			Render();

			timeAccumulated += deltaTime;
			frameCount++;

			if (timeAccumulated >= 0.5)
			{
				std::cout << "\rFPS: " << static_cast<double>(frameCount) / timeAccumulated << std::flush;
				frameCount = 0;
				timeAccumulated = 0.0;
			}

			const auto elapsed = std::chrono::high_resolution_clock::now() - currentTime;
			auto targetFrameTime = std::chrono::duration<double>(TARGET_FRAME_TIME);
			if (elapsed < targetFrameTime)
				std::this_thread::sleep_for(targetFrameTime - elapsed);

		}
	}
};

#pragma endregion

int main()
{
	Game game;
	game.Run();
}
