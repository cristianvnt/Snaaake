#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "Position.h"

class Snake
{
private:
	std::vector<Position> _pos;

public:
	explicit Snake(const std::vector<Position>& pos) : _pos{ pos } {}

	bool CheckSelfCollision(const Position& newHeadPos) const;
	Position GetHead();
	Position GetTail();
	void RemoveTail();
	void ClearPositions();
	const std::vector<Position> GetBodyPositions() const;
	Position GetPosition(int p);
	void SetBodyPositions(const std::vector<Position>& pos);
	void SetPosition(int p, const Position pos);
	void Move();
};

#endif
