#ifndef __ENTITY_H
#define __ENTITY_H

#include "objects.h"

#define atrEq(constant, base, IV, level) constant*base*(IV/100.0f+1)*(level/40.0f+1)

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

bool CopyEntity(struct entity *dest, struct entity from){
    dest->indentifier = from.indentifier;
    dest->pos = from.pos;
    dest->health = from.health;
    dest->level = from.level;
    dest->velo = from.velo;
    dest->isShiny = from.isShiny;
    dest->currentColor = from.currentColor;
    for(int i = 0; i < 6; i++){
        dest->IV[i] = from.IV[i];
        dest->atributes[i] = from.atributes[i];
    }
    dest->specie = from.specie;
    dest->nature = from.nature;
    for(int i = 0; i < 4; i++){
        dest->abilities[i] = from.abilities[i];
    }
    return TRUE;
}

void LevelUpEntity(struct entity *entity){
    entity->level++;
    UpdateEntity(entity);
}

bool ResetEntity(struct entity *entity, struct player jogador){
    int rndSpecie = rand()%30;
    do{
        rndSpecie = rand()%30;
    }while(allSpecies[rndSpecie].indentifier.symbol == '\0');
    entity->specie = allSpecies[rndSpecie];
    
    for(int j = 0; j < 4; j++){
        int sortedAbility = entity->specie.naturalAbilities[rand()%6];
        entity->abilities[j].damage.value = allAbilities[sortedAbility].damage.value;
        entity->abilities[j].damage.element = allAbilities[sortedAbility].damage.element;
        entity->abilities[j].damage.type = allAbilities[sortedAbility].damage.type;
        entity->abilities[j].accuracy = allAbilities[sortedAbility].accuracy;
        entity->abilities[j].indentifier.symbol = allAbilities[sortedAbility].indentifier.symbol;
        strcpy(entity->abilities[j].indentifier.name, allAbilities[sortedAbility].indentifier.name);
        strcpy(entity->abilities[j].logMessage[0], allAbilities[sortedAbility].logMessage[0]);
        strcpy(entity->abilities[j].logMessage[1], allAbilities[sortedAbility].logMessage[1]);
    }
    
    entity->isShiny = !(rand()%15);
    entity->indentifier.symbol = entity->specie.indentifier.symbol;
    strcpy(entity->indentifier.name, entity->specie.indentifier.name);
    entity->pos.x = (rand()%(MAX_COLUNA-1))+1;
    entity->pos.y = (rand()%(MAX_LINHA-1))+1;
    entity->velo = 1;
    entity->level = max(1,(rand()%3)-1+jogador.level);
    
    UpdateEntity(entity);
    return TRUE;
}

void ResetEnemies(struct entity enemies[], struct player jogador){
    for(int i = 0; i < enemyQnt; i++){
        ResetEntity(&enemies[i], jogador);
    }
}


#endif