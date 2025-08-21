#include "Food.h"

void Food::SetPosition(const Position& pos)
{
	_pos = pos;
}

Position Food::GetPosition() const
{
	return _pos;
}

