#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
    int pipefds[2];
    char read_buffer[256] = {0};

    // Criando o pipe
    if (pipe (pipefds) == -1)
    {
        perror ("pipe");
        exit (EXIT_FAILURE);
    }

    // Criando o processo filho
    pid_t pid = fork();

    if (pid == -1)
    {
        // Se a criação do processo filho falhar
        perror ("fork");
        exit (EXIT_FAILURE);
    }

    if (pid > 0)
    {
        // Código do processo pai
        // Fechando a extremidade de leitura do pipe no processo pai
        close (pipefds[0]);

        // Redirecionando stdout para a extremidade de escrita do pipe
        dup2 (pipefds[1], STDOUT_FILENO);

        // Executando o comando `pwd`
        execlp ("pwd", "pwd", NULL);

        // Fechando a extremidade de escrita do pipe no processo pai
        close (pipefds[1]);
    }
    else
    {
        // Código do processo filho
        // Fechando a extremidade de escrita do pipe no processo filho
        close (pipefds[1]);

        // Lendo a mensagem do pipe
        read (pipefds[0], read_buffer, sizeof (read_buffer));

        // Imprimindo a mensagem lida
        printf ("Current directory: %s", read_buffer);

        // Fechando a extremidade de leitura do pipe no processo filho
        close (pipefds[0]);

        // Finalizando o processo filho
        exit (EXIT_SUCCESS);
    }
}
