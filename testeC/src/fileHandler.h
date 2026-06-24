#ifndef __FILEHANDLER_H
#define __FILEHANDLER_H

#include <stdio.h>
#include <string.h>

#include "specie.h"
#include "objects.h"

#define MAXFILES 30

void OpenConfig(char filesName[MAXFILES][64]);
void LoadPointers(FILE *filesPtrs[2][MAXFILES], char filesName[MAXFILES][64]);
void LoadSpecies(struct specie specieArray[MAXFILES], FILE *filesPtrs[MAXFILES]);

void CopyString(FILE *filepointer, char dest[]){
    char buffer[64];    
    fgetc(filepointer);
    fgets(buffer, 64, filepointer);
    for(int i = 0; i < 64 && buffer[i] != '\0';i++){
        dest[i] = buffer[i];
        if(buffer[i] == '\n'){dest[i] = '\0'; break;}
    }
}

void OpenConfig(char filesName[MAXFILES][64]){
    FILE *configPtr = fopen("../pokes/config.txt", "r");
    if(!configPtr){printf("Config nao encontrada"); exit(1);}
    for(int i = 0; i < MAXFILES; i++){
        if(!fscanf(configPtr, "%s", filesName[i])){
            i--;
        }
        if(filesName[i][0] != '\0'){
            printf("fileName: %s -> Ok\n", filesName[i]);
        }
    }
    fclose(configPtr);
}

void LoadPointers(FILE *filesPtrs[2][MAXFILES], char filesName[MAXFILES][64]){
    for(int i = 0; i < MAXFILES; i++){filesPtrs[0][i] = NULL; filesPtrs[1][i] = NULL;}
    for(int i = 0, relativo[2] = {0}; i < MAXFILES && filesName[i][0] != '\0'; i++){
        char fileName[64];
        snprintf(fileName, 64, "../pokes/%s", filesName[i]);

        int divisor = 0;
        for(int j = 0; filesName[i][j] != '\0'; j++){
            if(filesName[i][j] == '.'){
                divisor = filesName[i][j+1] == 'a' && filesName[i][j+2] == 'b';
            }
        }
        filesPtrs[divisor][relativo[divisor]] = fopen(fileName, "r");
        if(filesPtrs[relativo[divisor]] != NULL){
            printf("file: %s -> Ok\n", fileName);
            relativo[divisor]++;
        }
        else{
            printf("file: %s -> ERROR\n", fileName);
        }
    }
}

void LoadSpecies(struct specie specieArray[MAXFILES], FILE *filesPtrs[MAXFILES]){
    for(int i = 0; i < MAXFILES && filesPtrs[i] != NULL; i++){
        fscanf(filesPtrs[i], "%c", &specieArray[i].indentifier.symbol);
        CopyString(filesPtrs[i], specieArray[i].indentifier.name);
        fscanf(filesPtrs[i], "%d", &specieArray[i].num);
        fscanf(filesPtrs[i], "%d", &specieArray[i].rarity);
        fscanf(filesPtrs[i], "%d", &specieArray[i].element[0]);
        fscanf(filesPtrs[i], "%d", &specieArray[i].element[1]);
        fscanf(filesPtrs[i], "%d", &specieArray[i].color[0].idle);
        fscanf(filesPtrs[i], "%d", &specieArray[i].color[0].pursuit);
        fscanf(filesPtrs[i], "%d", &specieArray[i].color[1].idle);
        fscanf(filesPtrs[i], "%d", &specieArray[i].color[1].pursuit);
        for(int j = 0; j < 6; j++){
            fscanf(filesPtrs[i], "%d", &specieArray[i].base[j]);
        }
        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 5; k++){
                CopyString(filesPtrs[i], specieArray[i].portrait.battle[j][k]);
            }
        }
        for(int j = 0; j < 6; j++){
            fscanf(filesPtrs[i], "%d", &specieArray[i].naturalAbilities[j]);
        }
        fclose(filesPtrs[i]);
        printf("Specie: %s -> Ok\n", specieArray[i].indentifier.name);
    }
}

void LoadAbilities(struct ability abilityArray[MAXFILES], FILE *filesPtrs[MAXFILES]){
    for(int i = 0; i < MAXFILES && filesPtrs[i] != NULL; i++){
        fscanf(filesPtrs[i], "%c", &abilityArray[i].indentifier.symbol);
        CopyString(filesPtrs[i], abilityArray[i].indentifier.name);
        fscanf(filesPtrs[i], "%d", &abilityArray[i].damage.value);
        fscanf(filesPtrs[i], "%d", &abilityArray[i].damage.element);
        fscanf(filesPtrs[i], "%d", &abilityArray[i].damage.type);
        fscanf(filesPtrs[i], "%d", &abilityArray[i].accuracy);
        CopyString(filesPtrs[i], abilityArray[i].logMessage[0]);
        CopyString(filesPtrs[i], abilityArray[i].logMessage[1]);
        fclose(filesPtrs[i]);
        printf("Ability: %s -> Ok\n", abilityArray[i].indentifier.name);
    }
}
#endif