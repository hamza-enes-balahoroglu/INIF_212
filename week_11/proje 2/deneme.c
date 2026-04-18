#include <stdio.h>
#include <stdlib.h>

/* Kendi mutlak değer fonksiyonumuz (Yerleşik fonksiyon kısıtlamasına uymak için) */
int mutlak_deger(int x) {
    return (x < 0) ? -x : x;
}

/* Dinamik olarak 8x8 oyun tahtası oluşturur ve bellek adresini döndürür */
int** tahta_olustur() {
    int **tahta = (int **)malloc(8 * sizeof(int *));
    for (int i = 0; i < 8; i++) {
        tahta[i] = (int *)malloc(8 * sizeof(int));
    }
    return tahta;
}

/* Tahtayı başlangıç kurallarına göre doldurur */
void tahtayi_baslat(int **tahta) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            tahta[i][j] = 0; /* Boş hücreler 0 */
            
            /* Siyah kareler üzerine yerleşim ((i+j) tek sayı ise) */
            if ((i + j) % 2 != 0) {
                if (i < 3) {
                    tahta[i][j] = 1; /* İlk 3 satır Player 1 */
                } else if (i > 4) {
                    tahta[i][j] = 2; /* Son 3 satır Player 2 */
                }
            }
        }
    }
}

/* Tahtanın güncel halini ekrana basar */
void tahtayi_yazdir(int **tahta) {
    printf("\n  0 1 2 3 4 5 6 7 (Sutunlar)\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", i);
        for (int j = 0; j < 8; j++) {
            if (tahta[i][j] == 0) printf(". ");
            else printf("%d ", tahta[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/* Hamlenin kurallara uygun olup olmadığını denetler */
int gecerli_hamle_mi(int **tahta, int oyuncu, int basR, int basC, int bitR, int bitC) {
    /* 1. Sınır kontrolü */
    if (bitR < 0 || bitR > 7 || bitC < 0 || bitC > 7) return 0;
    
    /* 2. Hedef hücre boş mu? */
    if (tahta[bitR][bitC] != 0) return 0;
    
    /* 3. Doğru taşı mı oynatıyor? */
    if (tahta[basR][basC] != oyuncu) return 0;

    int satir_farki = bitR - basR;
    int sutun_farki = bitC - basC;

    /* 4. Sadece ileri hareket kontrolü (Geriye hareket yasak) */
    if (oyuncu == 1 && satir_farki <= 0) return 0; /* Player 1 aşağı (+ yönde) gitmeli */
    if (oyuncu == 2 && satir_farki >= 0) return 0; /* Player 2 yukarı (- yönde) gitmeli */

    /* 5. Çapraz hareket kontrolü */
    if (mutlak_deger(satir_farki) != mutlak_deger(sutun_farki)) return 0;

    /* 6. Adım ve Taş Yeme Kontrolü */
    if (mutlak_deger(satir_farki) == 1) {
        return 1; /* Normal 1 birimlik adım */
    } 
    else if (mutlak_deger(satir_farki) == 2) {
        /* Üzerinden atlanan (ortadaki) taşı bul */
        int ortaR = (basR + bitR) / 2;
        int ortaC = (basC + bitC) / 2;
        int rakip = (oyuncu == 1) ? 2 : 1;
        
        if (tahta[ortaR][ortaC] == rakip) {
            return 1; /* Rakibin taşı üzerinden atlanıyor, geçerli */
        }
    }
    
    return 0; /* Diğer tüm senaryolar geçersiz */
}

/* Geçerli hamleyi tahta üzerinde uygular ve gerekirse yenen taşı siler */
void hamle_yap(int **tahta, int basR, int basC, int bitR, int bitC) {
    tahta[bitR][bitC] = tahta[basR][basC];
    tahta[basR][basC] = 0;

    /* Eğer 2 birimlik bir zıplama yapıldıysa ortadaki taşı sil */
    if (mutlak_deger(basR - bitR) == 2) {
        int ortaR = (basR + bitR) / 2;
        int ortaC = (basC + bitC) / 2;
        tahta[ortaR][ortaC] = 0;
    }
}

/* Kazanma durumunu kontrol eder. Oyunculardan birinin taşı bittiyse oyunu bitirir. */
int kazanan_kontrol(int **tahta) {
    int p1_tas = 0, p2_tas = 0;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (tahta[i][j] == 1) p1_tas++;
            if (tahta[i][j] == 2) p2_tas++;
        }
    }
    
    if (p1_tas == 0) return 2; /* Player 1'in taşı kalmadı, Player 2 kazandı */
    if (p2_tas == 0) return 1; /* Player 2'nin taşı kalmadı, Player 1 kazandı */
    
    return 0; /* Oyun devam ediyor */
}

/* Dinamik olarak ayrılan belleği işletim sistemine geri iade eder */
void tahta_temizle(int **tahta) {
    for (int i = 0; i < 8; i++) {
        free(tahta[i]);
    }
    free(tahta);
}

int main() {
    int **oyun_tahtasi = tahta_olustur();
    tahtayi_baslat(oyun_tahtasi);
    
    int aktif_oyuncu = 1;
    int durum = 0;
    
    printf("--- DAMA OYUNU ---\n");
    printf("Player 1: 1 (Asagi hareket eder)\n");
    printf("Player 2: 2 (Yukari hareket eder)\n\n");

    while ((durum = kazanan_kontrol(oyun_tahtasi)) == 0) {
        tahtayi_yazdir(oyun_tahtasi);
        
        int basR, basC, bitR, bitC;
        printf("Sira Player %d'de.\n", aktif_oyuncu);
        printf("Hareket ettirilecek tasin koordinati (Satir Sutun): ");
        scanf("%d %d", &basR, &basC);
        printf("Hedef koordinat (Satir Sutun): ");
        scanf("%d %d", &bitR, &bitC);
        
        if (gecerli_hamle_mi(oyun_tahtasi, aktif_oyuncu, basR, basC, bitR, bitC)) {
            hamle_yap(oyun_tahtasi, basR, basC, bitR, bitC);
            
            /* Turu diğer oyuncuya geçir */
            aktif_oyuncu = (aktif_oyuncu == 1) ? 2 : 1;
        } else {
            printf("\n[HATA] Gecersiz hamle! Lutfen kurallari ve sinirlari kontrol edip tekrar deneyin.\n");
        }
    }
    
    tahtayi_yazdir(oyun_tahtasi);
    printf("OYUN BITTI! KAZANAN: Player %d\n", durum);
    
    /* Program sonunda heap alanını serbest bırak */
    tahta_temizle(oyun_tahtasi);
    
    return 0;
}