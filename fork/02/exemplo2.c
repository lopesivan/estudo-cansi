#include <stdio.h>
#include <unistd.h>

void childProcess(int level) {
    if (level == 0) {
        printf("Folha da árvore de processos (nível %d)\n", level);
    } else {
        pid_t pid = fork();

        if (pid == 0) {
            // Código do processo filho
            printf("Processo filho (nível %d)\n", level);
            childProcess(level - 1);
        } else if (pid > 0) {
            // Código do processo pai
            printf("Processo pai (nível %d)\n", level);
        } else {
            // Erro ao criar o processo filho
            fprintf(stderr, "Erro ao criar o processo filho.\n");
            return;
        }
    }
}

int main() {
    int levels = 3;

    childProcess(levels);

    return 0;
}
