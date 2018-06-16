//
// Created by tony on 15/06/18.
//

#include "File.h"


#include <zconf.h>
#include <pwd.h>
#include <bitset>
#include "File.h"

/**
 * Guarda un archivo en los discos del RAID
 * @param file buffer del archivo
 * @param filename nombre del archivo
 * @param size tamaño del archivo
 */
void File::saveAllFile(char *file, char *fileName, long size) {
    saveFile(file,fileName,size, 1);
    saveFile(file,fileName,size, 2);
    delete[] file;
    createFile3(fileName);
}

/**
 * con polaridad reconstruye el archivo perdido
 * @param filename nombre del archivo
 * @param failure numero del archivo que recuperar
 */
void File::reBuildFile(char *filename,int failure) {
    string* path;
    string* to_reconstruct;
    string*  third_disc = filePath(3);
    third_disc->append("/");
    third_disc->append(filename);
    if(failure == 1){
        path  = filePath(2);
        path->append("/");
        path->append(filename);
        to_reconstruct  = filePath(1);
        to_reconstruct->append("/");
        to_reconstruct->append(filename);
    }
    else{
        path  = filePath(1);
        path->append("/");
        path->append(filename);
        to_reconstruct  = filePath(2);
        to_reconstruct->append("/");
        to_reconstruct->append(filename);
    }
    FILE *iFile =  fopen(path->c_str(), "rb");
    FILE *iFile2 =  fopen(third_disc->c_str(), "rb");
    FILE *iFile3 =  fopen(to_reconstruct->c_str(), "wb");
    if(iFile==NULL||iFile2==NULL){
        return;
    }
    fseek(iFile, 0, SEEK_END);
    long lSize = ftell(iFile);
    rewind(iFile);
    char *file1 = (char*)malloc(sizeof(char)*lSize);
    char* file2 = (char*)malloc(sizeof(char)*lSize);
    char* file3 = (char*)malloc(sizeof(char)*lSize);;
    fread(file1, 1, lSize, iFile);
    fread(file3, 1, lSize, iFile2);
    int x = 0;
    if(file1==NULL){
        delete[] file1;
        delete[] file2;
        delete[] file3;
        return;
    }
    while(x<lSize){
        file2[x] = reconstruct_from3rd(file1[x],file3[x]);
        x++;
    }
    fwrite(file2,1,lSize,iFile3);
    delete[] file1;
    delete[] file2;
    delete[] file3;

}

/**
 *
 * @param file1
 * @param file3
 * @return
 */
char File::reconstruct_from3rd(char file1, char file3) {
    bitset<8> character1 = bitset<8>(file1);
    bitset<8> character2 = bitset<8>(file3);
    bitset<8> character3 = bitset<8>();
    int x = 0;
    while(x<8){
        if(character2[x]==1){
            if(character1[x]==0){
                character3[x]=0;
            }
            else{
                character3[x]=1;
            }
        }
        else{
            if(character1[x]==0){
                character3[x]=1;
            }
            else{
                character3[x]=0;
            }
        }
        x++;
    }
    return static_cast<unsigned char>(character3.to_ullong());

}

/**
 * Guarda un archivo especifico en un disco
 * @param file buffer del archivo
 * @param filename nombre del archivo donde se va a guardar
 * @param size tamaño del archivo
 * @param number numero para ver en que disco se guardara
 */
void File::saveFile(char *file, char *filename, long size, int number) {
    char* file1=(char*)malloc(size/2);
    std::string* pathfile1;
    long index = 0;
    while(index<size/2){
        file1[index]= file[index];
        index++;
    }
    if (number ==1){
        pathfile1 = filePath(1);
    } else if (number == 2) {
        pathfile1 = filePath(2);
    }
    pathfile1->append("/");
    pathfile1->append(filename);
    FILE *iFile1;
    iFile1 = fopen(pathfile1->c_str(), "wb");
    fwrite(file1, size/2, 1, iFile1);
    delete[] file1;

}
/**
 * Obtiene el path para crear los discos
 * @param disc numero para diferenciar entre discos
 * @return el path del disco
 */
string* File::filePath(int disc) {
    struct passwd *pw = getpwuid(getuid());
    string homedirectory = pw->pw_dir;
    homedirectory.append("/Raid");
    //std::cout<<homedirectory<<endl;

    if(disc==1){
        homedirectory.append("/Disk 1");
        string data = homedirectory.c_str();
        //cout << data << endl;
        return new string(homedirectory.c_str());
    }
    if(disc ==2){
        homedirectory.append("/Disk 2");
    }else{
        homedirectory.append("/Disk 3");
    }
    //std::cout<<homedirectory<<endl;
    return new string(homedirectory.c_str());
}
/**
 * Crea el tercer disco
 * @param filename nombre del disco
 */
