#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "gconio.h"

#define MAX_COLUNA 90 /// X
#define MAX_LINHA 25  /// Y
#define MAXENEMIES 5
#define ATR_QNT 10

enum cena{MENU,WORLD_MAP,BATTLE,GAME_END};
enum entity{X, Y, VELO, LEVEL, HMAX, HCUR, ATK, DEF, DMG, COLOR};

// Dev Wise
int debugMode = 0;
int invencivel = 0;

// INIT

int scene = MENU;               /// Controle de cena
int running = 1;                /// Controle de jogo
int screenClearType = 0;        /// Tipo de clear
int battleTurn = 0;             /// Contador de turno batalha
int worldTurn = 0;              /// Contador de turno worldMap
int backgroundType = BLACK;     /// Cor do background de algumas cenas
int enemyIndex = -1;
int enemyQnt = 2;
char input = ' ';

// JOGADOR
int jogador[ATR_QNT] = {45, 13, 2, 1, 20, 20, 0, 0, 0, RED};
//

// Var
float behaviour = 0.0f;
int aggro_Enemy = 12;               /// Dist�ncia de aggro do inimigo
int yDist = 0;
int xDist = 0;
int dist = 0;
int attackRoll = 0;
char logPlayer[60] = "";
char logEnemy[60] = "";

// Battle Var
int heartsCounter[2] = {16,16};

// Scores
int walkedDistance = 0;
int enemiesKilled = 0;
int closestToDeath = 16;

void MENU_SCENE();
void BATTLE_SCENE(int enemyIndex, int enemies[ATR_QNT][MAXENEMIES]);
void ResetEnemies(int enemies[ATR_QNT][MAXENEMIES]);
void Init();

void Moldura(){
    for(int i = 0; i <= MAX_LINHA; i++){
        gotoxy(0,i);
        if(i == 0 || i == MAX_LINHA || i == 20){
            for(int k = 0; k <= MAX_COLUNA; k++){printf("=");}
        }
        else{
            for(int k = 0; k <= MAX_COLUNA; k++){
                if(k == 0 || k == MAX_COLUNA || (i > 20 && k == 60)){printf("|");}
                else{printf(" ");}
            }
        }
    }
    gotoxy(0,0); printf("+");
    gotoxy(0,20); printf("+");
    gotoxy(0,MAX_LINHA); printf("+");
    gotoxy(MAX_COLUNA,0); printf("+");
    gotoxy(MAX_COLUNA,20); printf("+");
    gotoxy(MAX_COLUNA,MAX_LINHA); printf("+");
    gotoxy(60,20); printf("+");
    gotoxy(60,MAX_LINHA); printf("+");
}
void CaveiraAnim(){
        for(int i = 0; i < MAX_LINHA+2; i++){
        if(i > 0 ){gotoxy(41,i-1); printf("         ");}
        if(i < 26){gotoxy(41,i+0); printf("  _____  ");}
        if(i < 25){gotoxy(41,i+1); printf(" /     \\ ");}
        if(i < 24){gotoxy(41,i+2); printf("| () () |");}
        if(i < 23){gotoxy(41,i+3); printf(" \\  ^  / ");}
        if(i < 22){gotoxy(41,i+4); printf("  |||||  ");}
        if(i < 21){gotoxy(41,i+5); printf("  |||||  ");}
        Sleep(150);
    }
}

