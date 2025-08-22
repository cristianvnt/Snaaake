#ifndef FOOD_H
#define FOOD_H

#include "Position.h"

class Food
{
public:
	explicit Food(Position pos) : _pos{ pos } {}

	void SetPosition(const Position& pos);
	Position GetPosition() const;

private:
	Position _pos{};
};

#endif
