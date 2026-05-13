#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "gconio.h"

#define MAX_COLUNA 90 /// X
#define MAX_LINHA 25  /// Y

#define listSize(lista) sizeof(lista)/sizeof(lista[0])
#define hearts(health) (health.current/health.max*1.0f)*16

struct cords{int x; int y;}; // 8 Bytes
struct resource{int current; int max;}; // 8 Bytes
struct atributes{int speed; int defense; int attack; int charm;}; // 16 Bytes
struct ability{char name[12]; int damage; int cost;}; // 20 Bytes
struct aggro{int range; int aggroed;}; // 8 Bytes


struct monster{ // 176 Bytes
    char name[24]; // 24 Bytes
    int level; // 4 Bytes
    struct cords position; // 8 Bytes
    int velocity; // 4 Bytes
    struct resource health; // 8 Bytes
    struct atributes atributes; // 16 Bytes
    struct ability abilities[4]; // 80 Bytes
    struct resource stamina; // 8 Bytes
    enum COLORS color; // 4 Bytes
    struct aggro aggro; // 8 Bytes
    struct cords distanceToPlayer; // 8 Bytes
    float behaviour; // 4 Bytes
};

struct jogador{ // 1088 Bytes
    struct cords position; // 8 Bytes
    int level; // 4 Bytes
    int velocity; // 4 Bytes
    struct monster monsters[6]; // 1080 Bytes
    struct monster *currentMonster; // 8 Bytes
    struct monster *currentEnemy; // 8 Bytes
};

enum cena{MENU,WORLD_MAP,BATTLE,GAME_END};

// Var
char selectedAbilityIndex = ' ';
char input = ' ';
struct ability selectedAbility = {0};
struct ability selectedAbilityEnemy = {0};
struct ability NullAbility = {0};
int attackRoll = 0;
char logPlayer[60] = "";
char logEnemy[60] = "";

// Scores
int walkedDistance = 0;
int enemiesKilled = 0;
int closestToDeath = 16;

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

void BattleScene(struct jogador *jogador, struct monster *enemy);
void SwitchMonster(struct jogador *jogador, char input);

