#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "gconio.h"
#include <locale.h>


enum scene{MENU, WORLD_MAP, BATTLE, GAME_END};
enum battleAction{FIGHT, SWITCH, FLEE};

int MAX_COLUNA = 90; /// X
int MAX_LINHA = 25;  /// Y

int main(){
    // Dev Wise
    int debugMode = 0;
    int invencivel = 0;

	// INIT
	int scene = MENU;               /// Controle de cena
	int running = 1;                /// Controle de jogo
    int screenClearType = 0;        /// Tipo de clear
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));



    // JOGADOR
    int posX = 45;                  /// Posição X do jogador
	int posY = 13;                  /// Posição Y do jogador

	int velo = 2;                   /// Velocidade do jogador
    int level = 1;                  /// Level do jogador
	int healthMax = level * 5 + 15; /// Vida maxima do jogador
    int health = healthMax          /// Vida do jogador
    int ataque = level * 2 - 2;     /// Ataque do jogador
    int defesa = level * 2 - 8;     /// Defesa do jogador
    int dano = level * 2 + 2;       /// Dano do jogador

    //

    // ENEMY
	int posX_Enemy = 80;                    /// Posição X do inimigo
	int posY_Enemy = 20;                    /// Posição Y do inimigo

	int health_Enemy = 100;                 /// Vida do inimigo
	int velo_Enemy = 1;                     /// Velocidade do inimigo

	int aggro_Enemy = 12;                   /// Distância de aggro do inimigo
    int aggroed_Enemy = 0;                  /// Valor booleano do aggro

    int color_Enemy = BLACK;                /// Cor do inimigo

    int level_Enemy = 0;                    /// Level do inimigo
    int ataque_Enemy = level_Enemy * 2 - 2; /// Ataque do jogador
    int defesa_Enemy = level_Enemy * 2 - 8; /// Defesa do jogador
    int dano_Enemy = level_Enemy * 2 + 2;   /// Dano do jogador
    //

    // Var
    char input = ' ';
    int yDist = 0;
    int xDist = 0;
    int dist = 0;


    while (running) {
        switch(scene){
            case MENU:
                clrscr();
                textbackground(BLACK); textcolor(BLACK);
                printf("#############################################################\n");
                printf("#                                                           #\n");
                // printf("#   \u2588\u2588\u2588\u2588\u2588\u2588\u2557 \u2588\u2588\u2557   \u2588\u2588\u2557\u2588\u2588\u2557  \u2588\u2588\u2557 \u2588\u2588\u2588\u2588\u2588\u2588\u2557 \u2588\u2588\u2588\u2557   \u2588\u2588\u2588\u2557 \u2588\u2588\u2588\u2588\u2588\u2588\u2557   #\n");
                // printf("#   \u2588\u2588\u2554\u2550\u2550\u2588\u2588\u2557\u2588\u2588\u2551   \u2588\u2588\u2551\u2588\u2588\u2551 \u2588\u2588\u2554\u255D\u2588\u2588\u2554\u2550\u2550\u2550\u2588\u2588\u2557\u2588\u2588\u2588\u2588\u2557 \u2588\u2588\u2588\u2588\u2551\u2588\u2588\u2554\u2550\u2550\u2550\u2588\u2588\u2557  #\n");
                // printf("#   \u2588\u2588\u2588\u2588\u2588\u2588\u2554\u255D\u2588\u2588\u2551   \u2588\u2588\u2551\u2588\u2588\u2588\u2588\u2588\u2554\u255D \u2588\u2588\u2551   \u2588\u2588\u2551\u2588\u2588\u2554\u2588\u2588\u2588\u2588\u2554\u2588\u2588\u2551\u2588\u2588\u2551   \u2588\u2588\u2551  #\n");
                // printf("#   \u2588\u2588\u2588\u2588\u2588\u2588\u2554\u255D\u2588\u2588\u2551   \u2588\u2588\u2551\u2588\u2588\u2588\u2588\u2588\u2554\u255D \u2588\u2588\u2551   \u2588\u2588\u2551\u2588\u2588\u2554\u2588\u2588\u2588\u2588\u2554\u2588\u2588\u2551\u2588\u2588\u2551   \u2588\u2588\u2551  #\n");
                // printf("#   \u2588\u2588\u2551     \u255A\u2588\u2588\u2588\u2588\u2588\u2588\u2554\u255D\u2588\u2588\u2551  \u2588\u2588\u2557\u255A\u2588\u2588\u2588\u2588\u2588\u2588\u2554\u255D\u2588\u2588\u2551 \u255A\u2550\u255D \u2588\u2588\u2551\u255A\u2588\u2588\u2588\u2588\u2588\u2588\u2554\u255D  #\n");
                // printf("#   \u255A\u2550\u255D      \u255A\u2550\u2550\u2550\u2550\u2550\u255D \u255A\u2550\u255D  \u255A\u2550\u255D \u255A\u2550\u2550\u2550\u2550\u2550\u255D \u255A\u2550\u255D     \u255A\u2550\u255D \u255A\u2550\u2550\u2550\u2550\u2550\u255D   #\n");
                //printf("#                                                           #\n");
                printf("#                  >>>  P U K O M O  <<<                    #\n");
                printf("#                                                           #\n");
                printf("############################################################\n\n");

                printf("          #########################################\n");
                printf("          #                                       #\n");
                printf("          #   EU VOU TRANCAR ESSA PORRA           #\n");
                printf("          #                                       #\n");
                printf("          #########################################\n\n");

                printf("                [1] INICIAR JOGO\n");
                printf("                [2] OPCOES\n");
                printf("                [3] SAIR\n\n");

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
                        break;
                    case '3':
                        clrscr();
                        gotoxy(MAX_COLUNA/2-10, MAX_LINHA/2);
                        printf(";(");
                        running = 0;
                        Sleep(3000);
                        break;
                }
                break;



            case WORLD_MAP:
                textbackground(GREEN);

                if(!level_Enemy){
                    level_Enemy = rand()%3+level-1;

                    posX_Enemy = rand()%MAX_COLUNA;
                    posY_Enemy = rand()%MAX_LINHA;

                    posX = rand()%MAX_COLUNA;
                    posY = rand()%MAX_LINHA;

                    healthMax = level * 5 + 15;
                    health = healthMax;
                    ataque = level * 2 - 2;
                    defesa = level * 2 + 8;
                    dano = level * 2 + 2;

                    health_Enemy = level_Enemy * 5 + 15;
                    ataque_Enemy = level_Enemy * 2 - 2;
                    defesa_Enemy = level_Enemy * 2 + 8;
                    dano_Enemy = level_Enemy * 2 + 2;
                }

                // Logica player
                input = getch();

                switch(input){
                    case 'w':
                    case '8':
                        posY -= velo;
                        break;
                    case 's':
                    case '2':
                        posY += velo;
                        break;
                    case 'a':
                    case '4':
                        posX -= velo;
                        break;
                    case 'd':
                    case '6':
                        posX += velo;
                        break;
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
                    color_Enemy = BLACK;
                    aggroed_Enemy = 0;
                }
                else if(dist > velo_Enemy){         /// Dentro da range
                    if(xDist != 0){
                        posX_Enemy += (xDist / abs(xDist)) * velo_Enemy;
                    }
                    if(yDist != 0){
                        posY_Enemy += (yDist / abs(yDist)) * velo_Enemy;
                    }
                    color_Enemy = RED;
                    aggroed_Enemy = 1;
                }
                else if(!invencivel){                               /// Encostou no jogador
                    color_Enemy = MAGENTA;
                    scene = BATTLE;
                    Sleep(500);
                }

                if      (posX_Enemy <= 0)           {posX_Enemy = 0;}
                else if (posX_Enemy >= MAX_COLUNA)  {posX_Enemy = MAX_COLUNA;}
                if      (posY_Enemy <= 0)           {posY_Enemy = 0;}
                else if (posY_Enemy >= MAX_LINHA)   {posY_Enemy = MAX_LINHA;}

                // RENDER
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

                    // RENDER Other

                    gotoxy(MAX_COLUNA,MAX_LINHA); textcolor(level+2); printf("%d", level);
                    // RENDER Entidades
                    gotoxy(posX, posY); textcolor(LIGHTRED); printf("@");
                    gotoxy(posX_Enemy, posY_Enemy); textcolor(color_Enemy); printf("%d", level_Enemy);
                    //
                //

                // Debug
                if(debugMode){
                    textbackground(WHITE);
                    gotoxy(MAX_COLUNA+20, MAX_LINHA/2-3); textcolor(MAGENTA);
                    printf(" DEBUG: ON ; INVINCIBLE: %d", invencivel);


                    gotoxy(MAX_COLUNA+20, MAX_LINHA/2-2); textcolor(RED);
                    printf(" PLAYER-> pos:[%d,%d] ; health: %d ; velo: %d ; input: %c ; dist: [%d, %d]-> %d ", posX,posY,health,velo,input, xDist, yDist, dist);

                    gotoxy(MAX_COLUNA+20, MAX_LINHA/2); textcolor(color_Enemy);
                    printf(" ENEMY-> pos:[%d,%d] ; health: %d ; velo: %d ; aggro_range: %d ; aggroed: %d ", posX_Enemy, posY_Enemy, health_Enemy, velo_Enemy, aggro_Enemy, aggroed_Enemy);
                }
                else{
                    textbackground(BLACK);
                    for(int i = 0; i <= MAX_LINHA/2; i++){
                        for(int k = MAX_COLUNA+20; k <= MAX_COLUNA+105; k++){
                            gotoxy(k, i);
                            printf(" ");
                        }
                    }
                }
                //
                gotoxy(MAX_COLUNA, MAX_LINHA+1);
                break;

            case BATTLE:
                textbackground(BLACK);

                input = getch();

                switch(input){
                    case 1:
                        //
                        break;
                }

                if(health_Enemy <= 0){scene = WORLD_MAP; level++; level_Enemy = 0;}
                else if(health <= 0){scene = GAME_END;}

                // RENDER
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

                //ToDo Layout

                //
                break;

            case GAME_END:
                textbackground(RED);
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
                gotoxy(MAX_COLUNA/2-10, MAX_LINHA/2); textcolor(BLACK); printf("GAME OVER");
                textcolor(BLACK);
                scanf("");
                running = 0;
                Sleep(3000);
                break;
            }
    }
    textbackground(BLACK);
    clrscr();
    return 0;
}
