#ifndef __WORLD_H
#define __WORLD_H

#include <math.h>
#include "objects.h"
#include "variables.h"

void PlayerWorldLogic(struct player *jogador){
    input = toupper(getch());

    switch(input){
        case 'W':
            jogador->pos.y -= jogador->velo;
            walkedDistance++;
            break;
        case 'S':
            jogador->pos.y += jogador->velo;
            walkedDistance++;
            break;
        case 'A':
            jogador->pos.x -= jogador->velo;
            walkedDistance++;
            break;
        case 'D':
            jogador->pos.x += jogador->velo;
            walkedDistance++;
            break;
    }

    switch(input){
        case 'O':
            invencivel = !invencivel;
            break;
        case 'P':
            debugMode = !debugMode;
            break;
        case 'R':
            worldTurn = -1;
            break;
        case 'M':
            scene = MENU;
    }

    if(jogador->pos.x <= 0){
        jogador->pos.x = 0;
    }
    else if(jogador->pos.x >= MAX_COLUNA){
        jogador->pos.x = MAX_COLUNA;
    }
    
    if(jogador->pos.y <= 0){
        jogador->pos.y = 0;
    }
    else if(jogador->pos.y >= MAX_LINHA-5){
        jogador->pos.y = MAX_LINHA-5;
    }
}

void EnemyWorldLogic(struct entity *enemy, struct player jogador, int i){
    int xDist = jogador.pos.x - enemy->pos.x;
    int yDist = jogador.pos.y - enemy->pos.y;
    int dist = sqrt(xDist*xDist + yDist*yDist); /// Distancia entre o jogador e o inimigo

    if(dist > aggro_Enemy){             /// Fora da range
        enemy->pos.x += (rand() % 3 - 1) * enemy->velo;
        enemy->pos.y += (rand() % 3 - 1) * enemy->velo;
        enemy->currentColor = enemy->specie.color[enemy->isShiny].idle;
    }
    else if(dist > enemy->velo){         /// Dentro da range
        if(xDist != 0){
            enemy->pos.x += (xDist / abs(xDist)) * enemy->velo;
        }
        if(yDist != 0){
            enemy->pos.y += (yDist / abs(yDist)) * enemy->velo;
        }
        enemy->currentColor = enemy->specie.color[enemy->isShiny].pursuit;
    }
    else if(!invencivel){              /// Encostou no jogador
        enemyIndex = i;
        scene = BATTLE;
        Sleep(200);
    }
    if(enemy->pos.x <= 0){
        enemy->pos.x = 0;
    }
    else if(enemy->pos.x >= MAX_COLUNA){
        enemy->pos.x = MAX_COLUNA;
    }

    if(enemy->pos.y <= 0){
        enemy->pos.y = 0;
    }
    else if(enemy->pos.y >= MAX_LINHA-5){
        enemy->pos.y = MAX_LINHA-5;
    }
}

#endif