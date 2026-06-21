#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "gconio.h"

#define MAX_COLUNA 90 /// X
#define MAX_LINHA 25  /// Y
#define MAXENEMIES 5

enum cena{MENU,WORLD_MAP,BATTLE,GAME_END};
enum element{fire, water, grass};
enum atr{hp, atk, spatk, def, spdef, spd};
enum type{physical, special};

struct color{enum COLORS idle; enum COLORS pursuit;};
struct damage{int value; enum element element; enum type type;};
struct nature{enum atr up; enum atr down;};
struct indentifier{char symbol; char name[20];};
struct pos{int x; int y;};
struct health{int current; int max;};
struct ability{struct indentifier indentifier; struct damage damage; int accuracy;};

struct specie{
    struct indentifier indentifier;
    int num;
    enum atr base[6];
    enum element element;
    struct color color[2];
};

struct entity{
    struct indentifier indentifier;
    struct pos pos;
    struct health health;
    int level;
    int velo;
    bool isShiny;
    enum COLORS currentColor;
    enum atr IV[6];
    enum atr atributes[6];
    struct specie specie;
    struct nature nature;
    struct ability abilities[4];
};

struct player{
    struct pos pos;
    struct entity bag[4];
    int currentEntity;
    int level;
    int velo;
};

float ElementEffectiviness(enum element attacker, enum element target){
    if((attacker+2)%4 == target){return 2.0f;}
    else if((attacker+1)%4 == target){return 1.0f;}
    else{return 0.5f;}
}

int doAttempt(struct entity target, struct entity attacker, int abilityIndex){
    int prob = rand()%100+1;
    if(prob>attacker.abilities[abilityIndex].accuracy)
        return 0;
    int dmg = (attacker.abilities[abilityIndex].damage.value/50)*attacker.atributes[atk+attacker.abilities[abilityIndex].damage.type] - target.atributes[def+attacker.abilities[abilityIndex].damage.type];
    target.health.current -= dmg;
    return dmg*ElementEffectiviness(attacker.abilities[abilityIndex].damage.element, target.specie.element);
}

int isCharInsideArray(char x, char array[]){
    for(int i = 0; i < strlen(array); i++){
        if(array[i] == x){return 1;}
    }
    return 0;
}

void PrintHearts(int hearts, int x, int y){
    for(int i = 0; i < hearts; i++){
        gotoxy(x+i,y);
        switch(i%4){
            case 0:
                printf("(");
                break;
            case 1:
                printf("\\");
                gotoxy(x+i,y+1);
                printf("\\");
                break;
            case 2:
                printf("/");
                gotoxy(x+i,y+1);
                printf("/");
                break;
            case 3:
                printf(")");
                break;
        }
    }

}

struct specie allSpecies[1] = {
    {{'P', "placeholder"}, 0, {0,0,0,0,0,0}, fire, {{BLUE, YELLOW}, {RED, GREEN}}}
};

// Dev Wise
bool debugMode = FALSE;
bool invencivel = FALSE;

// INIT
int scene = MENU;               /// Controle de cena
bool running = TRUE;                /// Controle de jogo
int screenClearType = 0;        /// Tipo de clear
int battleTurn = 0;             /// Contador de turno batalha
int worldTurn = 0;              /// Contador de turno worldMap
int backgroundType = BLACK;     /// Cor do background de algumas cenas
int enemyIndex = -1;
int enemyQnt = 2;
char input = ' ';

// Var
float behaviour = 0.0f;
int aggro_Enemy = 12;               /// Dist�ncia de aggro do inimigo

int attackRoll = 0;
char logPlayer[60] = "";
char logEnemy[60] = "";

// Battle Var
int heartsCounter[2] = {16,16};

// Scores
int walkedDistance = 0;
int enemiesKilled = 0;
int closestToDeath = 16;

