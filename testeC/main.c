#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include "gconio.h"

#define MAX_COLUNA 90 /// X
#define MAX_LINHA 25  /// Y
#define MAXENEMIES 5

#define SUPEREF 2.0f
#define EFFECTI 1.0f
#define NOTEFFE 0.5f
#define IMUNITY 0.0f

#define euler 2.8f

#define catchEq(x,lDiff,r, Diff) (24-x)/24 * pow(euler, lDiff/3) * (1/r) * 100 * (3/Diff)

enum cena{MENU,WORLD_MAP,BATTLE,GAME_END};
enum element{nullElement,normal,fire,water,eletric,grass,ice,fightining,poison,ground,flying,psychic,bug,rock,ghost,dragon,dark,steel,fairy};
enum atr{hp, atk, spatk, def, spdef, spd};
enum type{physical, special};

const float elementMatrix[18][19] = {
//            NUL      NOR      FIR      WAT      ELE      GRA      ICE      FIG      POI      GRD      FLY      PSY      BUG      ROC      GHO      DRA      DAR      STE      FAI
/* NOR */ { EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, NOTEFFE, IMUNITY, EFFECTI, EFFECTI, NOTEFFE, EFFECTI },
/* FIR */ { EFFECTI, EFFECTI, NOTEFFE, NOTEFFE, EFFECTI, SUPEREF, SUPEREF, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, SUPEREF, NOTEFFE, EFFECTI, NOTEFFE, EFFECTI, SUPEREF, EFFECTI },
/* WAT */ { EFFECTI, EFFECTI, SUPEREF, NOTEFFE, EFFECTI, NOTEFFE, EFFECTI, EFFECTI, EFFECTI, SUPEREF, EFFECTI, EFFECTI, EFFECTI, SUPEREF, EFFECTI, NOTEFFE, EFFECTI, EFFECTI, EFFECTI },
/* ELE */ { EFFECTI, EFFECTI, EFFECTI, SUPEREF, NOTEFFE, NOTEFFE, EFFECTI, EFFECTI, EFFECTI, IMUNITY, SUPEREF, EFFECTI, EFFECTI, EFFECTI, EFFECTI, NOTEFFE, EFFECTI, EFFECTI, EFFECTI },
/* GRA */ { EFFECTI, EFFECTI, NOTEFFE, SUPEREF, EFFECTI, NOTEFFE, EFFECTI, EFFECTI, NOTEFFE, SUPEREF, NOTEFFE, EFFECTI, NOTEFFE, SUPEREF, EFFECTI, NOTEFFE, EFFECTI, NOTEFFE, EFFECTI },
/* ICE */ { EFFECTI, EFFECTI, NOTEFFE, NOTEFFE, EFFECTI, SUPEREF, NOTEFFE, EFFECTI, EFFECTI, SUPEREF, SUPEREF, EFFECTI, EFFECTI, EFFECTI, EFFECTI, SUPEREF, EFFECTI, NOTEFFE, EFFECTI },
/* FIG */ { EFFECTI, SUPEREF, EFFECTI, EFFECTI, EFFECTI, EFFECTI, SUPEREF, EFFECTI, NOTEFFE, EFFECTI, NOTEFFE, NOTEFFE, NOTEFFE, SUPEREF, IMUNITY, EFFECTI, SUPEREF, SUPEREF, NOTEFFE },
/* POI */ { EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, SUPEREF, EFFECTI, EFFECTI, NOTEFFE, NOTEFFE, EFFECTI, EFFECTI, EFFECTI, NOTEFFE, NOTEFFE, EFFECTI, EFFECTI, IMUNITY, SUPEREF },
/* GRD */ { EFFECTI, EFFECTI, SUPEREF, EFFECTI, SUPEREF, NOTEFFE, EFFECTI, EFFECTI, SUPEREF, EFFECTI, IMUNITY, EFFECTI, NOTEFFE, SUPEREF, EFFECTI, EFFECTI, EFFECTI, SUPEREF, EFFECTI },
/* FLY */ { EFFECTI, EFFECTI, EFFECTI, EFFECTI, NOTEFFE, SUPEREF, EFFECTI, SUPEREF, EFFECTI, EFFECTI, EFFECTI, EFFECTI, SUPEREF, NOTEFFE, EFFECTI, EFFECTI, EFFECTI, NOTEFFE, EFFECTI },
/* PSY */ { EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, SUPEREF, SUPEREF, EFFECTI, EFFECTI, NOTEFFE, EFFECTI, EFFECTI, EFFECTI, EFFECTI, IMUNITY, NOTEFFE, EFFECTI },
/* BUG */ { EFFECTI, EFFECTI, NOTEFFE, EFFECTI, EFFECTI, SUPEREF, EFFECTI, NOTEFFE, NOTEFFE, EFFECTI, NOTEFFE, SUPEREF, EFFECTI, EFFECTI, NOTEFFE, EFFECTI, SUPEREF, NOTEFFE, NOTEFFE },
/* ROC */ { EFFECTI, EFFECTI, SUPEREF, EFFECTI, EFFECTI, EFFECTI, SUPEREF, NOTEFFE, EFFECTI, NOTEFFE, SUPEREF, EFFECTI, SUPEREF, EFFECTI, EFFECTI, EFFECTI, EFFECTI, NOTEFFE, EFFECTI },
/* GHO */ { EFFECTI, IMUNITY, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, SUPEREF, EFFECTI, EFFECTI, SUPEREF, EFFECTI, NOTEFFE, EFFECTI, EFFECTI },
/* DRA */ { EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, SUPEREF, EFFECTI, NOTEFFE, IMUNITY },
/* DAR */ { EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, NOTEFFE, EFFECTI, EFFECTI, EFFECTI, SUPEREF, EFFECTI, EFFECTI, SUPEREF, EFFECTI, NOTEFFE, EFFECTI, NOTEFFE },
/* STE */ { EFFECTI, EFFECTI, NOTEFFE, NOTEFFE, NOTEFFE, SUPEREF, SUPEREF, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, SUPEREF, EFFECTI, EFFECTI, EFFECTI, NOTEFFE, SUPEREF },
/* FAI */ { EFFECTI, EFFECTI, NOTEFFE, EFFECTI, EFFECTI, EFFECTI, EFFECTI, SUPEREF, NOTEFFE, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, EFFECTI, SUPEREF, SUPEREF, NOTEFFE, EFFECTI }
};

