#ifndef __STRUCTS_H
#define __STRUCTS_H

#include "objects.h"
#include "enums.h"

struct specie{
    struct indentifier indentifier;
    struct portrait portrait;
    int num;
    int rarity;
    enum atr base[6];
    enum element element[2];
    struct color color[2];
    int naturalAbilities[6];
};

struct entity{
    struct indentifier indentifier;
    struct pos pos;
    struct health health;
    int level;
    int velo;
    enum COLORS currentColor;
    int IV[6];
    int atributes[6];
    struct specie specie;
    struct nature nature;
    struct ability abilities[4];
    bool isShiny;
    bool inactive;
};

struct player{
    struct pos pos;
    struct entity bag[4];
    int currentEntity;
    int level;
    int velo;
    bool defeated;
};

#endif