#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Código do processo filho
        printf("Olá, eu sou o processo filho!\n");
    } else if (pid > 0) {
        // Código do processo pai
        printf("Olá, eu sou o processo pai!\n");
    } else {
        // Erro ao criar o processo filho
        fprintf(stderr, "Erro ao criar o processo filho.\n");
        return 1;
    }

    return 0;
}
