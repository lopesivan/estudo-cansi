#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main (int argc, char const* argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof (address);
    char buffer[1024] = {0};
    char* hello = "Hello from server";

    // Criando o descritor do socket do servidor
    if ((server_fd = socket (AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror ("Falha ao criar o descritor do socket");
        exit (EXIT_FAILURE);
    }

    // Anexando o socket ao endereço e porta especificados
    if (setsockopt (server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof (opt)))
    {
        perror ("Falha ao anexar o socket");
        exit (EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons (PORT);

    if (bind (server_fd, (struct sockaddr*)&address, sizeof (address))<0)
    {
        perror ("Falha na associação do socket");
        exit (EXIT_FAILURE);
    }

    // Ouvindo as conexões de entrada
    if (listen (server_fd, 3) < 0)
    {
        perror ("Erro ao escutar o socket");
        exit (EXIT_FAILURE);
    }
    if ((new_socket = accept (server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen))<0)
    {
        perror ("Erro ao aceitar a conexão");
        exit (EXIT_FAILURE);
    }

    // Lendo dados do cliente
    valread = read (new_socket, buffer, 1024);
    printf ("%s\n", buffer);

    // Enviando mensagem para o cliente
    send (new_socket, hello, strlen (hello), 0);
    printf ("Mensagem enviada\n");

    return 0;
}
