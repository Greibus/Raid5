#include <zconf.h>
#include <pwd.h>
#include <sys/stat.h>
#include <iostream>
#include <dirent.h>
#include "Controller/Controller.h"
#include "File/File.h"
using namespace std;
void writefile(){
    FILE *iFile1;
    iFile1 = fopen("ACHIVO.mp3", "wb");
    fwrite(Controller::getFile("Prueba_exceso.mp3"), 1000000, 1, iFile1);
}
int printfiles( char* filepath){
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (filepath)) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            char* cosa = ent->d_name;
            if(cosa!=".."&&cosa!="."){
                cout<<cosa<<endl;
            }
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }
}
void savefile(){
    string filename = "Kansas - Dust in the Wind.mp3";
    FILE *iFile =  fopen("/home/tony/Escritorio/Songs/Kansas - Dust in the Wind.mp3", "rb");
    fseek(iFile, 0, SEEK_END);
    long lSize = ftell(iFile);
    rewind(iFile);
    char* buffer = (char *) malloc(sizeof(char) * lSize);
    fread(buffer, 1, lSize, iFile);
    //cout<<buffer<<endl;
    File files =  File();
    Controller::saveFile(buffer,(char*)filename.c_str(),lSize);
}

int main(int argc, char **argv) {
    writefile();
    savefile();
    string filename = "Prueba_exceso.mp3";
    FILE *iFile = fopen("/home/tony/Raid/Disk 1", "rb");
    fseek(iFile, 0, SEEK_END);
    long lSize = ftell(iFile);
    rewind(iFile);
    char *buffer = (char *) malloc(sizeof(char) * lSize);
    fread(buffer, 1, lSize, iFile);
    cout << buffer << endl;
    Controller::check();
    Controller::editFile("editado.mp3", "xxx.mp3");
    File::reBuildFile("Prueba_exceso.mp3", 1);
    File files = File();
    printfiles((char *) files.filePath(1)->c_str());
    files.reBuildFile("KAKA.mp3", 1);
    files.deleteFile("KAKA.mp3");
}
