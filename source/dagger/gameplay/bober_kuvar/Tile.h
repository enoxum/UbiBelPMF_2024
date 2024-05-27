#pragma once
#include "OurEntity.h"

class Tile :
    public OurEntity
{
public:
    Tile(const std::string& sprite_path, const std::string& animation_path, bool collidable);
private:
    bool collidable_;
};

