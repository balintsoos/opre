#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen,strcmp,strcat,strcpy and more ....

//there is no string type - instead of it You have to use char array
int length(char * str)
{
 int l=0;
 while (*str++!=0)l++; //*str points to the next character
 return l;
}
int main()
{
              //char str1[80]; //it would be an 80 characters long empty "string", - it is empty 
  char str1[]="Hello world\0"; //it's length is the length of the text
  char str2[]="!!!";
  str1[0] = 'A';
  str1[1] = 0; // str1[1] = '\0';

  printf("Hello world, %s\n", str1);
  return 0;
}
