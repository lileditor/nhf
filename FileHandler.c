//
// Created by David on 26/10/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "debugmalloc.h"
#include "FileHandler.h"

const char *getRandomChar(char *c)
{
    char* chars = "abcdefghijklmnopqrstuvwxyz";
    int i = rand() % strlen(chars);
    *c =  chars[i];
    return c;
}

char* GenerateFileName(char * filename)
{
    int i = 0;
    while (i < 10)
    {
        char *c;
        strcat(filename, (const char *) getRandomChar(c));
        i++;
    }
    strcat(filename, ".txt");
    return filename;
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
    lines->lines = (Line *) malloc((sizeof (Line) + sizeof(Lines)) * 10);
    lines->lines[lines->size].chars = (char *)malloc(sizeof(char));
    lines->lines[lines->size].size = 0;
    char c;
    while ((c = (char) fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            lines->lines[lines->size].chars[lines->lines[lines->size].size] = '\0';
            lines->size++;
            lines->lines = (Line *) realloc(lines->lines, (lines->size + 1) * sizeof(Lines));
            lines->lines[lines->size].chars = (char *)malloc(sizeof(char));
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
            lines->lines[lines->size].chars = (char *)realloc( lines->lines[lines->size].chars, lines->lines[lines->size].size + 2 * sizeof(char));
            lines->lines[lines->size].chars[lines->lines[lines->size].size++] = c;
        }
    }
    lines->lines[lines->size].chars = (char *)realloc( lines->lines[lines->size].chars, (lines->lines[lines->size].size + 1) * sizeof(char));
    lines->lines[lines->size].chars[lines->lines[lines->size++].size] = '\0';
    fclose(file);
    return lines;
}

void SaveFile(const char* filename, Lines *lines)
{
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < lines->size; i++)
    {
        if (i == lines->size - 1){
            fprintf(file, "%s", lines->lines[i].chars);
        } else {
            fprintf(file, "%s\n", lines->lines[i].chars);
        }
    }
    fclose(file);
}