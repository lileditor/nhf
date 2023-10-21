//
// Created by David on 2023. 10. 16
//
#include <stdio.h>
#include <stdlib.h>
#include <SDL_video.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "WindowHandler.h"
#include "type.h"
#include <string.h>

Lines CreateBlankPage()
{
    Lines lines;
    Line line;
    line.size = 0;
    line.chars = (char*)malloc(100);
    line.chars[line.size] = '\0';
    lines.lines = (Line*)malloc(100);
    lines.lines[0] = line;
    lines.size = 1;
    return lines;
}

Lines OpenFile(const char* filename)
{
    Lines lines;
    lines.size = 0;
    lines.lines = (Line*)malloc(100);
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Nem sikerult megnyitni a file: %s\n", filename);
        exit(1);
    }
    char buffer[1024];
    char c = fgetc(file);
    int counter = 0;
    while (c != EOF)
    {
        while (c != '\n' && c != EOF)
        {
            if (c == '\t') {
                buffer[counter] = ' ';
                buffer[++counter] = ' ';
                buffer[++counter] = ' ';
                buffer[++counter] = ' ';
            } else
            {
                buffer[counter] = c;
            }
            ++counter;
            c = fgetc(file);
        }
        buffer[counter] = '\0';
        lines.lines[lines.size].chars = strdup(buffer);
        lines.lines[lines.size].size = counter;
        ++lines.size;
        counter = 0;
        c = fgetc(file);
    }
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