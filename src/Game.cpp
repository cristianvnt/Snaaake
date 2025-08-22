#include "Game.h"

#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <vector>
#include <conio.h>
#include <random>

constexpr double TARGET_FRAME_TIME = 1.0 / 60.0;
constexpr int DEFAULT_SNAKE_LENGTH = 3;
constexpr int HEAD = 0;

Game::Game() : _currentDirection{ RandomDirection() }, _snake{ RandomSnakeDefaultPosition() }, _oldTailPosition{ _snake.GetTail() }
{
	for (const auto& pos : _snake.GetBodyPositions())
	{
		if (!CheckBounds(_snake.GetPosition(HEAD)))
		{
			std::cerr << "Not gud position\n";
			_gameState = GameState::GAME_OVER;
			break;
		}
	}
}

bool Game::CheckBounds(const Position& pos)
{
	if (pos.x < 1 || pos.x > Map::WIDTH - 2)
		return false;

	if (pos.y < 1 || pos.y > Map::HEIGHT - 2)
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

	std::uniform_int_distribution<int> distX(5, Map::WIDTH - 5);
	std::uniform_int_distribution<int> distY(5, Map::HEIGHT - 5);

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
		for (int i = 1; i <= DEFAULT_SNAKE_LENGTH; i++)
			defaultBody.push_back({ head.x, head.y + i });
		break;
	case Direction::DOWN:
		for (int i = 1; i <= DEFAULT_SNAKE_LENGTH; i++)
			defaultBody.push_back({ head.x, head.y - i });
		break;
	case Direction::LEFT:
		for (int i = 1; i <= DEFAULT_SNAKE_LENGTH; i++)
			defaultBody.push_back({ head.x + i, head.y });
		break;
	case Direction::RIGHT:
		for (int i = 1; i <= DEFAULT_SNAKE_LENGTH; i++)
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

	if (!CheckBounds(newHead))
		return false;

	if (_snake.CheckSelfCollision(newHead))
		return false;

	_oldTailPosition = _snake.GetTail();

	_snake.Move();

	_snake.SetPosition(HEAD, newHead);

	return true;
}

void Game::GameOver()
{
	_gameState = GameState::GAME_OVER;
	_hasFirstInput = false;
}

void Game::RestartGame()
{
	_gameState = GameState::PAUSE;
	_hasFirstInput = false;
	_moveTimer = 0.0;
	_snake.ClearPositions();
	_snake.SetBodyPositions(RandomSnakeDefaultPosition());
}

void Game::UpdateGameplay(double dt)
{
	if (_gameState != GameState::PLAYING)
		return;

	if (!_hasFirstInput)
		return;

	_moveTimer += dt;

	if (_moveTimer >= 0.15)
	{
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

void Game::PrintMap(const std::array<std::string_view, Map::HEIGHT>& map, std::ostringstream& buffer)
{
	for (auto& line : map)
		buffer << line << '\n';
}

void Game::Render()
{
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
	for (size_t i = 0; i < bodyPos.size(); i++)
		buffer << "\033[" << (bodyPos[i].y + 1) << ";" << (bodyPos[i].x + 1) << "H" << (i == HEAD ? "S" : "T");

	buffer << "\033[" + std::to_string(Map::HEIGHT + 2) + ";1H";
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