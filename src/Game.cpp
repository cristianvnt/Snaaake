#include "Game.h"

#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <vector>
#include <conio.h>
#include <random>

Game::Game() : _currentDirection{ RandomDirection() }, _snake{ RandomSnakeDefaultPosition() }, 
_oldTailPosition{ _snake.GetTail() }, _food{ RandomFoodPosition() }
{
	for (const auto& pos : _snake.GetBodyPositions())
	{
		if (!CheckBounds(_snake.GetPosition(C::HEAD)))
		{
			std::cerr << "Not gud position\n";
			_gameState = GameState::GAME_OVER;
			break;
		}
	}
}

Position Game::RandomFoodPosition()
{
	std::random_device dev;
	std::mt19937 rng(dev());

	std::uniform_int_distribution<int> distX(1, C::MAP_WIDTH - 2);
	std::uniform_int_distribution<int> distY(1, C::MAP_HEIGHT - 2);

	Position foodPos{};
	bool validFoodPos = false;

	while (!validFoodPos)
	{
		foodPos = { distX(rng), distY(rng) };
		validFoodPos = true;

		for (const auto& pos : _snake.GetBodyPositions())
		{
			if (pos == foodPos)
			{
				validFoodPos = false;
				break;
			}
		}
	}

	return foodPos;
}

bool Game::CheckBounds(const Position& pos)
{
	if (pos.x < 1 || pos.x > C::MAP_WIDTH - 2)
		return false;

	if (pos.y < 1 || pos.y > C::MAP_HEIGHT - 2)
		return false;

	return true;
}

Direction Game::RandomDirection()
{
	std::random_device dev;
	std::mt19937 rng(dev());

	std::uniform_int_distribution<int> distrib(static_cast<int>(Direction::UP), static_cast<int>(Direction::RIGHT));

	return static_cast<Direction>(distrib(rng));
}

Position Game::RandomHeadPosition()
{
	std::random_device dev;
	std::mt19937 rng(dev());

	std::uniform_int_distribution<int> distX(C::MARGIN, C::MAP_WIDTH - C::MARGIN);
	std::uniform_int_distribution<int> distY(C::MARGIN, C::MAP_HEIGHT - C::MARGIN);

	return { distX(rng), distY(rng) };
}

std::vector<Position> Game::RandomSnakeDefaultPosition()
{
	Position head = RandomHeadPosition();
	std::vector<Position> defaultBody;
	defaultBody.push_back(head);

	switch (_currentDirection)
	{
	case Direction::UP:
		for (int i = 1; i <= C::SNAKE_DEF_LEN; i++)
			defaultBody.push_back({ head.x, head.y + i });
		break;
	case Direction::DOWN:
		for (int i = 1; i <= C::SNAKE_DEF_LEN; i++)
			defaultBody.push_back({ head.x, head.y - i });
		break;
	case Direction::LEFT:
		for (int i = 1; i <= C::SNAKE_DEF_LEN; i++)
			defaultBody.push_back({ head.x + i, head.y });
		break;
	case Direction::RIGHT:
		for (int i = 1; i <= C::SNAKE_DEF_LEN; i++)
			defaultBody.push_back({ head.x - i, head.y });
		break;
	}

	return defaultBody;
}

bool Game::IsOppositeDirection(Direction curr, Direction newDir)
{
	return (curr == Direction::UP && newDir == Direction::DOWN) ||
		(curr == Direction::LEFT && newDir == Direction::RIGHT) ||
		(curr == Direction::DOWN && newDir == Direction::UP) ||
		(curr == Direction::RIGHT && newDir == Direction::LEFT);
}

Position Game::GetNextHeadPosition(Direction dir)
{
	Position head = _snake.GetHead();
	switch (dir)
	{
	case Direction::UP:
		return { head.x, head.y - 1 };
		break;
	case Direction::DOWN:
		return { head.x, head.y + 1 };
		break;
	case Direction::LEFT:
		return { head.x - 1, head.y };
		break;
	case Direction::RIGHT:
		return { head.x + 1, head.y };
		break;
	default:
		return head;
	}
}

void Game::HandlePlayingInputStates(char input)
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
	default:
		return;
	}

	if (IsOppositeDirection(_currentDirection, newDirection))
		return;

	_currentDirection = newDirection;

	if (!_hasFirstInput)
	{
		_hasFirstInput = true;
		std::cout << "YAAAAAy!\n";
	}
}

