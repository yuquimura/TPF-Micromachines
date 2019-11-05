#include "GameMap.h"
#include "tiles/TilesFactory.h"
#include "../common/MapsLayer.h"

void GameMap::loadMap(const std::string &mapPath, SDL_Renderer *gRenderer) {
    MapsLayer mapsLayer(mapPath);
    _loadLayer(mapsLayer.ground, gRenderer,"ground");
    _loadLayer(mapsLayer.road, gRenderer, "road");
    _loadLayer(mapsLayer.extras, gRenderer, "extras");
    height = mapsLayer.height;
    width = mapsLayer.width;
}

void GameMap::_loadLayer(std::vector<std::vector<int>> layerMatrix, SDL_Renderer *gRenderer, std::string mapName){
    int x = 0;
    int y = 0;
    int wOld = 512;
    int hOld = 512;
    for (std::size_t i = 0; i < layerMatrix.size(); ++i){
        std::vector<int> row = layerMatrix[i];
        if (mapName == "ground") this->ground.emplace_back();
        else if (mapName == "road") this->road.emplace_back();
        else this->extras.emplace_back();
        for (std::size_t j = 0; j < row.size(); ++j){
            x = j * wOld;
            y = i * hOld;
            int tileType = row[j];
            if (tileType == 0){
                x = j * 512; // Maybe we need a check here, whether we are over height or width
                y = i * 512; // Same
                continue;
            }
            auto tile = tiles_factory.getTile(tileType, x, y, gRenderer);
            if (mapName == "ground") this->ground.back().push_back(tile);
            else if (mapName == "road") this->road.back().push_back(tile);
            else this->extras.back().push_back(tile);
//            SDL_Rect mBox = tile->getBox();
//            wOld = mBox.w;
//            hOld = mBox.h;
        }
    }
}

void GameMap::render(SDL_Rect &camera, SDL_Renderer *renderer){
    for (const auto& row: ground){
        for (const auto& tile: row){
          tile->renderWithSize(camera, renderer);
        }
    }
    for (const auto& row: road){
        for (const auto& tile: row){
            tile->renderWithSize(camera, renderer);
        }
    }
    for (const auto& row: extras){
        for (const auto& tile: row){
            tile->renderWithSize(camera, renderer);
        }
    }
}