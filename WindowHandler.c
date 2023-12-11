//
// Sebe-Norbert-Dávid-FJDH0C-16/10/2023-Szovegszerkezto
//
#include <stdio.h>
#include <stdlib.h>
#include <SDL_video.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "WindowHandler.h"
#include "debugmalloc.h"
#include "type.h"

Lines* CreateBlankPage()
{
    Lines *lines;
    lines = (Lines *)malloc(sizeof(Lines));
    lines->size = 0;
    lines->lines = (Line *)malloc(sizeof(Line));
    lines->lines[lines->size].size = 0;
    lines->lines[lines->size].chars = (char *)malloc(sizeof(char));
    lines->lines[lines->size].chars[lines->lines[lines->size].size] = '\0';
    lines->size++;
    return lines;
}

void FreeMemoryLines(Lines *lines)
{
    for (int i = 0; i < lines->size; i++) {
        free(lines->lines[i].chars);
    }
    free(lines->lines);
    free(lines);
}

WindowHandler CreateSDLWindow()
{
    WindowHandler Window;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }

    Window.window = SDL_CreateWindow("edits", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (Window.window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_SetWindowResizable(Window.window, SDL_TRUE);
    Window.renderer = SDL_CreateRenderer(Window.window, -1, SDL_RENDERER_SOFTWARE);
    if (Window.renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() < 0)
    {
        printf("Nem hozhato letre a TTF: %s\n", SDL_GetError());
        exit(1);
    }

    Window.font = TTF_OpenFont("../SourceCode.ttf", WINDOW_BPP);
    if (Window.font == NULL)
    {
        printf("Nem lehet betolteni a betutipust: %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Color fg = {255, 255, 255};
    Window.surface = TTF_RenderText_Solid(Window.font, "na csa!", fg);
    if (Window.surface == NULL)
    {
        printf("Nem lehet megjeliteni a szoveget: %s\n", TTF_GetError());
        exit(1);
    }
    Window.texture = SDL_CreateTextureFromSurface(Window.renderer, Window.surface);
    if (Window.texture == NULL)
    {
        printf("Nem lehet megjelenteni a texurat: %s\n", SDL_GetError());
        exit(1);
    }
    return Window;
}