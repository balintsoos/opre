#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int value = 2;
 pid_t child = fork();
 if (child < 0)
 {
   // error
 }
 else if (child > 0)
 {
   pid_t second_child = fork();
   pid_t mypid = getpid();
   pid_t parentpid = getppid();
   printf("Pidem: %i\n", mypid);
 }
 else
 {
   pid_t mypid = getpid();
   pid_t parentpid = getppid();
   printf("Pidem: %i\n", mypid);
 }
 return 0;
}
