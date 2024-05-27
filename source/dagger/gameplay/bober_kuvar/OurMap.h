#pragma once
#include "core/core.h"
#include "Room.h"
#include "Tile.h"

class OurMap
{
public:
	OurMap(int n, int room_size);
	int get_n();
	std::vector<std::vector<int>> get_matrix();
	std::vector<Room*> get_rooms();

private:
	int n_;
	int room_size_;
	std::vector<std::pair<int, int>> all_doors_coords_;
	std::vector<std::vector<int>> matrix_;
	std::vector<Room*> rooms_;
	std::vector<std::vector<Tile*>> tile_matrix_;

	void generateMap();
	void make_wall_matrix();
	void fill_rooms();
	void rearange_interior();
	void paint_map();
	int calculate_distance(std::pair<int, int> a, std::pair<int, int> b);
	void print_matrix_in_console(std::vector<std::vector<int>> matrix);
	void fill(int x, int y, int* min_x, int* min_y, int* max_x, int* max_y, std::vector<std::pair<int, int>> *doors_coords);
};

