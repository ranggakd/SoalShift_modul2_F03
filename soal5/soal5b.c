#include<stdio.h>
#include<unistd.h>

int main(){
    char* hapus[]={"killall", "soal5a", NULL};
    execv("/usr/bin/killall", hapus);
    return 0;
}
