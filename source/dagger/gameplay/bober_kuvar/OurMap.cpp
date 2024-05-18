#include "OurMap.h"
#include <random>;

OurMap::OurMap(int n, int room_size)
{
	n_ = n;
	room_size_ = room_size;
	matrix_ = std::vector<std::vector<int>>(n_, std::vector<int>(n_, 0));
	make_walls();
}

void OurMap::generateMap()
{

}

void OurMap::make_walls()
{
	std::vector<std::vector<int>> line_matrix(n_, std::vector<int>(n_, 0));
	std::vector<std::vector<int>> horizontal_restriction_matrix(n_, std::vector<int>(n_, 0));
	std::vector<std::vector<int>> vertical_restriction_matrix(n_, std::vector<int>(n_, 0));

	for (int j = 0; j < n_; j++) {
		for (int i = 0; i < n_; i++) {
			if (i == 0) {
				line_matrix[j][i] = 1;
				for (int k = 0; k < room_size_; k++)
					vertical_restriction_matrix[j][i + k] = 1;
			}
			if (j == 0) {
				line_matrix[j][i] = 1;
				for (int k = 0; k < room_size_; k++)
					horizontal_restriction_matrix[j + k][i] = 1;
			}
			if (i == n_-1) {
				line_matrix[j][i] = 1;
				for (int k = 0; k < room_size_; k++)
					vertical_restriction_matrix[j][i - k] = 1;
			}
			if (j == n_-1) {
				line_matrix[j][i] = 1;
				for (int k = 0; k < room_size_; k++)
					horizontal_restriction_matrix[j - k][i] = 1;
			}
		}
	}

	int tries = 0;
	int max_tries = 10;
	while (tries < max_tries) {
		int direction = rand() % 2;
		int random_x = rand() % n_;
		int random_y = rand() % n_;

		//horizontal
		if (direction == 0) {
			if (line_matrix[random_y][random_x] == 0 && horizontal_restriction_matrix[random_y][random_x] == 0) {
				tries = 0;

				//left
				int copy_x = random_x;
				while (line_matrix[random_y][copy_x] != 1) {
					line_matrix[random_y][copy_x] = 1;
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
				while (line_matrix[random_y][copy_x] != 1) {
					line_matrix[random_y][copy_x] = 1;
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
				line_matrix[random_y][random_doorway_x] = 2;
				doorways_.push_back(std::pair<int, int>(random_doorway_x, random_y));
			}
			else
				tries++;
		}
		//vertical
		else {
			if (line_matrix[random_y][random_x] == 0 && vertical_restriction_matrix[random_y][random_x] == 0) {
				tries = 0;

				//up
				int copy_y = random_y;
				while (line_matrix[copy_y][random_x] != 1) {
					line_matrix[copy_y][random_x] = 1;
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
				while (line_matrix[copy_y][random_x] != 1) {
					line_matrix[copy_y][random_x] = 1;
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
				line_matrix[random_doorway_y][random_x] = 2;
				doorways_.push_back(std::pair<int, int>(random_x, random_doorway_y));
			}
			else
				tries++;
		}
	}

	print_matrix_in_console(line_matrix);
	print_matrix_in_console(horizontal_restriction_matrix);
	print_matrix_in_console(vertical_restriction_matrix);
	//for (int i = 0; i < doorways_.size(); i++) {
	//	Logger::error(doorways_[i].first);
	//	Logger::error(doorways_[i].second);
	//	Logger::error("");
	//}
}

void OurMap::fill_rooms()
{

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
