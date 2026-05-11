#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "gconio.h"

int MAX_COLUNA = 90; /// X
int MAX_LINHA = 25;  /// Y

enum cena{MENU,WORLD_MAP,BATTLE,GAME_END};

int main(){
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
    srand(time(NULL));

    // JOGADOR
    int posX = 45;                  /// Posi��o X do jogador
	int posY = 13;                  /// Posi��o Y do jogador

	int velo = 2;                   /// Velocidade do jogador
    int level = 1;                  /// Level do jogador
	int healthMax = level * 5 + 15; /// Vida maxima do jogador
    int health = healthMax;         /// Vida do jogador
    int heartsCounter = 16;         /// Coracoes do jogador
    int ataque = 0;                 /// Ataque do jogador
    int defesa = 0;                 /// Defesa do jogador
    int dano = 0;                   /// Dano do jogador
    //

    // ENEMY
	int posX_Enemy = 80;                /// Posi��o X do inimigo
	int posY_Enemy = 20;                /// Posi��o Y do inimigo

	int health_Enemy = 100;             /// Vida do inimigo
    int healthMax_Enemy = 100;          /// Vida maxima do inimigo
    int heartsCounter_Enemy = 16;       /// Coracoes do inimigo
	int velo_Enemy = 1;                 /// Velocidade do inimigo

	int aggro_Enemy = 12;               /// Dist�ncia de aggro do inimigo
    int aggroed_Enemy = 0;              /// Valor booleano do aggro

    int color_Enemy = BLACK;            /// Cor do inimigo

    int level_Enemy = 0;                /// Level do inimigo
    int ataque_Enemy = 0;               /// Ataque do inimigo
    int defesa_Enemy = 0;               /// Defesa do inimigo
    int dano_Enemy = 0;                 /// Dano do inimigo

    float behaviour = 0.0f;
    //

    // Var
    char input = ' ';
    int yDist = 0;
    int xDist = 0;
    int dist = 0;
    int attackRoll = 0;
    char logPlayer[60] = "";
    char logEnemy[60] = "";

    // Scores
    int walkedDistance = 0;
    int enemiesKilled = 0;
    int closestToDeath = 16;

    while (running) {

        // Debug
        if(debugMode){
            textbackground(WHITE);
            gotoxy(1, MAX_LINHA+2); textcolor(MAGENTA);
            printf(" DEBUG: ON ; INVINCIBLE: %d", invencivel);


            gotoxy(1, MAX_LINHA+3); textcolor(RED);
            printf(" PLAYER-> pos:[%d,%d] ; health: %d/%d (%d) ; velo: %d ; input: %c ; dist: [%d, %d]-> %d ; ataque: %d ;  defesa: %d ; dano: %d ; level: %d "
                , posX, posY, health, healthMax, heartsCounter,velo,input, xDist, yDist, dist, ataque, defesa, dano, level);

            gotoxy(1, MAX_LINHA+4); textcolor(color_Enemy);
            printf(" ENEMY-> pos:[%d,%d] ; health: %d/%d (%d) ; velo: %d ; aggro_range: %d ; aggroed: %d ; ataque: %d, defesa: %d ; dano: %d ; level: %d "
                , posX_Enemy, posY_Enemy, health_Enemy, healthMax_Enemy, heartsCounter_Enemy, velo_Enemy, aggro_Enemy, aggroed_Enemy, ataque_Enemy, defesa_Enemy, dano_Enemy, level_Enemy);
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
                clrscr();
                textbackground(BLACK); textcolor(MAGENTA);
                printf("+========================================================================================+");
                printf("        *     +                                         .                                 \n");
                printf("'    '           *         TRABALHO PROGRAMACAO 1 - RAPHAEL              *                \n");
                printf("             .        '  .               +       +           +                            \n");
                printf("                                    o                                          .          \n");
                printf("                        .-'""'-.                                                          \n");
                printf("                      .' () .   '.   .          .                                         \n");
                printf("                     / .      o   \\                              o                      . \n");
                printf("    .               ; o    _   ()  ;                                                      \n");
                printf("            o o   . ;     (_)      ;  +                  '    .                     .:'   \n");
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

                for(int i = 1; i < MAX_LINHA-2; i++){
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

                if(!level_Enemy){
                    worldTurn = 0;
                    battleTurn = 0;
                    
                    level_Enemy = rand()%3+level-1;

                    posX_Enemy = rand()%MAX_COLUNA;
                    posY_Enemy = rand()%MAX_LINHA;

                    healthMax = level * 5 + 15;
                    health = healthMax;
                    ataque = level * 2 - 2;
                    defesa = level * 2 + 8;
                    dano = level * 2 + 2;

                    healthMax_Enemy = level_Enemy * 5 + 15;
                    health_Enemy = healthMax_Enemy;
                    ataque_Enemy = level_Enemy * 2 - 2;
                    defesa_Enemy = level_Enemy * 2 + 8;
                    dano_Enemy = level_Enemy * 2 + 2;
                }
                else{

                    // Logica player
                    input = getch();
    
                    switch(input){
                        case 'w':
                        case 'W':
                            posY -= velo;
                            walkedDistance++;
                            break;
                        case 's':
                        case 'S':
                            posY += velo;
                            walkedDistance++;
                            break;
                        case 'a':
                        case 'A':
                            posX -= velo;
                            walkedDistance++;
                            break;
                        case 'd':
                        case 'D':
                            posX += velo;
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
    
                    if      (posX <= 0)           {posX = 0;}
                    else if (posX >= MAX_COLUNA)  {posX = MAX_COLUNA;}
                    if      (posY <= 0)           {posY = 0;}
                    else if (posY >= MAX_LINHA)   {posY = MAX_LINHA;}
    
    
                    // Logica enemy
                    xDist = posX - posX_Enemy;
                    yDist = posY - posY_Enemy;
                    dist = sqrt(pow(xDist, 2) + pow(yDist, 2)); /// Distancia entre o jogador e o inimigo
    
                    if(dist > aggro_Enemy){             /// Fora da range
                        posX_Enemy += (rand() % 3 - 1) * velo_Enemy;
                        posY_Enemy += (rand() % 3 - 1) * velo_Enemy;
                        color_Enemy = YELLOW;
                        aggroed_Enemy = 0;
                    }
                    else if(dist > velo_Enemy){         /// Dentro da range
                        if(xDist != 0){
                            posX_Enemy += (xDist / abs(xDist)) * velo_Enemy;
                        }
                        if(yDist != 0){
                            posY_Enemy += (yDist / abs(yDist)) * velo_Enemy;
                        }
                        color_Enemy = CYAN;
                        aggroed_Enemy = 1;
                    }
                    else if(!invencivel){              /// Encostou no jogador
                        color_Enemy = WHITE;
                        scene = BATTLE;
                        Sleep(200);
                    }
                }

                worldTurn++;

                if(worldTurn > 99){level_Enemy = 0;}

                if      (posX_Enemy <= 0)           {posX_Enemy = 0;}
                else if (posX_Enemy >= MAX_COLUNA)  {posX_Enemy = MAX_COLUNA;}
                if      (posY_Enemy <= 0)           {posY_Enemy = 0;}
                else if (posY_Enemy >= MAX_LINHA)   {posY_Enemy = MAX_LINHA;}

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

                    gotoxy(MAX_COLUNA,MAX_LINHA); textcolor(level+2); printf("%d", level);
                    // RENDER Entidades
                    gotoxy(posX, posY); textcolor(YELLOW); printf("@");
                    gotoxy(posX_Enemy, posY_Enemy); textcolor(color_Enemy); printf("%d", level_Enemy);
                    //
                //


                gotoxy(MAX_COLUNA, MAX_LINHA+1);
                break;

            case BATTLE:
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

                    behaviour = (level - level_Enemy)*4;
                    strcpy(logPlayer,""); strcpy(logEnemy, "");
                    
                    battleTurn++;
                }
                else{
                    // In Turn
                    do{
                        input = getch();
                    }while(input != '1' && input != '2');
    
                    switch(input){
                        case '1':
                            attackRoll = (rand()%19+1) + ataque;
                            if(attackRoll > defesa_Enemy){
                                health_Enemy -= dano;
                                strcpy(logPlayer, "O jogador acertou o inimigo, causando %d de dano");
                            }
                            else{strcpy(logPlayer, "O jogador tentou acertar o inimigo mas errou");}
                            break;
                        case '2':
                            if(!(rand()%4)){
                                scene = WORLD_MAP;
                                level_Enemy = 0;
                                strcpy(logPlayer, "O jogador fugiu da batalha");
                            }
                            else{strcpy(logPlayer, "O jogador tentou fugir da batalha mas nao conseguiu");}
                            break;
                    }
    
                    if(rand()%11 >= behaviour){
                        attackRoll = (rand()%19+1) + ataque_Enemy;
                        if(attackRoll > defesa){
                            health -= dano_Enemy;
                            strcpy(logEnemy, "O inimigo acertou o jogador, causando %d de dano");
                        }
                        else{strcpy(logEnemy, "O inimigo tentou acertar o jogador mas errou");}
                    }
                    else{
                        if(!(rand()%10)){
                            scene = WORLD_MAP;
                            posX_Enemy = rand()%MAX_COLUNA;
                            posY_Enemy = rand()%MAX_LINHA;
                            worldTurn = 0; battleTurn = 0;
                            strcpy(logEnemy, "O inimigo fugiu da batalha");
                        }
                        else{strcpy(logEnemy, "O inimigo tentou fugir da batalha mas nao conseguiu");}
                    }

                    // Post Turn
                    
                    if(health_Enemy <= 0){
                        scene = WORLD_MAP;
                        level++;
                        enemiesKilled++;
                        closestToDeath = min(closestToDeath,heartsCounter);
                        level_Enemy = 0;
                    }
                    else if(health <= 0){scene = GAME_END;}
                    
                    heartsCounter_Enemy = (health_Enemy*1.0f/healthMax_Enemy)*16;
                    heartsCounter = (health*1.0f/healthMax)*16;
    
                    behaviour = (level-level_Enemy) * 3 + (heartsCounter_Enemy/16.0f) * 12;
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
                if(heartsCounter_Enemy>12){
                    printf(" _(-_-)_ ");
                }
                else if(heartsCounter_Enemy>8){
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
                if(heartsCounter>12){
                    printf(" /(.) (.)\\ ");
                }
                else if(heartsCounter>8){
                    printf(" /(O) (O)\\");
                }
                else{
                    printf(" /(X) (X)\\");
                }
                gotoxy(4,16); printf(";    O    ;");
                gotoxy(4,17); printf(" \\ }---{ / ");
                gotoxy(4,18); printf("  '-...-'  ");
                //
                for(int i = 0; i < heartsCounter_Enemy; i++){
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

                for(int i = 0; i < heartsCounter; i++){
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

                gotoxy(4,MAX_LINHA-2); printf(logEnemy, dano_Enemy);
                gotoxy(4,MAX_LINHA-3); printf(logPlayer, dano);
                gotoxy(65,MAX_LINHA-3); printf("1. ATACAR");
                gotoxy(65,MAX_LINHA-2); printf("2. FUGIR");

                gotoxy(2,2); printf("Turno %d", battleTurn);
                battleTurn++;
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
    textcolor(BLACK);

    clrscr();
    return 0;
}