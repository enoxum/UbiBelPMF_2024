#include "Tile.h"

Tile::Tile(const std::string& sprite_path, const std::string& animation_path, bool collidable)
	: OurEntity(sprite_path, animation_path, collidable, std::pair<int, int>(63, 63))
{
	collidable_ = collidable;
}
