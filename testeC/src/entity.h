#ifndef __ENTITY_H
#define __ENTITY_H

#include "objects.h"
#include "specie.h"

#define atrEq(constant, base, IV, level) constant*base*(IV/100.0f+1)*(level/40.0f+1)

struct entity{
    struct indentifier indentifier;
    struct pos pos;
    struct health health;
    int level;
    int velo;
    enum COLORS currentColor;
    enum atr IV[6];
    enum atr atributes[6];
    struct specie specie;
    struct nature nature;
    struct ability abilities[4];
    bool isShiny;
    bool inactive;
};

void UpdateEntity(struct entity *entity){
    entity->atributes[hp] = (int)atrEq(1.5f, entity->specie.base[hp], entity->IV[hp], entity->level);
    entity->atributes[atk] = (int)atrEq(1, entity->specie.base[atk], entity->IV[atk], entity->level);
    entity->atributes[spatk] = (int)atrEq(1, entity->specie.base[spatk], entity->IV[spatk], entity->level);
    entity->atributes[def] = (int)atrEq(1.2f, entity->specie.base[def], entity->IV[def], entity->level);
    entity->atributes[spdef] = (int)atrEq(1.2f, entity->specie.base[spdef], entity->IV[spdef], entity->level);
    entity->atributes[spd] = (int)atrEq(0.5f, entity->specie.base[spd], entity->IV[spd], entity->level);
    
    entity->health.max = entity->atributes[hp];
    entity->health.current = entity->health.max;
}
#endif