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
#include <time.h>

using namespace std;

//fungsi untuk format filename
string FileName(int flag)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    string fileName;
    if (flag==0)
    {
        //initiate first time
        fileName = to_string(tm.tm_mday) +":"+ to_string(tm.tm_mon+1) +":"+ to_string(tm.tm_year+1900);
        fileName += "-" + to_string(tm.tm_hour) +":"+ to_string(tm.tm_min);
        return fileName;
    }
    else if (flag==1)
    {
        //adding 30 min
        tm.tm_min+=30;
        //adjust the right time
        mktime(&tm);
        fileName = to_string(tm.tm_mday) +":"+ to_string(tm.tm_mon+1) +":"+ to_string(tm.tm_year+1900);
        fileName += "-" + to_string(tm.tm_hour) +":"+ to_string(tm.tm_min);
        return fileName;
    }
}

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

    //buat dir log sesuai soal
    string pathName="/home/rak/SoalShift_modul2_F03/soal5/log/";
    mkdir(pathName.c_str(),0777);

    //fork-kan daemon/orphan child
    pid = fork();
    if (pid==0)
    {
        //child
        //6 loop utama jika diperlukan program berjalan kontinyu
        while(true)
        {
            //buat dir format folder sesuai soal
            string folder=FileName(0);
            mkdir((pathName + folder).c_str(),0777);
            cout << "folder " << pathName + folder << endl;
            sleep(1800);
            //sleep 1800 == delayed hingga ke-1800 sec
        }
    }
    else if (pid>0)
    {
        //parent
        //6 loop utama jika diperlukan program berjalan kontinyu
        //counter filename, variabel pembeda, variabel filename+30 min scr berturut-turut
        int flag=0;
        int diff=0;
        string alterFolder;
        while(true)
        {
            //penentuan filename atau filename+30 min
            string folder;
            string now=FileName(0);

            //cek eksistensi folder
            struct stat st;
            // cout << "\tf " << folder << endl;
            // cout << "\tn " << now << endl;
            // cout << "\taf " << alterFolder << endl;
            if (diff==0)
            {   //kondisi awal
                folder=FileName(0);
                alterFolder=FileName(1);
                // cout << "diff0" << endl;
            }
            else if (diff!=0 && now==alterFolder && stat((pathName + alterFolder).c_str(),&st)==0)
            {   //kondisi setelah folder pertama:harus ada folder+30 min, cegah infinite loop
                folder=alterFolder;
                alterFolder=FileName(1);
                // cout << "diff1" << endl;
            }
            if (stat((pathName + folder).c_str(),&st)==0 && folder.empty()==false)
            {   //kondisi1=cek ada folder dan kondisi2=cek ada string folder
                while(true){
                    diff=1;
                    //get file syslog and generate log#.log
                    string systemLog="/var/log/syslog";
                    string artificialLog=pathName + folder + "/log" + to_string(flag+1) + ".log";
                    FILE *sL=fopen((char*)systemLog.c_str(),"r");
                    FILE *aL=fopen((char*)artificialLog.c_str(),"w");

                    //menghitung banyak character di syslog
                    string input;
                    int counter=0;
                    ifstream streamKu(systemLog.c_str());
                    if(streamKu.is_open())
                    {
                        while(getline(streamKu,input))
                            counter+=input.length();
                    }
                    streamKu.close();

                    //copying syslog to log#.log & close stream
                    string konten;
                    while((fgets((char*)konten.c_str(),counter,sL))!=NULL)
                        fputs((char*)konten.c_str(),aL);
                    cout << "file " << artificialLog << endl;
                    fclose(sL);
                    fclose(aL);

                    //increment filename
                    flag++;
                    if (flag%30==0) break;
                    sleep(60);
                    //sleep 60 == delayed hingga ke-60 sec
                }
            }
        }
    }
    exit(EXIT_SUCCESS);
}                                    