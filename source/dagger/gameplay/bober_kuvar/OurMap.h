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
	std::vector<std::pair<int, int>> all_doors_coords_;
	std::vector<std::vector<int>> matrix_;
	std::vector<Room*> rooms_;

	void generateMap();
	void make_wall_matrix();
	void fill_rooms();
	void rearange_interior();
	void print_matrix_in_console(std::vector<std::vector<int>> matrix);
	void fill(int x, int y, int* min_x, int* min_y, int* max_x, int* max_y, std::vector<std::pair<int, int>> *doors_coords);
};

