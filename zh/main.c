#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>
#include <time.h>
#include <poll.h>

struct message
{
    long mtype;
    char mtext[1024];
};

static int arrived = 0;

static void handleSignal(int signumber)
{
	if (signumber == SIGTERM)
	{
		arrived = 1;
	}
}

int main(int argc, char** argv)
{
  key_t key = ftok(argv[0], 1);

  int msgqueue = msgget(key, 0600 | IPC_CREAT);

  if (msgqueue < 0 ) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}

  int minNumber = 1;
  int maxNumber = atoi(argv[1]);

  signal(SIGTERM, handleSignal);
  pid_t cpid;
  cpid = fork();

  if (cpid < 0) // error
	{
		perror("Fork calling was not succesful\n");
		exit(EXIT_FAILURE);
	}
  else if (cpid > 0) // parent
  {
    int random = rand() % (maxNumber + 1 - minNumber) + minNumber;

    struct message ms = { 1, "" };
    sprintf(ms.mtext, "%i", random);
    msgsnd(msgqueue, &ms, strlen(ms.mtext), 0);
    sprintf(ms.mtext, "%i", maxNumber);
    msgsnd(msgqueue, &ms, strlen(ms.mtext), 0);

    pause();

    struct message m;
		msgrcv(msgqueue, &m, 1024, 1, 0);

    if (arrived)
		{
			printf("\nTrain arrived\n");
		}

    printf("Message from train: %s\n", m.mtext);
  }
  else // child
  {
    srand(time(NULL));

    int myNumber;
    int maxNumber;

    struct message m;
		msgrcv(msgqueue, &m, 1024, 1, 0);
    myNumber = atoi(m.mtext);
    msgrcv(msgqueue, &m, 1024, 1, 0);
    maxNumber = atoi(m.mtext);

    printf("My number: %i, Remaining: %i\n", myNumber, maxNumber - myNumber);

    double chance = (double)rand() / (double)RAND_MAX;

    char mode[100];

    if (chance < 0.9)
    {
      sprintf(mode, "%s", "I arrived safely");
    }
    else
    {
      if (rand() / (RAND_MAX + 1.0) < 0.5)
      {
        sprintf(mode, "%s", "I slipped on a banana peel");
      }
      else
      {
        sprintf(mode, "%s", "I am frozen");
      }
    }

    struct message ms = { 1, "" };
    sprintf(ms.mtext, "%s", mode);

    printf("%s", mode);

    kill(getppid(), SIGTERM);
    msgsnd(msgqueue, &ms, strlen(ms.mtext), 0);
  }
}
