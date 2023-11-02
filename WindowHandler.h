//
// Created by David on 2023. 10. 16.
//

#ifndef NHF_WINDOWHANDLER_H
#define NHF_WINDOWHANDLER_H

#include "type.h"

Lines *CreateBlankPage();
Lines *OpenFile(const char* filename);
WindowHandler CreateSDLWindow();
void FreeMemoryLines(Lines *lines);

#endif //NHF_WINDOWHANDLER_H
