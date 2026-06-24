#ifndef PIKOMONOBJECTS
#define PIKOMONOBJECTS

#include "enums.h"
#include "gconio.h"

struct color{enum COLORS idle; enum COLORS pursuit;};
struct damage{int value; enum element element; enum type type;};
struct nature{enum atr up; enum atr down;};
struct portrait{char battle[5][5][11];};
struct indentifier{char symbol; char name[14];};
struct pos{int x; int y;};
struct health{int current; int max;};
struct ability{struct indentifier indentifier; struct damage damage; int accuracy; char logMessage[2][60];};

#endif