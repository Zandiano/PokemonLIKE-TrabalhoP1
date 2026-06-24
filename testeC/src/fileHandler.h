#ifndef __FILEHANDLER_H
#define __FILEHANDLER_H

#include <stdio.h>
#include <string.h>
#include "specie.h"

void OpenConfig(char filesName[MAXSPECIES][20]);
void LoadPointers(FILE *filesPtrs[MAXSPECIES], char filesName[MAXSPECIES][20]);
void LoadSpecies(struct specie specieArray[MAXSPECIES], FILE *filesPtrs[MAXSPECIES]);

void OpenConfig(char filesName[MAXSPECIES][20]){
    FILE *configPtr = fopen("../pokes/config.txt", "r");
    if(!configPtr){printf("Config nao encontrada"); exit(1);}
    for(int i = 0; i < MAXSPECIES; i++){
        if(!fscanf(configPtr, "%s", filesName[i])){
            i--;
        }
        if(filesName[i][0] != '\0'){
            printf("fileName: %s -> Ok\n", filesName[i]);
        }
    }
    fclose(configPtr);
}

void LoadPointers(FILE *filesPtrs[MAXSPECIES], char filesName[MAXSPECIES][20]){
    for(int i = 0; i < MAXSPECIES && filesName[i][0] != '\0'; i++){
        char fileName[34];
        snprintf(fileName, 34, "../pokes/%s.specie", filesName[i]);
        filesPtrs[i] = fopen(fileName, "r");
        printf("file: %s -> Ok\n", fileName);
    }
}

void LoadSpecies(struct specie specieArray[MAXSPECIES], FILE *filesPtrs[MAXSPECIES]){
    for(int i = 0; i < MAXSPECIES && filesPtrs[i] != NULL; i++){
        fscanf(filesPtrs[i], "%c", &specieArray[i].indentifier.symbol);
        fscanf(filesPtrs[i], "%s", &specieArray[i].indentifier.name);
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
                fscanf(filesPtrs[i], "%s", &specieArray[i].portrait.battle[j][k]);
            }
        }
        fclose(filesPtrs[i]);
        printf("Specie: %s -> Ok\n", specieArray[i].indentifier.name);
    }
}
#endif