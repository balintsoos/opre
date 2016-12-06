#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

// Készítő:     Lestár Norbert
// Neptun kód:  A8UZ7T
// E-mail cím:  lestarnrbert@inf.elte.hu

struct passengers
{
  char name[80];
  int rating;
};

int readLine(int fo, char* buffer)
{
  int i = 0;
  int end = 1;
  char c;
  while ((end = read(fo, &c, sizeof(c))) && c != '\n')
  {
    buffer[i++] = c;
  }
  buffer[i++] = c;
  buffer[i] = '\0';
  return end;
}

int directoryExists(const char* absolutePath)
{
  struct stat status;
  stat(absolutePath, &status);
  return S_ISDIR(status.st_mode);
}

const char* currentDateTime(int add)
{
  time_t now = time(0);
  struct tm tstruct;
  char* buff = (char*)malloc(sizeof(char) * 80);
  tstruct = *localtime(&now);
  sprintf( buff, "%d. %d. %d. %d:%d"
          , tstruct.tm_year + 1900, tstruct.tm_mon + 1
          , tstruct.tm_mday + add, tstruct.tm_hour, tstruct.tm_min);
  
  return buff;
}
 
void printLN(const char * text)
{
  write(1, text, strlen(text));
  const char * newLine = "\n";
  write(1, newLine, strlen(newLine));
}

int writeToFile(const char* fileName, const char* line, int create)
{
  int fileDesc = open(fileName, O_WRONLY | O_APPEND);
  const char * newLine = "\n";
  
  if (fileDesc > 0 && create == 0)
    return -1;
  
  if (fileDesc < 0)
  {
    fileDesc = open(fileName, O_WRONLY | O_CREAT, 00666 | S_IWOTH);
    if (fileDesc < 0)
      return -1;
  }
  
  write(fileDesc, line, strlen(line));
  write(fileDesc, newLine, strlen(newLine));
  
  close(fileDesc);
  return 0;
}

void listRoute()
{
  DIR* dir;
  struct dirent * dp;
  struct stat st;
  
  if (!directoryExists("Routes"))
  {
    printLN("Nincs egyetlen járat se jelenleg!");
  } else
  {
    dir = opendir("Routes");
    printLN("Jelenleg induló járatok:");
    while ((dp = readdir(dir)))
    { 
      stat(dp->d_name, &st);
      if (dp->d_type == 8)
      {
        printLN(dp->d_name);
      }
    }
    closedir(dir);
  }
}

