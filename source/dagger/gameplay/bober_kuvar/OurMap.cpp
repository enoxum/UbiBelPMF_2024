#include "OurMap.h"

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
	print_matrix_in_console(line_matrix);
	print_matrix_in_console(horizontal_restriction_matrix);
	print_matrix_in_console(vertical_restriction_matrix);
}

void OurMap::fill_rooms()
{

}

void OurMap::connect_rooms()
{

}

void OurMap::print_matrix_in_console(std::vector<std::vector<int>> matrix)
{
	for (int j = 0; j < n_; j++) {
		std::string row = "";
		for (int i = 0; i < n_; i++) {
			std::string number = std::to_string(matrix[j][i]);
			row.append(number);
		}
		Logger::error(row);
	}
	Logger::error("\n");
}
