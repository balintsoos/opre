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
    char mtext[80];
};

static void handler(int signo)
{
    printf("%d jelzés küldése megtörtént! \n", signo);
    signal(signo, handler);
}

void sendMsg(const char* name, int readF, int writeF, int parent, int first, int msgSnd)
{
        printf("%s játékos jön! \n", name);
        char buffer[20];
        if (poll(&(struct pollfd){ .fd = readF, .events = POLLIN }, 1, 0) == 1
            && read(readF, buffer, 20) > 0)
        {
            printf("Másik játékos lépése: %s \n", buffer);
        }
        int random = (rand() + time(NULL)) % 4 + 1;
        struct message msg = { 1, "" };
        sleep(random);
        sprintf(msg.mtext, "%d", random);
        msgsnd(msgSnd, &msg, strlen(msg.mtext), 0);
        printf("Gondolkodtam %d másodpercet. \n", random);
        random = rand();
        sprintf(buffer, "[ A%d-B%d ]", random % 6, random % 18);
        write(writeF, buffer, strlen(buffer) + 1);
        kill(parent, SIGUSR1);
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    signal(SIGCONT, handler);
    signal(SIGUSR1, handler);
    key_t key = ftok(argv[0],1);
    int firstChild, secondChild, parent;
    int pipeMsg[2];
    int numOfMoves = atoi(argv[1]);
    int i = 0;
    int msgSnd = msgget(key, 0600 | IPC_CREAT);
    
    if (pipe(pipeMsg) == -1)
    {
        perror("Hiba a pipe létrehozásakor.");
        exit(EXIT_FAILURE);
    }
    
    parent = getpid();
    firstChild = fork();
    if (firstChild == 0)
    {
        sleep(1);
        printf("Első játékos játékra kész! \n");
        kill(parent, SIGUSR1);
        while (i < numOfMoves)
        {
            pause();
            sendMsg("Első", pipeMsg[0], pipeMsg[1], parent, i, msgSnd);
            ++i;
        }
    }
    else
    {
        secondChild = fork();
        if (secondChild == 0)
        {
            sleep(2);
            printf("Második játékos játékra kész! \n");
            kill(parent, SIGUSR1);
            while (i < numOfMoves)
            {
                pause();
                sendMsg("Második", pipeMsg[0], pipeMsg[1], parent, i, msgSnd);
                ++i;
            }
        }
        else
        {
            pause();
            pause();
            int random = rand();
            int firstThink = 0;
            int secondThink = 0;
            if (random % 2)
            {
                printf("%d számú (első) játékos kezdd! \n", firstChild);
                while (i < numOfMoves)
                {
                    struct message msg = { 1, "" };
                    kill(firstChild, SIGCONT);
                    pause();
                    msgrcv(msgSnd, &msg, 80, 1, 0);
                    printf("Első játékos gondolkodott %s másodpercet. \n", msg.mtext);
                    firstThink += atoi(msg.mtext);
                    kill(secondChild, SIGCONT);
                    pause();
                    msgrcv(msgSnd, &msg, 80, 1, 0);
                    printf("Második játékos gondolkodott %s másodpercet. \n", msg.mtext);
                    secondThink += atoi(msg.mtext);
                    ++i;
                }
            } else
            {
                printf("%d számú (második) játékos kezdd! \n", secondChild);
                while (i < numOfMoves)
                {
                    struct message msg;
                    kill(secondChild, SIGCONT);
                    pause();
                    msgrcv(msgSnd, &msg, 80, 1, 0);
                    printf("Második játékos gondolkodott %s másodpercet. \n", msg.mtext);
                    secondThink += atoi(msg.mtext);
                    kill(firstChild, SIGCONT);
                    pause();
                    msgrcv(msgSnd, &msg, 80, 1, 0);
                    printf("Első játékos gondolkodott %s másodpercet. \n", msg.mtext);
                    firstThink += atoi(msg.mtext);
                    ++i;
                }
            }
            if (firstThink < secondThink)
            {
                printf("Az első játékos nyert! \n");
            }
            else if (secondThink > firstThink)
            {
                printf("A második játékos nyert! \n");
            }
            else
            {
                printf("A játék döntetlennel véget ért! \n");
            }
        }
    }
    return 0;
}