int main(){
    srand(time(NULL));
    int enemies[ATR_QNT][MAXENEMIES] = {0};
    while (running) {

        // Debug
        if(debugMode){
            textbackground(WHITE);
            gotoxy(1, MAX_LINHA+2); textcolor(MAGENTA);
            printf(" DEBUG: ON ; INVINCIBLE: %d", invencivel);


            gotoxy(1, MAX_LINHA+3); textcolor(RED);
            printf(" PLAYER-> pos:[%d,%d] ; jogador[HCUR]: %d/%d ; jogador[VELO]: %d ; input: %c ; dist: [%d, %d]-> %d ; jogador[ATK]: %d ;  jogador[DEF]: %d ; jogador[DMG]: %d ; level: %d "
                , jogador[X], jogador[Y], jogador[HCUR], jogador[HMAX],jogador[VELO],input, xDist, yDist, dist, jogador[ATK], jogador[DEF], jogador[DMG], jogador[LEVEL]);
        }
        else{
            textbackground(BLACK);
            for(int i = MAX_LINHA+1; i <= MAX_LINHA+4; i++){
                for(int k = 0; k <= MAX_COLUNA; k++){
                    gotoxy(k, i);
                    printf(" ");
                }
            }
        }
        //

        switch(scene){
            case MENU:
                MENU_SCENE();
                break;
            case WORLD_MAP:
                textbackground(backgroundType);

                if(!worldTurn){
                    battleTurn = 0;
                    
                    ResetEnemies(enemies);

                    jogador[HMAX] = jogador[LEVEL] * 5 + 15;
                    jogador[HCUR] = jogador[HMAX];
                    jogador[ATK] = jogador[LEVEL] * 2 - 2;
                    jogador[DEF] = jogador[LEVEL] * 2 + 8;
                    jogador[DMG] = jogador[LEVEL] * 2 + 2;
                }
                else{
                    // Logica player
                    input = getch();
    
                    switch(input){
                        case 'w':
                        case 'W':
                            jogador[Y] -= jogador[VELO];
                            walkedDistance++;
                            break;
                        case 's':
                        case 'S':
                            jogador[Y] += jogador[VELO];
                            walkedDistance++;
                            break;
                        case 'a':
                        case 'A':
                            jogador[X] -= jogador[VELO];
                            walkedDistance++;
                            break;
                        case 'd':
                        case 'D':
                            jogador[X] += jogador[VELO];
                            walkedDistance++;
                            break;
                    }
                        
                    switch(input){
                    case 'o':
                        if(invencivel){invencivel = 0;}
                        else{invencivel = 1;}
                        break;
                    case 'p':
                        if(debugMode){debugMode = 0;}
                        else{debugMode = 1;}
                        break;
                    }
    
                    if      (jogador[X] <= 0)           {jogador[X] = 0;}
                    else if (jogador[X] >= MAX_COLUNA)  {jogador[X] = MAX_COLUNA;}
                    if      (jogador[Y] <= 0)           {jogador[Y] = 0;}
                    else if (jogador[Y] >= MAX_LINHA-5)   {jogador[Y] = MAX_LINHA-5;}
    
                    // Logica enemy
                    for(int i = 0; i < enemyQnt; i++){
                        xDist = jogador[X] - enemies[X][i];
                        yDist = jogador[Y] - enemies[Y][i];
                        dist = sqrt(xDist*xDist + yDist*yDist); /// Distancia entre o jogador e o inimigo
        
                        if(dist > aggro_Enemy){             /// Fora da range
                            enemies[X][i] += (rand() % 3 - 1) * enemies[VELO][i];
                            enemies[Y][i] += (rand() % 3 - 1) * enemies[VELO][i];
                            enemies[COLOR][i] = YELLOW;
                        }
                        else if(dist > enemies[VELO][i]){         /// Dentro da range
                            if(xDist != 0){
                                enemies[X][i] += (xDist / abs(xDist)) * enemies[VELO][i];
                            }
                            if(yDist != 0){
                                enemies[Y][i] += (yDist / abs(yDist)) * enemies[VELO][i];
                            }
                            enemies[COLOR][i] = CYAN;
                        }
                        else if(!invencivel){              /// Encostou no jogador
                            enemies[COLOR][i] = WHITE;
                            enemyIndex = i;
                            scene = BATTLE;
                            Sleep(200);
                        }
                        if      (enemies[X][i] <= 0)           {enemies[X][i] = 0;}
                        else if (enemies[X][i] >= MAX_COLUNA)  {enemies[X][i] = MAX_COLUNA;}
                        if      (enemies[Y][i] <= 0)           {enemies[Y][i] = 0;}
                        else if (enemies[Y][i] >= MAX_LINHA-5)   {enemies[Y][i] = MAX_LINHA-5;}
                    }
                }

                worldTurn++;

                worldTurn = worldTurn%100;

                // RENDER
                textcolor(MAGENTA);
                Moldura();

                    // RENDER Other
                    gotoxy(MAX_COLUNA,MAX_LINHA); textcolor(jogador[LEVEL]+2); printf("%d", jogador[LEVEL]);
                    // RENDER Entidades
                    gotoxy(jogador[X], jogador[Y]); textcolor(YELLOW); printf("@");
                    for(int i = 0; i < enemyQnt; i++){gotoxy(enemies[X][i], enemies[Y][i]); textcolor(enemies[COLOR][i]); printf("%d", enemies[LEVEL][i]);}
                    //
                //


                gotoxy(MAX_COLUNA, MAX_LINHA+1);
                break;

            case BATTLE:
                BATTLE_SCENE(enemyIndex, enemies);
                break;

            case GAME_END:
                textbackground(backgroundType);
                if(!screenClearType){
                    for(int i = 0; i <= MAX_LINHA; i++){
                        gotoxy(0, i);
                        for(int k = 0; k <= MAX_COLUNA; k++){
                            printf(" ");
                        }
                    }
                }
                else{
                    clrscr();
                }
                textcolor(CYAN);
                gotoxy(MAX_COLUNA/2-10, MAX_LINHA/2); printf("PERDEU MANE");
                gotoxy(MAX_COLUNA/2-30, MAX_LINHA/2+2); printf("Voce andou %d caracteres de distancia", walkedDistance);
                gotoxy(MAX_COLUNA/2-30, MAX_LINHA/2+4); printf("Voce matou %d inimigo(s)", enemiesKilled);
                gotoxy(MAX_COLUNA/2-30, MAX_LINHA/2+6); printf("Em sua batalha mais dificil voce ficou com %d coracoes de vida", closestToDeath);
                getch();
                scene = MENU;
                Sleep(3000);
                break;
            }

    }
    textbackground(BLACK);
    textcolor(BLACK);

    clrscr();
    return 0;
}