void addRoute()
{
  const char* directoryName = "Routes";
  if (!directoryExists(directoryName))
    mkdir(directoryName, 00766);

  printLN("Kérem adja meg a járat nevét!");
  char name[256];
  char path[256];
  read(1, name, 256);
  
  name[strcspn(name, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, name);
  writeToFile(path, currentDateTime(1), 0);
}

void deleteRoute()
{
  const char* directoryName = "Routes";

  printLN("Kérem adja meg a járat nevét!");
  char name[256];
  char path[256];
  read(1, name, 256);
  name[strcspn(name, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, name);
  if (!access(path, F_OK) && remove(path) < 0)
    printLN("A járat törlése nem sikerült!");
}

int readRoute(const char* fileName)
{
  int fileDesc = open(fileName, O_RDONLY);
  
  if (fileDesc < 0)
    return -1;
    
  char c;
  while (read(fileDesc, &c, sizeof(c)))
  {
    write(1, &c, sizeof(c));
  }
  
  close(fileDesc);
  return 0;
}

void routeInfo()
{
  const char* directoryName = "Routes";
  if (!directoryExists(directoryName))
    mkdir(directoryName, 0666);

  printLN("Melyik járatra kíváncsi?");
  char name[256];
  char path[256];
  read(1, name, 256);
  name[strcspn(name, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, name);
  
  if (readRoute(path) < 0)
    printLN("Nincs ilyen járat!");
}

static void handler(int signo)
{
  if (signo == SIGTERM)
        printf("Az utasok leszállítása megtörtént. \n");
}

void startRoute()
{
  const char* directoryName = "Routes";
  if (!directoryExists(directoryName))
  {
      printLN("Nincs egy járat se jelenleg!");
      return;
  }
  
  printLN("Melyik járatot kívánja indítani?");
  
  char name[256];
  char path[256];
  read(1, name, 256);
  name[strcspn(name, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, name);
  int fileDesc = open(path, O_RDONLY);
  
  if (fileDesc < 0)
  {
    printLN("Nincs ilyen járat!");
    return;
  }
    
  int parentToChild[2];
  pid_t cpid;

  if (pipe(parentToChild) == -1)
  {
    perror("Hiba a pipe létrehozásakor.");
    exit(EXIT_FAILURE);
  }
  
  struct msgbuf
  {
    long	mtype;
    char	mtext[80];
  } * msg;
  
  int id = 0;
  msg = (struct msgbuf*)malloc(sizeof(msg) + 1 + 80);
  
  if ((id = msgget(44, IPC_CREAT | 0666)) < 0)
  {
    perror("Nem hozható létre az üzenetsor.");
    exit(-1);
  }
  
  signal(SIGTERM, handler);
    
  cpid = fork();
  if (cpid == -1)
  {
    perror("Hiba forkolás közben.");
    exit(EXIT_FAILURE);
  }

  if (cpid == 0)
  {
    close(parentToChild[1]);
    char buffer[256];
    readLine(parentToChild[0], buffer);
    srand(time(NULL));
    int r;
    struct passengers names[20];
    int j = 0;
    int i = 0;
    
    while (readLine(parentToChild[0], buffer))
    {
      sscanf(buffer, "%s %s %d %s", names[j].name, NULL, NULL, NULL);
      names[j].rating = rand() % 5 + 1;
      j++;
    }
    close(parentToChild[0]);

    while (i < j)
    {
      sprintf(buffer, "%s értékelése az utazásról: %d", names[i].name, names[i].rating);
      strcpy(msg->mtext, buffer);
      msg->mtype = (i % j) + 1;
      msgsnd(id, msg, strlen(msg->mtext) + 1, 0);
      i++;
    }
    pause();
    _exit(EXIT_SUCCESS);
  } else
  {
    close(parentToChild[0]);
    char buffer[256];
    int i = 0;
    int j = 1;
    
    while (readLine(fileDesc, buffer))
    {
      write(parentToChild[1], buffer, strlen(buffer));
      i++;
    }
    close(parentToChild[1]);
    while (j < i)
    {
      //wait(NULL);
      msgrcv(id, msg, 80, j - 1, IPC_NOWAIT);
      printLN(msg->mtext);
      ++j;
    }
    kill(cpid, SIGTERM);
  }
  free(msg);
  
  close(fileDesc);
}

void addPassenger()
{
  const char* directoryName = "Routes";
  char fileName[256];
  char path[256];
  char passenger[256];
  char num[128];
  char name[128];
  if (!directoryExists(directoryName))
    mkdir(directoryName, 00766);

  printLN("Kérem adja meg a célállomás nevét!");
  read(1, fileName, 256);
  fileName[strcspn(fileName, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, fileName);
  
  printLN("Kérem adja meg a nevét!");
  read(1, name, 256);
  name[strcspn(name, "\n")] = '\0';
  printLN("Kérem adja meg a telefonszámát!");
  read(1, num, 256);
  num[strcspn(num, "\n")] = '\0';
  
  sprintf(passenger, "%s  %s  %s", name, num, currentDateTime(0));
  writeToFile(path, passenger, 1);
}

void deletePassenger()
{
  const char* directoryName = "Routes";
  const char* temp = "deleteTemp";
  char fileName[256];
  char path[256];
  char name[128];
  if (!directoryExists(directoryName))
    mkdir(directoryName, 00766);

  printLN("Melyik járat utasát kívánja törölni?");
  read(1, fileName, 256);
  fileName[strcspn(fileName, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, fileName);
  printLN("Kérem adja meg a nevét!");
  read(1, name, 256);
  name[strcspn(name, "\n")] = '\0';
  
  int fo = open(path, O_RDONLY);
  int go = open(temp, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fo < 0 || go < 0)
  {
    printLN("Hiba keletkezett a törlés során!");
  }
  
  char buffer[256];
  while (readLine(fo, buffer))
  {
    int dontWrite = 0;
    int j;
    for (j = 0; j < strlen(name); ++j)
    {
      if (name[j] == buffer[j])
        dontWrite++;
    }
    if (dontWrite != strlen(name))
      write(go, &buffer, strlen(buffer));
  }
  
  close(fo);
  remove(path);
  rename(temp, path);
  close(go);
}

void modifyPassenger()
{
  const char* directoryName = "Routes";
  const char* temp = "deleteTemp";
  char fileName[256];
  char path[256];
  char name[128];
  if (!directoryExists(directoryName))
  {
    printLN("Nincsen egy járat se!");
  } else
  {
    printLN("Melyik járat utasát kívánja módosítani?");
    read(1, fileName, 256);
    fileName[strcspn(fileName, "\n")] = '\0';
    sprintf(path, "%s/%s", directoryName, fileName);
    printLN("Kérem adja meg a nevét!");
    read(1, name, 256);
    name[strcspn(name, "\n")] = '\0';
    
    int fo = open(path, O_RDONLY);
    int go = open(temp, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    char* tempBuff = 0;
    if (fo < 0 || go < 0)
    {
      printLN("Nincs ilyen járat!");
    } else
    {
      char buffer[256];
      while (readLine(fo, buffer))
      {
        int dontWrite = 0;
        int j;
        for (j = 0; j < strlen(name); ++j)
        {
          if (name[j] == buffer[j])
            dontWrite++;
        }
        if (dontWrite != strlen(name))
          write(go, &buffer, strlen(buffer));
        
        if (dontWrite == strlen(name) && dontWrite != 0)
        {
          tempBuff = (char*)malloc(sizeof(char) * 256);
          memcpy(tempBuff, buffer, strlen(buffer) + 1);
        }
      }
      
      close(fo);
      if (!tempBuff)
      {
        printLN("Nincs ilyen nevű felhasználó!");
        remove(temp);
      } else
      {
        remove(path);
        rename(temp, path);
        
        char num[256];
        char passenger[256];
        char date[256];
        
        printLN("Kérem adja meg az új nevét!");
        read(1, name, 256);
        name[strcspn(name, "\n")] = '\0';
        printLN("Kérem adja meg az új telefonszámát!");
        read(1, num, 256);
        num[strcspn(num, "\n")] = '\0';
        printLN("Kérem adja meg az új időpontját az utazásnak!");
        read(1, date, 256);
        sprintf(passenger, "%s  %s  %s", name, num, date);
        writeToFile(path, passenger, 1);
        close(go);
      }
    }
  }
}

int main()
{
  printLN("Kérem válasszon az alábbi lehetőségek közül!");
  printLN("(A művelet sorszámát beírva az lefut)");
  printLN("-- Járatokkal kapcsolatos műveletek --");
  printLN("-> 1. Járatok listázása");
  printLN("-> 2. Járat felvétele");
  printLN("-> 3. Járat törlése");
  printLN("-> 4. Járat információk");
  printLN("-> 5. Járat indítása");
  printLN("-- Utasokkal kapcsolatos műveletek --");
  printLN("-> 6. Utas felvétele");
  printLN("-> 7. Utas módosítása");
  printLN("-> 8. Utas törlése");
  printLN("-- Egyéb műveletek --");
  printLN("-> 9. Kilépés");
  char answer = ' ';
  while (answer != '9')
  {
    read(1, &answer, 1);
    while (getchar() != '\n');
    switch (answer)
    {
      case '1':
        listRoute();
        break;
      case '2':
        addRoute();
        break;
      case '3':
        deleteRoute();
        break;
      case '4':
        routeInfo();
        break;
      case '5':
        startRoute();
        break;
      case '6':
        addPassenger();
        break;
      case '7':
        modifyPassenger();
        break;
      case '8':
        deletePassenger();
        break;
    }
  }
  return 0;
}
