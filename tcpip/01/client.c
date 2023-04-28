#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main (void)
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    const char* message = "Hello from client";

    // Cria o socket do cliente
    if ((sock = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf ("\nErro ao criar o socket\n");
        return -1;
    }

    // Define as opções de endereço e porta do servidor
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons (PORT);

    // Converte o endereço IP de string para binário
    if (inet_pton (AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {

        // Conecta ao servidor remoto
        if (connect (sock, (struct sockaddr*) &serv_addr, sizeof (serv_addr)) < 0)
        {
            printf ("\nErro ao conectar\n");
            return -1;
        }

        // Envia uma mensagem para o servidor
        send (sock, message, strlen (message), 0);
        printf ("Mensagem enviada: %s\n", message);

        // Lê a resposta do servidor
        valread = read (sock, buffer, BUFFER_SIZE);
        printf ("Resposta recebida: %s\n", buffer);

        // Encerra a conexão
        close (sock);

        return 0;
    }
}
