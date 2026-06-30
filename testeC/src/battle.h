#ifndef __BATTLE_H
#define __BATTLE_H

#include "objects.h"
#include "specie.h"
#include "enums.h"
#include "player.h"
#include "entity.h"

void PlayerBattleLogic(struct entity *enemy, struct player *jogador, int attempt[2], char logs[2][60], bool *win){
    if(forceSwitch){
        input = '2';
        battleTurn--;
    }
    else{
        do{
            input = getch();
        }while(!isCharInsideArray(input, "1234"));
    }

    switch(input){
        case '1':
            ActionBox(60,20,'+','|','=');
            WriteBox(jogador->bag[jogador->currentEntity].abilities[0].indentifier.name, jogador->bag[jogador->currentEntity].abilities[1].indentifier.name, jogador->bag[jogador->currentEntity].abilities[2].indentifier.name, jogador->bag[jogador->currentEntity].abilities[3].indentifier.name);
            ClearLog(0); ClearLog(1);
            
            bool haveAbilityOpen = FALSE;
            for(int i = 0; i < 4; i++){
                if(jogador->bag[jogador->currentEntity].abilities[i].indentifier.symbol != '\0'){
                    haveAbilityOpen = TRUE;
                    break;
                }
            }
            if(!haveAbilityOpen){
                strcpy(logs[0], "Nao possui ataques disponiveis");
                break;
            }
            
            do{
                input = getch();
            }while(
                !isCharInsideArray(input, "1234") || 
                jogador->bag[jogador->currentEntity].abilities[(int)input - '1'].indentifier.symbol == '\0'
            );
            
            attempt[0] = doAttempt(
                enemy, 
                jogador->bag[jogador->currentEntity].abilities[(int)input - '1'], 
                jogador->bag[jogador->currentEntity].atributes, logs[0]
            );
            
            break;

        case '2':
            ActionBox(60,20,'+','|','=');
            ClearLog(0); ClearLog(1);
            
            gotoxy(4,MAX_LINHA-3); printf("Selecione qual sera substituido");
            WriteBox(jogador->bag[0].indentifier.name, jogador->bag[1].indentifier.name, jogador->bag[2].indentifier.name, jogador->bag[3].indentifier.name);
            
            do{
                input = getch();
            }while(
                !isCharInsideArray(input, "1234") || 
                jogador->bag[(int)input - '1'].indentifier.symbol == '\0' || 
                jogador->bag[(int)input - '1'].health.current <= 0
            );
            
            jogador->currentEntity = (int)input - '1';
            forceSwitch = FALSE;
            
            break;
        
        case '3':
            ActionBox(60,20,'+','|','=');
            ClearLog(0); ClearLog(1);
            
            gotoxy(4,MAX_LINHA-3); printf("Selecione qual sera substituido");
            WriteBox(jogador->bag[0].indentifier.name, jogador->bag[1].indentifier.name, jogador->bag[2].indentifier.name, jogador->bag[3].indentifier.name);
            
            do{
                input = getch();
            }while(!isCharInsideArray(input, "1234"));
            
            if(CatchEntity(jogador, *enemy, heartsCounter[1],(int)input - '1', dificuldade, cheatMode)){
                enemy->inactive = TRUE;
                *win = TRUE;
            }
            else{
                strcpy(logs[0], "Nao foi capturado.");
            }
            
            break;
        
        case '4':
            if(!(rand()%4)){
                scene = WORLD_MAP; 
                strcpy(logs[0], "O jogador fugiu da batalha"); 
                battleTurn = 0;
            }
            else{
                strcpy(logs[0], "O jogador tentou fugir da batalha mas nao conseguiu");
            } 
            
            break;
    } 
}

void EnemyBattleLogic(struct entity *enemy, struct player *jogador, int attempt[2], char logs[2][60]){
    float behaviour = min((jogador->bag[jogador->currentEntity].level-enemy->level) * 3 + (16.0f/heartsCounter[0]) * 2, 9);

    if(enemy->health.current <= 0 || enemy->inactive || forceSwitch){}
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

            if(!haveAbilityOpen){
                strcpy(logs[1], "O inimigo nao possui ataques disponiveis");
            }
            else{
                attempt[1] = doAttempt(
                    &jogador->bag[jogador->currentEntity], 
                    enemy->abilities[rand()%4], 
                    enemy->atributes, 
                    logs[1]
                );
            }
        }
        else{
            if(!(rand()%10)){
                scene = WORLD_MAP;
                enemy->pos.x = rand()%MAX_COLUNA;
                enemy->pos.y = rand()%MAX_LINHA;
                battleTurn = 0;
                strcpy(logs[1], "O inimigo fugiu da batalha");
            }
            else{
                strcpy(logs[1], "O inimigo tentou fugir da batalha mas nao conseguiu");
            }
        }
    }
}

void BattleWinLogic(struct entity enemy, struct player *jogador, bool win){
    if(enemy.health.current <= 0 || win){
        scene = WORLD_MAP;
        LevelUpEntity(&jogador->bag[jogador->currentEntity]);

        closestToDeath = min(closestToDeath,heartsCounter[0]);
        worldTurn = 0; battleTurn = 0;
        
        if(enemy.health.current <= 0){
            enemiesKilled++;
            gotoxy(4,MAX_LINHA-2); printf("Inimigo perdeu a batalha");
        }
        else{
            gotoxy(4,MAX_LINHA-2); printf("Capturado com sucesso");
        }
        getch();
    }
    else if(jogador->bag[jogador->currentEntity].health.current <= 0){
        bool hasEntityLeft = FALSE;
        
        for(int i = 0; i < 4; i++){
            if(jogador->bag[i].health.current > 0){
                hasEntityLeft = TRUE;
            }
        }

        if(hasEntityLeft){
            forceSwitch = TRUE;
        }
        else{
            scene = GAME_END;
            gotoxy(4,MAX_LINHA-3); printf("Perdeu");
            getch();
        }
    }
}


#endif