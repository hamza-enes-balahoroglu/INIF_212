#include <ncurses/curses.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    char isim[50];
    int notu;
    char harf_notu[3];
} Ogrenci;

// Global değişkenler (Örnek kolaylığı için)
Ogrenci *liste;
int ogrenci_sayisi = 0;

void harf_notu_hesapla(int not, char *harf) {
    if (not >= 90) strcpy(harf, "AA");
    else if (not >= 80) strcpy(harf, "BA");
    else if (not >= 70) strcpy(harf, "BB");
    else if (not >= 60) strcpy(harf, "CB");
    else if (not >= 50) strcpy(harf, "CC");
    else if (not >= 40) strcpy(harf, "DC");
    else strcpy(harf, "FF");
}

void ana_menu_ciz() {
    clear();
    attron(A_BOLD | A_REVERSE);
    mvprintw(0, 0, " === OGRENCI NOT SISTEMI === ");
    attroff(A_BOLD | A_REVERSE);
    
    mvprintw(2, 2, "1. Tum Ogrenci Notlarini Listele");
    mvprintw(3, 2, "2. Sinif Ortalamasini Hesapla");
    mvprintw(4, 2, "3. En Yuksek ve En Dusuk Notu Bul");
    mvprintw(5, 2, "4. Gecen/But/Kalan Sayisini Goster");
    mvprintw(6, 2, "5. Harf Notlarini Listele");
    mvprintw(7, 2, "6. Basariya Gore Sirala (Azalan)");
    mvprintw(8, 2, "7. Harf Notu Dagilimini Say");
    mvprintw(9, 2, "8. Standart Sapma Hesapla");
    mvprintw(10, 2, "9. Cikis");
    mvprintw(12, 2, "Seciminiz: ");
    refresh();
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // --- KURULUM SAYFASI ---
    mvprintw(2, 2, "Sisteme kac ogrenci eklenecek?: ");
    echo();
    scanw("%d", &ogrenci_sayisi);
    noecho();

    liste = (Ogrenci *)malloc(ogrenci_sayisi * sizeof(Ogrenci));

    for(int i = 0; i < ogrenci_sayisi; i++) {
        clear();
        mvprintw(2, 2, "%d. Ogrenci Ismi: ", i + 1);
        echo();
        getnstr(liste[i].isim, 49);
        mvprintw(3, 2, "%d. Ogrenci Notu: ", i + 1);
        scanw("%d", &liste[i].notu);
        noecho();
        harf_notu_hesapla(liste[i].notu, liste[i].harf_notu);
    }

    // --- ANA DONGU ---
    int secim;
    while(1) {
        ana_menu_ciz();
        secim = getch();

        if (secim == '9') break;

        clear();
        switch(secim) {
            case '1':
                mvprintw(1, 2, "--- OGRENCI LISTESI ---");
                for(int i=0; i<ogrenci_sayisi; i++)
                    mvprintw(3+i, 2, "%s: %d", liste[i].isim, liste[i].notu);
                break;
                
            case '2':
                float toplam = 0;
                for(int i=0; i<ogrenci_sayisi; i++) toplam += liste[i].notu;
                mvprintw(3, 2, "Sinif Ortalamasi: %.2f", toplam / ogrenci_sayisi);
                break;

            case '8': // Standart Sapma
                float sum = 0, mean, sd = 0;
                for(int i=0; i<ogrenci_sayisi; i++) sum += liste[i].notu;
                mean = sum / ogrenci_sayisi;
                for(int i=0; i<ogrenci_sayisi; i++) sd += pow(liste[i].notu - mean, 2);
                mvprintw(3, 2, "Standart Sapma: %.2f", sqrt(sd / ogrenci_sayisi));
                break;

            // Diger caseleri (3,4,5,6,7) senin mantigina gore doldurabiliriz.
            default:
                mvprintw(3, 2, "Gecersiz secim! Bir tusa basin.");
                break;
        }
        
        mvprintw(ogrenci_sayisi + 5, 2, "Menuye donmek icin bir tusa basin...");
        refresh();
        getch();
    }

    free(liste);
    endwin();
    return 0;
}