

#ifndef MICROMACHINES_ENTITY_H
#define MICROMACHINES_ENTITY_H

#include "Box2D/Box2D.h"

struct statusEffect{
    double delay; //in seconds
    double duration; //in seconds
    int stacks;
    void (*f)(b2Fixture&);
};

//probably make this one a virtual class
class Entity {
    bool enabled;
    int y_size;
    int x_size;
    //std::string stats;

protected:
    b2Body* body;
public:

    //only supports making objects with a square bounding box
    Entity(b2Body* &newbody);

    virtual void resolveCollision(b2Fixture &collidedWith);

    virtual void addEffects(){};

    b2Vec2 getPosition();

    b2Vec2 getSize();

    float32 getAngle();

    void attachFixture(b2FixtureDef &boxFixtureDef);
};


#endif //MICROMACHINES_ENTITY_H
