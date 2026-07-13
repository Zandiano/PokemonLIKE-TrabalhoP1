#ifndef __BATTLE_H
#define __BATTLE_H

#include <math.h>
#include "objects.h"
#include "render.h"
#include "variables.h"
#include "entity.h"
#include "player.h"
#include "elements.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))

bool isCharInsideArray(char x, char array[]){
    int len = strlen(array);
    for(int i = 0; i < len; i++){
        if(array[i] == x){return TRUE;}
    }
    return FALSE;
}

int doAttempt(struct entity *target, struct ability ability, int atributes[6], char log[]){
    int prob = rand()%100+1;
    int danoBase = ability.damage.value;
    int ataque = atributes[atk+ability.damage.type];
    int defesa = target->atributes[def+ability.damage.type];
    
    float dmg = danoBase * fmax(ataque/100,1) / fmax(defesa/100,1) * ElementEffectiviness(ability.damage.element, target->specie.element[0], target->specie.element[1]);

    if(prob>ability.accuracy || dmg <= 0){
        if(ability.logMessage[1][0] != '\0'){
            strcpy(log, ability.logMessage[1]);
        }
        return 0;
    }

    if(ability.logMessage[0][0] != '\0'){
        strcpy(log, ability.logMessage[0]);
    }
    
    target->health.current -= dmg;
    return (int)dmg;
}

void Attack(struct player *jogador, struct entity *enemy, int attempt[2], char logs[2][60], bool type){
    if(type){
        ActionBox(60,20,'+','|','=');
        WriteBox(jogador->bag[jogador->currentEntity].abilities[0].indentifier.name, jogador->bag[jogador->currentEntity].abilities[1].indentifier.name, jogador->bag[jogador->currentEntity].abilities[2].indentifier.name, jogador->bag[jogador->currentEntity].abilities[3].indentifier.name);
        ClearLog(0); ClearLog(1);
        
        bool haveAbilityOpen = FALSE;
        for(int i = 0; i < 4; i++){
            if(jogador->bag[jogador->currentEntity].abilities[i].indentifier.symbol){
                haveAbilityOpen = TRUE;
                break;
            }
        }
        if(!haveAbilityOpen){
            strcpy(logs[0], "Nao possui ataques disponiveis");
            return;
        }
        
        do{
            input = getch();
        }while(
            !isCharInsideArray(input, "1234") || 
            !jogador->bag[jogador->currentEntity].abilities[(int)input - '1'].indentifier.symbol
        );
        
        attempt[0] = doAttempt(
            enemy, 
            jogador->bag[jogador->currentEntity].abilities[(int)input - '1'], 
            jogador->bag[jogador->currentEntity].atributes, logs[0]
        );
    }
    else{
        int randomEnemyAbility = rand()%4;
        bool haveAbilityOpen = FALSE;
        for(int i = 0; i < 4; i++){
            if(&enemy->abilities[randomEnemyAbility].indentifier.symbol){
                haveAbilityOpen = TRUE;
                break;
            }
            randomEnemyAbility = randomEnemyAbility + 1 % 4;
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

}

void Switch(struct player *jogador){
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
}

void TryCatch(struct player *jogador, struct entity *enemy, bool *win, char log[60], int attempt[2]){
    ActionBox(60,20,'+','|','=');
    ClearLog(0); ClearLog(1);
    
    gotoxy(4,MAX_LINHA-3); printf("Selecione qual sera substituido");
    WriteBox(jogador->bag[0].indentifier.name, jogador->bag[1].indentifier.name, jogador->bag[2].indentifier.name, jogador->bag[3].indentifier.name);
    
    do{
        input = getch();
    }while(!isCharInsideArray(input, "1234"));
    
    if(CatchEntity(jogador, *enemy, heartsCounter[1], (int)input - '1', &attempt[0])){
        strcpy(log, "Capturado com sucesso (%d%%)");
        enemy->inactive = TRUE;
        *win = TRUE;
    }
    else{
        strcpy(log, "Nao foi capturado (%d%%)");
    }
}

void TryFlee(struct entity *enemy, char logs[2][60], bool type){
    if(type){
        if(!(rand()%4)){
            scene = WORLD_MAP; 
            strcpy(logs[0], "O jogador fugiu da batalha"); 
            battleTurn = 0;
        }
        else{
            strcpy(logs[0], "O jogador tentou fugir da batalha mas nao conseguiu");
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

void PlayerBattleLogic(struct entity *enemy, struct player *jogador, int attempt[2], char logs[2][60], bool *win){
    if(forceSwitch){
        Switch(jogador);
        return;
    }

    do{
        input = getch();
    }while(!isCharInsideArray(input, "1234"));

    switch(input){
        case '1':
            Attack(jogador, enemy, attempt, logs, TRUE);
            break;

        case '2':
            Switch(jogador);
            break;
        
        case '3':
            TryCatch(jogador, enemy, win, logs[0], attempt);
            break;
        
        case '4':
            TryFlee(enemy, logs, TRUE);
            break;
    } 
}

void EnemyBattleLogic(struct entity *enemy, struct player *jogador, int attempt[2], char logs[2][60]){
    float behaviour = fmin((jogador->bag[jogador->currentEntity].level-enemy->level) * 3 + (16.0f/heartsCounter[0]) * 2, 9);

    if(enemy->health.current <= 0 || enemy->inactive || forceSwitch){}
    else{
        if(rand()%11 >= behaviour){
            Attack(jogador, enemy, attempt, logs, FALSE);
        }
        else{
            TryFlee(enemy, logs, FALSE);
        }
    }
}

void BattleWinLogic(struct entity *enemy, struct player *jogador, bool win, char logs[2][60]){
    if(enemy->health.current <= 0 || win){
        scene = WORLD_MAP;
        LevelUpEntity(&jogador->bag[jogador->currentEntity]);

        HealParty(jogador->bag);

        closestToDeath = min(closestToDeath,heartsCounter[0]);
        worldTurn = 0; battleTurn = 0;
        
        if(enemy->health.current <= 0){
            enemiesKilled++;
            KillEntity(enemy);
            strcpy(logs[1], "Inimigo perdeu a batalha");
        }
    }
    else if(jogador->bag[jogador->currentEntity].health.current <= 0){
        KillEntity(&jogador->bag[jogador->currentEntity]);

        hasEntityLeft = FALSE;
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
            strcpy(logs[0], "Perdeu");
        }
    }
}

#endif