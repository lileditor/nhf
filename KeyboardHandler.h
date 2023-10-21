//
// Created by David on 21/10/2023.
//

#ifndef NHF_KEYBOARDHANDLER_H
#define NHF_KEYBOARDHANDLER_H

#include "type.h"

void KeyboardHandler(SDL_Event *event, Lines *lines, Cursor *cursor, WindowHandler *Window);
void UpdateBPP(int bpp);
int GetBPP();
#endif //NHF_KEYBOARDHANDLER_H
