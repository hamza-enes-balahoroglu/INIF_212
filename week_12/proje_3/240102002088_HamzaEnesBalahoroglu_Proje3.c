#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//________ Local Macros ________

//________ Local Structures ________

typedef int (*ActionFunc)(void *self, void *arg);

typedef struct
{
    ActionFunc present;
    ActionFunc kill;
} EntityHeader;

typedef struct
{
    const EntityHeader *base;
} GenericObject;

typedef struct
{
    const EntityHeader *base;
    float x;
    float y;
} Location;

typedef struct
{
    const EntityHeader *base;
    char *chr;
    size_t count;
} DString;

typedef struct Customer
{
    const EntityHeader *base;
    struct Customer *next;
    Location *location;
    DString *name; // Structure padding
    int id;
} Customer;

typedef struct Product
{
    const EntityHeader *base;
    struct Product *next;
    DString *name;
    float price;
    int quantity;
    int id;
} Product;

typedef struct Storehouse
{
    const EntityHeader *base;
    struct Storehouse *next;
    Location *location;
    int id;
} Storehouse;

//________ Local Prototypes ________

DString *DSTR_Create(char *char_list, int count);
DString *DSTR_GetLine();
int DSTR_Present(void *self, void *arg);
int DSTR_Kill(void *self, void *arg);

Location *LOC_Create(int x, int y);
float LOC_CalculateDistance(Location *l1, Location *l2);
int LOC_Present(void *self, void *arg);

Customer *CUS_Create(int id, DString *name, int x, int y);
int CUS_Present(void *self, void *arg);
int CUS_Kill(void *self, void *arg);

Storehouse *STORE_Create(int id, int x, int y);
int STORE_Kill(void *self, void *arg);
int STORE_Present(void *self, void *arg);

Product *PROD_Create(int id, DString *name, float price, int quantity);
int PROD_Kill(void *self, void *arg);
int PROD_Present(void *self, void *arg);

int Object_Kill(void *self);
int Object_Present(void *self);
int General_Free(void *self, void *arg);

void Add_To_Customer_List(Customer *new_cus);
void Add_To_Product_List(Product *new_prod);
void Add_To_Store_List(Storehouse *new_store);

void Handle_Add_Customer(void);
void Handle_Add_Product(void);
void Handle_Add_Storehouse(void);

void View_All_Customers(void);
void View_All_Products(void);
void View_All_Storehouses(void);

void Quit_System();

void Analyze_Purchase();

int Get_Int(const char *prompt);
int Object_Present(void *self);
void Destroy_Object(void *self);

//__________ Constant Variables ___________

const EntityHeader Customer_Functions = {
    .kill = CUS_Kill,
    .present = CUS_Present};

const EntityHeader Location_Functions = {
    .kill = General_Free,
    .present = LOC_Present};

const EntityHeader DString_Functions = {
    .kill = DSTR_Kill,
    .present = DSTR_Present};

const EntityHeader Store_Functions = {
    .kill = STORE_Kill,
    .present = STORE_Present};

const EntityHeader Product_Functions = {
    .kill = PROD_Kill,
    .present = PROD_Present};

//_______________ Variables _______________
Customer *customer_list = NULL;
Product *product_list = NULL;
Storehouse *storehouse_list = NULL;

//______________ MAIN _________________
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

//_____________Local Functions ____________

Product *PROD_Create(int id, DString *name, float price, int quantity)
{
    Product *ptr = (Product *)malloc(sizeof(Product));
    if (ptr == NULL)
    {
        return NULL;
    }

    ptr->base = &Product_Functions;
    ptr->next = NULL;
    ptr->id = id;
    ptr->name = name;
    ptr->price = price;
    ptr->quantity = quantity;

    return ptr;
}

int PROD_Kill(void *self, void *arg)
{
    if (self == NULL)
        return 0;

    Product *prod = (Product *)self;

    if (prod->name != NULL && prod->name->base != NULL && prod->name->base->kill != NULL)
    {
        Object_Kill(prod->name);
    }

    free(prod);
    return 1;
}

