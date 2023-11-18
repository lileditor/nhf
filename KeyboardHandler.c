//
// Created by David on 21/10/2023.
//

#include "KeyboardHandler.h"
#include "cursor.h"
#include "type.h"
#include "debugmalloc.h"
#include <stdbool.h>

int keyDown[SDL_NUM_SCANCODES] = {0};
int BPP = WINDOW_BPP;

void UpdateBPP(int bpp) {
    BPP += bpp;
}
int GetBPP() {
    return BPP;
}

char swapChar1;
int swapChar1pos;
char swapChar2;
int swapChar2pos;

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
            char* buffer = malloc((lines->lines[cursor->line].size - cursor->x + 1));
            strncpy(buffer, lines->lines[cursor->line].chars + cursor->x, lines->lines[cursor->line].size - cursor->x + 1);
            if (cursor->line == lines->size - 1) {
                lines->lines = (Line *) realloc(lines->lines, (lines->size+1) * sizeof (Line));
                lines->lines[lines->size].chars = malloc(lines->lines[cursor->line].size - cursor->x + 1);
                strcpy(lines->lines[lines->size].chars,buffer);
                free(buffer);
                buffer = NULL;
                lines->lines[lines->size].size = lines->lines[cursor->line].size - cursor->x;
                lines->size++;
            } else {
                lines->size++;
                for (int i = lines->size; i > cursor->line; --i) {
                    lines->lines[i] = lines->lines[i - 1];
                }
                lines->lines[cursor->line + 1].chars = (char *) malloc(lines->lines[cursor->line].size - cursor->x + 1);
                strcpy(lines->lines[cursor->line + 1].chars,buffer);
                lines->lines[cursor->line + 1].size = lines->lines[cursor->line].size - cursor->x;
            }
            lines->lines[cursor->line].size = cursor->x;
            lines->lines[cursor->line].chars[cursor->x] = '\0';
            cursor->line++;
            cursor->x = 0;
        } else
        {
            if (cursor->line == lines->size - 1) {
                lines->size++;
                lines->lines[lines->size].chars = (char *) malloc(sizeof (char));
                lines->lines[lines->size].chars[0] = '\0';
                lines->lines[lines->size].size = 0;
            } else {
                lines->size++;
                lines->lines[lines->size].chars = (char *) malloc(cursor->x * sizeof (char));
                lines->lines[lines->size].size = cursor->x;
                for (int i = lines->size; i > cursor->line + 1; i--) {
                    lines->lines[i] = lines->lines[i - 1];
                }
                lines->lines[cursor->line + 1].size = 0;
                lines->lines[cursor->line + 1].chars = (char *) malloc(sizeof (char));
                lines->lines[cursor->line + 1].chars[0] = '\0';
            }
            lines->lines[cursor->line].chars[cursor->x] = '\0';
            lines->lines[cursor->line].size = cursor->x;
            cursor->line++;
            cursor->x = 0;
        }
        return true;
    } else if (keyDown[SDL_SCANCODE_BACKSPACE]) {
        if (cursor->x > 0) {
            cursor->x--;
            for (int i = cursor->x; i < lines->lines[cursor->line].size; i++) {
                lines->lines[cursor->line].chars[i] = lines->lines[cursor->line].chars[i + 1];
            }
            lines->lines[cursor->line].size--;
        } else {
            if (cursor->line > 0) {
                char* buffer = malloc(lines->lines[cursor->line].size);
                strncpy(buffer, lines->lines[cursor->line].chars, lines->lines[cursor->line].size);
                for (int i = cursor->line + 1; i < lines->size - 1; i++) {
                    lines->lines[i] = lines->lines[i + 1];
                }
                lines->size--;
                lines->lines[cursor->line - 1].size += lines->lines[cursor->line].size;
                lines->lines[cursor->line - 1].chars = (char *) realloc(lines->lines[cursor->line - 1].chars, lines->lines[cursor->line - 1].size * sizeof (char));
                strcat(lines->lines[cursor->line - 1].chars, buffer);
                cursor->x = lines->lines[cursor->line - 1].size - lines->lines[cursor->line].size;
                cursor->line--;
                lines->lines[cursor->line].chars[lines->lines[cursor->line].size] = '\0';
                free(buffer);
                buffer = NULL;
            }
        }
        return true;
    } else if (keyDown[SDL_SCANCODE_DELETE]) {
        if (cursor->x < lines->lines[cursor->line].size) {
            for (int i = cursor->x; i < lines->lines[cursor->line].size; i++) {
                lines->lines[cursor->line].chars[i] = lines->lines[cursor->line].chars[i + 1];
            }
            lines->lines[cursor->line].size--;
        } else {
            if (cursor->line < lines->size - 1) {
                char* buffer = malloc(lines->lines[cursor->line + 1].size);
                strncpy(buffer, lines->lines[cursor->line + 1].chars, lines->lines[cursor->line + 1].size);
                for (int i = cursor->line + 1; i < lines->size - 1; i++) {
                    lines->lines[i] = lines->lines[i + 1];
                }
                lines->size--;
                lines->lines[cursor->line].size += lines->lines[cursor->line + 1].size;
                lines->lines[cursor->line].chars = (char *) realloc(lines->lines[cursor->line].chars, lines->lines[cursor->line].size * sizeof (char));
                strcat(lines->lines[cursor->line].chars, buffer);
                lines->lines[cursor->line].chars[lines->lines[cursor->line].size] = '\0';
                free(buffer);
                buffer = NULL;
            }
        }
        return true;
    } else if (keyDown[SDL_SCANCODE_TAB]) {
        if (cursor->x == lines->lines[cursor->line].size) {
            lines->lines[cursor->line].chars = realloc(lines->lines[cursor->line].chars,
                                                       lines->lines[cursor->line].size + TAB_SPACE_COUNT);
            for (size_t i = 0; i < TAB_SPACE_COUNT; i++) {
                lines->lines[cursor->line].chars[cursor->x++] = ' ';
            }
            lines->lines[cursor->line].chars[cursor->x] = '\0';
            lines->lines[cursor->line].size += TAB_SPACE_COUNT;
        }
        else
        {
            char* buffer = malloc((lines->lines[cursor->line].size - cursor->x + 1));
            strncpy(buffer, lines->lines[cursor->line].chars + cursor->x, lines->lines[cursor->line].size - cursor->x + 1);
            for (size_t i = 0; i < TAB_SPACE_COUNT; i++) {
                lines->lines[cursor->line].chars[cursor->x + i] = ' ';
            }
            strcat(lines->lines[cursor->line].chars, buffer);
            lines->lines[cursor->line].size += (int) strlen(buffer);
            lines->lines[cursor->line].size += TAB_SPACE_COUNT;
            lines->lines[cursor->line].chars[lines->lines[cursor->line].size] = '\0';
            free(buffer);
        }
        return true;
    } else if(keyDown[SDL_SCANCODE_F1]) {
        swapChar1 = lines->lines[cursor->line].chars[cursor->x];
        swapChar1pos = cursor->x;
        return true;
    } else if(keyDown[SDL_SCANCODE_F2]) {
        swapChar2 = lines->lines[cursor->line].chars[cursor->x];
        swapChar2pos = cursor->x;
        return true;
    } else if(keyDown[SDL_SCANCODE_F3]) {
        lines->lines[cursor->line].chars[swapChar2pos] = swapChar1;
        lines->lines[cursor->line].chars[swapChar1pos] = swapChar2;
        return true;
    } else if (keyDown[SDL_SCANCODE_LCTRL] && keyDown[SDL_SCANCODE_S] ) {
        SDL_Quit();
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
            lines->lines[cursor->line].size++;
            lines->lines[cursor->line].chars = (char *) realloc(lines->lines[cursor->line].chars, lines->lines[cursor->line].size * sizeof(char));
            lines->lines[cursor->line].chars[cursor->x] = (char) keycode;
            lines->lines[cursor->line].chars[lines->lines[cursor->line].size] = '\0';
            cursor->x++;
        }
    } else {
        if (lines->lines[cursor->line].size > cursor->x) {
            char *buffer = malloc((lines->lines[cursor->line].size - cursor->x + 1) * sizeof(char));
            memcpy(buffer, lines->lines[cursor->line].chars + cursor->x, lines->lines[cursor->line].size - cursor->x);
            lines->lines[cursor->line].size++;
            lines->lines[cursor->line].chars = (char *) realloc(lines->lines[cursor->line].chars, lines->lines[cursor->line].size * sizeof(char) + 1);
            lines->lines[cursor->line].chars[cursor->x] = (char) keycode;
            lines->lines[cursor->line].chars[lines->lines[cursor->line].size] = '\0';
            lines->lines[cursor->line].chars[++cursor->x] = *buffer;
        } else {
            lines->lines[cursor->line].size++;
            lines->lines[cursor->line].chars = (char *) realloc(lines->lines[cursor->line].chars, (lines->lines[cursor->line].size + 1) * sizeof(char));
            lines->lines[cursor->line].chars[cursor->x] = (char) keycode;
            lines->lines[cursor->line].chars[lines->lines[cursor->line].size] = '\0';
            cursor->x++;
        }
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