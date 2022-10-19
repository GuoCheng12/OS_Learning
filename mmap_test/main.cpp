#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
const int RESULT_SIZE = 128;
int mmap_write(const char* path){
    int fd;
    if((fd = open(path,O_RDWR)) < 0){ // fd —— 文件描述符
        perror("open\n");
        return -1;
    }
    struct stat sb;
    if(fstat(fd,&sb) == -1){
        perror("fstat");
    }
    char *mmapped = (char*)mmap(NULL,sb.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(mmapped == (void*)-1){
        perror("error\n");
    }
    close(fd);
    mmapped[2] = 'z';
    if((msync((void*)mmapped,sb.st_size,MS_SYNC)) == -1){
        perror("msync\n");
    }
    if((munmap((void*)mmapped,sb.st_size)) == -1){
        perror("munmap\n");
    }
}
void mmap_read(const char* path){
    char buf[RESULT_SIZE] = {0};
    int fd;
    if((fd = open(path,O_RDWR)) < 0){
        perror("open\n");
        return;
    }
    struct stat sb;
    if(fstat(fd,&sb) == -1){
        perror("fastat");
    }
    char *mmapped;
    if((mmapped = (char*)mmap(NULL,sb.st_size,PROT_READ,MAP_SHARED,fd,0)) == (void*)-1){
        perror("mmap\n");
    }
    close(fd);
    printf("[sxd]----buf:\n%s",mmapped);
}

int main() {
    const char* file_name = "/Users/wuguocheng/Desktop/temp/test.txt";
    mmap_write(file_name);
    mmap_read(file_name);
    return 0;
}
