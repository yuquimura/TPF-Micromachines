//
// Created by brian on 10/30/19.
//

#include "Button.h"

void Button::setPosition(int x, int y) {
  mPosition.x = x;
  mPosition.y = y;
}
bool Button::handleEvent(SDL_Event *e) {
  //If mouse event happened
  if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
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
      //Pone el enum que representa que el mouse no esta encima;
      mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
    }
      //Mouse is inside button
    else {
      //Set mouse over sprite
      switch (e->type) {
        case SDL_MOUSEMOTION:mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
          return false;
        case SDL_MOUSEBUTTONDOWN:mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
          return true;
        case SDL_MOUSEBUTTONUP:mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
          return false;
      }
    }
  }

}
void Button::render() {
  //Show current button sprite
  gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &gSpriteClips[mCurrentSprite]);
}

Button::Button(SDL_Window *window, SDL_Renderer *sdl_renderer) {
  mPosition.x = 0;
  mPosition.y = 0;
  mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
  this->gRenderer = sdl_renderer;
  this->gWindow = window;
}