void MENU_SCENE(){
    clrscr();
    textbackground(BLACK); textcolor(MAGENTA);
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
            Init();
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
            printf("Quantos inimigos? [Default: 2] (1-5): ");
            scanf("%d", &enemyQnt);
            enemyQnt = min(max(enemyQnt,1),5);
            break;
            
        case '3':
            clrscr();
            gotoxy(MAX_COLUNA/2-10, MAX_LINHA/2);
            printf(";(");
            running = 0;
            Sleep(3000);
            break;
    }
}

void BATTLE_SCENE(int enemyIndex, int enemies[ATR_QNT][MAXENEMIES]){
    // Pre Turn
    if(!battleTurn){
        textbackground(backgroundType); textcolor(CYAN);
        if(!screenClearType){
            for(int i = 0; i <= MAX_LINHA; i++){
                gotoxy(0, i);
                for(int k = 0; k <= MAX_COLUNA; k++){
                    printf(" ");
                }
            }
        }
        else{
            CaveiraAnim();
            clrscr();
        }
        

        behaviour = (jogador[LEVEL] - enemies[LEVEL][enemyIndex])*4;
        strcpy(logPlayer,""); strcpy(logEnemy, "");
        
        battleTurn++;
    }
    else{
        // In Turn
        do{input = getch();}while(input != '1' && input != '2');

        switch(input){
            case '1':
                attackRoll = (rand()%19+1) + jogador[ATK];
                if(attackRoll > enemies[DEF][enemyIndex]){
                    enemies[HCUR][enemyIndex] -= jogador[DMG];
                    strcpy(logPlayer, "O jogador acertou o inimigo, causando %d de dano");
                }
                else{strcpy(logPlayer, "O jogador tentou acertar o inimigo mas errou");}
                break;
            case '2':
                if(!(rand()%4)){
                    scene = WORLD_MAP;
                    strcpy(logPlayer, "O jogador fugiu da batalha");
                }
                else{strcpy(logPlayer, "O jogador tentou fugir da batalha mas nao conseguiu");} 
                break;
        } 

        if(rand()%11 >= behaviour){
            attackRoll = (rand()%19+1) + enemies[ATK][enemyIndex];
            if(attackRoll > jogador[DEF]){
                jogador[HCUR] -= enemies[DMG][enemyIndex];
                strcpy(logEnemy, "O inimigo acertou o jogador, causando %d de dano");
            }
            else{strcpy(logEnemy, "O inimigo tentou acertar o jogador mas errou");}
        }
        else{
            if(!(rand()%10)){
                scene = WORLD_MAP;
                enemies[X][enemyIndex] = rand()%MAX_COLUNA;
                enemies[Y][enemyIndex] = rand()%MAX_LINHA;
                worldTurn = 0; battleTurn = 0;
                strcpy(logEnemy, "O inimigo fugiu da batalha");
            }
            else{strcpy(logEnemy, "O inimigo tentou fugir da batalha mas nao conseguiu");}
        }

        // Post Turn
        
        if(enemies[HCUR][enemyIndex] <= 0){
            scene = WORLD_MAP;
            jogador[LEVEL]++;
            enemiesKilled++;
            closestToDeath = min(closestToDeath,heartsCounter[0]);
            worldTurn = 0; battleTurn = 0;
        }
        else if(jogador[HCUR] <= 0){scene = GAME_END;}
        
        heartsCounter[1] = (enemies[HCUR][enemyIndex]*1.0f/enemies[HMAX][enemyIndex])*16;
        heartsCounter[0] = (jogador[HCUR]*1.0f/jogador[HMAX])*16;

        behaviour = min((jogador[LEVEL]-enemies[LEVEL][enemyIndex]) * 3 + (16.0f/heartsCounter[0]) * 2, 9);
    }

    // RENDER
    textbackground(BLACK); textcolor(MAGENTA);
    Moldura();             



    // RENDER CHARS
    gotoxy(78,2); printf("   ,-.   ");
    gotoxy(78,3);
    if(heartsCounter[1]>12){
        printf(" _(-_-)_ ");
    }
    else if(heartsCounter[1]>8){
        printf(" _(O_O)_ ");
    }
    else{
        printf(" _(*_*)_ ");
    }
    gotoxy(78,4); printf("(_  o  _)");
    gotoxy(78,5); printf("  / o \\  ");
    gotoxy(78,6); printf(" (_/ \\_) ");

    gotoxy(4,14); printf("  .-'''-.  ");
    gotoxy(4,15);
    if(heartsCounter[0]>12){
        printf(" /(.) (.)\\ ");
    }
    else if(heartsCounter[0]>8){
        printf(" /(O) (O)\\");
    }
    else{
        printf(" /(X) (X)\\");
    }
    gotoxy(4,16); printf(";    O    ;");
    gotoxy(4,17); printf(" \\ }---{ / ");
    gotoxy(4,18); printf("  '-...-'  ");
    //
    for(int i = 0; i < heartsCounter[1]; i++){
        gotoxy(72+i,9);
        switch(i%4){
            case 0:
                printf("(");
                break;
            case 1:
                printf("\\");
                gotoxy(72+i,10);
                printf("\\");
                break;
            case 2:
                printf("/");
                gotoxy(72+i,10);
                printf("/");
                break;
            case 3:
                printf(")");
                break;
        }
    }

    for(int i = 0; i < heartsCounter[0]; i++){
        gotoxy(4+i,11);
        switch(i%4){
            case 0:
                printf("(");
                break;
            case 1:
                printf("\\");
                gotoxy(4+i,12);
                printf("\\");
                break;
            case 2:
                printf("/");
                gotoxy(4+i,12);
                printf("/");
                break;
            case 3:
                printf(")");
                break;
        }
    }

    gotoxy(4,MAX_LINHA-2); printf(logEnemy, enemies[DMG][enemyIndex]);
    gotoxy(4,MAX_LINHA-3); printf(logPlayer, jogador[DMG]);
    gotoxy(65,MAX_LINHA-3); printf("1. ATACAR");
    gotoxy(65,MAX_LINHA-2); printf("2. FUGIR");

    gotoxy(2,2); printf("Turno %d", battleTurn);
    battleTurn++;
}

void ResetEnemies(int enemies[ATR_QNT][MAXENEMIES]){
    for(int i = 0; i < enemyQnt; i++){
        enemies[X][i] = (rand()%(MAX_COLUNA-1))+1;
        enemies[Y][i] = (rand()%(MAX_LINHA-1))+1;
        enemies[VELO][i] = 1;//(rand()%2)+1;
        enemies[LEVEL][i] = max(1,(rand()%3)-1+jogador[LEVEL]);
        enemies[HMAX][i] = enemies[LEVEL][i] * 5 + 10 + (rand()%11);
        enemies[HCUR][i] = enemies[HMAX][i];
        enemies[ATK][i] = enemies[LEVEL][i] * 2 - 4 + (rand()%5);
        enemies[DEF][i] = enemies[LEVEL][i] * 2 + 6 + (rand()%5); 
        enemies[DMG][i] = enemies[LEVEL][i] * 2 + (rand()%5);
        enemies[COLOR][i] = CYAN;
    }
}

void Init(){
    // Scores
    walkedDistance = 0;
    enemiesKilled = 0;
    closestToDeath = 16;
}
