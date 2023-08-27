#include "helpers.h"
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>  // Biblioteca para Windows
#else
#include <termios.h>  // Biblioteca para Linux
#include <unistd.h>
#include <fcntl.h>
#endif

#ifdef _WIN32 // Versão para detectar entradas do teclado no windows
int kbhit() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD dwEvents;
    GetNumberOfConsoleInputEvents(hStdin, &dwEvents);
    return dwEvents > 0;
}
#else // Versão para detectar entradas de teclado em sistemas Unix-like
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#endif