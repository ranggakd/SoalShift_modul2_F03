#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

int main()
{
    pid_t proses;
    int pipeF[2];
    //buat channel pipe
    pipe(pipeF);

    //fork pertama
    proses = fork();
    if (proses==0)
    {
        //child1
        //melakukan unzip file
        char* zip[]={"unzip", "/home/rak/SoalShift_modul2_F03/soal3/campur2.zip", NULL};
        execv("/usr/bin/unzip",zip);
    }
    else if (proses>0)
    {
        //parent1
        //fork kedua
        int status2;
        proses = fork();
        if (proses==0)
        {
            //child2
            //membuat file output
            char* bash[]={"bash", "-c", "> ~/SoalShift_modul2_F03/soal3/daftar.txt", NULL};
            execv("/bin/bash",bash);
        }
        else if (proses>0)
        {
            //parent2
            //wait untuk child selesai dulu
            while((wait(&status2))>0);

            //fork ketiga
            int status3;
            proses = fork();
            if (proses==0)
            {
                //child3
                //menutup stdin pipe yg tdk terpakai
                close(pipeF[0]);

                //melakukan copying file descriptor, lalu stdout dari program akan tertulis di channel pipe
                dup2(pipeF[1], 1);

                //melakukan filtering file .txt
                char* bash2[]=
                {
                "bash", "-c",
                "for f in ~/SoalShift_modul2_F03/soal3/campur2/*; do f=${f:45}; echo $f; done | grep -e txt$ > ~/SoalShift_modul2_F03/soal3/daftar.txt",
                NULL
                };
                execv("/bin/bash",bash2);
            }
            else if (proses>0) {
                //parent3
                //menutup stdout  pipe yg tdk terpakai
                close(pipeF[1]);

                //melakukan read dari channel pipe untuk di write ke file
                char tmp[500];
                FILE *dst=fopen("/home/rak/SoalShift_modul2_F03/soal3/daftar.txt","w");
                while(read(pipeF[0], tmp, sizeof(tmp))!=0)
                    fputs(tmp,dst);
                fclose(dst);
            }
        }
    }
    return 0;
}