int main(){
    srand(time(NULL));

    // JOGADOR
    
    struct jogador jogadorUm = {};

    //

    // ENEMY

	struct ability allAbilities[20] = {
        {"Porrada", 3, 0},
        {"Rasteira", 5, 1},
        {"Arranhao", 4, 0},
        {"Tapa", 2, 0},
        {"Corte", 6, 1},
        {"Mordida", 8, 2}
    };

    struct monster allMonsters[5] = {
        {"Gabiru", 0, {0,0}, 1, {20,20}, {5,5,5,5}, {allAbilities[0], allAbilities[3], allAbilities[4], allAbilities[1]}, {10,10}, LIGHTGRAY, {8,0}, 0.0f},
        {"Adomak", 0, {0,0}, 1, {22,22}, {7,4,4,5}, {allAbilities[2], allAbilities[5], allAbilities[3], allAbilities[1]}, {10,10}, GREEN, {10,0}, 0.0f}
    };

    struct monster loadedMonsters[3] = {0};

    //

    while (running) {
        input = ' ';
        // Debug
        if(debugMode){
            textbackground(WHITE);
            gotoxy(1, MAX_LINHA+2); textcolor(MAGENTA);
            printf(" DEBUG: ON ; INVINCIBLE: %d", invencivel);


            gotoxy(1, MAX_LINHA+3); textcolor(RED);
            printf(" PLAYER-> pos:[%d,%d] ; jogadorUm.velocity: %d ; input: %c ; jogadorUm.level: %d ", jogadorUm.position.x, jogadorUm.position.y, jogadorUm.velocity, input, jogadorUm.level);
        }
        else{
            textbackground(BLACK);
            for(int i = MAX_LINHA+1; i <= MAX_LINHA+3; i++){
                for(int k = 0; k <= MAX_COLUNA; k++){
                    gotoxy(k, i);
                    printf(" ");
                }
            }
        }
        //

        switch(scene){
            case MENU:
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
                printf("    |   '         '        | 1.INICIAR                  |   +   o                         \n");
                printf("                           | 2.SAIR        +           .:'                                \n");
                printf("        +                  +=======================_.::=+                 .      *       .\n");
                printf("          '                      - o -            (_.'                +                   \n");
                printf("             o  . ' +       '      |   o                      .          +                \n");
                printf("                                         +                                         .  +   \n");
                printf("                         '                    +                                           \n");
                printf("                   o               .  +         .                                     .   \n");
                printf(" ZANDIADEV       +    +  o                                      .            *    +       \n");
                printf("+========================================================================================+");

                for(int i = 1; i < MAX_LINHA-1; i++){
                    gotoxy(0,i); printf("|");
                    gotoxy(MAX_COLUNA-1,i); printf("|");
                }

                input = getch();

                switch(input){
                    case '1':
                        clrscr();
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
                        gotoxy(MAX_COLUNA/2-10, MAX_LINHA/2);
                        printf(";(");
                        running = 0;
                        Sleep(3000);
                        break;
                }
                break;



            case WORLD_MAP:
                textbackground(backgroundType);
                if(!worldTurn){

                }
                else{
                    if(worldTurn%20==0){
                        for(int i = 0; i < listSize(loadedMonsters); i++){
                            if(!rand()%50 && loadedMonsters[i].health.current <= 0){
                                loadedMonsters[i] = allMonsters[rand()%sizeof(allMonsters)/sizeof(struct monster)];
                                
                            }
                        }
                    }
                    for(int i = 0; i < listSize(loadedMonsters); i++){
                        if(!loadedMonsters[i].level){                        
                            loadedMonsters[i].level = max(rand()%5+jogadorUm.level-2, 1);
    
                            loadedMonsters[i].position.x = rand()%MAX_COLUNA;
                            loadedMonsters[i].position.y = rand()%MAX_LINHA;
        
                            loadedMonsters[i].health.max *=  1 + loadedMonsters[i].level/10;
                            loadedMonsters[i].health.current = loadedMonsters[i].health.max;
                            loadedMonsters[i].atributes.speed *= 1 + loadedMonsters[i].level/10;
                            loadedMonsters[i].atributes.attack *= 1 + loadedMonsters[i].level/10;
                            loadedMonsters[i].atributes.defense *= 1 + loadedMonsters[i].level/10;
                        }
                    }

                    // Logica player
                    input = getch();
    
                    switch(input){
                        case 'w':
                        case 'W':
                            jogadorUm.position.y -= jogadorUm.velocity;
                            walkedDistance++;
                            break;
                        case 's':
                        case 'S':
                            jogadorUm.position.y += jogadorUm.velocity;
                            walkedDistance++;
                            break;
                        case 'a':
                        case 'A':
                            jogadorUm.position.x -= jogadorUm.velocity;
                            walkedDistance++;
                            break;
                        case 'd':
                        case 'D':
                            jogadorUm.position.x += jogadorUm.velocity;
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
    
                    if      (jogadorUm.position.x <= 0)           {jogadorUm.position.x = 0;}
                    else if (jogadorUm.position.x >= MAX_COLUNA)  {jogadorUm.position.x = MAX_COLUNA;}
                    if      (jogadorUm.position.y <= 0)           {jogadorUm.position.y = 0;}
                    else if (jogadorUm.position.y >= MAX_LINHA)   {jogadorUm.position.y = MAX_LINHA;}
    
    
                    // Logica enemy
                    for(int i = 0; i < listSize(loadedMonsters); i++){
                        loadedMonsters[i].distanceToPlayer.x = jogadorUm.position.x - loadedMonsters[i].position.x;
                        loadedMonsters[i].distanceToPlayer.y = jogadorUm.position.y - loadedMonsters[i].position.y;

                        int dist = sqrt(pow(loadedMonsters[i].distanceToPlayer.x, 2) + pow(loadedMonsters[i].distanceToPlayer.y, 2)); /// Distancia entre o jogador e o inimigo
        
                        if(dist > loadedMonsters[i].aggro.range){             /// Fora da range
                            loadedMonsters[i].position.x += (rand() % 3 - 1) * loadedMonsters[i].velocity;
                            loadedMonsters[i].position.y += (rand() % 3 - 1) * loadedMonsters[i].velocity;
                            loadedMonsters[i].aggro.aggroed = 0;
                        }
                        else if(dist > loadedMonsters[i].velocity){         /// Dentro da range
                            if(loadedMonsters[i].distanceToPlayer.x != 0){
                                loadedMonsters[i].position.x += (loadedMonsters[i].distanceToPlayer.x / abs(loadedMonsters[i].distanceToPlayer.x)) * loadedMonsters[i].velocity;
                            }
                            if(loadedMonsters[i].distanceToPlayer.y != 0){
                                loadedMonsters[i].position.y += (loadedMonsters[i].distanceToPlayer.y / abs(loadedMonsters[i].distanceToPlayer.y)) * loadedMonsters[i].velocity;
                            }
                            loadedMonsters[i].aggro.aggroed = 1;
                        }
                        else if(!invencivel){              /// Encostou no jogador
                            jogadorUm.currentEnemy = &loadedMonsters[i];
                            scene = BATTLE;
                            Sleep(200);
                        }
                        
                        if      (loadedMonsters[i].position.x <= 0)           {loadedMonsters[i].position.x = 0;}
                        else if (loadedMonsters[i].position.x >= MAX_COLUNA)  {loadedMonsters[i].position.x = MAX_COLUNA;}
                        if      (loadedMonsters[i].position.y <= 0)           {loadedMonsters[i].position.y = 0;}
                        else if (loadedMonsters[i].position.y >= MAX_LINHA)   {loadedMonsters[i].position.y = MAX_LINHA;}
                    }
                }

                worldTurn++;

                // RENDER
                textcolor(MAGENTA);
                if(!screenClearType){
                    for(int i = 0; i <= MAX_LINHA; i++){
                        gotoxy(0, i);
                        for(int k = 0; k <= MAX_COLUNA; k++){
                            if(k == 0 || k == MAX_COLUNA){printf("|");}
                            else{printf(" ");}
                        }
                    }
                }
                else{
                    clrscr();
                }

                    // RENDER Other

                    gotoxy(MAX_COLUNA,MAX_LINHA); textcolor(jogadorUm.level+2); printf("%d", jogadorUm.level);
                    // RENDER Entidades
                    gotoxy(jogadorUm.position.x, jogadorUm.position.y); textcolor(YELLOW); printf("@");
                    for(int i = 0; i < listSize(loadedMonsters); i++){
                        gotoxy(loadedMonsters[i].position.x, loadedMonsters[i].position.y); textcolor(loadedMonsters[i].color); printf("%d", loadedMonsters[i].level);
                    }
                    //
                //


                gotoxy(MAX_COLUNA, MAX_LINHA+1);
                break;

            case BATTLE:
                BattleScene(&jogadorUm, jogadorUm.currentEnemy);
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
                running = 0;
                Sleep(3000);
                break;
            }

    }
    textbackground(BLACK);
    textcolor(WHITE);
    clrscr();
    return 0;
}

void BattleScene(struct jogador *jogador, struct monster *enemy){
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
            clrscr();
        }
        for(int i = 0; i <= 26; i++){
            if(i > 0 ){gotoxy(41,i-1); printf("         ");}
            if(i < 26){gotoxy(41,i+0); printf("  _____  ");}
            if(i < 25){gotoxy(41,i+1); printf(" /     \\ ");}
            if(i < 24){gotoxy(41,i+2); printf("| () () |");}
            if(i < 23){gotoxy(41,i+3); printf(" \\  ^  / ");}
            if(i < 22){gotoxy(41,i+4); printf("  |||||  ");}
            if(i < 21){gotoxy(41,i+5); printf("  |||||  ");}
            Sleep(150);
        }

        enemy->behaviour = (jogador->level - enemy->level)*4;
        strcpy(logPlayer,""); strcpy(logEnemy, "");
        
        battleTurn++;
    }
    else{
        // In Turn
        do{
            input = getch();
        }while(!(input == '1' || input == '2' || input == '3'));

        switch(input){
            case '1':
                selectedAbility = NullAbility;
                attackRoll = (rand()%19+1) + jogador->currentMonster->atributes.attack;
                do{
                    selectedAbilityIndex = getch();
                    switch(selectedAbilityIndex){
                        case '1':
                            selectedAbility = jogador->currentMonster->abilities[0];
                            break;
                        case '2':
                            selectedAbility = jogador->currentMonster->abilities[1];
                            break;
                        case '3':
                            selectedAbility = jogador->currentMonster->abilities[2];
                            break;
                        case '4':
                            selectedAbility = jogador->currentMonster->abilities[3];
                            break;
                    }
                }while(!(input == '1' || input == '2' || input == '3' || input == '4') && (selectedAbility.name == "" || jogador->currentMonster->stamina.current - selectedAbility.cost >= 0));
                if(attackRoll > enemy->atributes.defense){
                    enemy->health.current -= selectedAbility.damage;
                    strcpy(logPlayer, "O jogador acertou o inimigo com %s, causando %d de dano");
                }
                else{strcpy(logPlayer, "O jogador tentou acertar o inimigo mas errou");}
                break;
            case '2':
                if(!(rand()%4)){
                    scene = WORLD_MAP;
                    worldTurn = 0;
                    strcpy(logPlayer, "O jogador fugiu da batalha");
                }
                else{strcpy(logPlayer, "O jogador tentou fugir da batalha mas nao conseguiu");}
                break;
            case '3':
                input = getch();
                SwitchMonster(jogador, input);
                break;
        }

        if(rand()%11 >= enemy->behaviour){
            attackRoll = (rand()%19+1) + enemy->atributes.attack;
            if(attackRoll > jogador->currentMonster->atributes.defense){
                selectedAbilityEnemy = enemy->abilities[rand()%4];
                jogador->currentMonster->health.current -= selectedAbilityEnemy.damage;
                strcpy(logEnemy, "O inimigo acertou o jogador com %s, causando %d de dano");
            }
            else{strcpy(logEnemy, "O inimigo tentou acertar o jogador mas errou");}
        }
        else{
            if(!(rand()%10)){
                scene = WORLD_MAP;
                enemy->position.x = rand()%MAX_COLUNA;
                enemy->position.y = rand()%MAX_LINHA;
                worldTurn = 0; battleTurn = 0;
                strcpy(logEnemy, "O inimigo fugiu da batalha");
            }
            else{strcpy(logEnemy, "O inimigo tentou fugir da batalha mas nao conseguiu");}
        }

        // Post Turn
        
        if(enemy->health.current <= 0){
            scene = WORLD_MAP;
            jogador->currentMonster->level++;
            enemiesKilled++;
            closestToDeath = min(closestToDeath,hearts(jogador->currentMonster->health));
        }
        else if(jogador->currentMonster->health.current <= 0){scene = GAME_END;}
        
        enemy->behaviour = min((jogador->currentMonster->level-enemy->level) * 3 + (16.0f/hearts(enemy->health)) * 2, 9);
    }
    // RENDER
    textbackground(BLACK); textcolor(MAGENTA);                
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
    // RENDER CHARS
    gotoxy(78,2); printf("   ,-.   ");
    gotoxy(78,3);
    if(hearts(enemy->health)>12){
        printf(" _(-_-)_ ");
    }
    else if(hearts(enemy->health)>8){
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
    if(hearts(jogador->currentMonster->health)>12){
        printf(" /(.) (.)\\ ");
    }
    else if(hearts(jogador->currentMonster->health)>8){
        printf(" /(O) (O)\\");
    }
    else{
        printf(" /(X) (X)\\");
    }
    gotoxy(4,16); printf(";    O    ;");
    gotoxy(4,17); printf(" \\ }---{ / ");
    gotoxy(4,18); printf("  '-...-'  ");
    //
    for(int i = 0; i < hearts(enemy->health); i++){
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

    for(int i = 0; i < hearts(jogador->currentMonster->health); i++){
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

    gotoxy(4,MAX_LINHA-2); printf(logEnemy, selectedAbilityEnemy.name, selectedAbilityEnemy.damage);
    gotoxy(4,MAX_LINHA-3); printf(logPlayer, selectedAbility.name, selectedAbility.damage);
    gotoxy(65,MAX_LINHA-3); printf("1. ATACAR");
    gotoxy(76,MAX_LINHA-3); printf("2. FUGIR");
    gotoxy(65,MAX_LINHA-2); printf("3. TROCAR");

    gotoxy(2,2); printf("Turno %d", battleTurn);
    battleTurn++;
}

void SwitchMonster(struct jogador *jogador, char input){
    switch(input){
        case '1':
            if(jogador->monsters[0].health.current > 0){jogador->currentMonster = &jogador->monsters[0];}
            break;
        case '2':
            if(jogador->monsters[1].health.current > 0){jogador->currentMonster = &jogador->monsters[1];}
            break;
        case '3':
            if(jogador->monsters[2].health.current > 0){jogador->currentMonster = &jogador->monsters[3];}
            break;
        case '4':
            if(jogador->monsters[3].health.current > 0){jogador->currentMonster = &jogador->monsters[3];}
            break;
        case '5':
            if(jogador->monsters[4].health.current > 0){jogador->currentMonster = &jogador->monsters[4];}
            break;
        case '6':
            if(jogador->monsters[5].health.current > 0){jogador->currentMonster = &jogador->monsters[5];}
            break;
    }
}