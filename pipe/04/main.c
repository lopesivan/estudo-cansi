#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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
        // Fechando a extremidade de escrita do pipe no processo pai
        close (pipefds[1]);

        // Lendo a mensagem do pipe
        read (pipefds[0], read_buffer, sizeof (read_buffer));
        printf ("Current directory: %s", read_buffer);

        // Fechando a extremidade de leitura do pipe no processo pai
        close (pipefds[0]);

        // Esperando o processo filho terminar
        wait (NULL);
    }
    else
    {
        // Código do processo filho
        // Fechando a extremidade de leitura do pipe no processo filho
        close (pipefds[0]);

        // Convertendo o descritor de arquivo para string
        char pipe_fd_str[10];
        snprintf (pipe_fd_str, 10, "%d", pipefds[1]);

        // Executando o script Bash
        //execl ("/bin/bash", "/bin/bash", "script.sh", pipe_fd_str, NULL);
        execl ("/bin/bash", "bash", "./script.sh", pipe_fd_str, NULL);

        // Fechando a extremidade de escrita do pipe no processo filho
        close (pipefds[1]);

        // Finalizando o processo filho
        exit (EXIT_SUCCESS);
    }
}
