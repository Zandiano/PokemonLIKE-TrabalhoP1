#ifndef SPECIE
#define SPECIE

#include "objects.h"
#define MAXSPECIES 30

struct specie{
    struct indentifier indentifier;
    struct portrait portrait;
    int num;
    int rarity;
    enum atr base[6];
    enum element element[2];
    struct color color[2];
};

#endif