int PROD_Present(void *self, void *arg)
{
    if (self == NULL)
        return 0;

    Product *prod = (Product *)self;

    printf("ID : %d\t\tUrun: ", prod->id);

    Object_Present(prod->name);

    printf("\tFiyat: %.2f TL\tStok: %d\n", prod->price, prod->quantity);

    return 1;
}

Storehouse *STORE_Create(int id, int x, int y)
{
    Storehouse *store = calloc(1, sizeof(Storehouse));

    if (store == NULL)
    {
        return NULL;
    }

    store->base = &Store_Functions;
    store->id = id;

    Location *temp_loc = LOC_Create(x, y);

    if (temp_loc == NULL)
    {
        Object_Kill(store);
        return NULL;
    }

    store->location = temp_loc;
    store->next = NULL;

    return store;
}

int STORE_Kill(void *self, void *arg)
{
    if (self == NULL)
        return 0;

    Storehouse *store = (Storehouse *)self;

    Object_Kill(store->location);

    free(store);
    return 1;
}

int STORE_Present(void *self, void *arg)
{
    if (self == NULL)
        return 0;

    Storehouse *store = (Storehouse *)self;

    printf("ID : %d\t\t", store->id);
    Object_Present(store->location);

    printf("\n");
    return 1;
}

Location *LOC_Create(int x, int y)
{
    Location *ptr = calloc(1, sizeof(Location));

    if (ptr == NULL)
    {
        return NULL;
    }

    ptr->base = &Location_Functions;
    ptr->x = x;
    ptr->y = y;

    return ptr;
}

Customer *CUS_Create(int id, DString *name, int x, int y)
{
    if (name == NULL)
        return NULL;

    Customer *customer = (Customer *)calloc(1, sizeof(Customer));

    if (customer == NULL)
        return NULL;

    customer->base = &Customer_Functions;
    customer->id = id;
    customer->name = name;

    Location *temp_loc = LOC_Create(x, y);

    if (temp_loc == NULL)
    {
        Object_Kill(customer);
    }

    customer->location = temp_loc;

    customer->next = NULL;
}

int CUS_Kill(void *self, void *arg)
{
    Customer *cus = (Customer *)self;

    Object_Kill(cus->name);
    cus->name = NULL;

    Object_Kill(cus->location);
    cus->location = NULL;

    free(cus);
}

DString *DSTR_Create(char *char_list, int count)
{
    DString *str = (DString *)calloc(1, sizeof(DString));
    if (str == NULL)
        return NULL;

    char *chr = (char *)calloc(count + 1, sizeof(char));

    if (chr == NULL)
    {
        free(str);
        return NULL;
    }

    str->chr = chr;
    str->count = count;
    str->base = &DString_Functions;

    for (int i = 0; i < count; i++)
    {
        str->chr[i] = char_list[i];
    }

    str->chr[count] = '\0';

    return str;
}

int DSTR_Kill(void *self, void *arg)
{
    DString *str = (DString *)self;

    free(str->chr);
    str->chr = NULL;

    free(str);

    return 1;
}

int DSTR_Present(void *self, void *arg)
{
    if (self == NULL)
    {
        return 0;
    }

    DString *str = (DString *)self;

    if (str->chr == NULL)
    {
        return 0;
    }

    printf("%s", str->chr);

    return 1;
}

