#include <stdio.h>
#include <stdlib.h>
#include "gconio.h"
#include <time.h>
#include <math.h>

#define MAX_LINHA 25
#define MAX_COLUNA 90

int main()
{
    // JOGADOR
    int posX = 0;
    int posY = 0;

    int health = 100;
    int velo = 1;
    //

    // ENEMY
    int posX_Enemy = 0;
    int posY_Enemy = 0;
    int health_Enemy = 100;
    int velo_Enemy = 1;
    int aggro_Enemy = 10;
    //


    while(1){
        srand(time(NULL));
        textbackground(GREEN);

        /*for(int i = 0; i <= MAX_LINHA; i++){
            for(int k = 0; k <= MAX_COLUNA; k++){
                printf(" ");
            }
            printf("\n");
        } */

        clrscr();
        gotoxy(0,0);


        // Logica Entidades
        gotoxy(posX, posY); textcolor(RED); printf("@");
        gotoxy(posX_Enemy, posY_Enemy); textcolor(BLACK); printf("#");
        //

        char input = getch();

        switch(input){
            case '8':
                posY -= velo;
                break;
            case '2':
                posY += velo;
                break;
            case '4':
                posX -= velo;
                break;
            case '6':
                posX += velo;
                break;
            case '9':
                posX += velo;
                posY -= velo;
                break;
            case '7':
                posX -= velo;
                posY -= velo;
                break;
            case '1':
                posX -= velo;
                posY += velo;
                break;
            case '3':
                posX += velo;
                posY += velo;
                break;
        }

        if(sqrt(pow((posX-posX_Enemy), 2) - pow((posY-posY_Enemy), 2)) <= aggro_Enemy){
            posX_Enemy += ((posX-posX_Enemy)/ abs(posX-posX_Enemy))* velo_Enemy;
            posY_Enemy += ((posY-posY_Enemy)/ abs(posY-posY_Enemy))* velo_Enemy;
        }
        else{
            posX_Enemy += (rand() % 2 - 1)* velo_Enemy;
            posY_Enemy += (rand() % 2 - 1) * velo_Enemy;
        }

        /*
        if(posX > 90){posX = 0;}
        else if(posX < 0){posX = 90;}
        if(posY > 25){posY = 0;}
        else if(posY < 0){posY = 25;}

        if(posX_Enemy > 90){posX_Enemy = 0;}
        else if(posX_Enemy < 0){posX_Enemy = 90;}
        if(posY_Enemy > 25){posY_Enemy = 0;}
        else if(posY_Enemy < 0){posY_Enemy = 25;}
        */

        gotoxy(0,0);
    }
    return 0;
}
