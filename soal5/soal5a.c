#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>

//fungsi untuk format filename
char* FileName(int flag)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    static char fileName[50];
    if (flag==1)
    {
        //adding 30 min for next time
//        tm.tm_min+=30;
	tm.tm_sec+=30;
        //adjust the right time
        mktime(&tm);
    }
    //initiate first time
    sprintf(fileName, "%d:%d:%d-%d:%d:%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return fileName;
}
/*
int main(){
	printf("now %s\n",FileName(0));
	printf("now+30 %s\n",FileName(1));
}
*/

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
    char pathName[100]={"/home/rak/log/"};
    mkdir(pathName,0777);

    //fork-kan daemon/orphan child
    pid = fork();
    if (pid==0)
    {
        //child
        //6 loop utama jika diperlukan program berjalan kontinyu
        while(1)
        {
            //buat dir format folder sesuai soal
            char folder[100];
	    strcpy(folder,pathName);
	    strcat(folder,FileName(0));
            mkdir(folder, 0777);
            printf("folder %s\n",folder);
//            sleep(1800);
	    sleep(30);
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
        char alterFolder[100];
        while(1)
        {
            //penentuan filename atau filename+30 min
            char folder[100];
	    strcpy(folder,"");
	    printf("===ini folder %s\n",folder);
            char now[100];
	    strcpy(now,FileName(0));

	    //untuk kebutuhan elif
	    char pathAlter[100];
	    strcpy(pathAlter,pathName);
	    strcat(pathAlter,alterFolder);

            //cek eksistensi folder
            struct stat st;

	    //percabangan untuk plotting time +30
            if (diff==0)
            {   //kondisi awal
		strcpy(folder,FileName(0));
		strcpy(alterFolder,FileName(1));
                printf("==================================================\n");
            }
            else if (diff!=0 && strcmp(now,alterFolder)==0 && stat(pathAlter,&st)==0)
            {   //kondisi setelah folder pertama:harus ada folder+30 min, cegah infinite loop
		strcpy(folder,alterFolder);
		strcpy(alterFolder,FileName(1));
                printf("diff1\tf %s  af %s\n",folder,alterFolder);
            }

	    //untuk kebutuhan if generate file
            char pathFolder[100];
            strcpy(pathFolder,pathName);
            strcat(pathFolder,folder);

	    //debug
            printf("\tf %s\n", folder);
            printf("\tn %s\n", now);
            printf("\taf %s\n", alterFolder);
            printf("\tpa %s\n", pathAlter);
            printf("\tpf %s\n", pathFolder);

	    //kondisi untuk generate file
            if (stat(pathFolder,&st)==0 && strlen(folder)!=0)
            {   //kondisi1=cek ada folder dan kondisi2=cek ada string folder
                while(1){
                    diff=1;
                    //get file syslog and generate log#.log
                    char systemLog[100]={"/var/log/syslog"};
                    char artificialLog[100];
		    strcpy(artificialLog,pathName);
		    strcat(artificialLog,folder);
		    strcat(artificialLog,"/log");

		    //convert int to string
		    char angka[10];
		    sprintf(angka,"%d.log",flag+1);
		    strcat(artificialLog,angka);
                    FILE *sL=fopen(systemLog,"r");
                    FILE *aL=fopen(artificialLog,"w");

                    //copying syslog to log#.log & close stream
                    char konten;
                    while((konten=fgetc(sL))!=EOF)
                        fputc(konten,aL);
                    printf("file %s\n", artificialLog);
                    fclose(sL);
                    fclose(aL);

                    //increment filename
                    flag++;
                    if (flag%15==0) break;
                    sleep(2);
                    //sleep 60 == delayed hingga ke-60 sec
                }
            }
        }
    }
    exit(EXIT_SUCCESS);
}
