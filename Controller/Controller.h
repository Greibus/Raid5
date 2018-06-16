//
// Created by tony on 15/06/18.
//

#ifndef RAID5_CONTROLLER_H
#define RAID5_CONTROLLER_H


#include <string>
#include <zconf.h>
#include <pwd.h>
#include <sys/stat.h>
#include <iostream>

using namespace std;
class Controller {
public:
    static void check();
    static bool existDisk(int number);
    bool firstTime();
    static void reBuildDisk(int number);
    static void saveFile(char *file, char *fileName, long size);
    static void deleteFile(char* filename);
    static void editFile(char* oldName,char* newName);
    static char* getFile(string fileName);

};



#endif //RAID5_CONTROLLER_H
