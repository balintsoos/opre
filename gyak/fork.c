#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

void childProcess()
{
	pid_t child = fork();

	if (child < 0) // error
	{
		perror("The fork calling was not succesful\n");
		exit(1);
	}
	else if (child > 0) // the parent process, it can see the returning value of fork
	{
		printf("==== Starting parent process ====\n");

		printf("My pid is %i\n", getpid());
		printf("My parent's pid is %i\n", getppid());

		int status;
		waitpid(child, &status, 0);

		printf("==== Ending parent process ====\n");
	}
	else // child process
	{
		printf("==== Starting child process ====\n");

		printf("My pid is %i\n", getpid());
		printf("My parent's pid is %i\n", getppid());

		printf("==== Ending child process ====\n");
	}
}

int main(int argc, char** argv)
{
	childProcess();

  return 0;
}
