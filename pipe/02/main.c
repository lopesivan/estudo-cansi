#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void main()
{
    int pipefds[2];
    char write_msg[256] = "Hello from parent!";
    char read_buffer[256] = {0};

    // Criando o pipe
    if (pipe (pipefds) == -1)
    {
        perror ("pipe");
        return;
    }

    // Imprimindo o PID do processo pai
    printf ("Parent PID: %d\n", getpid());

    // Criando o processo filho
    pid_t pid = fork();

    if (pid == -1)
    {
        // Se a criação do processo filho falhar
        perror ("fork");
        return;
    }

    if (pid > 0)
    {
        // Código do processo pai

        // Fechando a extremidade de leitura do pipe no processo pai
        close (pipefds[0]);

        // Escrevendo mensagem no pipe
        write (pipefds[1], write_msg, strlen (write_msg));

        // Fechando a extremidade de escrita do pipe no processo pai
        close (pipefds[1]);

        // Esperando o processo filho terminar
        wait (NULL);
    }
    else
    {
        // Código do processo filho

        // Fechando a extremidade de escrita do pipe no processo filho
        close (pipefds[1]);

        // Lendo a mensagem do pipe
        read (pipefds[0], read_buffer, sizeof (read_buffer));

        // Imprimindo a mensagem lida
        printf ("Child PID: %d\n", getpid());
        printf ("Got from pipe: %s\n", read_buffer);

        // Fechando a extremidade de leitura do pipe no processo filho
        close (pipefds[0]);
    }
}
