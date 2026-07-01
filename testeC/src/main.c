#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "bool.h"
#include "gconio.h"

#include "objects.h"
#include "fileHandler.h"
#include "elements.h"
#include "entity.h"
#include "player.h"
#include "world.h"
#include "battle.h"

void END_SCENE(struct player *jogador){
    textcolor(CYAN);
    gotoxy(MAX_COLUNA/2-10, MAX_LINHA/2); printf("PERDEU MANE");
    gotoxy(MAX_COLUNA/2-30, MAX_LINHA/2+2); printf("Voce andou %d caracteres de distancia", walkedDistance);
    gotoxy(MAX_COLUNA/2-30, MAX_LINHA/2+4); printf("Voce matou %d inimigo(s)", enemiesKilled);
    gotoxy(MAX_COLUNA/2-30, MAX_LINHA/2+6); printf("Em sua batalha mais dificil voce ficou com %d coracoes de vida", closestToDeath);
    getch();
    scene = MENU;
    jogador->defeated = TRUE;
    Sleep(3000);
}

void MENU_SCENE(struct player *jogador){
    clrscr();
    textcolor(MAGENTA);
    printf("+========================================================================================+\n");
    printf("        *     +            TRABALHO PROGRAMACAO 1 - RAPHAEL                    +          \n");
    printf("'    '           *               .              o                         *               \n");
    printf("             .        '  .               +       +           +                            \n");
    printf("                                    o         ___ _____        ___          ___    __     \n");
    printf("                        .-''''-.             / _ \\\\_   \\/\\ /\\ /___\\/\\/\\    /___\\/\\ \\ \\    \n");
    printf("                      .' () .   '.   .      / /_)/ / /\\/ //_///  //    \\  //  //  \\/ /    \n");
    printf("                     / .      o   \\        / ___/\\/ /_/ __ \\/ \\_// /\\/\\ \\/ \\_// /\\  /   . \n");
    printf("    .               ; o    _   ()  ;       \\/   \\____/\\/  \\/\\___/\\/    \\/\\___/\\_\\ \\/     \n");
    printf("            o o   . ;     (_)      ;  +    turn-based rpg    .                     .:'   \n");
    printf("                     \\ .        o /                                 +           _.::'     \n");
    printf("                      '.  O  .  .'                        .            '     + (_.'      +\n");
    printf("                  .     '-....-'             .  .                '                        \n");
    printf("    |     + ' .                       *                     +            .       + .      \n");
    printf("  --o--                    +============================+          +                 .    \n");
    printf("    |   '         '        | 1.INICIAR            .     |   +   o                         \n");
    printf("                           | 2.OPCOES                   |                                 \n");
    printf("                           | 3.SAIR        +           .:'                                \n");
    printf("        +                  +=======================_.::=+                 .      *       .\n");
    printf("          '                      - o -            (_.'                +                   \n");
    printf("             o  . ' +       '      |   o                      .          +                \n");
    printf("                                         +                                         .  +   \n");
    printf("                         '                    +                                           \n");
    printf("                   o               .  +         .                                     .   \n");
    printf(" ZANDIADEV       +    +  o                                      .            *    +       \n");
    printf("+========================================================================================+");

    for(int i = 1; i < MAX_LINHA; i++){
        gotoxy(0,i); printf("|");
        gotoxy(MAX_COLUNA-1,i); printf("|");
    }

    input = getch();

    switch(input){
        case '1':
            clrscr();
            if(jogador->defeated){
                Init(jogador);
            }
            gotoxy(MAX_COLUNA/2-10, MAX_LINHA/2);
            printf("CARREGANDO");
            for(int i = 0; i < 3; i++){
                Sleep(1000);
                printf(".");
            }
            scene = WORLD_MAP;
            break;
        case '2':
            clrscr();
            gotoxy(0,0);
            printf("Quantos inimigos - [Default: 2] (1-10): ");
            gotoxy(0,1);
            printf("Dificuldade - [Default: 3] (1-10): ");
            gotoxy(0,2);
            printf("CheatMode -1 [Default: 0] (0-1): ");
            gotoxy(40,0); scanf("%d", &enemyQnt);
            gotoxy(40,1); scanf("%d", &dificuldade);
            gotoxy(40,2); scanf("%d", &cheatMode);
            enemyQnt = min(max(enemyQnt,1),10);
            dificuldade = min(max(dificuldade,1),10);
            break;
            
        case '3':
            clrscr();
            gotoxy(MAX_COLUNA/2-10, MAX_LINHA/2);
            printf(";(");
            running = FALSE;
            Sleep(3000);
            break;
    }
}

