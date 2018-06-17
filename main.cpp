#include <zconf.h>
#include <iostream>
#include <pwd.h>
#include <sys/stat.h>
#include <dirent.h>

#include "File/File.h"
#include "Controller/Controller.h"

/**
 * Metodo Write
 */
void writefile() {

    FILE *iFile1;
    iFile1 = fopen("FILE.mp3", "wb");
    fwrite(Controller::getFile("test.mp3"), 1000000, 1, iFile1);

}

int Print_Files(char *FilePath) {

    DIR *dir;
    struct dirent *Dirent;

    if ((dir = opendir (FilePath)) != NULL) {

        while ((Dirent = readdir (dir)) != NULL) {

            char* x = Dirent -> d_name;

            if( x != ".." && x != "." ) {

                std::cout << x << std::endl;

            }
        }

        closedir (dir);

    } else {

        /**
         * En caso de problemas al abrir el directorio
         */
        perror ("Error");
        return EXIT_FAILURE;

    }

}

void SaveF() {

    std::string filename = "Kansas - Dust in the Wind.mp3";
//    FILE *iFile =  fopen("/home/tony/Escritorio/Songs/Kansas - Dust in the Wind.mp3", "rb");
    FILE *iFile =  fopen("/home/deiber/Desktop/Songs/test.mp3", "rb");
    fseek(iFile, 0, SEEK_END);
    long lSize = ftell(iFile);
    rewind(iFile);
    char* buffer = (char *) malloc(sizeof(char) * lSize);
    fread(buffer, 1, lSize, iFile);
    //std::cout<<buffer<<std::endl;
    File files =  File();
    Controller::saveFile(buffer,(char*)filename.c_str(),lSize);

}

int main (int argc, char **argv) {

    writefile();
    SaveF();

    std::string filename = "Prueba.mp3";
//    FILE *iFile = fopen("/home/tony/Raid/Disk 1", "rb");
    FILE *iFile = fopen("/home/deiber/Desktop/Songs/test.mp3", "rb");
    fseek(iFile, 0, SEEK_END);
    long lSize = ftell(iFile);
    rewind(iFile);
    char *buffer = (char *) malloc(sizeof(char) * lSize);
    fread(buffer, 1, lSize, iFile);
    std::cout << buffer << std::endl;
    Controller::CheckDisks();
    Controller::editFile("editado.mp3", "x.mp3");
    File::reBuildFile("Prueba.mp3", 1);
    File files = File();
    Print_Files((char *) files.filePath(1)->c_str());
    files.reBuildFile("OK.mp3", 1);
    files.deleteFile("OK.mp3");

}
