//
// Created by tony on 15/06/18.
//

#ifndef RAID5_FILE_H
#define RAID5_FILE_H

#include <zconf.h>
#include <pwd.h>
#include <bitset>
#include <sys/stat.h>
#include "iostream"
using namespace std;
class File {
public:
    static void editFile(char* oldname,char* newname);
    static void saveAllFile(char* file,char* filename,long size);
    static void create_disks(){
        struct passwd *pw = getpwuid(getuid());
        const char *homedir = pw->pw_dir;
        string name = homedir;
        name.append("/Raid");
        string da = name + "/Disk 1";
        mkdir(da.c_str(), 0777);
        da = name + "/Disk 2";
        mkdir(da.c_str(), 0777);
        da = name + "/Disk 3";
        mkdir(da.c_str(), 0777);
    }
    static void createDisk1(int disk){
        struct passwd *pw = getpwuid(getuid());
        const char *homedir = pw->pw_dir;
        string name = homedir;
        name.append("/Raid");
        string da;
        if(disk==1){
            da = name + "/Disk 1";
            mkdir(da.c_str(), 0777);
        }
        else if(disk==2){
            da = name + "/Disk 2";
            mkdir(da.c_str(), 0777);}
        else{
            da = name + "/Disk 3";
            mkdir(da.c_str(), 0777);}
    }
    static string* filePath(int disc);
    static void saveFile(char* file,char* filename,long size, int number);
    static void reBuildFile(char*filename, int failure);
    static  void makefile(char* file1,char*file2,long size,int disc);
    static  char makecharacter(char char1,char char2);
    static char reconstruct_from3rd(char file1,char file3);
    static char* getFile(string filename);
    static void deleteFile(char* filename);
    static void createFile3(char*file1);
};



#endif //RAID5_FILE_H
