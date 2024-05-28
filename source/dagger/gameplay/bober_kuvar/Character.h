/*#pragma once
#include "OurEntity.h"
#include "Health.h"


using namespace health_system;

class Character :
    public OurEntity
{
public:
    //void move(Vector3 vector);
    
    double getSpeed();
    Character(double hp, double speed, double strength, const std::string& sprite_path, const std::string& animation_path, bool collidable, std::pair<int, int> collision_size);

    virtual void spawn(const std::pair<int, int>&, const std::pair<int, int>&, const std::vector<std::vector<int>>&);
    
private:
    virtual void die();
    virtual void collision();
    void createHealth();


    Entity healthBarEntity;
    Entity backgroundEntity;
    Entity fillEntity;
    HealthComponent health;
    
protected:
    double hp_;
    double speed_;
    double strength_;
    bool collidable_;
    std::pair<int, int> collision_size_;
};
*/