void Debug(struct player jogador){
    if(debugMode){
            textbackground(WHITE); textcolor(MAGENTA);
            gotoxy(1, MAX_LINHA+2);
            printf(" DEBUG: ON ; INVINCIBLE: %d", invencivel);
            gotoxy(1, MAX_LINHA+3);
            printf(" JOGADOR{ POS: %d,%d ; LEVEL: %d ; VELO: %d", jogador.pos.x, jogador.pos.y, jogador.level, jogador.velo);
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
}

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

void ResetEnemies(struct entity enemies[MAXENEMIES], struct player jogador){
    for(int i = 0; i < enemyQnt; i++){
        enemies[i].specie = allSpecies[rand()%1];
        enemies[i].indentifier.symbol = enemies->specie.indentifier.symbol;
        strcpy(enemies[i].indentifier.name, enemies->specie.indentifier.name);
        enemies[i].pos.x = (rand()%(MAX_COLUNA-1))+1;
        enemies[i].pos.y = (rand()%(MAX_LINHA-1))+1;
        enemies[i].velo = 1;
        enemies[i].level = max(1,(rand()%3)-1+jogador.level);
        enemies[i].health.max = enemies[i].level * 5 + 10 + (rand()%11);
        enemies[i].health.current = enemies[i].health.max;
    }
}

void Init(){
    // Scores
    walkedDistance = 0;
    enemiesKilled = 0;
    closestToDeath = 16;
}

void END_SCENE(){
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
            running = FALSE;
            Sleep(3000);
            break;
    }
}

void WORLD_SCENE(struct entity enemies[], struct player jogador){
    textbackground(backgroundType);
    if(!worldTurn){
        battleTurn = 0;
        
        ResetEnemies(enemies, jogador);
    }
    else{
        // Logica player
        input = toupper(getch());

        switch(input){
            case 'W':
                jogador.pos.y -= jogador.velo;
                walkedDistance++;
                break;
            case 'S':
                jogador.pos.y += jogador.velo;
                walkedDistance++;
                break;
            case 'A':
                jogador.pos.x -= jogador.velo;
                walkedDistance++;
                break;
            case 'D':
                jogador.pos.x += jogador.velo;
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
        }

        if      (jogador.pos.x <= 0)           {jogador.pos.x = 0;}
        else if (jogador.pos.x >= MAX_COLUNA)  {jogador.pos.x = MAX_COLUNA;}
        if      (jogador.pos.y <= 0)           {jogador.pos.y = 0;}
        else if (jogador.pos.y >= MAX_LINHA-5)   {jogador.pos.y = MAX_LINHA-5;}

        // Logica enemy
        for(int i = 0; i < enemyQnt; i++){
            int xDist = jogador.pos.x - enemies[i].pos.x;
            int yDist = jogador.pos.y - enemies[i].pos.y;
            int dist = sqrt(xDist*xDist + yDist*yDist); /// Distancia entre o jogador e o inimigo

            if(dist > aggro_Enemy){             /// Fora da range
                enemies[i].pos.x += (rand() % 3 - 1) * enemies[i].velo;
                enemies[i].pos.y += (rand() % 3 - 1) * enemies[i].velo;
                enemies[i].currentColor = enemies[i].specie.color[enemies[i].isShiny].idle;
            }
            else if(dist > enemies[i].velo){         /// Dentro da range
                if(xDist != 0){
                    enemies[i].pos.x += (xDist / abs(xDist)) * enemies[i].velo;
                }
                if(yDist != 0){
                    enemies[i].pos.y += (yDist / abs(yDist)) * enemies[i].velo;
                }
                enemies[i].currentColor = enemies[i].specie.color[enemies[i].isShiny].pursuit;
            }
            else if(!invencivel){              /// Encostou no jogador
                enemyIndex = i;
                scene = BATTLE;
                Sleep(200);
            }
            if      (enemies[i].pos.x <= 0)           {enemies[i].pos.x = 0;}
            else if (enemies[i].pos.x >= MAX_COLUNA)  {enemies[i].pos.x = MAX_COLUNA;}
            if      (enemies[i].pos.y <= 0)           {enemies[i].pos.y = 0;}
            else if (enemies[i].pos.y >= MAX_LINHA-5)   {enemies[i].pos.y = MAX_LINHA-5;}
        }
    }

    worldTurn++;

    worldTurn = worldTurn%100;

    // RENDER
    textcolor(MAGENTA);
    Moldura();

        // RENDER Other
        gotoxy(MAX_COLUNA,MAX_LINHA); textcolor(jogador.level+2); printf("%d", jogador.level);
        // RENDER Entidades
        gotoxy(jogador.pos.x, jogador.pos.y); textcolor(YELLOW); printf("@");
        for(int i = 0; i < enemyQnt; i++){gotoxy(enemies[i].pos.x, enemies[i].pos.y); textcolor(enemies[i].currentColor); printf("%c", enemies[i].indentifier.symbol);}
        //
    //
    gotoxy(MAX_COLUNA, MAX_LINHA+1);
}

