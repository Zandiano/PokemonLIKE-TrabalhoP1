#ifndef __PLAYER_H
#define __PLAYER_H

#include "objects.h"
#include "entity.h"
#include "variables.h"

#define euler 2.8f
#define catchEq(hearts, levelDiff, rarity, Difficulty) (24 - hearts) * pow(euler, levelDiff/3) * 125 / Difficulty * rarity * 6

void Init(struct player *jogador){
    // Scores
    walkedDistance = 0;
    enemiesKilled = 0;
    closestToDeath = 16;

    worldTurn = 0;
    battleTurn = 0;

    jogador->pos.x = rand()%MAX_COLUNA;
    jogador->pos.y = rand()%MAX_LINHA;

    jogador->defeated = FALSE;
    jogador->level = 0;
    memset(jogador->bag, 0, sizeof(jogador->bag[0])*4);
    ResetEntity(&jogador->bag[0], *jogador);
}

void CalcPlayerLevel(struct player *jogador){
    int i, media = 0;
    for(i = 0; jogador->bag[i].indentifier.symbol != '\0'; i++){
        media += jogador->bag[i].level;
    }
    jogador->level = media/(i+1)+1;
}

bool CatchEntity(struct player *catcher, struct entity catch, int currentHeart, int slot){
    if(rand()%100 > catchEq(currentHeart, catch.level-catcher->level, catch.specie.rarity, dificuldade) && !cheatMode)
        return FALSE;
    CopyEntity(&catcher->bag[slot], catch);
    return TRUE;
}

#endif