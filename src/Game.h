#ifndef GAME_H
#define GAME_H

#include "Map.h"
#include "Snake.h"
#include "GameState.h"
#include "Direction.h"
#include "Position.h"

class Game
{
private:
	Map _map;
	GameState _gameState;
	Direction _currentDirection;
	Snake _snake;

	bool _hasFirstInput;
	bool _isRunning;
	Position _oldTailPosition;
	double _moveTimer;

public:
	explicit Game();

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
	void PrintMap(const std::vector<std::string>& map, std::string& buffer);

	void Update(double deltaTime);
	void Render();
	void Run();
};

#endif
