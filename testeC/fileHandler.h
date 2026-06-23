#ifndef FILEHANDLER
#define FILEHANDLER

#include <stdio.h>
#include <string.h>
#include "specie.h"

void OpenConfig(char filesName[30][20]){
    FILE *configPtr = fopen("pokes/config.txt", "r");
    for(int i = 0; i < 30 && fscanf(configPtr, "%s", filesName[i] != NULL); i++);
    fclose(configPtr);
}

void LoadPointers(FILE *filesPtrs[30] ,char filesName[30][20]){
    for(int i = 0; i < 30 && filesName[i] != ""; i++){
        char fileName[] = "pokes/";
        strcat(fileName, filesName[i]);
        strcat(fileName, ".specie");

        filesPtrs[i] = fopen(fileName, "r");
    }
}

void LoadSpecies(struct specie specieArray[30], FILE *filesPtrs[30]){
    for(int i = 0; i < 30 && filesPtrs[i] != NULL; i++){
        fscanf(filesPtrs[i], "%c", specieArray[i].indentifier.symbol);
        fscanf(filesPtrs[i], "%s", specieArray[i].indentifier.name);
        fscanf(filesPtrs[i], "%d", specieArray[i].num);
        fscanf(filesPtrs[i], "%d", specieArray[i].rarity);
        fscanf(filesPtrs[i], "%d", specieArray[i].element[0]);
        fscanf(filesPtrs[i], "%d", specieArray[i].element[1]);
        fscanf(filesPtrs[i], "%d", specieArray[i].color[0].idle);
        fscanf(filesPtrs[i], "%d", specieArray[i].color[0].pursuit);
        fscanf(filesPtrs[i], "%d", specieArray[i].color[1].idle);
        fscanf(filesPtrs[i], "%d", specieArray[i].color[1].pursuit);
        //TODO
    }
}

#endif