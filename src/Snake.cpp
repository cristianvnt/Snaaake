#include "Snake.h"

constexpr int HEAD = 0;

void Snake::Move()
{
	for (size_t i = _pos.size() - 1; i > HEAD; i--)
		_pos[i] = _pos[i - 1];
}

bool Snake::CheckSelfCollision(const Position& newHeadPos) const
{
	for (size_t i = 1; i < _pos.size(); i++)
		if (_pos[i] == newHeadPos)
			return true;
	return false;
}

Position Snake::GetHead() const
{
	return _pos.front();
}

Position Snake::GetTail() const
{
	return _pos.back();
}

void Snake::RemoveTail()
{
	_pos.pop_back();
}

void Snake::ClearPositions()
{
	if (!_pos.empty())
		_pos.clear();
}

const std::vector<Position>& Snake::GetBodyPositions() const
{
	return _pos;
}

Position Snake::GetPosition(int p)
{
	return _pos[p];
}

void Snake::SetBodyPositions(const std::vector<Position>& pos)
{
	for (auto& bp : pos)
		_pos.push_back(bp);
}

void Snake::SetPosition(int p, const Position& pos)
{
	_pos[p].x = pos.x;
	_pos[p].y = pos.y;
}