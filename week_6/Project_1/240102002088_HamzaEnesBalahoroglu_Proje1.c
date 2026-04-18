#include <stdio.h>
#include <math.h>
//#include "ncurses/curses.h"


int GuvenliSayiAl();
void SkorlariGir(int ids[], int midterms[], int finals[], int quizzes[], float success[], int n);
void SkorlariListele(int ids[], int midterms[], int finals[], int quizzes[], float success[], int n);
float OrtalamaHesapla(float success[], int n);
float EnYuksekBul(float success[], int n);
float EnDusukBul(float success[], int n);
void DurumSayisi(float success[], int n);
void HarfNotlariniListele(float success[], int n);
void OgrencileriSirala(int ids[], int midterms[], int finals[], int quizzes[], float success[], int n);
void HarfNotuDagilimi(float success[], int n);
float StandartSapmaHesapla(float success[], int n, float ortalama);

int main() 
{
    int n;

    printf("Ogrenci sayisini giriniz: ");
    scanf("%d", &n);

    int ids[n], midterms[n], finals[n], quizzes[n];
    float successScores[n];

    int secim;
    int veriGirildi = 0;

    do {
        printf("\n--- STUDENT PERFORMANCE EVALUATION SYSTEM ---\n");
        printf("1. Ogrenci Skorlarini Gir\n");
        printf("2. Tum Skorlari Listele\n");
        printf("3. Sinif Ortalamasini Hesapla\n");
        printf("4. En Yuksek ve En Dusuk Skorlari Bul\n");
        printf("5. Gecen/Butunleme/Kalan Sayisini Goster\n");
        printf("6. Ogrenci Harf Notlarini Listele\n");
        printf("7. Basari Puanina Gore Sirala (Azalan)\n");
        printf("8. Harf Notu Dagilimini Say\n");
        printf("9. Standart Sapma Hesapla\n");
        printf("0. Cikis\n");
        printf("Seciminiz: ");

        scanf("%d", &secim);

        if (secim != 1 && secim != 0 && veriGirildi == 0) {
            printf("Once veri girisi yapmalisiniz (Secenek 1).\n");
            continue;
        }

        switch (secim) {
            case 1:
                SkorlariGir(ids, midterms, finals, quizzes, successScores, n);
                veriGirildi = 1;
                break;
            case 2:
                SkorlariListele(ids, midterms, finals, quizzes, successScores, n);
                break;
            case 3:
                printf("Sinif Ortalamasi: %.2f\n", OrtalamaHesapla(successScores, n));
                break;
            case 4:
                printf("En Yuksek Skor: %.2f\n", EnYuksekBul(successScores, n));
                printf("En Dusuk Skor: %.2f\n", EnDusukBul(successScores, n));
                break;
            case 5:
                DurumSayisi(successScores, n);
                break;
            case 6:
                HarfNotlariniListele(successScores, n);
                break;
            case 7:
                OgrencileriSirala(ids, midterms, finals, quizzes, successScores, n);;
                break;
            case 8:
                HarfNotuDagilimi(successScores, n);
                break;
            case 9:
                printf("Standart Sapma: %.2f\n", 
                    StandartSapmaHesapla(successScores, n, OrtalamaHesapla(successScores, n)));
                break;
            case 0:
                printf("Programdan cikiliyor...\n");
                break;
            default:
                printf("Gecersiz secim!\n");
        }
    } while (secim != 0);

    return 0;
}

int GuvenliSayiAl() 
{
    int sayi;
    while (scanf("%d", &sayi) != 1) {
        while (getchar() != '\n'); 
        printf("Hata! Gecersiz karakter. Lutfen bir tam sayi giriniz: ");
    }
    return sayi;
}

