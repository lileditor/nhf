//
// Created by David on 2023. 10. 16
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
    return lines;
}

Lines* OpenFile(const char* filename)
{
    Lines *lines;
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Couldn't open file: %s\n", filename);
    }
    lines = (Lines *)malloc(sizeof(Lines));
    lines->size = 0;
    lines->lines = (Line *)malloc(sizeof(Line));
    lines->lines[lines->size].chars = (char *)malloc(sizeof(char));
    lines->lines[lines->size].size = 0;
    char c;
    while ((c = (char) fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            lines->lines[lines->size].chars[lines->lines[lines->size].size] = '\0';
            lines->lines[++lines->size].chars = (char *)malloc(sizeof(char));
            lines->lines[lines->size].size = 0;
        }
        else if (c == '\t')
        {
            for (int i = 0; i < 4; i++)
            {
                lines->lines[lines->size].chars = (char *)realloc( lines->lines[lines->size].chars, lines->lines[lines->size].size + 1 * sizeof(char));
                lines->lines[lines->size].chars[lines->lines[lines->size].size] = ' ';
                lines->lines[lines->size].size++;
            }
        }
        else
        {
            lines->lines[lines->size].chars = (char *)realloc( lines->lines[lines->size].chars, lines->lines[lines->size].size + 1 * sizeof(char));
            lines->lines[lines->size].chars[lines->lines[lines->size].size] = c;
            lines->lines[lines->size].size += 1;
        }
    }
    lines->lines[lines->size].chars = (char *)realloc( lines->lines[lines->size].chars, lines->lines[lines->size].size + 1 * sizeof(char));
    lines->lines[lines->size].chars[lines->lines[lines->size++].size] = '\0';
    fclose(file);
    return lines;
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
        printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
        exit(1);
    }

    Window.font = TTF_OpenFont("../SourceCode.ttf", WINDOW_BPP);
    if (Window.font == NULL)
    {
        printf("Couldn't load font: %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Color fg = {255, 255, 255};
    Window.surface = TTF_RenderText_Solid(Window.font, "na csa!", fg);
    if (Window.surface == NULL)
    {
        printf("Couldn't render text: %s\n", TTF_GetError());
        exit(1);
    }
    Window.texture = SDL_CreateTextureFromSurface(Window.renderer, Window.surface);
    if (Window.texture == NULL)
    {
        printf("Couldn't create texture: %s\n", SDL_GetError());
        exit(1);
    }
    return Window;
}