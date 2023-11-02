//
// Created by David on 2023. 10. 16.
//

#ifndef NHF_TYPE_H
#define NHF_TYPE_H

#include <SDL_ttf.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_BPP 18
#define TAB_SPACE_COUNT 4

typedef struct Line {
    char *chars;
    int size;
} Line;

typedef struct Lines {
    Line *lines;
    int size;
} Lines;

typedef struct Cursor {
    int x, line;
} Cursor;

typedef struct WindowHandler {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Surface *surface;
    TTF_Font *font;
} WindowHandler;


#endif //NHF_TYPE_H
