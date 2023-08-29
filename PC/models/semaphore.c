#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

#include <windows.h>

#define BUF_SIZE 1024

HANDLE h_map_file;
LPCTSTR p_buf;

HANDLE create_shared_memory()
{
    return CreateFileMapping(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        BUF_SIZE,
        TEXT("Local\\MyFileMap"));
}

LPCTSTR map_shared_memory(HANDLE h_map_file)
{
    return (LPCTSTR)MapViewOfFile(h_map_file, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);
}

void write_to_shared_memory(LPCTSTR p_buf, const char *data)
{
    // strcpy_s((char *)p_buf, BUF_SIZE, data);
    strncpy((char *)p_buf, data, BUF_SIZE - 1);
    ((char *)p_buf)[BUF_SIZE - 1] = '\0';
}

void unmap_shared_memory(LPCTSTR p_buf)
{
    UnmapViewOfFile(p_buf);
}

void close_shared_memory()
{
    UnmapViewOfFile(p_buf);
    CloseHandle(h_map_file);
}

void init_shared_memory()
{
    h_map_file = create_shared_memory();
    if (h_map_file == NULL)
    {
        perror("Erro ao criar memória compartilhada");
        exit(1);
    }

    p_buf = map_shared_memory(h_map_file);

    if (p_buf == NULL)
    {
        perror("Erro ao alocar memória compartilhada");
        close_shared_memory();
        exit(1);
    }
}

void write_in_shared_memory(const char *message)
{
    write_to_shared_memory(p_buf, message);
}

char *get_shared_value()
{
    return (char *)p_buf;
}

#else

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

#endif
