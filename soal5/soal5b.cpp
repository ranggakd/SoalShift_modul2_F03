#include<stdio.h>
#include<unistd.h>

int main(){
    char* hapus[]={(char*)"killall", (char*)"soal5c", NULL};
    execv("/usr/bin/killall", hapus);
    return 0;
}