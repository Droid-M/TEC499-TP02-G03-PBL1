#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 1234
#define MAX_STRING_SIZE 100

int shmid;
char *shared_string;

void init_shared_memory()
{
    shmid = shmget(SHM_KEY, MAX_STRING_SIZE, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("Erro ao criar memória compartilhada");
        exit(1);
    }

    shared_string = (char *)shmat(shmid, NULL, 0);
    if (shared_string == (char *)-1)
    {
        perror("Erro ao alocar memória compartilhada");
        exit(1);
    }
}

void write_in_shared_memory(const char *message)
{
    strncpy(shared_string, message, MAX_STRING_SIZE - 1);
    shared_string[MAX_STRING_SIZE - 1] = '\0';
}

void close_shared_memory()
{
    if (shmdt(shared_string) == -1 || shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("\nErro ao remover memória compartilhada!\n");
    }
}

char *get_shared_value()
{
    return (char *)shared_string;
}