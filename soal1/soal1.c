#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main() {
    pid_t pid, sid;
    //1 fork parent
    pid = fork();
    //error handling
    if (pid < 0) exit(EXIT_FAILURE);
    //1 stop parent = orphan process
    if (pid > 0) exit(EXIT_SUCCESS);
    //2 ubah mode file agar dapat di r/w
    umask(0);
    //3 buat session id unik dari kernel agar dpt berjalan
    sid = setsid();
    //error handling
    if (sid < 0) exit(EXIT_FAILURE);
    //4 ubah dir kerja ke tempat yang pasti selalu ada
    if ((chdir("/")) < 0) exit(EXIT_FAILURE);
    //5 tutup file descriptor std
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //main program
    while(1)
    {
        //fork yg dilooping
        pid=fork();
        int status;

        if (pid==0)
        {
            char* bash2[]=
            {
            "bash", "-c",
            "for f in ~/SoalShift_modul2_F03/soal1/gambar/*.png; do a=${f:44}; a=${a:0:-4} ; a=${a}_grey.png ; mv $f /home/rak/modul2/gambar/$a ; done",
            NULL
            };
            execv("/bin/bash",bash2);
        }
        else if (pid>0)
            //wait child
            while((wait(&status))>0);
    }
    exit(EXIT_SUCCESS);
}