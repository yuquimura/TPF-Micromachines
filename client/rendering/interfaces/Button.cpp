//
// Created by brian on 10/30/19.
//

#include "Button.h"
#include "ButtonAnswer.h"

Button::Button(SDL_Renderer *sdl_renderer, LTexture *buttonSpriteSheet) {
  mPosition.x = 0;
  mPosition.y = 0;
  colorChangeDuration = -1;
  this->texture = buttonSpriteSheet;
  this->gRenderer = sdl_renderer;
  this->id = texture->get_string_name();
}

void Button::setPosition(int x, int y) {
  mPosition.x = x;
  mPosition.y = y;
  set_area(x, y);
}

void Button::changeColor(int r, int g, int b, int duration){
    this->texture->set_color(r, g, b);
    colorChangeDuration = duration;
}

bool Button::handleEvent(SDL_Event *e) {
  //If mouse event happened

  if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
    //Get mouse position
    int x, y;
    SDL_GetMouseState(&x, &y);

    //Check if mouse is in button
    bool inside = true;

    //Mouse is left of the button
    if (x < mPosition.x) {
      inside = false;
    }
      //Mouse is right of the button
    else if (x > mPosition.x + BUTTON_WIDTH) {
      inside = false;
    }
      //Mouse above the button
    else if (y < mPosition.y) {
      inside = false;
    }
      //Mouse below the button
    else if (y > mPosition.y + BUTTON_HEIGHT) {
      inside = false;
    }

    //Mouse is outside button
    if (!inside) {
    }
      //Mouse is inside button
    else {
      //Set mouse over sprite
      switch (e->type) {
        case SDL_MOUSEMOTION:;
          return false;
        case SDL_MOUSEBUTTONDOWN:;
          this->changeColor(100, 100, 100, 200);
          callCallbackFunctions();
          return true;
        case SDL_MOUSEBUTTONUP:;
          return false;
      }
    }
  }
  return  false;
}
void Button::render() {
  if (colorChangeDuration > -1) colorChangeDuration--;
  if (colorChangeDuration == 0) changeColor(255,255,255,-1);
  SDL_RenderCopy(gRenderer, texture->get_texture(), nullptr, &area);
}

void Button::set_area(int x, int y) {
  this->area = {x, y, BUTTON_WIDTH, BUTTON_HEIGHT};
}

void Button::callCallbackFunctions() {
  for (auto &function : callbacks) {
    function(this->id);
    }
}

void Button::addCallbackFunction(std::function<void(std::string)> cf) {
    callbacks.push_back(cf);
}

std::string Button::getId(){
    return this->id;
}


