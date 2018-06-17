//
// Created by tony on 15/06/18.
//

#ifndef RAID5_CONTROLLER_H
#define RAID5_CONTROLLER_H

#include <sys/stat.h>
#include <iostream>
#include <string>
#include <zconf.h>
#include <pwd.h>

class Controller {

public:

    static void CheckDisks();
    static bool existDisk(int number);
    bool FirstTime();
    static void reBuildDisk(int Num);
    static void saveFile(char *file, char *fileName, long size);
    static void deleteFile(char* filename);
    static void editFile(char* oldName,char* newName);
    static char* getFile(std::string fileName);

};

#endif //RAID5_CONTROLLER_H
