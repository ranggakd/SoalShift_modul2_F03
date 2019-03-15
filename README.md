# SoalShift_modul2_F03
Keperluan tugas laboratorium Sistem Operasi 2019  

<center>

![image](cover.gif "Sisop Art Online")

</center>

---

## NO 1

Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “[namafile]_grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori /home/[user]/modul2/gambar.  

Catatan : Tidak boleh menggunakan crontab.

**Jawaban**

1. Buat program dibawah ini dengan nama file soal1.cpp di folder yang ditentukan.

```cpp
//source
```
Penjelasan:
```cpp
//source code here
```
sub
```cpp
//source code here
```
sub
```cpp
//source code here
```
sub

---
## NO 2

Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.  

Catatan: Tidak boleh menggunakan crontab

**Jawaban**

1. Buat program dibawah ini dengan nama file soal2.cpp di folder yang ditentukan.

```cpp
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
```
Penjelasan:
```cpp
//source code here
```
sub
```cpp
//source code here
```
sub
```cpp
//source code here
```
sub

---
## NO 3

Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”.  
Buatlah program C yang dapat :
1. mengekstrak file zip tersebut.
2. menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt.

Catatan:
* Gunakan fork dan exec.
* Gunakan minimal 3 proses yang diakhiri dengan exec.
* Gunakan pipe
* Pastikan file daftar.txt dapat diakses dari text editor

**Jawaban**

1. Buat program dibawah ini dengan nama file soal3.c di folder yang ditentukan.

```c
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
```
Penjelasan:
```cpp
//source code here
```
sub
```cpp
//source code here
```
sub
```cpp
//source code here
```
sub

---

## NO 4

Dalam direktori /home/[user]/Documents/makanan terdapat file makan_enak.txt yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi makan_enak.txt karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file makan_enak.txt pernah dibuka setidaknya 30 detik yang lalu (rentang 0 - 30 detik).
Jika file itu pernah dibuka, program Anda akan membuat 1 file makan_sehat#.txt di direktori /home/[user]/Documents/makanan dengan '#' berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet.  

Contoh:  
File makan_enak.txt terakhir dibuka pada detik ke-1  
Pada detik ke-10 terdapat file makan_sehat1.txt dan makan_sehat2.txt

Catatan: 
-	dilarang menggunakan crontab
-	Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst

**Jawaban**

1. Buat program dibawah ini dengan nama file soal4.cpp di folder yang ditentukan.

```cpp
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
    string pathEnak="/home/rak/Documents/makanan/makan_enak.txt";
    int inc=1;
    //6 loop utama jika diperlukan program berjalan kontinyu
    while(true)
    {
        //membandingkan range 0-30
        struct stat statEnak;
        stat(pathEnak.c_str(), &statEnak);
        time_t timeProgram = time(NULL);
        time_t timeEnak = statEnak.st_atime;

        //jika perbedaan detik program dengan detik akses terakhir file <= 30
        if (difftime(timeProgram,timeEnak)<=30) {
            //generate file diet
            string pathSehat="/home/rak/Documents/makanan/makan_sehat";
            pathSehat += to_string(inc) + ".txt";
            open((char*)pathSehat.c_str(),O_CREAT,0777);
            inc++;
        }
        //setiap 5 detik delay = berjalan setiap 5 detik
        sleep(5);
    }
    exit(EXIT_SUCCESS);
}
```
Penjelasan:
```cpp
//source code here
```
sub
```cpp
//source code here
```
sub
```cpp
//source code here
```
sub

---

## NO 5

Kerjakan poin a(1) dan b(2) di bawah:  
1. Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log  
Ket:  
    * Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]
    * Per menit memasukkan log#.log ke dalam folder tersebut
    * ‘#’ : increment per menit. Mulai dari 1
2. Buatlah program c untuk menghentikan program di atas.

NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.


**Jawaban**

1. Buat program dibawah ini dengan nama file soal5a.cpp di folder yang ditentukan.

```cpp
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
    string pathName="/home/rak/log/";
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
```
Penjelasan:
```cpp
//source code here
```
sub
```cpp
//source code here
```
sub
```cpp
//source code here
```
sub

2. Buat program dibawah ini dengan nama file soal5b.cpp di folder yang ditentukan.

```cpp
#include<stdio.h>
#include<unistd.h>

int main(){
    char* hapus[]={(char*)"killall", (char*)"soal5a", NULL};
    execv("/usr/bin/killall", hapus);
    return 0;
}
```
Penjelasan:
```cpp
//source code here
```
sub
```cpp
//source code here
```
sub
```cpp
//source code here
```
sub

---

## Referensi

manual from terminal ubuntu1804  
http://stackoverflow.com/questions/1442116/ddg#1442131  
http://stackoverflow.com/questions/347949/ddg#347959  
https://www.programmingwithbasics.com/2016/03/write-c-program-for-count-character-in.html  
https://www.unix.com/programming/24159-how-check-if-directory-file-exist-using-c-c.html  
http://stackoverflow.com/questions/7663709/ddg#7664227  
https://codereview.stackexchange.com/questions/5089/adding-time-to-struct-tm  
https://stackoverflow.com/questions/25198242/comparing-time-t-values-using-comparison-operators  
http://stackoverflow.com/questions/40504281/ddg#40504396  
https://superuser.com/questions/464290/why-is-cat-not-changing-the-access-time  
https://www.unixtutorial.org/atime-ctime-mtime-in-unix-filesystems  
https://stackoverflow.com/questions/28490290/how-can-i-pass-the-redirection-operator-as-an-argument-for-execv/28490398#28490398  
https://stackoverflow.com/questions/20858381/what-does-bash-c-do  
https://www.geeksforgeeks.org/dup-dup2-linux-system-call/  
https://www.geeksforgeeks.org/pipe-system-call/  
https://stackoverflow.com/questions/7328327/how-to-get-files-owner-name-in-linux-using-c  

