#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include <SDL3/SDL.h>

void InitGamepads();
void GamepadEvent(const SDL_Event *event);
void UpdateGamepads(bool inmenu);

/* The pad feeds the same state the game polls for the keyboard and mouse. */
void GamepadKeys(unsigned char *keys);
bool GamepadButton();
void GamepadLook(float *x, float *y);

/* True while the pad is driving the menu cursor; x and y are fractions of the window. */
bool GamepadCursor(float *x, float *y);

#endif
