//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

void fv(int handle)
{
  char nev[] = "Kiss Robin";
  write(handle, &nev, sizeof(nev));  
}

int main(int argc,char** argv){
 int handle = open("./nev.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
 fv(handle);/*
 char c;
 int returnValue = 1;
 while (returnValue != 0)
 {
  returnValue = read(handle, &c, sizeof(c));
  printf("%c", c);
 }
 printf("\n");*/
 close(handle);
 return 0;
}
