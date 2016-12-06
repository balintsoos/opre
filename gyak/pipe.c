#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>

int signalFlag = 0;

void handler(int signumber)
{
  signalFlag = signumber;
}

char* convertSignalToStr(int signumber)
{
  switch (signumber)
  {
    case SIGTERM: return "SIGTERM";
    case SIGUSR1: return "SIGUSR1";
    default: return "UNKNOWN";
  }
}

//
// unnamed pipe example
//
int main(int argc, char *argv[])
{
  signal(SIGTERM, handler);
  signal(SIGUSR1, handler);

  int pipefd[2]; // unnamed pipe file descriptor array
  pid_t pid;

  char sz[100];  // char array for reading from pipe

  if (pipe(pipefd) == -1)
  {
    perror("Hiba a pipe nyitaskor!");
    exit(EXIT_FAILURE);
  }

  pid = fork(); // creating parent-child processes

  if (pid == -1)
  {
    perror("Fork hiba");
    exit(EXIT_FAILURE);
  }

  if (pid == 0)
  {
    // child process
    sleep(3);	// sleeping a few seconds, not necessary
    kill(getppid(), SIGTERM);
    sleep(3);
    kill(getppid(), SIGUSR1);
    close(pipefd[1]); // Usually we close the unused write end
    printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
    read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
    printf("Gyerek olvasta uzenet: %s\n",sz);
    read(pipefd[0], sz, sizeof(sz));
    printf("Masodik uzenet: %s\n", sz);
    printf("\n");
    close(pipefd[0]); // finally we close the used read end
  }
  else
  {
    // szulo process
    printf("Szulo indul!\n");
    close(pipefd[0]); // Usually we close unused read end
    pause();
    write(pipefd[1], convertSignalToStr(signalFlag),20);
    pause();
    write(pipefd[1], convertSignalToStr(signalFlag), 20);
    close(pipefd[1]); // Closing write descriptor
    printf("Szulo beirta az adatokat a csobe!\n");
    fflush(NULL); // flushes all write buffers (not necessary)

    int status;
    wait(&status); // waiting for child process (not necessary)
    // try it without wait()
    printf("Szulo befejezte!\n");
  }

  exit(EXIT_SUCCESS);	// force exit, not necessary
}