void WORLD_SCENE(struct entity enemies[], struct player *jogador){
    textbackground(backgroundType);
    if(!worldTurn){
        battleTurn = 0;
        
        ResetEnemies(enemies, *jogador);
        CalcPlayerLevel(jogador);
    }
    else{
        PlayerWorldLogic(jogador);

        for(int i = 0; i < enemyQnt; i++){
            EnemyWorldLogic(&enemies[i], *jogador, i);
        }
    }

    worldTurn++;
    worldTurn = worldTurn%100;

    // RENDER
    textcolor(MAGENTA);
    Moldura(TRUE, '+', '|', '=');
    
    int viewX[2] = {2,30}; int viewY[2] = {22,23};
    EntityView(jogador->bag, viewX, viewY);
    
    gotoxy(MAX_COLUNA,MAX_LINHA); textcolor(jogador->level+2); printf("%d", jogador->level);
    gotoxy(jogador->pos.x, jogador->pos.y); textcolor(YELLOW); printf("@");
    
    for(int i = 0; i < enemyQnt; i++){
        gotoxy(enemies[i].pos.x, enemies[i].pos.y); 
        textcolor(enemies[i].currentColor); printf("%c", enemies[i].indentifier.symbol);
    }
    
    gotoxy(MAX_COLUNA, MAX_LINHA+1);
}

void BATTLE_SCENE(struct entity *enemy, struct player *jogador){
    int attempt[2] = {0};
    char logs[2][60] = {"",""};
    bool win = FALSE;
    
    // Pre Turn
    if(!battleTurn){
        CaveiraAnim(backgroundType, CYAN);
        jogador->currentEntity = 0;
        enemy->inactive = FALSE;
        jogador->bag[jogador->currentEntity].inactive = FALSE;
        battleTurn++;
    }
    else{
        PlayerBattleLogic(enemy, jogador, attempt, logs, &win);
        EnemyBattleLogic(enemy, jogador, attempt, logs);        

        heartsCounter[1] = (enemy->health.current*1.0f/enemy->health.max)*16;
        heartsCounter[0] = (jogador->bag[jogador->currentEntity].health.current*1.0f/jogador->bag[jogador->currentEntity].health.max)*16;
    }

    // RENDER
    textbackground(backgroundType); textcolor(MAGENTA);
    Moldura(TRUE, '+', '|', '=');
    
    // RENDER CHARS
    PrintPortrait(78,2,enemy->specie.portrait,heartsCounter[1]);
    PrintHearts(heartsCounter[1], 72, 9);

    PrintPortrait(4,14,jogador->bag[jogador->currentEntity].specie.portrait,heartsCounter[0]);
    PrintHearts(heartsCounter[0], 4, 11);

    for(int i = 0; i < 2; i++){
        snprintf(logs[i], 60, "%s (%d)", logs[i], attempt[i]);
    }

    gotoxy(4,MAX_LINHA-3); printf(logs[0]);
    gotoxy(4,MAX_LINHA-2); printf(logs[1]);

    WriteBox("ATTACK", "SWITCH", "CATCH", "FLEE");

    gotoxy(2,2); printf("Turno %d", battleTurn);

    BattleWinLogic(*enemy, jogador, win);

    battleTurn++;
}

int main(){
    OpenConfig(filesName);
    LoadPointers(filesPtrs, filesName);
    LoadAbilities(allAbilities, filesPtrs[1]);
    LoadSpecies(allSpecies, filesPtrs[0]);
    getch();
    srand(time(NULL));
    
    struct player jogador = {{45,13}, {0}, 0, 1, 2, FALSE};
    struct entity enemies[MAXENEMIES] = {0};

    ResetEnemies(enemies, jogador);
    CopyEntity(&jogador.bag[0], enemies[0]);
    while (running) {
        Debug(jogador);
        switch(scene){
            case MENU:
                MENU_SCENE(&jogador);
                break;

            case WORLD_MAP:
                WORLD_SCENE(enemies, &jogador);
                break;

            case BATTLE:
                BATTLE_SCENE(&enemies[enemyIndex], &jogador);
                break;

            case GAME_END:
                END_SCENE(&jogador);
                break;
            }
    }
    textbackground(BLACK);
    textcolor(BLACK);

    clrscr();
    return 0;
}