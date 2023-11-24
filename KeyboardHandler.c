//
// Sebe-Norbert-Dávid-FJDH0C-21/10/2023-Szovegszerkezto
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

bool isFuntionKey() {
    if (keyDown[SDL_SCANCODE_UNDO] || keyDown[SDL_SCANCODE_LCTRL] || keyDown[SDL_SCANCODE_RCTRL] || keyDown[SDL_SCANCODE_END] || keyDown[SDL_SCANCODE_HOME] || keyDown[SDL_SCANCODE_PAGEDOWN] || keyDown[SDL_SCANCODE_PAGEUP] || keyDown[SDL_SCANCODE_LSHIFT] || keyDown[SDL_SCANCODE_RSHIFT] || keyDown[SDL_SCANCODE_LALT] || keyDown[SDL_SCANCODE_RALT] || keyDown[SDL_SCANCODE_LGUI] || keyDown[SDL_SCANCODE_RGUI] || keyDown[SDL_SCANCODE_PRINTSCREEN] || keyDown[SDL_SCANCODE_PAUSE] || keyDown[SDL_SCANCODE_F4] || keyDown[SDL_SCANCODE_F5] || keyDown[SDL_SCANCODE_F6] || keyDown[SDL_SCANCODE_F7] || keyDown[SDL_SCANCODE_F8] || keyDown[SDL_SCANCODE_F9] || keyDown[SDL_SCANCODE_F10] || keyDown[SDL_SCANCODE_F11] || keyDown[SDL_SCANCODE_F12]) {
        return true;
    }
    return false;
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
    }
    else if (keyDown[SDL_SCANCODE_RETURN] || keyDown[SDL_SCANCODE_KP_ENTER]) {
        if (cursor->x == lines->lines[cursor->line].size){
            lines->size++;
            lines->lines = realloc(lines->lines, (lines->size + 1) * sizeof(Line));
            for (int i = lines->size - 1; i > cursor->line; i--) {
                lines->lines[i] = lines->lines[i - 1];
            }
            cursor->x = 0;
            cursor->line++;
            lines->lines[cursor->line].chars = (char *) malloc(1);
            lines->lines[cursor->line].chars[0] = '\0';
            lines->lines[cursor->line].size = 0;
        } else
        {
            int stringSize = lines->lines[cursor->line].size - cursor->x + 1;
            char* buffer = malloc(stringSize);
            strncpy(buffer, lines->lines[cursor->line].chars + cursor->x, stringSize);
            lines->size++;
            lines->lines = realloc(lines->lines, (lines->size + 1) * sizeof(Line));
            lines->lines[cursor->line].chars[cursor->x] = '\0';
            lines->lines[cursor->line].size = cursor->x;
            for (int i = lines->size; i > cursor->line; i--) {
                lines->lines[i] = lines->lines[i - 1];
            }
            cursor->x = 0;
            cursor->line++;
            lines->lines[cursor->line].chars = (char *) malloc(stringSize + 1);
            strncpy(lines->lines[cursor->line].chars, buffer, stringSize);
            lines->lines[cursor->line].size = stringSize;
            free(buffer);
            buffer = NULL;
        }
        return true;
    }
    else if (keyDown[SDL_SCANCODE_BACKSPACE]) {
        if (cursor->x > 0) {
            for (int i = cursor->x - 1; i < lines->lines[cursor->line].size; i++) {
                lines->lines[cursor->line].chars[i] = lines->lines[cursor->line].chars[i + 1];
            }
            cursor->x--;
            lines->lines[cursor->line].size--;
        } else
        {
            if (cursor->line > 0) {
                if (lines->lines[cursor->line].size > 0) {
                    char *buffer = malloc(lines->lines[cursor->line].size);
                    strncpy(buffer, lines->lines[cursor->line].chars, lines->lines[cursor->line].size);
                    strcat(lines->lines[cursor->line - 1].chars, buffer);
                    cursor->x = lines->lines[cursor->line - 1].size;
                    lines->lines[cursor->line - 1].size += lines->lines[cursor->line].size;
                    for (int i = cursor->line; i < lines->size - 1; i++) {
                        lines->lines[i] = lines->lines[i + 1];
                    }
                    lines->lines = realloc(lines->lines, lines->size * sizeof(Line));
                    cursor->line--;
                    lines->lines[cursor->line].chars[lines->lines[cursor->line].size] = '\0';
                    lines->size--;
                    free(buffer);
                    buffer = NULL;
                } else {
                    cursor->x = lines->lines[cursor->line - 1].size;
                    cursor->line--;
                    for (int i = cursor->line + 1; i < lines->size; i++) {
                        lines->lines[i] = lines->lines[i + 1];
                    }
                    lines->size--;
                }
            }
        }
        return true;
    }
    else if (keyDown[SDL_SCANCODE_DELETE]) {
        if (cursor->x < lines->lines[cursor->line].size) {
            for (int i = cursor->x; i < lines->lines[cursor->line].size; i++) {
                lines->lines[cursor->line].chars[i] = lines->lines[cursor->line].chars[i + 1];
            }
            lines->lines[cursor->line].size--;
        } else {
            if (cursor->line < lines->size - 1) {
                if (lines->lines[cursor->line + 1].size > 0) {
                    char *buffer = malloc(lines->lines[cursor->line + 1].size + 1);
                    strncpy(buffer, lines->lines[cursor->line + 1].chars, lines->lines[cursor->line + 1].size + 1);
                    lines->lines[cursor->line].size += lines->lines[cursor->line + 1].size;
                    for (int i = cursor->line + 1; i < lines->size - 1; i++) {
                        lines->lines[i] = lines->lines[i + 1];
                    }
                    lines->size--;
                    lines->lines[cursor->line].chars = (char *) realloc(lines->lines[cursor->line].chars, (lines->lines[cursor->line].size + 1) * sizeof(char));
                    strcat(lines->lines[cursor->line].chars, buffer);
                    lines->lines[cursor->line].chars[lines->lines[cursor->line].size] = '\0';
                    free(buffer);
                    buffer = NULL;
                } else {
                    for (int i = cursor->line + 1; i < lines->size - 1; i++) {
                        lines->lines[i] = lines->lines[i + 1];
                    }
                    lines->lines = realloc(lines->lines, lines->size * sizeof(Line));
                    lines->lines[cursor->line].chars = realloc(lines->lines[cursor->line].chars, lines->lines[cursor->line].size + 1);
                    lines->lines[cursor->line].chars[lines->lines[cursor->line].size] = '\0';
                    lines->size--;
                }
            }
        }
        return true;
    }
    else if (keyDown[SDL_SCANCODE_TAB]) {
        if (cursor->x == lines->lines[cursor->line].size) {
            lines->lines[cursor->line].chars = realloc(lines->lines[cursor->line].chars,
                                                       lines->lines[cursor->line].size + TAB_SPACE_COUNT);
            for (size_t i = 0; i < TAB_SPACE_COUNT; i++) {
                lines->lines[cursor->line].chars[cursor->x++] = ' ';
            }
            lines->lines[cursor->line].chars[cursor->x] = '\0';
            lines->lines[cursor->line].size += TAB_SPACE_COUNT;
        } else {
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
    }
    else if(keyDown[SDL_SCANCODE_F1]) {
        swapChar1 = lines->lines[cursor->line].chars[cursor->x];
        swapChar1pos = cursor->x;
        return true;
    }
    else if(keyDown[SDL_SCANCODE_F2]) {
        swapChar2 = lines->lines[cursor->line].chars[cursor->x];
        swapChar2pos = cursor->x;
        return true;
    }
    else if(keyDown[SDL_SCANCODE_F3]) {
        lines->lines[cursor->line].chars[swapChar2pos] = swapChar1;
        lines->lines[cursor->line].chars[swapChar1pos] = swapChar2;
        return true;
    }
    else if (keyDown[SDL_SCANCODE_LCTRL] && keyDown[SDL_SCANCODE_S] ) {
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
        if (lines->lines[cursor->line].size > cursor->x)
        {
            char* buffer = malloc(lines->lines[cursor->line].size - cursor->x + 1);
            strncpy(buffer, lines->lines[cursor->line].chars + cursor->x, lines->lines[cursor->line].size - cursor->x + 1);
            lines->lines[cursor->line].chars = realloc(lines->lines[cursor->line].chars,(lines->lines[cursor->line].size + 2) * sizeof(char));
            lines->lines[cursor->line].chars[cursor->x] = (char) keycode;
            lines->lines[cursor->line].chars[++cursor->x] = '\0';
            strcat(lines->lines[cursor->line].chars, buffer);
            lines->lines[cursor->line].chars[++lines->lines[cursor->line].size] = '\0';
            free(buffer);
            buffer = NULL;
        } else
        {
            lines->lines[cursor->line].size++;
            lines->lines[cursor->line].chars = realloc(lines->lines[cursor->line].chars, (lines->lines[cursor->line].size + 1) * sizeof(char));
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
                if (!isAnyFunction(cursor, lines, Window) && !isFuntionKey())
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