struct color{enum COLORS idle; enum COLORS pursuit;};
struct damage{int value; enum element element; enum type type;};
struct nature{enum atr up; enum atr down;};
struct portrait{char battle[5][5][12];};
struct indentifier{char symbol; char name[14];};
struct pos{int x; int y;};
struct health{int current; int max;};
struct ability{struct indentifier indentifier; struct damage damage; int accuracy; char logMessage[2][60];};

struct specie{
    struct indentifier indentifier;
    struct portrait portrait;
    int num;
    int rarity;
    enum atr base[6];
    enum element element[2];
    struct color color[2];
};

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

struct player{
    struct pos pos;
    struct entity bag[4];
    int currentEntity;
    int level;
    int velo;
};

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
    for(int i = 0; i < 4; i++){dest->abilities[i] = from.abilities[i];}
    return TRUE;
}

bool CatchEntity(struct player *catcher, struct entity catch, int currentHeart, int slot, int diff){
    if(rand()%100 > catchEq(currentHeart, catch.level-catcher->level, catch.specie.rarity, diff))
        return FALSE;
    CopyEntity(&catcher->bag[slot], catch);
    return TRUE;
}

float ElementEffectiviness(enum element attacker, enum element targetFirst, enum element targetSecond){
    return elementMatrix[attacker][targetFirst] * elementMatrix[attacker][targetSecond];
}

int doAttempt(struct entity *target, struct entity attacker, int abilityIndex, char log[]){
    int prob = rand()%100+1;
    if(prob>attacker.abilities[abilityIndex].accuracy){
        if(attacker.abilities[abilityIndex].logMessage != NULL){strcpy(log, attacker.abilities[abilityIndex].logMessage[1]);}
        return 0;
    }
    if(attacker.abilities[abilityIndex].logMessage != NULL){strcpy(log, attacker.abilities[abilityIndex].logMessage[0]);}
    int dmg = (attacker.abilities[abilityIndex].damage.value/50)*attacker.atributes[atk+attacker.abilities[abilityIndex].damage.type] - target->atributes[def+attacker.abilities[abilityIndex].damage.type];
    dmg *= ElementEffectiviness(attacker.abilities[abilityIndex].damage.element, target->specie.element[0], target->specie.element[1]);
    target->health.current -= dmg;
    return dmg;
}

