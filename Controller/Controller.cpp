//
// Created by tony on 15/06/18.
//

#include <dirent.h>
#include <iostream>

#include "Controller.h"
#include "../File/File.h"

bool Controller::FirstTime() {

    struct passwd *pw = getpwuid(getuid());
    std::string homedirectory = pw->pw_dir;
    homedirectory.append("/Disk 1");
    std::string homedirectory2 = pw->pw_dir;
    homedirectory2.append("/Disk 2");
    std::string homedirectory3 = pw->pw_dir;
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
 * Revisa si el disco existe.
 * @param number: Numero de disco a revisar.
 * @return True: Si el disco existe, False: Si el disco no existe.
 */
bool Controller::existDisk(int number) {

    struct passwd *pw = getpwuid(getuid());
    std::string homedirectory = pw->pw_dir;

    if(number == 1) {
        homedirectory.append("RAID5/Disk 1");
    } else if(number == 2){
        homedirectory.append("RAID5/Disk 2");
    } else if (number == 3){
        homedirectory.append("RAID5/Disk 3");
    }

    DIR *pDir;
    //std::cout<<homedirectory<<std::endl;
    bool FlagExist = false;
    pDir = opendir (homedirectory.c_str());

    if (pDir != NULL) {

        FlagExist = true;
        (void) closedir (pDir);
    }
    return FlagExist;

}

/**
 * Revisa si existen los discos, si no existen se crean.
 */
void Controller::CheckDisks() {

    bool ExistDisk1= existDisk(1);
    bool ExistDisk2 = existDisk(2);
    bool ExistDisk3 = existDisk(3);

    if ( ExistDisk1 == false && ExistDisk2 == false && ExistDisk3 == false ) {
        return File::CreateDisks();
    }

    if ( ExistDisk1 == false ) {
        File::createDisk1(1);
        reBuildDisk(1);
        return;
    }

    if ( ExistDisk2 == false ) {
        File::createDisk1(2);
        reBuildDisk(2);
        return;
    }

    if ( ExistDisk3 == false ){
        File::createDisk1(3);
        reBuildDisk(3);
        return;

    } else {
        std::cout<<"Discos creados"<<std::endl;
    }

}

/**
 * Regenera un disco perdido.
 * @param Num: Numero del disco a recuperar.
 */
void Controller::reBuildDisk(int Num) {

    std::string * directory;

    if ( Num != 3 ) {
        directory = File::filePath(3);
    }
    else {
        directory = File::filePath(1);
    }

    DIR *dir;
    struct dirent *ent;

    if ( ( dir = opendir (directory -> c_str()) ) != NULL ) {

        // imprime todos los archivos y directorios dentro del directorio
        while ( (ent = readdir (dir)) != NULL ) {

            char* x = ent -> d_name;
            std::cout << x << std::endl;

            if( Num == 3 ) {
                File::createFile3(x);
            } else {
                File::reBuildFile(x,Num);
            }

        }
        closedir (dir);
    }

}

/**
 * Guarda un archivo (mediante el uso de la funcion de File: saveAllFile).
 * @param file: Buffer del archivo.
 * @param filename: Nombre del archivo.
 * @param size: Tamaño del archivo.
 */
void Controller::saveFile(char *file, char *filename, long size) {

    CheckDisks();
    File::SaveAllFile(file, filename, size);

}

/**
 * Obtiene lo que contiene de un archivo.
 * @param filename: Nombre del archivo.
 * @return : Lo que está en el archivo.
 */
char* Controller::getFile(std::string filename) {

    return File::getFile(filename);

}

/**
 * Cambia el nombre de un archivo.
 * @param oldName: Nombre del archivo
 * @param newName: Nombre ha asignar el archivo
 */
void Controller::editFile(char *OldName, char *newName) {

    File::EditFile(OldName, newName);

}

/**
 * Borra un archivo
 * @param fileName el nombre del archivo que se va a borrar
 */
void Controller::deleteFile(char *fileName) {

    File::deleteFile(fileName);

}