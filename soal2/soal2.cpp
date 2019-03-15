#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <grp.h>
#include <pwd.h>

using namespace std;

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
    string lokasi="/home/rak/SoalShift_modul2_F03/soal2/hatiku/elen.ku";
    //6 loop utama jika diperlukan program berjalan kontinyu
    while(true)
    {
        //mendapatkan owner dan groupname dari file
        struct stat info;
        stat((char*)lokasi.c_str(), &info);
        struct passwd *pw = getpwuid(info.st_uid);
        struct group  *gr = getgrgid(info.st_gid);
        string usernm(pw->pw_name);
        string groupnm(gr->gr_name);

        //membandingkan jika samadengan www-data
        if (usernm=="www-data" && groupnm=="www-data")
            remove((char*)lokasi.c_str());

        //setiap 3 detik delay = berjalan setiap 3 detik
        sleep(3);
    }
    exit(EXIT_SUCCESS);
}