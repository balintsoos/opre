#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen,strcmp,strcat,strcpy and more ....

void feladat1()
{
    char* str = "Hello World!";
    printf("1. fel: %i\n", strlen(str)); // %i printf-ben az 'i'ntegerek kiirasara
}

void feladat2()
{
    char* str = "Hello";
    char* str2 = "World";
    if(strcmp(str,str2) == 0)
    {
       printf("2. fel: Megegyeznek.\n");
    }
    else
    {
       printf("2. fel: Nem egyeznek meg.\n");
    }
}

void feladat2alt()
{
    char* str = "Hello";
    char* str2 = "World";
    char* ptr1 = str;
    char* ptr2 = str2;
    while (*ptr1 != 0 && *ptr2 !=0)
    {
        if (*ptr1 != *ptr2)
        {
            printf("2. fel.alt: Nem egyeznek meg.\n");
            return;
        }
        ++ptr1;
        ++ptr2;
    }
    // Kell meg egy ellenorzes, hogy elkeruljuk, hogy egyik a masiknak csak a substing-je legyen.
    if (*ptr1 == *ptr2)
    {
        printf("2. fel.alt: Megegyeznek\n");
    }
    else
    {
        printf("2. fel.alt: Nem egyeznek meg\n");
    }
}

void feladat3()
{
    char* str = "Hello World!";
    char* str2 = malloc((strlen(str)+1)*sizeof(char)); // +1 hogy legyen hely a '\0' karakternek
    strcpy(str2, str);
    if (str == str2)
    {
        printf("3. fel: A ket string ugyanott van, probalkozz ujra.\n"); 
    }
    else
    {
        printf("3. fel: Nem ugyanott van.\n");
    }
    free(str2);
}

void feladat3alt()
{
    char* str = "Hello World!";
    char* str2 = strdup(str); // strdup allokalja is nekünk a teruletet...
    if (str == str2)
    {
        printf("3. fel.alt: A ket string ugyanott van.\n");
    }
    else 
    {
        printf("3. fel.alt: Nem ugyanott van.\n");
    }
    free(str2); // de ugyanugy nekunk kell felszabaditani.
}

void feladat4(int argc, char** argv)
{
    int i = 0; // jo gyakorlat mindig kezdoerteket adni egy valtozonak, akkor is, ha kovetkezo sorban masik erteket kap.
    for (i=0; i<argc; i++) 
    {
       printf("4. fel: %s\n", argv[i]);
    }
    for (i=argc-1; i>-1; i--)
    {
        printf("4. fel: %s\n", argv[i]);
    }
}

void feladat5(int argc, char** argv)
{
    // A feladat szovege nem ter ki ra kulon, igy argv[0]-t tetszes szerint bevesszuk vagy nem.
    int i = 1; // argv[0] nincs benne, ha i=0, akkor benne van.
    int hossz = 0;
    for (i=1; i < argc; i++)
    {
        hossz = hossz + strlen(argv[i]);
    }
    printf("5.fel: %i\n", hossz);
}

void feladat6(int argc, char** argv)
{
     // Itt ertelemszeruen az argv[0] nem szamit bele.
     if (argc < 3) // argc-ben benne van a 0. is, igy ket parameternel argc erteke 3.
     {
          printf("6.fel: legalabb 2 parameter kell\n");
          return;
     }
     else
     { 
        char* minta = argv[1]; // Ezzel nem masoltam ki ujra string-be, csak egy "hivatkozasom" van ra. Hasonloan, mintha c++-ban csak referenciat allitanek.
        int i = 2;
        for (i=2; i<argc; i++)
        {
            if(strcmp(minta, argv[i]) != 0)
            {
                printf("6.fel: Van, ami nem egyezik: %s\n", argv[i]);
                return;
            }
        }
        printf("6.fel: Egyeznek a parameterek.\n");
     }
}

void feladat7(int argc, char** argv)
{
     if (argc != 3)
     {
         printf("7.fel: Pontosan ket bemeneti parameter kell.\n");
     }
     else
     {
         // Teruletet foglalunk: a ket parameter hosszanak osszege + 0 karakter.
         char* concat = malloc((strlen(argv[1])+strlen(argv[2]))*sizeof(char)+1);
         strcpy(concat, argv[1]);
         strcat(concat, argv[2]);
         printf("7.fel: %s\n",concat);
         free(concat); // Ne felejtsuk el.
     }
}

int main(int argc, char** argv)
{
  feladat1();
  feladat2();
  feladat2alt();
  feladat3();
  feladat3alt();
  feladat4(argc, argv);
  feladat5(argc, argv);
  feladat6(argc, argv);
  feladat7(argc, argv);
  return 0;
}
