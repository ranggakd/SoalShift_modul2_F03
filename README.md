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

1. 

---
## NO 2

Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.  

Catatan: Tidak boleh menggunakan crontab


**Jawaban**

1.

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

1. 

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

1. 

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

1. 

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

