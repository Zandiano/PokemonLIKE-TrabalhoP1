#ifndef __FILEHANDLER_C
#define __FILEHANDLER_C

#include "fileHandler.h"

void OpenConfig(char filesName[30][20]){
    FILE *configPtr = fopen("pokes/config.txt", "r");
    for(int i = 0; i < 30 && fscanf(configPtr, "%s", filesName[i]) != 0; i++){
        printf("%s -> Ok", filesName[i]);
    }
    fclose(configPtr);
}

void LoadPointers(FILE *filesPtrs[30], char filesName[30][20]){
    for(int i = 0; i < 30 && filesName[i] != ""; i++){
        char fileName[34] = "pokes/";
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
        for(int j = 0; j < 6; j++){
            fscanf(filesPtrs[i], "%d", specieArray[i].base[j]);
        }
        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 5; k++){
                fscanf(filesPtrs[i], "%s", specieArray[i].portrait.battle[j][k]);
            }
        }
        fclose(filesPtrs[i]);
    }
}

#endif