void SkorlariGir(int ids[], int midterms[], int finals[], int quizzes[], float success[], int n) 
{
    for (int i = 0; i < n; i++) {
        printf("\n%d. Ogrenci ID: ", i + 1);
        ids[i] = GuvenliSayiAl();
        
        do {
            printf("Midterm (0-100): ");
            midterms[i] = GuvenliSayiAl();
            if(midterms[i] < 0 || midterms[i] > 100) 
                printf("Gecersiz aralik! \n");
        } while (midterms[i] < 0 || midterms[i] > 100);

        do {
            printf("Final (0-100): ");
            finals[i] = GuvenliSayiAl();
            if(finals[i] < 0 || finals[i] > 100) 
                printf("Gecersiz aralik! \n");
        } while (finals[i] < 0 || finals[i] > 100);

        do {
            printf("Quiz (0-100): ");
            quizzes[i] = GuvenliSayiAl();
            if(quizzes[i] < 0 || quizzes[i] > 100) 
                printf("Gecersiz aralik! \n");
        } while (quizzes[i] < 0 || quizzes[i] > 100);

        // 0.3*Mid + 0.5*Fin + 0.2*Quiz
        success[i] = (midterms[i] * 0.3) + (finals[i] * 0.5) + (quizzes[i] * 0.2);
    }
}

void SkorlariListele(int ids[], int midterms[], int finals[], int quizzes[], float success[], int n) 
{
    printf("\nID\tMid\tFin\tQuiz\tSuccess\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%.2f\n", ids[i], midterms[i], finals[i], quizzes[i], success[i]);
    }
}

float OrtalamaHesapla(float success[], int n) 
{
    float toplam = 0;
    for (int i = 0; i < n; i++) toplam += success[i];
    return toplam / n;
}

float EnYuksekBul(float success[], int n) 
{
    float max = success[0];
    for (int i = 1; i < n; i++) if (success[i] > max) max = success[i];
    return max;
}

float EnDusukBul(float success[], int n) 
{
    float min = success[0];
    for (int i = 1; i < n; i++) if (success[i] < min) min = success[i];
    return min;
}

void DurumSayisi(float success[], int n) 
{
    int gecen = 0, but = 0, kalan = 0;
    for (int i = 0; i < n; i++) {
        if (success[i] >= 60) gecen++;
        else if (success[i] >= 50) but++;
        else kalan++;
    }
    printf("Gecen: %d, Butunleme: %d, Kalan: %d\n", gecen, but, kalan);
}

void HarfNotlariniListele(float success[], int n) 
{
    for (int i = 0; i < n; i++) {
        printf("Ogrenci %d Basari Puani: %.2f - Harf Notu: ", i + 1, success[i]);
        if (success[i] >= 90) printf("AA\n");
        else if (success[i] >= 85) printf("BA\n");
        else if (success[i] >= 80) printf("BB\n");
        else if (success[i] >= 70) printf("CB\n");
        else if (success[i] >= 60) printf("CC\n");
        else if (success[i] >= 50) printf("DC\n");
        else printf("FF\n");
    }
}

void OgrencileriSirala(int ids[], int midterms[], int finals[], int quizzes[], float success[], int n) 
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (success[i] < success[j]) {
                
                float tempS = success[i];
                success[i] = success[j];
                success[j] = tempS;

                int tempID = ids[i];
                ids[i] = ids[j];
                ids[j] = tempID;

                int tempMid = midterms[i];
                midterms[i] = midterms[j];
                midterms[j] = tempMid;

                int tempFin = finals[i];
                finals[i] = finals[j];
                finals[j] = tempFin;

                int tempQuiz = quizzes[i];
                quizzes[i] = quizzes[j];
                quizzes[j] = tempQuiz;
            }
        }
    }
    printf("\nTum ogrenci verileri basari puanina gore (en yuksekten en dusuge) siralandi.\n");
    printf("Listelemek icin menuden 2. secenegi kullanabilirsiniz.\n");
}

void HarfNotuDagilimi(float success[], int n) 
{
    int aa=0, ba=0, bb=0, cb=0, cc=0, dc=0, ff=0;
    for (int i = 0; i < n; i++) {
        if (success[i] >= 90) aa++;
        else if (success[i] >= 85) ba++;
        else if (success[i] >= 80) bb++;
        else if (success[i] >= 70) cb++;
        else if (success[i] >= 60) cc++;
        else if (success[i] >= 50) dc++;
        else ff++;
    }
    printf("AA: %d, BA: %d, BB: %d, CB: %d, CC: %d, DC: %d, FF: %d\n", aa, ba, bb, cb, cc, dc, ff);
}

float StandartSapmaHesapla(float success[], int n, float ortalama) 
{
    float toplamFarkKare = 0;
    for (int i = 0; i < n; i++) {
        toplamFarkKare += pow(success[i] - ortalama, 2);
    }
    return sqrt(toplamFarkKare / n);
}
