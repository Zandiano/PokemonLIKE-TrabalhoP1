#ifndef __RENDER_H
#define __RENDER_H

#include "variables.h"
#include "gconio.h"
#include "objects.h"

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
    for(int i = 0; i < 5; i++){
        gotoxy(x,y+i); 
        printf("%s", portrait.battle[(int)(currentHeart/4)][i]);
    }
}

void ActionBox(int middle, int height, char corner, char vertical, char horizontal){
    gotoxy(0,0);
    for(int i = height; i <= MAX_LINHA; i++){
        if(i == height || i == MAX_LINHA){
            for(int k = middle; k <= MAX_COLUNA; k++){
                gotoxy(k,i); printf("%c",horizontal);
            }
        }
        else{
            for(int k = middle; k <= MAX_COLUNA; k++){
                if(k == middle || k == MAX_COLUNA){
                    gotoxy(k,i); printf("%c",vertical);
                }
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
            for(int k = 0; k <= MAX_COLUNA; k++){
                printf("%c",horizontal);
            }
        }
        else{
            for(int k = 0; k <= MAX_COLUNA; k++){
                if(k == 0 || k == MAX_COLUNA){
                    printf("%c",vertical);
                }
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
            for(int k = 0; k <= MAX_COLUNA; k++){
                printf("%c",horizontal);
            }
        }
        else{
            for(int k = 0; k <= MAX_COLUNA; k++){
                if(k == 0 || k == MAX_COLUNA){
                    printf("%c",vertical);
                }
                else{printf(" ");}
            }
        }
    }
    if(isActionBarActive){
        ActionBar(60,20, corner, vertical, horizontal);
    }
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
    gotoxy(2,MAX_LINHA-3-logNum); printf("                                                           ");
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

void EntityView(struct entity entities[4], int x[2], int y[2]){
    gotoxy(x[0], y[0]);
    printf("1. %s (%d) : %d/%d", entities[0].indentifier.name, entities[0].level, entities[0].health.current, entities[0].health.max);
    gotoxy(x[1], y[0]);
    printf("2. %s (%d) : %d/%d", entities[1].indentifier.name, entities[1].level, entities[1].health.current, entities[1].health.max);
    gotoxy(x[0], y[1]);
    printf("3. %s (%d) : %d/%d", entities[2].indentifier.name, entities[2].level, entities[2].health.current, entities[2].health.max);
    gotoxy(x[1], y[1]);
    printf("4. %s (%d) : %d/%d", entities[3].indentifier.name, entities[3].level, entities[3].health.current, entities[3].health.max);

}

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


#endif