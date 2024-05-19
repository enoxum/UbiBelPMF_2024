#include "OurMap.h"
#include <random>;

OurMap::OurMap(int n, int room_size)
{
	n_ = n;
	room_size_ = room_size;
	matrix_ = std::vector<std::vector<int>>(n_, std::vector<int>(n_, 0));
	generateMap();
}

void OurMap::generateMap()
{
	make_wall_matrix();
	fill_rooms();
	rearange_interior();
}

void OurMap::make_wall_matrix()
{
	std::vector<std::vector<int>> horizontal_restriction_matrix(n_, std::vector<int>(n_, 0));
	std::vector<std::vector<int>> vertical_restriction_matrix(n_, std::vector<int>(n_, 0));

	for (int j = 0; j < n_; j++) {
		for (int i = 0; i < n_; i++) {
			if (i == 0) {
				matrix_[j][i] = 1;
				for (int k = 0; k < room_size_; k++)
					vertical_restriction_matrix[j][i + k] = 1;
			}
			if (j == 0) {
				matrix_[j][i] = 1;
				for (int k = 0; k < room_size_; k++)
					horizontal_restriction_matrix[j + k][i] = 1;
			}
			if (i == n_-1) {
				matrix_[j][i] = 1;
				for (int k = 0; k < room_size_; k++)
					vertical_restriction_matrix[j][i - k] = 1;
			}
			if (j == n_-1) {
				matrix_[j][i] = 1;
				for (int k = 0; k < room_size_; k++)
					horizontal_restriction_matrix[j - k][i] = 1;
			}
		}
	}

	int tries = 0;
	int max_tries = 10;
	while (tries < max_tries) {
		int direction = rand() % 2;
		int random_x = rand() % (n_ - 1) + 1;
		int random_y = rand() % (n_ - 1) + 1;

		//horizontal
		if (direction == 0) {
			if (matrix_[random_y][random_x] == 0 && horizontal_restriction_matrix[random_y][random_x] == 0) {
				tries = 0;

				//left
				int copy_x = random_x;
				while (matrix_[random_y][copy_x] != 1) {
					matrix_[random_y][copy_x] = 1;
					for (int k = 0; k < room_size_; k++) {
						horizontal_restriction_matrix[random_y + k][copy_x] = 1;
						horizontal_restriction_matrix[random_y - k][copy_x] = 1;
					}
					copy_x--;
				}
				for (int k = 0; k < room_size_; k++) {
					horizontal_restriction_matrix[random_y + k][copy_x] = 1;
					horizontal_restriction_matrix[random_y - k][copy_x] = 1;
				}
				int min_x = copy_x + 1;

				//right
				copy_x = random_x + 1;
				while (matrix_[random_y][copy_x] != 1) {
					matrix_[random_y][copy_x] = 1;
					for (int k = 0; k < room_size_; k++) {
						horizontal_restriction_matrix[random_y + k][copy_x] = 1;
						horizontal_restriction_matrix[random_y - k][copy_x] = 1;
					}
					copy_x++;
				}
				for (int k = 0; k < room_size_; k++) {
					horizontal_restriction_matrix[random_y + k][copy_x] = 1;
					horizontal_restriction_matrix[random_y - k][copy_x] = 1;
				}

				//door
				int max_x = copy_x - 1;
				int random_doorway_x = rand() % (max_x - min_x) + min_x;
				matrix_[random_y][random_doorway_x] = 2;
				all_doors_coords_.push_back(std::pair<int, int>(random_y, random_doorway_x));
			}
			else
				tries++;
		}
		//vertical
		else {
			if (matrix_[random_y][random_x] == 0 && vertical_restriction_matrix[random_y][random_x] == 0) {
				tries = 0;

				//up
				int copy_y = random_y;
				while (matrix_[copy_y][random_x] != 1) {
					matrix_[copy_y][random_x] = 1;
					for (int k = 0; k < room_size_; k++) {
						vertical_restriction_matrix[copy_y][random_x + k] = 1;
						vertical_restriction_matrix[copy_y][random_x - k] = 1;
					}
					copy_y--;
				}
				for (int k = 0; k < room_size_; k++) {
					vertical_restriction_matrix[copy_y][random_x + k] = 1;
					vertical_restriction_matrix[copy_y][random_x - k] = 1;
				}
				int min_y = copy_y + 1;

				//down
				copy_y = random_y + 1;
				while (matrix_[copy_y][random_x] != 1) {
					matrix_[copy_y][random_x] = 1;
					for (int k = 0; k < room_size_; k++) {
						vertical_restriction_matrix[copy_y][random_x + k] = 1;
						vertical_restriction_matrix[copy_y][random_x - k] = 1;
					}
					copy_y++;
				}
				for (int k = 0; k < room_size_; k++) {
					vertical_restriction_matrix[copy_y][random_x + k] = 1;
					vertical_restriction_matrix[copy_y][random_x - k] = 1;
				}

				//door
				int max_y = copy_y - 1;
				int random_doorway_y = rand() % (max_y - min_y) + min_y;
				matrix_[random_doorway_y][random_x] = 2;
				all_doors_coords_.push_back(std::pair<int, int>(random_doorway_y, random_x));
			}
			else
				tries++;
		}
	}

	print_matrix_in_console(matrix_);
	//print_matrix_in_console(horizontal_restriction_matrix);
	//print_matrix_in_console(vertical_restriction_matrix);
	//for (int i = 0; i < all_doors_coords_.size(); i++) {
	//	Logger::error(all_doors_coords_[i].second);
	//	Logger::error(all_doors_coords_[i].first);
	//	Logger::error("");
	//}
}

