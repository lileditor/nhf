//
// Sebe-Norbert-Dávid-FJDH0C-18/10/2023-Szovegszerkezto
//

#include "cursor.h"
#include "type.h"

Cursor MoveCursor(Cursor *cursor, Lines *lines, int Direction)
{
    switch (Direction) {
        case 1:
            if (cursor->x > 0)
                --cursor->x;
            else if (cursor->line > 0)
            {
                --cursor->line;
                cursor->x = lines->lines[cursor->line].size;
            }
            break;
        case 2:
            if (lines->lines[cursor->line].size >= cursor->x + 1)
                ++cursor->x;
            else {
                if (cursor->line < lines->size - 1)
                {
                    ++cursor->line;
                    cursor->x = 0;
                }
            }
            break;
        case 3:
            if (cursor->line > 0)
            {
                --cursor->line;
                if (cursor->x > lines->lines[cursor->line].size)
                    cursor->x = lines->lines[cursor->line].size;
            }
            break;
        case 4:
            if (cursor->line < lines->size - 1)
            {
                ++cursor->line;
                if (cursor->x > lines->lines[cursor->line].size)
                    cursor->x = lines->lines[cursor->line].size;
            }
            break;
        default:
            break;
    }
}