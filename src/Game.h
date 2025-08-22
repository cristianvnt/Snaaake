#ifndef GAME_H
#define GAME_H

#include "Map.h"
#include "Snake.h"
#include "GameState.h"
#include "Direction.h"
#include "Position.h"
#include "Food.h"

class Game
{
public:
	Game();

	// food
	void SpawnFood();
	Position RandomFoodPosition();

	bool CheckBounds(const Position& pos);
	Direction RandomDirection();
	Position RandomHeadPosition();
	std::vector<Position> RandomSnakeDefaultPosition();
	bool IsOppositeDirection(Direction curr, Direction newDir);
	Position GetNextHeadPosition(Direction dir);

	void HandlePlayingInputStates(char input);
	void ProcessInput();
	bool IsRunning() const;
	bool MoveSnake();
	void GameOver();
	void RestartGame();
	void UpdateGameplay(double dt);
	void PrintMap(const std::array<std::string_view, C::MAP_HEIGHT>& map, std::ostringstream& buffer);

	void Update(double deltaTime);
	void Render();
	void Run();

private:
	GameState _gameState{ GameState::PAUSE };
	Direction _currentDirection{};
	Snake _snake{ std::vector<Position>({{1, 1}, {1, 2}, {1, 3}, {1, 4}}) };
	Food _food{ {0, 0} };

	bool _hasFirstInput{ false };
	bool _isRunning{ true };
	Position _oldTailPosition{};
	double _moveTimer{ 0.0 };
};

#endif