void OurMap::fill_rooms()
{
	int room_id = 0;
	for (int j = 1; j < n_ - 1; j++) {
		for (int i = 1; i < n_ - 1; i++) {
			int min_x = n_;
			int min_y = n_;
			int max_x = 0;
			int max_y = 0;
			std::vector<std::pair<int, int>> doors_coords;
			if (matrix_[j][i] == 0) {
				fill(i, j, &min_x, &min_y, &max_x, &max_y, &doors_coords);
				Room *room = new Room(room_id, std::pair<int, int>(min_y, min_x), std::pair<int, int>(max_y, max_x), doors_coords);
				room_id++;
				rooms_.push_back(room);
			}
		}
	}

	print_matrix_in_console(matrix_);
}

void OurMap::rearange_interior()
{
	for (auto room : rooms_) {
		std::pair<int, int> topLeft = (*room).getTopLeft();
		std::pair<int, int> bottomRight = (*room).getBottomRight();
		std::pair<int, int> center = std::pair<int, int>((bottomRight.first + topLeft.first)/2, (bottomRight.second + topLeft.second)/2);

		std::vector<std::pair<int, int>> doors_coords = (*room).getDoorsCoords();
		int door_number = doors_coords.size();
		int max_distance = 0;
		std::vector<int> distances(door_number);
		for (int k = 0; k < door_number; k++) {
			int distance = sqrt((doors_coords[k].first - center.first) * (doors_coords[k].first - center.first) + (doors_coords[k].second - center.second) * (doors_coords[k].second - center.second));
			distances[k] = distance;
			if (max_distance < distance)
				max_distance = distance;
		}

		for (int j = topLeft.first; j <= bottomRight.first; j++) {
			for (int i = topLeft.second; i <= bottomRight.second; i++) {
				//distance from current point to the door
				for (int k = 0; k < door_number; k++) {
					int distance = sqrt((doors_coords[k].first - j) * (doors_coords[k].first - j) + (doors_coords[k].second - i) * (doors_coords[k].second - i));
					if (distance < distances[k])
						matrix_[j][i] = 0;
					else if (distance < distances[k] + 1 && matrix_[j][i] != 0)
						matrix_[j][i] = 3;
					else if (distance > distances[k] && matrix_[j][i] != 0 && matrix_[j][i] != 3)
						matrix_[j][i] = 4;
				}
				//distance from current point to the center
				int distance = sqrt((center.first - j) * (center.first - j) + (center.second - i) * (center.second - i));
				if (distance < max_distance/2)
					matrix_[j][i] = 0;
				else if (distance < max_distance/2 + 1 && matrix_[j][i] != 0)
					matrix_[j][i] = 3;
				else if (distance > max_distance/2 && matrix_[j][i] != 0 && matrix_[j][i] != 3)
					matrix_[j][i] = 4;
			}
		}
	}

	print_matrix_in_console(matrix_);
}

void OurMap::fill(int x, int y, int *min_x, int *min_y, int *max_x, int *max_y, std::vector<std::pair<int, int>> *doors_coords) {
	if (matrix_[y][x] == 0) {
		matrix_[y][x] = -1;
		*min_x = x < *min_x ? x : *min_x;
		*min_y = y < *min_y ? y : *min_y;
		*max_x = x > *max_x ? x : *max_x;
		*max_y = y > *max_y ? y : *max_y;
		fill(x - 1, y, min_x, min_y, max_x, max_y, doors_coords);
		fill(x + 1, y, min_x, min_y, max_x, max_y, doors_coords);
		fill(x, y - 1, min_x, min_y, max_x, max_y, doors_coords);
		fill(x, y + 1, min_x, min_y, max_x, max_y, doors_coords);
	}
	else if (matrix_[y][x] == 2) {
		int occurences = count((*doors_coords).begin(), (*doors_coords).end(), std::pair<int, int>(y, x));
		if(occurences == 0)
			(*doors_coords).push_back(std::pair<int, int>(y, x));
	}
}

void OurMap::print_matrix_in_console(std::vector<std::vector<int>> matrix)
{
	for (int j = 0; j < n_; j++) {
		std::string row = "";
		for (int i = 0; i < n_; i++) {
			std::string number = std::to_string(matrix[j][i]);
			row.append(number);
			row.append(" ");
		}
		Logger::error(row);
	}
	Logger::error("\n");
}
