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
constexpr int DEFAULT_SNAKE_LENGTH = 3;

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

	void ClearPositions()
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
		if (_pos.empty())
		{
			std::cerr << "Cant set snake head since there isnt one\n";
			return;
		}

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

	Direction _spawnDirection;
	Direction _currentDirection;
	bool _hasFirstInput;

	bool _isRunning;
	Position _oldTailPosition{};

public:
	explicit Game() : _snake{ RandomSnakeDefaultPosition() }, _spawnDirection{ }, _currentDirection { }, _gameState{ GameState::PAUSE },
		_isRunning{ true }, _hasFirstInput{ false }
	{
		if (!CheckBounds(_snake.GetPosition(HEAD)))
		{
			std::cerr << "Not gud position\n";
		}
	}

	bool CheckBounds(const Position& pos)
	{
		if (pos.x < 2 || pos.x > _map.GetWidth() - 3)
			return false;

		if (pos.y < 2 || pos.y > _map.GetHeight() - 3)
			return false;

		return true;
	}

	Direction RandomDirection()
	{
		std::random_device dev;
		std::mt19937 rng(dev());

		std::uniform_int_distribution<int> distrib(static_cast<int>(Direction::UP), static_cast<int>(Direction::RIGHT));

		return static_cast<Direction>(distrib(rng));
	}

	Position RandomHeadPosition()
	{
		std::random_device dev;
		std::mt19937 rng(dev());

		std::uniform_int_distribution<int> distX(3, _map.GetWidth() - 4);
		std::uniform_int_distribution<int> distY(3, _map.GetHeight() - 4);

		return { distX(rng), distY(rng) };
	}

	std::vector<Position> RandomSnakeDefaultPosition()
	{
		Position head = RandomHeadPosition();
		Position bodyP1{}, bodyP2{};
		_spawnDirection = RandomDirection();

		switch (_spawnDirection)
		{
		case Direction::UP:
			bodyP1 = { head.x, head.y + 1 };
			bodyP2 = { head.x, head.y + 2 };
			break;
		case Direction::DOWN:
			bodyP1 = { head.x, head.y - 1 };
			bodyP2 = { head.x, head.y - 2 };
			break;
		case Direction::LEFT:
			bodyP1 = { head.x + 1, head.y };
			bodyP2 = { head.x + 2, head.y };
			break;
		case Direction::RIGHT:
			bodyP1 = { head.x - 1, head.y };
			bodyP2 = { head.x - 2, head.y };
			break;
		default:
			bodyP1 = { head.x, head.y + 1 };
			bodyP2 = { head.x, head.y + 2 };
			break;
		}

		_currentDirection = _spawnDirection;
		_hasFirstInput = false;
		return { head, bodyP1, bodyP2 };
	}

	bool IsOppositeDirection(Direction curr, Direction newDir)
	{
		return (curr == Direction::UP && newDir == Direction::DOWN ||
			curr == Direction::LEFT && newDir == Direction::RIGHT ||
			curr == Direction::DOWN && newDir == Direction::UP ||
			curr == Direction::RIGHT && newDir == Direction::LEFT);
	}

	void HandlePlayingInputStates(char input)
	{
		if (input == 'p')
		{
			_gameState = GameState::PAUSE;
			return;
		}

		if (input == 'r')
		{
			_gameState = GameState::RESTART;
			return;
		}

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
		}

		if (!_hasFirstInput)
		{
			if (!IsOppositeDirection(_spawnDirection, newDirection))
			{
				_currentDirection = newDirection;
				_hasFirstInput = true;
			}
		}
		else
		{
			if (!IsOppositeDirection(_currentDirection, newDirection))
				_currentDirection = newDirection;
		}
	}

	void ProcessInput()
	{
		if (_kbhit())
		{
			static auto lastInputTime = std::chrono::steady_clock::now();
			auto now = std::chrono::steady_clock::now();

			if (_gameState != GameState::PLAYING && std::chrono::duration<double>(now - lastInputTime).count() < 0.1)
				return;

			char input = _getch();
			lastInputTime = now;

			if (input == 'q')
			{
				_isRunning = false;
				return;
			}

			switch (_gameState)
			{
			case GameState::PLAYING:
				HandlePlayingInputStates(input);
				break;
			case GameState::PAUSE:
				if (input == 13)
					_gameState = GameState::PLAYING;
				break;
			case GameState::GAME_OVER:
				if (input == 'r')
					RestartGame();
				break;
			case GameState::RESTART:
				_hasFirstInput = false;
				break;
			default:
				break;
			}
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

		if (_snake.GetAllPositions().size() > DEFAULT_SNAKE_LENGTH)
			_snake.RemoveTail();
	}

	void GameOver()
	{
		_gameState = GameState::GAME_OVER;
		_hasFirstInput = false;
	}

	void RestartGame()
	{
		_gameState = GameState::PAUSE;
		_hasFirstInput = false;
		_snake.ClearPositions();
		_snake.SetAllPositions(RandomSnakeDefaultPosition());
	}

	void UpdateGameplay(double dt)
	{
		if (_gameState != GameState::PLAYING)
			return;

		if (!_hasFirstInput && _spawnDirection == _currentDirection)
			return;

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
		std::string buffer;
		buffer.reserve(1024);
		buffer += "\033[?25l";

		if (_gameState == GameState::GAME_OVER)
		{
			buffer += "\033[2J\033[H";
			PrintMap(_map.GetGameOverMap(), buffer);
			std::cout << buffer << std::flush;
			return;
		}

		if (_gameState == GameState::PAUSE)
		{
			buffer += "\033[2J\033[H";
			PrintMap(_map.GetGameMenuMap(), buffer);
			std::cout << buffer << std::flush;
			return;
		}

		if (!_hasFirstInput)
		{
			buffer += "\033[2J\033[H";
			PrintMap(_map.GetMap(), buffer);

			for (int i = 0; i < _snake.GetAllPositions().size(); i++)
				buffer += "\033[" + std::to_string(_snake.GetPosition(i).y + 1) + ";" + 
					std::to_string(_snake.GetPosition(i).x + 1) + "H" + (i == HEAD ? "S" : "T");
		}
		else
		{
			buffer += "\033[2J\033[H";
			PrintMap(_map.GetMap(), buffer);

			// erase old tail position
			buffer += "\033[" + std::to_string(_oldTailPosition.y + 1) + ";" + std::to_string(_oldTailPosition.x + 1) + "H" + " ";

			for (int i = 0; i < _snake.GetAllPositions().size(); i++)
				buffer += "\033[" + std::to_string(_snake.GetPosition(i).y + 1) + ";" + std::to_string(_snake.GetPosition(i).x + 1) + "H" + (i == HEAD ? "S" : "T");
		}

		// set cursor at the bottom to avoid text overwriting
		buffer += "\033[" + std::to_string(_map.GetHeight() + 2) + ";1H";
		std::cout << buffer << std::flush;
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
