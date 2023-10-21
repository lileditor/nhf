//
// Created by David on 21/10/2023.
//

#include "KeyboardHandler.h"
#include "cursor.h"
#include "type.h"

int keyDown[SDL_NUM_SCANCODES] = {0};

int BPP = WINDOW_BPP;

void UpdateBPP(int bpp) {
    BPP += bpp;
}
int GetBPP() {
    return BPP;
}

void isAnyFunction(Cursor *cursor, Lines *lines, WindowHandler *Window) {
    if (keyDown[SDL_SCANCODE_LEFT]) {
        MoveCursor(cursor, lines, 1);
    }
    if (keyDown[SDL_SCANCODE_RIGHT]) {
        MoveCursor(cursor, lines, 2);
    }
    if (keyDown[SDL_SCANCODE_UP]) {
        MoveCursor(cursor, lines, 3);
    }
    if (keyDown[SDL_SCANCODE_DOWN]) {
        MoveCursor(cursor, lines, 4);
    }
    if (keyDown[SDL_SCANCODE_LCTRL] && keyDown[SDL_SCANCODE_KP_PLUS]) {
        UpdateBPP(2);
        Window->font = TTF_OpenFont("../SourceCode.ttf", BPP);
    }
    if (keyDown[SDL_SCANCODE_LCTRL] && keyDown[SDL_SCANCODE_KP_MINUS]) {
        UpdateBPP(-2);
        Window->font = TTF_OpenFont("../SourceCode.ttf", BPP);
    }
}

void KeyboardHandler(SDL_Event *event, Lines *lines, Cursor *cursor, WindowHandler *Window)
{
    switch (event->type) {
        case SDL_KEYDOWN:
            if (keyDown[event->key.keysym.scancode] == SDL_SCANCODE_INSERT)
            {
                if (keyDown[SDL_SCANCODE_INSERT]) {
                    keyDown[SDL_SCANCODE_INSERT] = 0;
                }
            }
            keyDown[event->key.keysym.scancode] = 1;
            isAnyFunction(cursor, lines, Window);
            break;
        case SDL_KEYUP:
            if (keyDown[event->key.keysym.scancode] != SDL_SCANCODE_INSERT)
                keyDown[event->key.keysym.scancode] = 0;
            break;
    }
}