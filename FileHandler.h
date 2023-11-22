//
// Sebe-Norbert-Dávid-FJDH0C-26/10/2023-Szovegszerkezto
//

#ifndef NHF_FILEHANDLER_H
#define NHF_FILEHANDLER_H
#include "type.h"

char *GenerateFileName(char *filename);
Lines* OpenFile(const char* filename);
void SaveFile(const char* filename, Lines *lines);

#endif //NHF_FILEHANDLER_H