void File::createFile3(char *filename) {
    string *path1 = filePath(1);
    path1->append("/");
    path1->append(filename);
    string *path2 = filePath(2);
    path2->append("/");
    path2->append(filename);
    string *path3 = filePath(3);
    path3->append("/");
    path3->append(filename);
    FILE *iFile =  fopen(path1->c_str(), "rb");
    FILE *iFile2 =  fopen(path2->c_str(), "rb");
    if(iFile==NULL||iFile2==NULL){
        return;
    }
    fseek(iFile, 0, SEEK_END);
    long lSize = ftell(iFile);
    rewind(iFile);
    fseek(iFile2, 0, SEEK_END);
    lSize = ftell(iFile2);
    rewind(iFile2);
    char* buffer = (char *) malloc(sizeof(char) * lSize);
    char* buffer2 = (char *) malloc(sizeof(char) * lSize);
    char* buffer3 = (char *) malloc(sizeof(char) * lSize);
    fread(buffer, 1, lSize, iFile);
    fread(buffer2, 1, lSize, iFile2);
    int x = 0;
    if(buffer==NULL){
        return;
    }
    while(x<lSize){
        buffer3[x] = makecharacter(buffer2[x],buffer[x]);
        x++;
    }
    FILE* iFile3 = fopen(path3->c_str(), "wb");
    fwrite(buffer3, lSize, 1, iFile3);
    delete[] buffer;
    delete[] buffer2;
    delete[] buffer3;
}

/**
 *
 * @param char1
 * @param char2
 * @return
 */
char File::makecharacter(char char1, char char2) {
    bitset<8> character1 = bitset<8>(char1);
    bitset<8> character2 = bitset<8>(char2);
    bitset<8> character3 = bitset<8>();
    //  cout<<character1<<endl;
    int x = 0;
    while(x<8){
        if(character1[x]==character2[x]){
            character3[x] = 1;
        }else{
            character3[x] = 0;
        }
        x++;
    }
    return static_cast<unsigned char>(character3.to_ullong());
}

/**
 * Obtiene lo del archivo
 * @param filename nombre del archivo del cual se le va a sacar la informacion
 * @return informacion del archivo
 */
char* File::getFile(string filename) {
    string *path1 = filePath(1);
    path1->append("/");
    path1->append(filename);
    string *path2 = filePath(2);
    path2->append("/");
    path2->append(filename);
    FILE *iFile =  fopen(path1->c_str(), "rb");
    FILE *iFile2 =  fopen(path2->c_str(), "rb");
    fseek(iFile, 0, SEEK_END);
    long lSize = ftell(iFile);
    rewind(iFile);
    fseek(iFile2, 0, SEEK_END);
    lSize = ftell(iFile2);
    rewind(iFile2);
    char* buffer = (char *) malloc(sizeof(char) * lSize);
    char* buffer2 = (char *) malloc(sizeof(char) * lSize);
    char* buffer3 = (char *) malloc(sizeof(char) * lSize*2);
    fread(buffer, 1, lSize, iFile);
    fread(buffer2, 1, lSize, iFile2);
    int x =0;
    while(x<lSize){
        buffer3[x] = buffer[x];
        buffer3[x+lSize]=buffer2[x];
        x++;
    }
    delete[] buffer;
    delete[] buffer2;
    return buffer3;

}

/**
 * edita el nombre de un archivo
 * @param oldname nombre del archivo
 * @param newname nombre para el nuevo archivo
 */
void File::editFile(char *oldname, char *newname) {
    string *path1 = filePath(1);
    path1->append("/");
    path1->append(oldname);
    string *path2 = filePath(2);
    path2->append("/");
    path2->append(oldname);
    string *path3 = filePath(3);
    path3->append("/");
    path3->append(oldname);
    string *path1_new = filePath(1);
    path1_new->append("/");
    path1_new->append(newname);
    string *path2_new = filePath(2);
    path2_new->append("/");
    path2_new->append(newname);
    string *path3_new = filePath(3);
    path3_new->append("/");
    path3_new->append(newname);
    rename( (char*)path1->c_str() , (char*)path1_new->c_str() );
    rename( (char*) path2->c_str(), (char*) path2_new->c_str());
    rename( (char*)path3->c_str() , (char*) path3_new->c_str());
    path1->clear();
    path1_new->clear();
    path2->clear();
    path2_new->clear();
    path3->clear();
    path3_new->clear();
}

/**
 * elimina un archivo
 * @param filename nombre del archivo a eliminar
 */
void File::deleteFile(char *filename) {
    string *path1 = filePath(1);
    path1->append("/");
    path1->append(filename);
    string *path2 = filePath(2);
    path2->append("/");
    path2->append(filename);
    string *path3 = filePath(3);
    path3->append("/");
    path3->append(filename);
    remove((char*)path1->c_str() );
    remove((char*)path2->c_str() );
    remove((char*)path3->c_str() );

}