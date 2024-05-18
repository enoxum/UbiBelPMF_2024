#pragma once
#include "core/core.h"
#include "Room.h"
class OurMap
{
public:
	OurMap(int n, int room_size);

private:
	int n_;
	int room_size_;
	std::vector<std::pair<int, int>> doorways_;
	std::vector<std::vector<int>> matrix_;
	std::vector<std::vector<Room>> rooms_;


	void generateMap();
	void make_walls();
	void fill_rooms();
	void print_matrix_in_console(std::vector<std::vector<int>> matrix);
};

