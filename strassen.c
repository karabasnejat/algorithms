#include <stdio.h>
#include <stdlib.h>


// matrisOlustur fonksiyonu ile bir matris oluşturuyoruz
int **matrisOlustur(int satir, int sutun) {
    // satir sayısı kadar int pointerları için bellek ayırıyoruz
    int **matris = (int **)malloc(satir * sizeof(int *));
    // Her satır için döngü ile bellek ayırıyoruz
    for (int i = 0; i < satir; i++) {
        // Her satır için sutun sayısı kadar int bellek ayırıyoruz
        matris[i] = (int *)malloc(sutun * sizeof(int));
    }
    // Oluşturduğumuz matrisi geri döndürüyoruz
    return matris;
}

// matrisSerbestBirak fonksiyonu ile bir matrisi serbest bırakıyoruz
void matrisSerbestBirak(int **matris, int satir) {
    // Her satır için döngü ile belleği serbest bırakıyoruz
    for (int i = 0; i < satir; i++) {
        // Her bir satırın bellek alanını serbest bırakıyoruz
        free(matris[i]);
    }
    // Tüm matris için ayrılmış bellek alanını serbest bırakıyoruz
    free(matris);
}
// Row-wise Cache-Friendly Multiplication fonksiyonu

void matrisCarp(int **A, int **B, int **C, int m, int n, int l) {
    // m satır sayısı kadar dış döngüyü başlatıyoruz
    for (int i = 0; i < m; i++) {
        // n sütun sayısı kadar orta döngüyü başlatıyoruz
        for (int k = 0; k < n; k++) {
            // l sütun sayısı kadar iç döngüyü başlatıyoruz
            for (int j = 0; j < l; j++) {
                // C[i][j] elemanına A[i][k] ve B[k][j] çarpımını ekliyoruz
                // Bu, A matrisinin bir satırını ve B matrisinin bir sütununu dikkate alarak
                // hesaplama yapar. Böylece önbellek dostu bir erişim şekli sağlarız
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}
/* örneğin eğer satır "Row-wise Cache-Friendly Multiplication" kullanmasaydık, kullanmamız gereken matrix çarpma fonksiyonu şu
şekilde olabilirdi:

// Standart matris çarpma fonksiyonu
void matrisCarp(int **A, int **B, int **C, int m, int n, int l) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < l; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

*/
// Strassen algoritması için yardımcı fonksiyonlar
void matrisTopla(int **A, int **B, int **C, int boyut) {
    for (int i = 0; i < boyut; i++) {
        for (int j = 0; j < boyut; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void matrisCikar(int **A, int **B, int **C, int boyut) {
    for (int i = 0; i < boyut; i++) {
        for (int j = 0; j < boyut; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

// Strassen algoritması
void strassen(int **A, int **B, int **C, int boyut) {
    if (boyut == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int yeniBoyut = boyut / 2;
    int **A11 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **A12 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **A21 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **A22 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **B11 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **B12 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **B21 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **B22 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **C11 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **C12 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **C21 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **C22 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **M1 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **M2 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **M3 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **M4 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **M5 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **M6 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **M7 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **temp1 = matrisOlustur(yeniBoyut, yeniBoyut);
    int **temp2 = matrisOlustur(yeniBoyut, yeniBoyut);

    for (int i = 0; i < yeniBoyut; i++) {
        for (int j = 0; j < yeniBoyut; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + yeniBoyut];
            A21[i][j] = A[i + yeniBoyut][j];
            A22[i][j] = A[i + yeniBoyut][j + yeniBoyut];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + yeniBoyut];
            B21[i][j] = B[i + yeniBoyut][j];
            B22[i][j] = B[i + yeniBoyut][j + yeniBoyut];
        }
    }

    // M1 = (A11 + A22) * (B11 + B22)
    matrisTopla(A11, A22, temp1, yeniBoyut);
    matrisTopla(B11, B22, temp2, yeniBoyut);
    strassen(temp1, temp2, M1, yeniBoyut);

    // M2 = (A21 + A22) * B11
    matrisTopla(A21, A22, temp1, yeniBoyut);
    strassen(temp1, B11, M2, yeniBoyut);

    // M3 = A11 * (B12 - B22)
    matrisCikar(B12, B22, temp1, yeniBoyut);
    strassen(A11, temp1, M3, yeniBoyut);

    // M4 = A22 * (B21 - B11)
    matrisCikar(B21, B11, temp1, yeniBoyut);
    strassen(A22, temp1, M4, yeniBoyut);

    // M5 = (A11 + A12) * B22
    matrisTopla(A11, A12, temp1, yeniBoyut);
    strassen(temp1, B22, M5, yeniBoyut);

    // M6 = (A21 - A11) * (B11 + B12)
    matrisCikar(A21, A11, temp1, yeniBoyut);
    matrisTopla(B11, B12, temp2, yeniBoyut);
    strassen(temp1, temp2, M6, yeniBoyut);

    // M7 = (A12 - A22) * (B21 + B22)
    matrisCikar(A12, A22, temp1, yeniBoyut);
    matrisTopla(B21, B22, temp2, yeniBoyut);
    strassen(temp1, temp2, M7, yeniBoyut);

    // C11 = M1 + M4 - M5 + M7
    matrisTopla(M1, M4, temp1, yeniBoyut);
    matrisCikar(temp1, M5, temp2, yeniBoyut);
    matrisTopla(temp2, M7, C11, yeniBoyut);

    // C12 = M3 + M5
    matrisTopla(M3, M5, C12, yeniBoyut);

    // C21 = M2 + M4
    matrisTopla(M2, M4, C21, yeniBoyut);

    // C22 = M1 - M2 + M3 + M6
    matrisCikar(M1, M2, temp1, yeniBoyut);
    matrisTopla(temp1, M3, temp2, yeniBoyut);
    matrisTopla(temp2, M6, C22, yeniBoyut);

    for (int i = 0; i < yeniBoyut; i++) {
        for (int j = 0; j < yeniBoyut; j++) {
            C[i][j] = C11[i][j];
            C[i][j + yeniBoyut] = C12[i][j];
            C[i + yeniBoyut][j] = C21[i][j];
            C[i + yeniBoyut][j + yeniBoyut] = C22[i][j];
        }
    }

    matrisSerbestBirak(A11, yeniBoyut);
    matrisSerbestBirak(A12, yeniBoyut);
    matrisSerbestBirak(A21, yeniBoyut);
    matrisSerbestBirak(A22, yeniBoyut);
    matrisSerbestBirak(B11, yeniBoyut);
    matrisSerbestBirak(B12, yeniBoyut);
    matrisSerbestBirak(B21, yeniBoyut);
    matrisSerbestBirak(B22, yeniBoyut);
    matrisSerbestBirak(C11, yeniBoyut);
    matrisSerbestBirak(C12, yeniBoyut);
    matrisSerbestBirak(C21, yeniBoyut);
    matrisSerbestBirak(C22, yeniBoyut);
    matrisSerbestBirak(M1, yeniBoyut);
    matrisSerbestBirak(M2, yeniBoyut);
    matrisSerbestBirak(M3, yeniBoyut);
    matrisSerbestBirak(M4, yeniBoyut);
    matrisSerbestBirak(M5, yeniBoyut);
    matrisSerbestBirak(M6, yeniBoyut);
    matrisSerbestBirak(M7, yeniBoyut);
    matrisSerbestBirak(temp1, yeniBoyut);
    matrisSerbestBirak(temp2, yeniBoyut);
}

// Ana program
int main() {
    /*
    şimdi fonksiyonlarımızı test edebiliriz
    fonksiyonlarımızı test etmek amaçlı ve sonuçlarını ekranda rahat görebilmek için
    3x3 matris oluşturalım. ardından bunu kendi fonksiyonlarımız ile deneyip doğruluğunu test edelim
    
    
    */
    // matris boyutu belirleyelim
    int m = 3; // İlk matrisin satır sayısı
    int n = 3; // İlk matrisin sütun sayısı ve ikinci matrisin satır sayısı
    int l = 3; // İkinci matrisin sütun sayısı
    
    

    int **A = matrisOlustur(m, n);
    int **B = matrisOlustur(n, l);
    int **C = matrisOlustur(m, l);

    // A matrisini doldur
    A[0][0] = 1; A[0][1] = 2; A[0][2] = 3;
    A[1][0] = 4; A[1][1] = 5; A[1][2] = 6;
    A[2][0] = 7; A[2][1] = 8; A[2][2] = 9;

    // B matrisini doldur
    B[0][0] = 9; B[0][1] = 8; B[0][2] = 7;
    B[1][0] = 6; B[1][1] = 5; B[1][2] = 4;
    B[2][0] = 3; B[2][1] = 2; B[2][2] = 1;

    // Matris çarpımını gerçekleştir
    if (m == n && n == l && (m & (m - 1)) == 0) {
        strassen(A, B, C, m); // Strassen algoritması kullan
    } else {
        matrisCarp(A, B, C, m, n, l); // Standart matris çarpımı kullan
    }

   
   for (int i = 0; i < m; i++) {
        for (int j = 0; j < l; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    // Matrisleri serbest bırak
    matrisSerbestBirak(A, m);
    matrisSerbestBirak(B, n);
    matrisSerbestBirak(C, m);
    // beklenen çıktı aşağıdaki gibi olmalı
    /*
    30,24,18,
    84,69,54,
    138,114,90
    */
    // bundan sorna artık kodumuzu normal bir şekilde çalıştırabiliriz.
    
    int yeni_m = 512; // İlk matrisin satır sayısı
    int yeni_n = 512; // İlk matrisin sütun sayısı ve ikinci matrisin satır sayısı
    int yeni_l = 512; // İkinci matrisin sütun sayısı
    // matrislerin boyutlarını kontrol edebiliriz.
    if((yeni_l > 1000 || yeni_l < 100) || (yeni_m > 1000 || yeni_m < 100) || (yeni_n > 1000 || yeni_n < 100)) {
        printf("Geçersiz matris boyutlari !");
        return 0;
    }

    int **yeni_A = matrisOlustur(yeni_m, yeni_n);
    int **yeni_B = matrisOlustur(yeni_n, yeni_l);
    int **yeni_C = matrisOlustur(yeni_m, yeni_l);

       // Matrisleri doldur (örnek olarak rastgele değerler atanıyor)
    for (int i = 0; i < yeni_m; i++) {
        for (int j = 0; j < yeni_n; j++) {
            yeni_A[i][j] = rand() % 100;
        }
    }

    for (int i = 0; i < yeni_n; i++) {
        for (int j = 0; j < yeni_l; j++) {
            yeni_B[i][j] = rand() % 100;
        }
    }

    // Matris çarpımını gerçekleştir
    if (yeni_m == yeni_n && yeni_n == yeni_l && (yeni_m & (yeni_m - 1)) == 0) {
        strassen(yeni_A, yeni_B, yeni_C, yeni_m); // Strassen algoritması kullan
    } else {
        matrisCarp(yeni_A, yeni_B, yeni_C, yeni_m, yeni_n, yeni_l); // Standart matris çarpımı kullan
    }
    // kodun çok kalabalık olmaması için 10 satırı bastırsak yeterli olabilir

    int max_satir = 10;
    int satir_sayisi = (yeni_m < max_satir) ? yeni_m : max_satir;
    for (int i = 0; i < satir_sayisi; i++) {
        for (int j = 0; j < satir_sayisi; j++) {
            printf("%d ", yeni_C[i][j]);
        }
        printf("\n");
    }

    // Matrisleri serbest bırak
    matrisSerbestBirak(yeni_A, yeni_m);
    matrisSerbestBirak(yeni_B, yeni_n);
    matrisSerbestBirak(yeni_C, yeni_m);

    return 0;
}
