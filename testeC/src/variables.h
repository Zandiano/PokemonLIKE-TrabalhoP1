#ifndef __VARIABLES_H
#define __VARIABLES_H

#include "limits.h"
#include "objects.h"

struct specie allSpecies[MAXFILES] = {0};
struct ability allAbilities[MAXFILES] = {0};
char filesName[MAXFILES][64] = {0};
FILE *filesPtrs[2][MAXFILES] = {NULL};

// Dev Wise
bool debugMode = FALSE;
bool invencivel = FALSE;
bool cheatMode = FALSE;

// INIT
int scene = MENU;
bool running = TRUE;
int worldTurn = 0;
int backgroundType = BLACK;
int enemyIndex = -1;
int enemyQnt = 2;
int dificuldade = 2;
char input = ' ';

// Battle
int aggro_Enemy = 12;
int battleTurn = 0;
int heartsCounter[2] = {16,16};
bool forceSwitch = FALSE;

// Scores
int walkedDistance = 0;
int enemiesKilled = 0;
int closestToDeath = 16;


#endif