void BATTLE_SCENE(struct entity enemy, struct player jogador){
    int attempt[2] = {0};
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
        

        behaviour = (jogador.level - enemy.level)*4;
        strcpy(logPlayer,""); strcpy(logEnemy, "");
        jogador.currentEntity = 0;
        
        battleTurn++;
    }
    else{
        // In Turn
        do{input = getch();}while(isCharInsideArray(input, "12"));

        switch(input){
            case '1':
                do{input = getch();}while(isCharInsideArray(input, "1234"));
                attempt[0] = doAttempt(enemy, jogador.bag[jogador.currentEntity], (int)input - '1');
                if(attempt[0]){strcpy(logPlayer, "O jogador acertou o inimigo, causando %d de dano");}
                else{strcpy(logPlayer, "O jogador tentou acertar o inimigo mas errou");}
                break;
            case '2':
                if(!(rand()%4)){scene = WORLD_MAP; strcpy(logPlayer, "O jogador fugiu da batalha");}
                else{strcpy(logPlayer, "O jogador tentou fugir da batalha mas nao conseguiu");} 
                break;
        } 

        if(rand()%11 >= behaviour){
            attempt[1] = doAttempt(jogador.bag[jogador.currentEntity], enemy, rand()%4);
            if(attempt[1]){strcpy(logEnemy, "O inimigo acertou o jogador, causando %d de dano");}
            else{strcpy(logEnemy, "O inimigo tentou acertar o jogador mas errou");}
        }
        else{
            if(!(rand()%10)){
                scene = WORLD_MAP;
                enemy.pos.x = rand()%MAX_COLUNA;
                enemy.pos.y = rand()%MAX_LINHA;
                battleTurn = 0;
                strcpy(logEnemy, "O inimigo fugiu da batalha");
            }
            else{strcpy(logEnemy, "O inimigo tentou fugir da batalha mas nao conseguiu");}
        }

        // Post Turn
        
        if(enemy.health.current <= 0){
            scene = WORLD_MAP;
            jogador.level++;
            enemiesKilled++;
            closestToDeath = min(closestToDeath,heartsCounter[0]);
            worldTurn = 0; battleTurn = 0;
        }
        else if(jogador.bag[jogador.currentEntity].health.current <= 0){scene = GAME_END;}
        
        heartsCounter[1] = (enemy.health.current*1.0f/enemy.health.max)*16;
        heartsCounter[0] = (jogador.bag[jogador.currentEntity].health.current*1.0f/jogador.bag[jogador.currentEntity].health.max)*16;

        behaviour = min((jogador.level-enemy.level) * 3 + (16.0f/heartsCounter[0]) * 2, 9);
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
    PrintHearts(heartsCounter[1], 72, 9);
    PrintHearts(heartsCounter[0], 4, 11);

    gotoxy(4,MAX_LINHA-2); printf(logEnemy, attempt[1]);
    gotoxy(4,MAX_LINHA-3); printf(logPlayer, attempt[0]);
    gotoxy(65,MAX_LINHA-3); printf("1. ATACAR");
    gotoxy(65,MAX_LINHA-2); printf("2. FUGIR");

    gotoxy(2,2); printf("Turno %d", battleTurn);
    battleTurn++;
}

int main(){
    srand(time(NULL));
    struct player jogador = {0};
    jogador.pos.x = 45;
    jogador.pos.y = 13;
    jogador.level = 1;
    jogador.velo = 2;
    struct entity enemies[MAXENEMIES] = {0};
    while (running) {
        Debug(jogador);
        switch(scene){
            case MENU:
                MENU_SCENE();
                break;
            case WORLD_MAP:
                WORLD_SCENE(enemies, jogador);
            break;

            case BATTLE:
                BATTLE_SCENE(enemies[enemyIndex],jogador);
                break;

            case GAME_END:
                END_SCENE();
                break;
            }

    }
    textbackground(BLACK);
    textcolor(BLACK);

    clrscr();
    return 0;
}

