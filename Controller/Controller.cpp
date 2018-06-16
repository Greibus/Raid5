//
// Created by tony on 15/06/18.
//

#include "Controller.h"

#include <dirent.h>
#include <iostream>
#include "../File/File.h"

bool Controller::firstTime() {
    struct passwd *pw = getpwuid(getuid());
    string homedirectory = pw->pw_dir;
    homedirectory.append("/Disk 1");
    string homedirectory2 = pw->pw_dir;
    homedirectory2.append("/Disk 2");
    string homedirectory3 = pw->pw_dir;
    homedirectory3.append("/Disk 3");
    DIR * pDir1,*pDir2,*pDir3;
    pDir1 = opendir (homedirectory.c_str());
    pDir2 = opendir(homedirectory2.c_str());
    pDir3 = opendir(homedirectory3.c_str());
    bool existe = false;
    if(pDir1 != NULL&& pDir2!=NULL&&pDir3!=NULL) {
        existe = true;
        (void) closedir (pDir1);
        (void) closedir (pDir2);
        (void) closedir (pDir3);
    }
    return existe;

}

/**
 * Revisa si el disco existe
 * @param number numero de disco a revisar
 * @return True si el disco existe, False si el disco no existe
 */
bool Controller::existDisk(int number) {
    struct passwd *pw = getpwuid(getuid());
    string homedirectory = pw->pw_dir;
    if(number == 1) {
        homedirectory.append("Raid/Disk 1");
    } else if(number == 2){
        homedirectory.append("Raid/Disk 2");
    } else if (number == 3){
        homedirectory.append("Raid/Disk 3");
    }
    DIR *pDir;
    //cout<<homedirectory<<endl;
    bool bExists = false;
    pDir = opendir (homedirectory.c_str());
    if (pDir != NULL)
    {
        bExists = true;
        (void) closedir (pDir);
    }
    return bExists;
}
/**
 * Revisa si existen los discos, si no existe uno lo crea
 */
void Controller::check() {

    bool ok1= existDisk(1);
    bool ok2 = existDisk(2);
    bool ok3 = existDisk(3);
    if(ok1 == false && ok2 == false && ok3 == false){
        return File::create_disks();
    }
    if(ok1 == false){
        File::createDisk1(1);
        reBuildDisk(1);
        return;
    }
    if(ok2 == false){
        File::createDisk1(2);
        reBuildDisk(2);
        return;
    }
    if(ok3 == false){
        File::createDisk1(3);
        reBuildDisk(3);
        return;
    }
    else{
        cout<<"Creado exitosamente"<<endl;
    }

}
/**
 * Regenera un disco perdido
 * @param number numero del disco a recuperar
 */
void Controller::reBuildDisk(int number) {
    string * directory;
    if(number!= 3){
        directory = File::filePath(3);
    }
    else{
        directory = File::filePath(1);
    }
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (directory->c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            char* cosa = ent->d_name;
            cout<<cosa<<endl;
            if(number==3){
                File::createFile3(cosa);
            }
            else{
                File::reBuildFile(cosa,number);
            }

        }
        closedir (dir);
    }

}
/**
 * Guarda un archivo, usa la funcion de File saveAllFile
 * @param file buffer del archivo
 * @param filename nombre del archivo
 * @param size tamaño del archivo
 */
void Controller::saveFile(char *file, char *filename, long size) {
    check();
    File::saveAllFile(file,filename,size);
}
/**
 * Obtiene un lo de un archivo
 * @param filename nombre del archivo
 * @return lo que esta en el archivo
 */
char* Controller::getFile(string filename) {
    return File::getFile(filename);
}
/**
 * Cambia el nombre de un archivo
 * @param oldName nombre del archivo
 * @param newName_nombre para el archivo
 */
void Controller::editFile(char *OldName, char *newName) {
    File::editFile(OldName,newName);
}
/**
 * Borra un archivo
 * @param fileName el nombre del archivo que se va a borrar
 */
void Controller::deleteFile(char *fileName) {
    File::deleteFile(fileName);
}