//
// Created by David on 21/10/2023.
//

#include "KeyboardHandler.h"
#include "cursor.h"
#include "type.h"
#include <stdbool.h>

int keyDown[SDL_NUM_SCANCODES] = {0};

int BPP = WINDOW_BPP;


void UpdateBPP(int bpp) {
    BPP += bpp;
}
int GetBPP() {
    return BPP;
}

bool isAnyFunction(Cursor *cursor, Lines *lines, WindowHandler *Window) {
    if (keyDown[SDL_SCANCODE_LEFT]) {
        MoveCursor(cursor, lines, 1);
        return true;
    }
    else if (keyDown[SDL_SCANCODE_RIGHT]) {
        MoveCursor(cursor, lines, 2);
        return true;
    }
    else if (keyDown[SDL_SCANCODE_UP]) {
        MoveCursor(cursor, lines, 3);
        return true;
    }
    else if (keyDown[SDL_SCANCODE_DOWN]) {
        MoveCursor(cursor, lines, 4);
        return true;
    }
    else if (keyDown[SDL_SCANCODE_LCTRL] && keyDown[SDL_SCANCODE_KP_PLUS]) {
        UpdateBPP(2);
        Window->font = TTF_OpenFont("../SourceCode.ttf", BPP);
        return true;
    }
    else if (keyDown[SDL_SCANCODE_LCTRL] && keyDown[SDL_SCANCODE_KP_MINUS]) {
        UpdateBPP(-2);
        Window->font = TTF_OpenFont("../SourceCode.ttf", BPP);
        return true;
    } else if (keyDown[SDL_SCANCODE_RETURN] || keyDown[SDL_SCANCODE_KP_ENTER]) {
        if (lines->lines[cursor->line].size > cursor->x)
        {
            char *buffer = malloc(lines->lines[cursor->line].size + 1 - cursor->x);
            memcpy(buffer, lines->lines[cursor->line].chars + cursor->x, lines->lines[cursor->line].size - cursor->x);
            lines->lines[++lines->size].size = lines->lines[lines->size].size;
            lines->lines[lines->size].chars = (char *) malloc(lines->lines[lines->size].size);
            for (int i = lines->size; i > cursor->line + 1; i--) {
                lines->lines[i] = lines->lines[i - 1];
            }
            lines->lines[cursor->line + 1].size = lines->lines[cursor->line].size - cursor->x;
            lines->lines[cursor->line + 1].chars = malloc(lines->lines[cursor->line + 1].size);
            lines->lines[cursor->line + 1].chars[lines->lines[cursor->line + 1].size] = '\0';
            lines->lines[cursor->line + 1].chars = buffer;
            lines->lines[cursor->line].chars[cursor->x] = '\0';
            lines->lines[cursor->line].size = cursor->x;
        } else
        {
            lines->lines[++lines->size].size = lines->lines[lines->size].size;
            lines->lines[lines->size].chars = (char *) malloc(lines->lines[lines->size].size);
            for (int i = lines->size; i > cursor->line + 1; i--) {
                lines->lines[i] = lines->lines[i - 1];
            }
            lines->lines[cursor->line + 1].size = 0;
            lines->lines[cursor->line + 1].chars = malloc(sizeof(char));
            lines->lines[cursor->line + 1].chars[lines->lines[cursor->line + 1].size] = '\0';
            lines->lines[cursor->line].chars[cursor->x] = '\0';
            lines->lines[cursor->line].size = cursor->x;
        }
        return true;
    }
    return false;
}

void handleInputText(Lines *lines, Cursor *cursor, SDL_Keycode keycode) {
    if (keyDown[SDL_SCANCODE_INSERT])
    {
        if (lines->lines[cursor->line].size > cursor->x)
        {
            lines->lines[cursor->line].chars[cursor->x] = (char) keycode;
            cursor->x++;
        }
        else
        {
            lines->lines[cursor->line].chars[cursor->x] = (char) keycode;
            lines->lines[cursor->line].size++;
            lines->lines[cursor->line].chars[lines->lines[cursor->line].size] = '\0';
            cursor->x++;
        }
    } else {
        // append text after the cursor
    }

}

void KeyboardHandler(SDL_Event *event, Lines *lines, Cursor *cursor, WindowHandler *Window)
{
    switch (event->type) {
        case SDL_KEYDOWN:
            if (event->key.keysym.scancode != SDL_SCANCODE_INSERT)
            {
                keyDown[event->key.keysym.scancode] = 1;
                if (!isAnyFunction(cursor, lines, Window))
                    handleInputText(lines, cursor, event->key.keysym.sym);
            } else {
                keyDown[SDL_SCANCODE_INSERT] = !keyDown[SDL_SCANCODE_INSERT];
            }
            break;
        case SDL_KEYUP:
            if (event->key.keysym.scancode != SDL_SCANCODE_INSERT)
                keyDown[event->key.keysym.scancode] = 0;
            break;
    }
}