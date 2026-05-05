#ifndef __PROGRAM_DATATYPES__
#define __PROGRAM_DATATYPES__
#include <stddef.h>

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

#endif

#ifndef __PROGRAM_FUNCTIONS__
#define __PROGRAM_FUNCTIONS__
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

DString *DSTR_Create(char *char_list, int count);
DString *DSTR_GetLine();
int DSTR_Present(void *self, void *arg);
int DSTR_Kill(void *self, void *arg);

Location *LOC_Create(float x, float y);
float LOC_CalculateDistance(Location *l1, Location *l2);
int LOC_Present(void *self, void *arg);

Customer *CUS_Create(int id, DString *name, float x, float y);
int CUS_Present(void *self, void *arg);
int CUS_Kill(void *self, void *arg);

Storehouse *STORE_Create(int id, float x, float y);
int STORE_Kill(void *self, void *arg);
int STORE_Present(void *self, void *arg);

Product *PROD_Create(int id, DString *name, float price, int quantity);
int PROD_Kill(void *self, void *arg);
int PROD_Present(void *self, void *arg);

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
float Get_Float(const char *prompt);
int General_Free(void *self, void *arg);
int Object_Present(void *self);
int Object_Kill(void *self);


#endif