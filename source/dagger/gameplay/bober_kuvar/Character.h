#pragma once
#include "OurEntity.h"
#include "Health.h"


using namespace health_system;

class Character :
    public OurEntity
{
public:
    Character(const std::string& sprite_path, const std::string& animation_path, bool collidable, std::pair<int, int> collision_size);
private:
    virtual void spawn();
    virtual void die();
    virtual void collision();
    void createHealth();


    Entity healthBarEntity;
    Entity backgroundEntity;
    Entity fillEntity;
    HealthComponent health;
    
    double speed_;
    double strength_;
    bool collidable_;
    std::pair<int, int> collision_size_;
};