#include<iostream>
#include<cstring>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<utime.h>

using namespace std;

int main()
{
    string a="/home/rak/SoalShift_modul2_F03/soal4/Documents/makanan/makan_enak.txt";
    /*
            Membuka file diharapkan meng-update waktu akses pada file.
            Namun, kenyataannya tidak demikian. Melalui man inode, untuk
            meng-update waktu akses pada file dapat dilakukan oleh command
            salah satunya utime. Dengan menitikkberatkan dari hal tersebut,
            maka membuka diartikan mengubah waktu akses pada file.
    */
    //utime melakukan pengubahan last access time menjadi waktu saat itu
    utime((char*)a.c_str(),NULL);
    return 0;
}