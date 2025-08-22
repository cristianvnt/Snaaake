#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "Position.h"
#include <utility>

class Snake
{
public:
	explicit Snake(std::vector<Position> pos) : _pos{ std::move(pos) } {}

	Snake(const Snake&) = default;
	Snake& operator=(const Snake&) = default;

	Snake(Snake&&) noexcept = default;
	Snake& operator=(Snake&&) noexcept = default;

	~Snake() = default;

	bool CheckSelfCollision(const Position& newHeadPos) const;
	Position GetHead() const;
	Position GetTail() const;
	void RemoveTail();
	void ClearPositions();
	const std::vector<Position>& GetBodyPositions() const;
	Position GetPosition(int p);
	void SetBodyPositions(const std::vector<Position>& pos);
	void SetPosition(int p, const Position& pos);
	void Move();

private:
	std::vector<Position> _pos;
};

#endif
