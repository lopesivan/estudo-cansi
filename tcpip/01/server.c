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
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof (address);
    char buffer[BUFFER_SIZE] = {0};
    const char* response = "Mensagem recebida";

    // Cria o socket do servidor
    if ((server_fd = socket (AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror ("Erro ao criar o socket");
        exit (EXIT_FAILURE);
    }

    // Define as opções de endereço e porta do servidor
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons (PORT);

    // Vincula o socket do servidor ao endereço e porta especificados
    if (bind (server_fd, (struct sockaddr*) &address, sizeof (address)) < 0)
    {
        perror ("Erro ao vincular o socket");
        exit (EXIT_FAILURE);
    }

    // Aguarda por conexões
    if (listen (server_fd, 1) < 0)
    {
        perror ("Erro ao aguardar conexoes");
        exit (EXIT_FAILURE);
    }

    // Aceita uma conexão
    if ((new_socket = accept (server_fd, (struct sockaddr*) &address, (socklen_t*) &addrlen)) < 0)
    {
        perror ("Erro ao aceitar a conexao");
        exit (EXIT_FAILURE);
    }

    // Lê a mensagem enviada pelo cliente
    valread = read (new_socket, buffer, BUFFER_SIZE);
    printf ("Mensagem recebida: %s\n", buffer);

    // Envia uma resposta para o cliente
    send (new_socket, response, strlen (response), 0);
    printf ("Resposta enviada: %s\n", response);

    // Encerra a conexão
    close (new_socket);
    close (server_fd);

    return 0;
}

