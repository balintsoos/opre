#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <time.h>
#include <string.h>

void elso()
{
  int handle = open("./nev.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
  if (handle < 0)
  {
      printf("Fajl hiba.\n");
      return;
  }
  char nev[] = "Kiss Robin";
  write(handle, &nev, sizeof(nev));  
  close(handle);
}

struct Szamok
{
  int x;
  int y;
};

void masodik(int x, int y, char* fajlnev)
{
   struct Szamok kiirando;
   kiirando.x = x;
   kiirando.y = y;
   int handle = open("./szamok.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
   if (handle < 0)
   {
       printf("Fajl hiba.");
   }
   write(handle, &kiirando, sizeof(kiirando));
   struct Szamok beolvasando;
   lseek(handle, 0, SEEK_SET);
   read(handle, &beolvasando, sizeof(beolvasando));
   printf("masodik fel: x:%i, y:%i\n", beolvasando.x, beolvasando.y);
   if (kiirando.x == beolvasando.x && kiirando.y == beolvasando.y)
   {
       printf("masodik fel: megegyeznek.\n");
   }
   close(handle);
}

void harmadik()
{
   time_t most = time(0);
   printf("Harmadik fel: mostani ido: %s\n", ctime(&most));
}

void negyedik()
{
   int handle = open("./nev.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
   if (handle < 0)
   {
       return;
   }
   char c='N';
   lseek(handle, -2*sizeof(c), SEEK_END);
   write(handle, &c, sizeof(c));
   close(handle);
}

struct SortedLine
{
   char line[80];
   struct SortedLine *next;
};

// Megjegyzes: ez egy kicsit nehezebbre sikerult, mint terveztem, ez nem tartozik a tananyaghoz, de tanulni lehet belole.
void otodik()
{
   FILE * handle;
   handle = fopen("abc.txt", "r");
   if (handle == NULL)
   {
      return;
   }
   char buffer[80];
   struct SortedLine *start = 0;
   while(fgets(buffer, sizeof(buffer), handle) != 0)
   {
      struct SortedLine *inserter = start;
      if (inserter == 0)
      {
        // Itt biztos, hogy ez az elso sor, amit beolvastunk.
        start = malloc(sizeof(struct SortedLine));
        // Include-olni kell string.h-t strcpy miatt.
        strcpy((*start).line, buffer);
        (*start).next = 0;
      }
      else if (strcmp((*inserter).line, buffer) > 0)
      {
         // Ha az elejere kell beilleszteni. Ilyenkor az elejet is ujra kell allitani.
         struct SortedLine *newStart = malloc(sizeof(struct SortedLine));
         strcpy ((*newStart).line, buffer);
         (*newStart).next = inserter;
         start = newStart;
      }
      else
      {
         // Ha nem az elejere kell beilleszteni.
         while (inserter != 0)
         {
            if ((*inserter).next == 0 || strcmp((*(*inserter).next).line, buffer) > 0 )
            {
               struct SortedLine *newElement = malloc(sizeof(struct SortedLine));
               strcpy((*newElement).line, buffer);
               (*newElement).next = (*inserter).next;
               (*inserter).next = newElement;
               break;
            }
            else
            {
               inserter = (*inserter).next;
            }
         }
      }
   }
   fclose(handle);
   while(start != 0)
   {
     printf("%s", (*start).line);
     struct SortedLine *current = start;
     start = (*start).next;
     free(current);
   }
   printf("\n");
}

void hatodik()
{
   srand(time(0));
   int result = (rand()%6 +1) + (rand()%6 + 1);
   printf("A dobas eredmenye: %i\n", result);
}

int main(int argc,char** argv){
 elso();
 masodik(10, 2, "./szamok.txt");
 if (argc == 3)
 { 
    // 2. b feladat
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);
    masodik(x, y, "./szamokb.txt");
 }
 harmadik();
 negyedik();
 otodik();
 hatodik();
 return 0;
}

