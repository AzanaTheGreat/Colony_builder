#ifndef AZANALIB

#include "AzanaLib/SDL.c"
#include "AzanaLib/Shapes.c"
#include "AzanaLib/Shapes3D.c"
#include "AzanaLib/Text.c"
#include <time.h>

char AZL_WHITE[] = {255, 255, 255, 255};
char AZL_LIGHT_GRAY[] = {225, 225, 225, 225};
char AZL_GRAY[] = {200, 200, 200, 255};
char AZL_DARK_GRAY[] = {150, 150, 150, 255};
char AZL_DARKER_GRAY[] = {100, 100, 100, 255};
char AZL_RED[] = {255, 0, 0, 255};
char AZL_GREEN[] = {0, 255, 0, 255};
char AZL_BLUE[] = {0, 0, 255, 255};
char AZL_BLACK[] = {0, 0, 0, 255};

char AZL_CUSTOM_COLOR[4];

void AZL_CREATE_CUSTOM_COLOR(char r, char g, char b, char a) {
    AZL_CUSTOM_COLOR[0] = r;
    AZL_CUSTOM_COLOR[1] = g;
    AZL_CUSTOM_COLOR[2] = b;
    AZL_CUSTOM_COLOR[3] = a;
}

#endif