#pragma once
#include "core/core.h"
#include "Room.h"
class OurMap
{
private:
	std::list<Room> rooms;

	void generate_map();
};

