#include "Menu.h"
#include "../../../config/constants.h"
#include "CarButton.h"
#include "ConnectButton.h"
#include "MapButton.h"
#include "Font.h"

bool Menu::load_media() {
  bool success = true;
  success *= carBlue.load_from_file("client/rendering/assets/cars/blue_car.png", gRenderer);
  success *= carBlack.load_from_file("client/rendering/assets/cars/black_car.png", gRenderer);
  success *= carRed.load_from_file("client/rendering/assets/cars/red_car.png", gRenderer);
  success *= carWhite.load_from_file("client/rendering/assets/cars/white_car.png", gRenderer);
  success *= wallpaper.load_from_file("client/rendering/assets/all_images/Decor/B.png", gRenderer);
  success *= connectButton.load_from_file("client/rendering/assets/buttons/connect.png", gRenderer);
  if (success) {
    gButtons.push_back(new CarButton(gRenderer, &carBlue));
    gButtons.push_back(new CarButton(gRenderer, &carWhite));
    gButtons.push_back(new CarButton(gRenderer, &carRed));
    gButtons.push_back(new CarButton(gRenderer, &carBlack));
    gButtons.push_back(new ConnectButton(gRenderer, &connectButton));
  }
  return success;
}

void Menu::init(SDL_Renderer *sdl_renderer, std::queue<SDL_Event> *gQueue) {
  this->queue = gQueue;
  this->gRenderer = sdl_renderer;
  if (!load_media()) {
    printf("Failed to initialize!\n"); //todo exception here
  }
  //Set positions of the three buttons buttons
  gButtons[0]->setPosition(BLUE_CAR_BUTTON_X, BLUE_CAR_BUTTON_Y);//Boton asociado al primer vehiculo.
  gButtons[1]->setPosition(BLACK_CAR_BUTTON_X, BLACK_CAR_BUTTON_Y);//Boton asociado al segundo vehiculo.
  gButtons[2]->setPosition(RED_CAR_BUTTON_X, RED_CAR_BUTTON_Y);//Boton asociado al tercero.
  gButtons[3]->setPosition(WHITE_CAR_BUTTON_X, WHITE_CAR_BUTTON_Y);//Boton asociado al 4to.
  gButtons[4]->setPosition(PLAY_BUTTON_X, PLAY_BUTTON_Y);
  font.Font_init("Hola", gRenderer);
}

bool Menu::processEvents(Button_answer &button_answer) {
    while (!queue->empty()) {
        for (auto &button : gButtons) {
          button->handleEvent(&queue->front(), &button_answer);
          font.receive_input(&queue->front());
          if (button_answer.get_state()) {

            while (!queue->empty()) queue->pop();
            return button_answer.get_state();
            }
        }
        queue->pop();
    }
  return button_answer.get_state();
}

void Menu::render_first_menu() {
    bool clicked = false;
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    wallpaper.render_with_size(0, 0, 0, gRenderer, SCREEN_HEIGHT, SCREEN_WIDTH, true);
    for (auto &button : gButtons) {
      button->render();
    }
  font.render();
    SDL_RenderPresent(gRenderer);
}

void Menu::dummy_init_as_leader() {//todo fix
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(gRenderer);
  LTexture map1;
  LTexture map2;
  LTexture map3;
  map1.load_from_file("client/rendering/assets/all_images/Decor/dragon.png", gRenderer);
  mapButtons.push_back(new MapButton(gRenderer, &map1));
  map2.load_from_file("client/rendering/assets/all_images/Decor/dragon.png", gRenderer);
  mapButtons.push_back(new MapButton(gRenderer, &map2));
  map3.load_from_file("client/rendering/assets/all_images/Decor/dragon.png", gRenderer);
  mapButtons.push_back(new MapButton(gRenderer, &map3));
  mapButtons[0]->setPosition(MAP_BUTTON_1_X, MAP_BUTTON_1_Y);
  mapButtons[1]->setPosition(MAP_BUTTON_2_X, MAP_BUTTON_2_Y);
  mapButtons[2]->setPosition(MAP_BUTTON_3_X, MAP_BUTTON_3_Y);
  wallpaper.render_with_size(0, 0, 0, gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT,true);
  LTexture msg;
  msg.load_from_file("client/rendering/assets/all_images/Decor/ChooseMsg.png", gRenderer);
  msg.render_with_size(720, 500, 0, gRenderer, 800, 500,false);
  for (auto &button : mapButtons) {
    button->render();
  }
  SDL_RenderPresent(gRenderer);
}

//Todo hay que poner esta funcion en el thread de cuando cambia de una pantalla a la otra
void Menu::close_first_menu() {
  for (auto &button : gButtons) {
    button->free_texture();
  }
}

void Menu::init_as_follower() {
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(gRenderer);
  wallpaper.load_from_file("client/rendering/assets/all_images/Decor/waiting.png", gRenderer);
  wallpaper.render_with_size(0, 0, 0, gRenderer, 0, 0,true);
  SDL_RenderPresent(gRenderer);
}