void Game::ProcessInput()
{
	// windows specific
	// checks if keyboard was pressed
	if (_kbhit())
	{
		static auto lastInputTime = std::chrono::steady_clock::now();
		auto now = std::chrono::steady_clock::now();

		if (_gameState != GameState::PLAYING && std::chrono::duration<double>(now - lastInputTime).count() < 0.1)
			return;

		// gets the key pressed w/o echo
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
				_gameState = GameState::RESTART;
			break;
		case GameState::RESTART:
			break;
		default:
			break;
		}
	}
}

bool Game::IsRunning() const
{
	return _isRunning;
}

bool Game::MoveSnake()
{
	Position newHead = GetNextHeadPosition(_currentDirection);

	if (!CheckBounds(newHead) || _snake.CheckSelfCollision(newHead))
		return false;

	_oldTailPosition = _snake.GetTail();

	_snake.Move();

	_snake.SetPosition(C::HEAD, newHead);

	return true;
}

void Game::GameOver()
{
	_gameState = GameState::GAME_OVER;
	_hasFirstInput = false;
	_score = 0;
}

void Game::RestartGame()
{
	_gameState = GameState::PAUSE;
	_hasFirstInput = false;
	_moveTimer = 0.0;
	_score = 0;
	_snake.ClearPositions();
	_snake.SetBodyPositions(RandomSnakeDefaultPosition());
	_food.SetPosition(RandomFoodPosition());
}

void Game::UpdateGameplay(double dt)
{
	if (_gameState != GameState::PLAYING)
		return;

	if (!_hasFirstInput)
		return;

	_moveTimer += dt;

	if (_moveTimer >= C::MOVE_DELAY)
	{
		if (_snake.GetHead() == _food.GetPosition())
		{
			_food.SetPosition(RandomFoodPosition());
			_snake.Grow();
			_score += C::PTS;
		}

		if (!MoveSnake())
			_gameState = GameState::GAME_OVER;

		_moveTimer = 0.0;
	}
}

void Game::Update(double deltaTime)
{
	switch (_gameState)
	{
	case GameState::PLAYING:
		return UpdateGameplay(deltaTime);
	case GameState::RESTART:
		return RestartGame();
	case GameState::PAUSE:
	case GameState::GAME_OVER:
	default:
		return;
	}
}

void Game::PrintMap(const std::array<std::string_view, C::MAP_HEIGHT>& map, std::ostringstream& buffer)
{
	for (const auto& line : map)
		buffer << line << '\n';
}

void Game::Render()
{
	// ansi codes to clear screen, make cursor invisible, 
	// move cursor below map for FPS counter so it doesnt overlap
	std::ostringstream buffer;
	buffer << "\033[?25l";
	buffer << "\033[2J\033[H";

	if (_gameState == GameState::GAME_OVER)
	{
		buffer << "\033[2J\033[H";
		PrintMap(Map::GAME_OVER_MAP, buffer);
		std::cout << buffer.str() << std::flush;
		return;
	}

	if (_gameState == GameState::PAUSE)
	{

		PrintMap(Map::GAME_MENU_MAP, buffer);
		std::cout << buffer.str() << std::flush;
		return;
	}

	PrintMap(Map::MAP, buffer);

	const auto& bodyPos = _snake.GetBodyPositions();
	const auto& foodPos = _food.GetPosition();

	for (size_t i = 0; i < bodyPos.size(); i++)
		buffer << "\033[" << (bodyPos[i].y + 1) << ";" << (bodyPos[i].x + 1) << "H" << (i == C::HEAD ? C::SNAKE_HEAD : C::SNAKE_TAIL);

	buffer << "\033[" << (foodPos.y + 1) << ";" << (foodPos.x + 1) << "H" << C::FOOD;

	buffer << "\033[" << (C::MAP_HEIGHT + 1) << ";1H";
	buffer << "Score: " << _score << " ";

	buffer << "\033[" << (C::MAP_HEIGHT + 2) << ";1H";
	buffer << "FPS: " << _fps;

	std::cout << buffer.str() << std::flush;
}

void Game::Run()
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

		if (timeAccumulated >= 0.01)
		{
			_fps = static_cast<double>(frameCount) / timeAccumulated;
			frameCount = 0;
			timeAccumulated = 0.0;
		}

		const auto elapsed = std::chrono::high_resolution_clock::now() - currentTime;
		auto targetFrameTime = std::chrono::duration<double>(C::FRAME_TIME);
		if (elapsed < targetFrameTime)
			std::this_thread::sleep_for(targetFrameTime - elapsed);
	}
}