bool isCharInsideArray(char x, char array[]){
    int len = strlen(array);
    for(int i = 0; i < len; i++){
        if(array[i] == x){return TRUE;}
    }
    return FALSE;
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

void PrintPortrait(int x, int y, struct portrait portrait, int currentHeart){
    for(int i = 0; i < 5; i++){gotoxy(x,y+i); printf("%s", portrait.battle[(int)(currentHeart/4)][i]);}
}

void ActionBox(int middle, int height, char corner, char vertical, char horizontal){
    gotoxy(0,0);
    for(int i = height; i <= MAX_LINHA; i++){
        if(i == height || i == MAX_LINHA){
            for(int k = middle; k <= MAX_COLUNA; k++){gotoxy(k,i); printf("%c",horizontal);}
        }
        else{
            for(int k = middle; k <= MAX_COLUNA; k++){
                if(k == middle || k == MAX_COLUNA){gotoxy(k,i); printf("%c",vertical);}
                else{printf(" ");}
            }
        }
    }
    gotoxy(middle,height); printf("%c",corner);
    gotoxy(MAX_COLUNA,height); printf("%c",corner);
    gotoxy(middle,MAX_LINHA); printf("%c",corner);
    gotoxy(MAX_COLUNA,MAX_LINHA); printf("%c",corner);
}

void ActionBar(int middle, int height, char corner, char vertical, char horizontal){
    gotoxy(0,0);
    for(int i = height; i <= MAX_LINHA; i++){
        gotoxy(0,i);
        if(i == height || i == MAX_LINHA){
            for(int k = 0; k <= MAX_COLUNA; k++){printf("%c",horizontal);}
        }
        else{
            for(int k = 0; k <= MAX_COLUNA; k++){
                if(k == 0 || k == MAX_COLUNA){printf("%c",vertical);}
                else{printf(" ");}
            }
        }
    }
    ActionBox(middle, height, corner, vertical, horizontal);
    gotoxy(0,height); printf("%c",corner);
    gotoxy(middle,height); printf("%c",corner);
    gotoxy(MAX_COLUNA,height); printf("%c",corner);
    gotoxy(0,MAX_LINHA); printf("%c",corner);
    gotoxy(middle,MAX_LINHA); printf("%c",corner);
    gotoxy(MAX_COLUNA,MAX_LINHA); printf("%c",corner);
}

void Moldura(bool isActionBarActive, char corner, char vertical, char horizontal){
    for(int i = 0; i <= MAX_LINHA; i++){
        gotoxy(0,i);
        if(i == 0 || i == MAX_LINHA){
            for(int k = 0; k <= MAX_COLUNA; k++){printf("%c",horizontal);}
        }
        else{
            for(int k = 0; k <= MAX_COLUNA; k++){
                if(k == 0 || k == MAX_COLUNA){printf("%c",vertical);}
                else{printf(" ");}
            }
        }
    }
    if(isActionBarActive){ActionBar(60,20, corner, vertical, horizontal);}
    gotoxy(0,0); printf("%c",corner);
    gotoxy(0,MAX_LINHA); printf("%c",corner);
    gotoxy(MAX_COLUNA,0); printf("%c",corner);
    gotoxy(MAX_COLUNA,MAX_LINHA); printf("%c",corner);

}

void WriteBox(char first[14], char second[14], char third[14], char fourth[14]){
    gotoxy(65,MAX_LINHA-3); printf("1. %s", first);
    gotoxy(75,MAX_LINHA-3); printf("2. %s", second);
    gotoxy(65,MAX_LINHA-2); printf("3. %s", third);
    gotoxy(75,MAX_LINHA-2); printf("4. %s", fourth);
}

void ClearLog(int logNum){
    gotoxy(2,MAX_LINHA-3-logNum); printf("                                                            ");
}

void CaveiraAnim(enum COLORS background, enum COLORS text){
    clrscr();
    textbackground(background); textcolor(text);
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

struct specie allSpecies[1] = {
    {{'P', "placeholder"}, 
    {{
        {"", "", "", "", ""},
        {"", "", "", "", ""},
        {"", "", "", "", ""},
        {"", "", "", "", ""},
        {"", "", "", "", ""}}},
    0, 1, {0,0,0,0,0,0}, {fire, nullElement}, {{BLUE, YELLOW}, {RED, GREEN}}}
};

// Dev Wise
bool debugMode = FALSE;
bool invencivel = FALSE;

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

// Scores
int walkedDistance = 0;
int enemiesKilled = 0;
int closestToDeath = 16;

void Debug(struct player jogador){
    if(debugMode){
        textbackground(WHITE); textcolor(MAGENTA);
        gotoxy(2, MAX_LINHA+2);
        printf("DEBUG: ON ; INVINCIBLE: %d", invencivel);
        gotoxy(2, MAX_LINHA+3);
        printf("JOGADOR{ POS: %d,%d ; LEVEL: %d ; VELO: %d }", jogador.pos.x, jogador.pos.y, jogador.level, jogador.velo);
        gotoxy(2, MAX_LINHA+4);
        printf("CurrentEnt{ Name: (%c) %s ; Specie: (%c) %s ; Health: %d/%d ; Level: %d ;", jogador.bag[jogador.currentEntity].indentifier.symbol, jogador.bag[jogador.currentEntity].indentifier.name, jogador.bag[jogador.currentEntity].specie.indentifier.symbol, jogador.bag[jogador.currentEntity].specie.indentifier.name, jogador.bag[jogador.currentEntity].health.current, jogador.bag[jogador.currentEntity].health.max, jogador.bag[jogador.currentEntity].level);
        gotoxy(2, MAX_LINHA+5);
        printf("IV { HP: %d ; ATK: %d ; SPATK: %d ; DEF: %d ; SPDEF: %d ; SPD: %d } ;", jogador.bag[jogador.currentEntity].IV[hp], jogador.bag[jogador.currentEntity].IV[atk], jogador.bag[jogador.currentEntity].IV[spatk], jogador.bag[jogador.currentEntity].IV[def], jogador.bag[jogador.currentEntity].IV[spdef], jogador.bag[jogador.currentEntity].IV[spd]);
        gotoxy(2, MAX_LINHA+6);
        printf("ATR{ HP: %d ; ATK: %d ; SPATK: %d ; DEF: %d ; SPDEF: %d ; SPD: %d } }", jogador.bag[jogador.currentEntity].atributes[hp], jogador.bag[jogador.currentEntity].atributes[atk], jogador.bag[jogador.currentEntity].atributes[spatk], jogador.bag[jogador.currentEntity].atributes[def], jogador.bag[jogador.currentEntity].atributes[spdef], jogador.bag[jogador.currentEntity].atributes[spd]);
        
        textbackground(backgroundType);
    }
    else{
        textbackground(BLACK);
        for(int i = MAX_LINHA+1; i <= MAX_LINHA+6; i++){
            for(int k = 0; k <= MAX_COLUNA; k++){
                gotoxy(k, i);
                printf(" ");
            }
        }
    }
}

void EntityView(struct entity entities[4], int x[2], int y[2]){
    gotoxy(x[0], y[0]);
    printf("1. %s : %d/%d", entities[0].indentifier.name, entities[0].health.current, entities[0].health.max);
    gotoxy(x[1], y[0]);
    printf("2. %s : %d/%d", entities[1].indentifier.name, entities[1].health.current, entities[1].health.max);
    gotoxy(x[0], y[1]);
    printf("3. %s : %d/%d", entities[2].indentifier.name, entities[2].health.current, entities[2].health.max);
    gotoxy(x[1], y[1]);
    printf("4. %s : %d/%d", entities[3].indentifier.name, entities[3].health.current, entities[3].health.max);

}

bool ResetEnemies(struct entity enemies[MAXENEMIES], struct player jogador){
    for(int i = 0; i < enemyQnt; i++){
        enemies[i].specie = allSpecies[rand()%1];
        enemies[i].isShiny = !(rand()%15);
        enemies[i].indentifier.symbol = enemies->specie.indentifier.symbol;
        strcpy(enemies[i].indentifier.name, enemies->specie.indentifier.name);
        enemies[i].pos.x = (rand()%(MAX_COLUNA-1))+1;
        enemies[i].pos.y = (rand()%(MAX_LINHA-1))+1;
        enemies[i].velo = 1;
        enemies[i].level = max(1,(rand()%3)-1+jogador.level);
        enemies[i].health.max = enemies[i].level * 5 + 10 + (rand()%11);
        enemies[i].health.current = enemies[i].health.max;
    }
    return TRUE;
}

void Init(){
    // Scores
    walkedDistance = 0;
    enemiesKilled = 0;
    closestToDeath = 16;
}

void END_SCENE(){
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
            printf("Quantos inimigos - [Default: 2] (1-5): ");
            gotoxy(0,1);
            printf("Dificuldade - [Default: 3] (1-5): ");
            gotoxy(40,0); scanf("%d", &enemyQnt);
            gotoxy(40,1);scanf("%d", &dificuldade);
            enemyQnt = min(max(enemyQnt,1),5);
            dificuldade = min(max(dificuldade,1),5);
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
    }
    else{
        // Logica player
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
        }

        if      (jogador->pos.x <= 0)           {jogador->pos.x = 0;}
        else if (jogador->pos.x >= MAX_COLUNA)  {jogador->pos.x = MAX_COLUNA;}
        if      (jogador->pos.y <= 0)           {jogador->pos.y = 0;}
        else if (jogador->pos.y >= MAX_LINHA-5) {jogador->pos.y = MAX_LINHA-5;}

        // Logica enemy
        for(int i = 0; i < enemyQnt; i++){
            int xDist = jogador->pos.x - enemies[i].pos.x;
            int yDist = jogador->pos.y - enemies[i].pos.y;
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
    Moldura(TRUE, '+', '|', '=');
    int viewX[2] = {2,30}; int viewY[2] = {22,23};
    EntityView(jogador->bag, viewX, viewY);
    gotoxy(MAX_COLUNA,MAX_LINHA); textcolor(jogador->level+2); printf("%d", jogador->level);
    gotoxy(jogador->pos.x, jogador->pos.y); textcolor(YELLOW); printf("@");
    for(int i = 0; i < enemyQnt; i++){gotoxy(enemies[i].pos.x, enemies[i].pos.y); textcolor(enemies[i].currentColor); printf("%c", enemies[i].indentifier.symbol);}
    gotoxy(MAX_COLUNA, MAX_LINHA+1);
}

void BATTLE_SCENE(struct entity *enemy, struct player *jogador){
    int attempt[2] = {0};
    char logs[2][60] = {"",""};
    float behaviour = 0.0f;
    // Pre Turn
    if(!battleTurn){
        CaveiraAnim(backgroundType, CYAN);
        behaviour = (jogador->level - enemy->level)*4;
        jogador->currentEntity = 0;
        
        battleTurn++;
    }
    else{
        // In Turn
        do{input = getch();}while(!isCharInsideArray(input, "1234"));
        switch(input){
            case '1':
                ActionBox(60,20,'+','|','=');
                ClearLog(0); ClearLog(1);
                bool haveAbilityOpen = FALSE;
                for(int i = 0; i < 4; i++){
                    if(jogador->bag[jogador->currentEntity].abilities[i].indentifier.symbol != '\0'){
                        haveAbilityOpen = TRUE;
                        break;
                    }
                }
                if(!haveAbilityOpen){strcpy(logs[0], "Nao possui ataques disponiveis"); break;}
                do{input = getch();}while(!isCharInsideArray(input, "1234") || jogador->bag[jogador->currentEntity].abilities[(int)input - '1'].indentifier.symbol == '\0');
                attempt[0] = doAttempt(enemy, jogador->bag[jogador->currentEntity], (int)input - '1', logs[0]);
                break;
            case '2':
                ActionBox(60,20,'+','|','=');
                ClearLog(0); ClearLog(1);
                gotoxy(4,MAX_LINHA-3); printf("Selecione qual sera substituido");
                WriteBox(jogador->bag[0].indentifier.name, jogador->bag[1].indentifier.name, jogador->bag[2].indentifier.name, jogador->bag[3].indentifier.name);
                do{input = getch();}while(!isCharInsideArray(input, "1234") || jogador->bag[(int)input - '1'].indentifier.symbol == '\0');
                jogador->currentEntity = (int)input - '1';
                break;
            case '3':
                ActionBox(60,20,'+','|','=');
                ClearLog(0); ClearLog(1);
                gotoxy(4,MAX_LINHA-3); printf("Selecione qual sera substituido");
                WriteBox(jogador->bag[0].indentifier.name, jogador->bag[1].indentifier.name, jogador->bag[2].indentifier.name, jogador->bag[3].indentifier.name);
                do{input = getch();}while(!isCharInsideArray(input, "1234"));
                if(CatchEntity(jogador, *enemy, heartsCounter[1],(int)input - '1', dificuldade)){
                    strcpy(logs[0], "Capturado com sucesso");
                    enemy->inactive = TRUE;
                    scene = WORLD_MAP;
                    battleTurn = 0; worldTurn = 0;
                    jogador->level++;
                }
                else{strcpy(logs[0], "Nao foi capturado.");}
                break;
            case '4':
                if(!(rand()%4)){scene = WORLD_MAP; strcpy(logs[0], "O jogador fugiu da batalha"); battleTurn = 0;}
                else{strcpy(logs[0], "O jogador tentou fugir da batalha mas nao conseguiu");} 
                break;
        } 
        if(enemy->inactive){}
        else{
            if(rand()%11 >= behaviour){
                int randomEnemyAbility = rand()%4;
                bool haveAbilityOpen = FALSE;
                for(int i = 0; i < 4; i++){
                    if(&enemy->abilities[randomEnemyAbility].indentifier.symbol != '\0'){
                        haveAbilityOpen = TRUE;
                        break;
                    }
                    randomEnemyAbility++;
                    randomEnemyAbility = randomEnemyAbility%4;
                }
                if(!haveAbilityOpen){strcpy(logs[1], "O inimigo nao possui ataques disponiveis");}
                else{attempt[1] = doAttempt(&jogador->bag[jogador->currentEntity], *enemy, rand()%4, logs[1]);}
            }
            else{
                if(!(rand()%10)){
                    scene = WORLD_MAP;
                    enemy->pos.x = rand()%MAX_COLUNA;
                    enemy->pos.y = rand()%MAX_LINHA;
                    battleTurn = 0;
                    strcpy(logs[1], "O inimigo fugiu da batalha");
                }
                else{strcpy(logs[1], "O inimigo tentou fugir da batalha mas nao conseguiu");}
            }
        }

        // Post Turn
        if(enemy->health.current <= 0){
            scene = WORLD_MAP;
            jogador->level++;
            enemiesKilled++;
            closestToDeath = min(closestToDeath,heartsCounter[0]);
            worldTurn = 0; battleTurn = 0;
        }
        else if(jogador->bag[jogador->currentEntity].health.current <= 0){scene = GAME_END;}
        
        heartsCounter[1] = (enemy->health.current*1.0f/enemy->health.max)*16;
        heartsCounter[0] = (jogador->bag[jogador->currentEntity].health.current*1.0f/jogador->bag[jogador->currentEntity].health.max)*16;

        behaviour = min((jogador->level-enemy->level) * 3 + (16.0f/heartsCounter[0]) * 2, 9);
    }

    // RENDER
    textbackground(backgroundType); textcolor(MAGENTA);
    Moldura(TRUE, '+', '|', '=');
    
    // RENDER CHARS
    PrintPortrait(78,2,enemy->specie.portrait,heartsCounter[1]);
    PrintHearts(heartsCounter[1], 72, 9);

    PrintPortrait(4,14,jogador->bag[jogador->currentEntity].specie.portrait,heartsCounter[0]);
    PrintHearts(heartsCounter[0], 4, 11);

    gotoxy(4,MAX_LINHA-3); printf(logs[0], attempt[0]);
    gotoxy(4,MAX_LINHA-2); printf(logs[1], attempt[1]);

    WriteBox("ATTACK", "SWITCH", "CATCH", "FLEE");

    gotoxy(2,2); printf("Turno %d", battleTurn);
    battleTurn++;
}

int main(){
    srand(time(NULL));
    struct player jogador = {{45,13}, {0}, 0, 1, 2};
    struct entity enemies[MAXENEMIES] = {0};
    ResetEnemies(enemies, jogador);
    CopyEntity(&jogador.bag[0], enemies[0]);
    while (running) {
        Debug(jogador);
        switch(scene){
            case MENU:
                MENU_SCENE();
                break;
            case WORLD_MAP:
                WORLD_SCENE(enemies, &jogador);
            break;

            case BATTLE:
                BATTLE_SCENE(&enemies[enemyIndex], &jogador);
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

