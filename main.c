//
// Sebe-Norbert-D�vid-FJDH0C-18/10/2023-Szovegszerkezto
//

#include <SDL.h>
#include "WindowHandler.h"
#include "type.h"
#include "FileHandler.h"
#include "debugmalloc.h"
#include "KeyboardHandler.h"

int quit = 0;
int main(int argc, char *argv[]) {
    SDL_Event event;
    WindowHandler Window = CreateSDLWindow();
    if (argc > 1)
    {
        Lines *lines = OpenFile(argv[1]);
        Cursor Cursor = {0, 0};
        SDL_Color fg = {255, 255, 255};
        while (!quit) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = 1;
                }
                KeyboardHandler(&event, lines, &Cursor, &Window);
            }
            int bpp = GetBPP();
            SDL_SetRenderDrawColor(Window.renderer, 0, 0, 0, 0);
            SDL_RenderClear(Window.renderer);
            int y = 0;
            for (int i = 0; i < lines->size; i++) {
                char* line = lines->lines[i].chars;
                if (*line != '\0') {
                    SDL_Surface *lineSurface = TTF_RenderText_Solid(Window.font, line, fg);
                    if (lineSurface == NULL) {
                        printf("Couldn't render text: %s\n", TTF_GetError());
                        exit(1);
                    }
                    SDL_Texture *lineTexture = SDL_CreateTextureFromSurface(Window.renderer, lineSurface);
                    if (lineTexture == NULL) {
                        printf("Couldn't create texture from surface: %s\n", SDL_GetError());
                        exit(1);
                    }
                    SDL_Rect destRect;
                    destRect.x = 0;
                    destRect.y = y;
                    destRect.w = lineSurface->w;
                    destRect.h = lineSurface->h;
                    SDL_RenderCopy(Window.renderer, lineTexture, NULL, &destRect);
                    SDL_FreeSurface(lineSurface);
                    SDL_DestroyTexture(lineTexture);
                    if (i == Cursor.line) {
                        int cursorX = bpp % 10 >= 5 ? Cursor.x * ((bpp /2) + bpp/10 + 1) : Cursor.x * ((bpp /2) + bpp/10);
                        int cursorY = y;
                        SDL_SetRenderDrawColor(Window.renderer, 255, 0, 0, 255);
                        SDL_Rect cursorRect;
                        cursorRect.x = cursorX;
                        cursorRect.y = cursorY + 2;
                        cursorRect.w = 2;
                        cursorRect.h = bpp;
                        SDL_RenderFillRect(Window.renderer, &cursorRect);
                    }
                    y += lineSurface->h;
                }
                else
                {
                    SDL_Surface *lineSurface = TTF_RenderText_Solid(Window.font, "", fg);
                    SDL_Rect destRect;
                    destRect.x = 0;
                    destRect.y = y;
                    destRect.w = Window.surface->w;
                    destRect.h = Window.surface->h;
                    SDL_RenderCopy(Window.renderer, Window.texture, NULL, &destRect);
                    SDL_FreeSurface(lineSurface);
                    SDL_DestroyTexture(Window.texture);
                    if (i == Cursor.line) {
                        int cursorX = bpp % 10 >= 5 ? Cursor.x * ((bpp /2) + bpp/10 + 1) : Cursor.x * ((bpp /2) + bpp/10);
                        int cursorY = y;
                        SDL_SetRenderDrawColor(Window.renderer, 255, 0, 0, 255);
                        SDL_Rect cursorRect;
                        cursorRect.x = cursorX;
                        cursorRect.y = cursorY;
                        cursorRect.w = 2;
                        cursorRect.h = bpp;
                        SDL_RenderFillRect(Window.renderer, &cursorRect);
                    }
                    y += Window.surface->h;
                }
            }

            SDL_RenderPresent(Window.renderer);
        }
        SaveFile(argv[1], lines);
        SDL_FreeSurface(Window.surface);
        SDL_DestroyTexture(Window.texture);
        FreeMemoryLines(lines);
        debugmalloc_log_file("memlog.txt");
    }
    else
    {
        Lines *lines = CreateBlankPage();
        Cursor Cursor = {0, 0};
        SDL_Color fg = {255, 255, 255};

        while (!quit)
        {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = 1;
                }
                KeyboardHandler(&event, lines, &Cursor, &Window);
            }
            int bpp = GetBPP();
            int y = 0;
            SDL_SetRenderDrawColor(Window.renderer, 0, 0, 0, 0);
            SDL_RenderClear(Window.renderer);
            for (int i = 0; i < lines->size; i++) {
                char* line = lines->lines[i].chars;
                if (*line != '\0') {
                    SDL_Surface *lineSurface = TTF_RenderText_Solid(Window.font, line, fg);
                    if (lineSurface == NULL) {
                        printf("Couldn't render text: %s\n", TTF_GetError());
                        exit(1);
                    }
                    SDL_Texture *lineTexture = SDL_CreateTextureFromSurface(Window.renderer, lineSurface);
                    if (lineTexture == NULL) {
                        printf("Couldn't create texture from surface: %s\n", SDL_GetError());
                        exit(1);
                    }
                    SDL_Rect destRect;
                    destRect.x = 0;
                    destRect.y = y;
                    destRect.w = lineSurface->w;
                    destRect.h = lineSurface->h;
                    SDL_RenderCopy(Window.renderer, lineTexture, NULL, &destRect);
                    SDL_FreeSurface(lineSurface);
                    SDL_DestroyTexture(lineTexture);
                    if (i == Cursor.line) {
                        int cursorX = bpp % 10 >= 5 ? Cursor.x * ((bpp /2) + bpp/10 + 1) : Cursor.x * ((bpp /2) + bpp/10);
                        int cursorY = y;
                        SDL_SetRenderDrawColor(Window.renderer, 255, 0, 0, 255);
                        SDL_Rect cursorRect;
                        cursorRect.x = cursorX;
                        cursorRect.y = cursorY + 2;
                        cursorRect.w = 2;
                        cursorRect.h = bpp;
                        SDL_RenderFillRect(Window.renderer, &cursorRect);
                    }
                    y += lineSurface->h;
                }
                else
                {
                    SDL_Surface *lineSurface = TTF_RenderText_Solid(Window.font, "", fg);
                    SDL_Rect destRect;
                    destRect.x = 0;
                    destRect.y = y;
                    destRect.w = Window.surface->w;
                    destRect.h = Window.surface->h;
                    SDL_RenderCopy(Window.renderer, Window.texture, NULL, &destRect);
                    SDL_FreeSurface(lineSurface);
                    SDL_DestroyTexture(Window.texture);
                    if (i == Cursor.line) {
                        int cursorX = bpp % 10 >= 5 ? Cursor.x * ((bpp /2) + bpp/10 + 1) : Cursor.x * ((bpp /2) + bpp/10);
                        int cursorY = y;
                        SDL_SetRenderDrawColor(Window.renderer, 255, 0, 0, 255);
                        SDL_Rect cursorRect;
                        cursorRect.x = cursorX;
                        cursorRect.y = cursorY;
                        cursorRect.w = 2;
                        cursorRect.h = bpp;
                        SDL_RenderFillRect(Window.renderer, &cursorRect);
                    }
                    y += Window.surface->h;
                }
            }
            SDL_RenderPresent(Window.renderer);
        }
        char filename[FILENAME_LENGTH + 1] = "";
        SaveFile(GenerateFileName(filename), lines);
        SDL_FreeSurface(Window.surface);
        SDL_DestroyTexture(Window.texture);
        FreeMemoryLines(lines);
        debugmalloc_log_file("memlog.txt");
    }
    SDL_Quit();

    return 0;
}