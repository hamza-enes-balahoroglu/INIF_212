#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "240102002088_headers.h"

int main()
{
    int choice;
    while (1)
    {
        printf("\n========== ANAMENU ==========\n");
        printf("1. Musteri Ekle\n");
        printf("2. Urun Ekle\n");
        printf("3. Depo Ekle\n");
        printf("4. Musterileri Listele\n");
        printf("5. Urunleri Listele\n");
        printf("6. Depolari Listele\n");
        printf("7. Siparis Olustur\n");
        printf("8. Cikis\n");

        choice = Get_Int("Seciminiz: ");

        switch (choice)
        {
        case 1:
            Handle_Add_Customer();
            break;
        case 2:
            Handle_Add_Product();
            break;
        case 3:
            Handle_Add_Storehouse();
            break;
        case 4:
            View_All_Customers();
            break;
        case 5:
            View_All_Products();
            break;
        case 6:
            View_All_Storehouses();
            break;
        case 7:
            Analyze_Purchase();
            break;
        case 8:
            Quit_System();
            return 0;
        default:
            printf("Gecersiz secim!\n");
        }
    }
    return 0;
}
