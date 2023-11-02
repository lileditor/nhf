//
// Created by David on 26/10/2023.
//

#ifndef NHF_FILEHANDLER_H
#define NHF_FILEHANDLER_H
#include "type.h"

Lines* OpenFile(const char* filename);
void SaveFile(const char* filename, Lines *lines);

#endif //NHF_FILEHANDLER_H
