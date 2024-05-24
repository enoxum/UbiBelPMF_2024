#include "Tile.h"

Tile::Tile(const std::string& sprite_path, const std::string& animation_path, bool collidable)
	: OurEntity(sprite_path, animation_path, collidable, std::pair<int, int>(64, 64))
{
	collidable_ = collidable;
}
