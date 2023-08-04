/*
    Changes your wallpapers based on your location!

    +──────────────────────────+
    |    ____             _    |
    |   |  _ \ __ _ _ __ / |   |         
    |   | |_) / _` | '_ \| |   |
    |   |  _ < (_| | |_) | |   |
    |   |_| \_\__,_| .__/|_|   |
    |            |_|           |
    +──────────────────────────+
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <shlobj.h>

int imageFile(const char* filename){

    char* imageExtensions[] = {".jpg", ".jpeg", ".png", ".gif", ".bmp"};

    for (int i = 0; i < (sizeof(imageExtensions) / sizeof(imageExtensions[i])); i++){

        if (strstr(filename, imageExtensions[i]) != NULL){
            return 1;
        }
    }
    
    return 0;
}

void SetWallpaper(const char* FinalPath){
    SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (void*)FinalPath, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
}

void WallpaperSelect(const char* wallpapersPath, const char* selectedPath, int i){

    short selectedWallpaper; //Usei short em vez de int porque vai ser so um valor

    WIN32_FIND_DATA findFileData;
    HANDLE hfind = FindFirstFile(selectedPath, &findFileData);

    if (hfind == INVALID_HANDLE_VALUE){
        printf("Error opening the directory");
    }

    while (selectedWallpaper != i){
        
        printf("\n-> Select Wallpaper: ");
        scanf("%hd", &selectedWallpaper);

        if (selectedWallpaper >= 0 && selectedWallpaper < i){

            hfind = FindFirstFile(selectedPath, &findFileData);

            int j = 0;

            do{
                if (imageFile(findFileData.cFileName)){

                    if (j == selectedWallpaper){

                        char FinalPath[MAX_PATH];

                        snprintf(FinalPath, sizeof(FinalPath), "%s\\%s", wallpapersPath, findFileData.cFileName);
                        printf("\n-> Wallpaper changed to: %s\n", findFileData.cFileName);
                        SetWallpaper(FinalPath);
                    }
                    j++;
                }
            }while (FindNextFile(hfind, &findFileData) != 0);
        }
    }

    FindClose(hfind);

    Sleep(500);
    printf("\n-> Exiting.");

    for (int b = 0; b < 4; b++){
        Sleep(300);
        printf(".");
    }
    Sleep(500);

}

void listImage(const char* filePath){

    char wallpapersPath[MAX_PATH]; 

    FILE* file;
    file = fopen(filePath, "r");

    if (fgets(wallpapersPath, MAX_PATH, file) != NULL){
        
        printf("-> Wallpapers Location: %s\n", wallpapersPath);
        Sleep(1000);
    }  
    else{
        printf("-> Error reading the file.");
    } 

    fclose(file);

    char selectedPath[MAX_PATH];
    snprintf(selectedPath, MAX_PATH, "%s\\*", wallpapersPath);

    WIN32_FIND_DATA findFileData;
    HANDLE hfind = FindFirstFile(selectedPath, &findFileData);

    if (hfind == INVALID_HANDLE_VALUE){
        printf("-> Erro opening directory");
        return;
    }

    printf("\n-> Wallpapers List:\n\n");

    int i = 0;

    do{
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            continue;
        }

        if(imageFile(findFileData.cFileName)){

            printf("-> %d: %s\n", i - 1, findFileData.cFileName, i++);
        }

    }while(FindNextFile(hfind, &findFileData) != 0);

    FindClose(hfind);

    printf("-> %d: Exit\n", i);

    WallpaperSelect(wallpapersPath, selectedPath, i);

}

int CreatingFile(char* nijikaPath){

    char* filenameTXT = "\\wallpapersLocation.txt";
    char wallpapersPath[MAX_PATH];
    char tempNijikaPath[MAX_PATH];

    strcpy(tempNijikaPath, nijikaPath);

    char* filePath = strcat(tempNijikaPath, filenameTXT);

    FILE* file;
    file = fopen(filePath, "r"); //Le para ver se tem um arquivo

    //Verifica se o arquivo existe
    if (file == NULL){

        //Nao tem entao criamos um
        file = fopen(filePath, "w");
        
        if (file == NULL){

            printf("-> Error creating the file.");
            return 1;
        }
        else{

            printf("-> Enter your Wallpapers Location: ");
            scanf("%s", wallpapersPath);

            fprintf(file, wallpapersPath);
            fclose(file);

            Sleep(1000);

            listImage(filePath);
        }
    }
    else{
        //Já existe o arquivo
        fclose(file);

        listImage(filePath);

    }

    return 0;
}

int CreatingNijikaFolder(){

    char documentsPath[MAX_PATH];
    char nijikaPath[MAX_PATH];

    if (SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, 0, documentsPath) != S_OK){

        printf("-> Error trying to get Documents Path.\n");
        return 1;
    }
    
    snprintf(nijikaPath, MAX_PATH, "%s\\NIJIKA", documentsPath);

    if (CreateDirectory(nijikaPath, NULL) || ERROR_ALREADY_EXISTS == GetLastError()){

        printf("\n-> Wallpapers Location File: %s\n", nijikaPath);
        Sleep(1000);

        CreatingFile(nijikaPath);
    }
    else{

        printf("-> Error creating NIJIKA folder.\n");
        return 1;
    }

    return 0;
    
}

int main(){

    printf("\nWelcome to NIJIKA !\n\n");
    printf("########################################\n");
    printf("#                                      #\n");
    printf("#               NIJIKA 1.0             #\n");
    printf("#                                      #\n");
    printf("#          NIJIKA is a wallpaper       #\n");
    printf("#    changer based in one directory    #\n");
    printf("#                                      #\n");
    printf("########################################\n\n");

    system("pause");

    CreatingNijikaFolder();

    return 0;
}