DString *DSTR_GetLine()
{
    int capacity = 8; // Başlangıç kapasitesi
    int length = 0;
    char *buffer = (char *)malloc(capacity * sizeof(char));
    int ch;

    if (buffer == NULL)
        return NULL;

    while ((ch = getchar()) != '\n' && ch != EOF)
    {

        if (length + 1 >= capacity)
        {
            capacity *= 2;
            char *temp = (char *)realloc(buffer, capacity * sizeof(char));
            if (temp == NULL)
            {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }
        buffer[length++] = (char)ch;
    }

    DString *str = DSTR_Create(buffer, length);

    free(buffer);

    return str;
}
/*
 *  Bu fonksiyonu kullandıktan sonra ptr = NULL; demeyi unutma
 *
 */
int General_Free(void *self, void *arg)
{
    free(self);
}

int Object_Kill(void *self)
{
    if (self == NULL)
        return 0;

    GenericObject *obj = (GenericObject *)self;

    obj->base->kill(obj, NULL);
    return 1;
}

int Object_Present(void *self)
{
    if (self == NULL)
        return 0;

    GenericObject *obj = (GenericObject *)self;

    obj->base->present(obj, NULL);
    return 1;
}

int LOC_Present(void *self, void *arg)
{
    Location *loc = (Location *)self;

    printf("x = %.2f  y = %.2f", loc->x, loc->y);
}

int CUS_Present(void *self, void *arg)
{
    Customer *cus = (Customer *)self;
    Location *loc = (Location *)cus->location;

    printf("ID : %d\t\tAdi: ", cus->id);

    Object_Present(cus->name);

    printf("\t\t");

    Object_Present(cus->location);

    printf("\n");
}

float LOC_CalculateDistance(Location *l1, Location *l2)
{
    return sqrt(pow(l2->x - l1->x, 2) + pow(l2->y - l1->y, 2));
}

int Get_Int(const char *prompt)
{
    int value;
    int status;

    while (1)
    {
        printf("%s", prompt);
        status = scanf("%d", &value);

        if (status == 1)
        {
            while (getchar() != '\n')
                ;
            return value;
        }
        else
        {
            // Harf veya yanlis bir sey girildi.
            printf("Hatali giris! Lutfen sadece sayi girin.\n");
            while (getchar() != '\n')
                ;
        }
    }
}

float Get_Float(const char *prompt)
{
    float value;
    int status;

    while (1)
    {
        printf("%s", prompt);
        status = scanf("%f", &value);

        if (status == 1)
        {
            while (getchar() != '\n')
                ;
            return value;
        }
        else
        {
            printf("Hatali giris! Lutfen sadece sayi (orn: 1.5) girin.\n");
            while (getchar() != '\n')
                ;
        }
    }
}

void Add_To_Customer_List(Customer *new_cus)
{
    new_cus->next = customer_list;
    customer_list = new_cus;
}

void Add_To_Product_List(Product *new_prod)
{
    new_prod->next = product_list;
    product_list = new_prod;
}

void Add_To_Store_List(Storehouse *new_store)
{
    new_store->next = storehouse_list;
    storehouse_list = new_store;
}

void Handle_Add_Customer()
{
    int id = Get_Int("Musteri ID giriniz: ");
    printf("Musteri Ismi giriniz: ");
    DString *name = DSTR_GetLine();
    int x = Get_Float("X Koordinati: ");
    int y = Get_Float("Y Koordinati: ");

    Customer *c = CUS_Create(id, name, x, y);
    Add_To_Customer_List(c);
    printf("Musteri basariyla eklendi!\n");
}

void Handle_Add_Product()
{
    int id = Get_Int("Urun ID giriniz: ");
    printf("Urun Adi giriniz: ");
    DString *name = DSTR_GetLine();
    float price = Get_Float("Birim Fiyat (TL): ");
    int qty = Get_Int("Stok Miktari: ");

    Product *p = PROD_Create(id, name, price, qty);
    Add_To_Product_List(p);
    printf("Urun basariyla eklendi!\n");
}

void Handle_Add_Storehouse()
{
    int id = Get_Int("Depo ID giriniz: ");
    int x = Get_Int("Depo X Koordinati: ");
    int y = Get_Int("Depo Y Koordinati: ");

    Storehouse *s = STORE_Create(id, x, y);
    Add_To_Store_List(s);
    printf("Depo basariyla eklendi!\n");
}

void View_All_Customers()
{
    Customer *curr = customer_list;
    printf("\n--- MUSTERI LISTESI ---\n");
    while (curr != NULL)
    {
        Object_Present(curr);
        curr = curr->next;
    }
}

void View_All_Products()
{
    Product *curr = product_list;
    printf("\n--- URUN LISTESI ---\n");
    while (curr != NULL)
    {
        Object_Present(curr);
        curr = curr->next;
    }
}

void View_All_Storehouses()
{
    Storehouse *curr = storehouse_list;
    printf("\n--- DEPO LISTESI ---\n");
    while (curr != NULL)
    {
        Object_Present(curr);
        curr = curr->next;
    }
}

void Analyze_Purchase()
{

    if (customer_list == NULL || product_list == NULL || storehouse_list == NULL)
    {
        printf("[HATA] Analiz yapabilmek icin en az 1 musteri, 1 urun ve 1 depo eklemelisiniz!\n");
        return;
    }

    printf("\n--- SATIN ALMA ISLEMI ---\n");

    View_All_Customers();

    int c_id = Get_Int("Musteri ID giriniz: ");

    Customer *curr_c = customer_list;
    while (curr_c != NULL && curr_c->id != c_id)
    {
        curr_c = curr_c->next;
    }
    if (curr_c == NULL)
    {
        printf("[HATA] %d ID'li musteri bulunamadi!\n", c_id);
        return;
    }

    View_All_Products();

    int p_id = Get_Int("Satin alinacak Urun ID giriniz: ");

    Product *curr_p = product_list;
    while (curr_p != NULL && curr_p->id != p_id)
    {
        curr_p = curr_p->next;
    }
    if (curr_p == NULL)
    {
        printf("[HATA] %d ID'li urun bulunamadi!\n", p_id);
        return;
    }

    int buy_qty = Get_Int("Kac adet alinacak?: ");

    if (curr_p->quantity < buy_qty)
    {
        printf("[HATA] Yetersiz stok! (Mevcut Stok: %d)\n", curr_p->quantity);
        return;
    }

    Storehouse *curr_s = storehouse_list;
    Storehouse *nearest_s = NULL;
    float min_dist = -1.0f;

    while (curr_s != NULL)
    {
        float dist = LOC_CalculateDistance(curr_c->location, curr_s->location);

        if (min_dist < 0 || dist < min_dist)
        {
            min_dist = dist;
            nearest_s = curr_s;
        }
        curr_s = curr_s->next;
    }

    if (nearest_s == NULL)
    {
        printf("[HATA] Gecerli bir depo hesabi yapilamadi!\n");
        return;
    }

    float product_cost = curr_p->price * buy_qty;
    float shipping_cost = min_dist * 5.0f;
    float total_cost = product_cost + shipping_cost;

    curr_p->quantity -= buy_qty;

    printf("\n================ FATURA VE ANALIZ ================\n");
    printf("Satin Alan Musteri ID : %d\n", curr_c->id);
    printf("Alinan Urun ID        : %d (Miktar: %d adet)\n", curr_p->id, buy_qty);
    printf("Gonderici Depo ID     : %d\n", nearest_s->id);
    printf("Mesafe                : %.2f km\n", min_dist);
    printf("--------------------------------------------------\n");
    printf("Urun Bedeli           : %.2f TL\n", product_cost);
    printf("Kargo Bedeli          : %.2f TL (%.2f x 5 TL)\n", shipping_cost, min_dist);
    printf("TOPLAM TUTAR          : %.2f TL\n", total_cost);
    printf("Kalan Urun Stoku      : %d\n", curr_p->quantity);
    printf("==================================================\n");
}

void Quit_System()
{
    Customer *curr_c = customer_list;
    Customer *next_c;
    while (curr_c != NULL)
    {
        next_c = curr_c->next;
        Object_Kill(curr_c);
        curr_c = next_c;
    }

    Product *curr_p = product_list;
    Product *next_p;
    while (curr_p != NULL)
    {
        next_p = curr_p->next;
        Object_Kill(curr_p);
        curr_p = next_p;
    }

    Storehouse *curr_s = storehouse_list;
    Storehouse *next_s;
    while (curr_s != NULL)
    {
        next_s = curr_s->next;
        Object_Kill(curr_s);
        curr_s = next_s;
    }
    
    printf("Sistemden cikiliyor...\n");
}