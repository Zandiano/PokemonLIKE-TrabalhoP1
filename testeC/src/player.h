#ifndef __PLAYER_H
#define __PLAYER_H

#include "objects.h"
#include "entity.h"

struct player{
    struct pos pos;
    struct entity bag[4];
    int currentEntity;
    int level;
    int velo;
    bool defeated;
};
#endif