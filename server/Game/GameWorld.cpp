#include <iostream>
#include <memory>
#include "GameWorld.h"
#include "../../config/constants.h"


#define PTM_TILE (float)TILE_SIZE/PTM

//local use namespace for local functions
namespace {
    b2Body* makeNewBody(b2World &world, b2BodyType bodyType, float32 x, float32 y){
        b2BodyDef myBodyDef;
        myBodyDef.type = bodyType;
        myBodyDef.angle = 0;
        myBodyDef.position.Set(x, y);
        return world.CreateBody(&myBodyDef);
    }

    void createAndAddFixture(Entity* obj, float hx, float hy, float density,
                             uint16 catBits, uint16 maskBits, bool isSensor){

        b2PolygonShape boxShape;
        boxShape.SetAsBox(hx/2, hy/2);
        b2FixtureDef boxFixtureDef;
        boxFixtureDef.shape = &boxShape;
        boxFixtureDef.density = density;
        boxFixtureDef.filter.categoryBits = catBits;
        boxFixtureDef.filter.maskBits = maskBits;
        boxFixtureDef.isSensor = isSensor;
        obj->attachFixture(boxFixtureDef);
    }
}

GameWorld::GameWorld() : world(b2Vec2(0,0)) {
    world.SetContactListener(&cl);
    finishingLine = nullptr;
}

void GameWorld::loadWorld(std::string worldName){
    map.load("maps/" + worldName+".yaml");
    for (int j = 0; j < map.road.size(); j++){
        auto row = map.road[j];
        for (int i= 0; i<row.size();i++){
            if (row[i] <= ROAD_END_TYPE && row[i] >= ROAD_START_TYPE){
                createRoad(i * PTM_TILE + PTM_TILE/2 , j*PTM_TILE + PTM_TILE/2, row[i]);
            }
        }
    }
    // TODO: Load  all the extras (the ones you just crash with)
    for (size_t j = 0; j < map.extras.size(); j++){
        auto row = map.extras[j];
        for (size_t i= 0; i < row.size(); i++){
            if (row[i] == finish_line){
                createFinishingLine(i * PTM_TILE + PTM_TILE/2 , j*PTM_TILE + PTM_TILE/2);
            }
            else if (row[i] == pavilion_blue_tile) {
                createExtras(i * PTM_TILE + PTM_TILE/2 , j*PTM_TILE + PTM_TILE/2, row[i]);
            }
        }
    }
}

InfoBlock GameWorld::status(){
    InfoBlock ib;
    ib[PLAYERS_AMOUNT] = this->cars.size();
    for (auto & car : cars){
        std::string car_id = std::to_string(car.id);
        car.loadStateToInfoBlock(ib);
    }
    ib[OBJECTS_AMOUNT] = 0; // here goes something like this->objects.size();
    int cont = 0;
    for (auto & item : dynamic_objs){
        std::string obj_id = std::to_string(cont);
        item->loadPosToInfoBlock(ib, cont);
        cont++;
    }
    return ib;
}

void GameWorld::processEvent(int id, InfoBlock event){
    getCar(id).drive(event);
}

void GameWorld::Step(float timestep) {
    for (auto & car : cars){
        car.step(timestep);
    }
    int32 velocityIterations = 8;//how strongly to correct velocity
    int32 positionIterations = 3;//how strongly to correct position
    world.Step(timestep, velocityIterations, positionIterations);
    this->timeModifiers += timestep;

    // TODO: make this random maybe a little better, Flor task
    // We create random items
    int randomTime = rand() % (TIME_TO_ITEMS - TIME_FROM_ITEMS + 1) + TIME_FROM_ITEMS; // Use Constants
    if ( (int) this->timeModifiers > randomTime){
        std::cout << "We generate random item\n";
        _createItem();
        this->timeModifiers = 0;
    } else if (this->timeModifiers > TIME_TO_ITEMS) this->timeModifiers = 0;
}

RaceCar &GameWorld::getCar(int id) {
    auto it = cars.begin();
    for (int i = 0; i<id;i++){
        it++;
    }
    return *it;
}

void GameWorld::_createItem(){
    // TODO: we pass to body the tipe of entity we want?
    //b2Body* body = new b2Body();
    //this->dynamic_objs.emplace_back(body);
    return;
}


void GameWorld::createRoad(int x, int y, int tileType) {
    b2Body* newBody = makeNewBody(world, b2_staticBody, x, y);
    this->road_bodies.emplace_back(std::to_string(tileType)+"x: "+ std::to_string(x/512) + " y: "+std::to_string(y/512),newBody);
    createAndAddFixture(&(this->road_bodies.back()), PTM_TILE, PTM_TILE, 0, TILE, SENSOR, false);
}

void GameWorld::createExtras(int x, int y, int tileType) {
    b2Body* newBody = makeNewBody(world, b2_staticBody, x, y);
    auto ptr = std::make_shared<Entity>(newBody);
    this->static_objs.push_back(ptr);
    createAndAddFixture(this->static_objs.back().get(), PTM_TILE, PTM_TILE, 0, TILE, PLAYER, false);
}

int GameWorld::createCar(InfoBlock carStats) {
    auto fpos = finishingLine->getPosition();
    int size = cars.size();
    int y = fpos.y - cars.size()*CAR_HEIGHT/PTM;
    int x = fpos.x - CAR_WIDTH/PTM + (2*CAR_WIDTH/PTM)*(size%2);
    b2Body* newBody = makeNewBody(world, b2_dynamicBody,x,y);
    int carId = size;
    cars.emplace_back(carId, carStats, newBody);
    newBody->SetBullet(true);

    createAndAddFixture(&(cars.back()),1,2,1,PLAYER, 0, false);
    createAndAddFixture(&(cars.back()), (float)CAR_WIDTH/PTM, (float)CAR_HEIGHT/PTM,0,PLAYER, PLAYER, false);
    createAndAddFixture(&(cars.back()),1,1,0,SENSOR, TILE, true);

    auto ptr = std::shared_ptr<StatusEffect>(new LapCooldown(10));
    cars.back().addEffect(ptr);
    return carId;
}

void GameWorld::createFinishingLine(int x, int y) {
    b2Body* newBody = makeNewBody(world, b2_staticBody, x, y);
    finishingLine = std::make_shared<FinishingLine>(newBody);
    createAndAddFixture(finishingLine.get(),PTM_TILE,(float)FINISH_LINE_HEIGHT/PTM,0,TILE, SENSOR, false);
}
