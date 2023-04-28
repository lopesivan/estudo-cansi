#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main (int argc, char const* argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char* hello = "Hello from client";
    char buffer[1024] = {0};

    // Criando o descritor do socket do cliente
    if ((sock = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf ("\nFalha ao criar o descritor do socket\n");
        return -1;
    }

    memset (&serv_addr, '0', sizeof (serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons (PORT);

    // Convertendo o endereço IP do formato texto para binário
    if (inet_pton (AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf ("\nEndereço inválido/ Endereço não suportado\n");
        return -1;
    }

    // Conectando ao servidor
    if (connect (sock, (struct sockaddr*)&serv_addr, sizeof (serv_addr)) < 0)
    {
        printf ("\nConexão falhou\n");
        return -1;
    }

    // Enviando mensagem para o servidor
    send (sock, hello, strlen (hello), 0);
    printf ("Mensagem enviada\n");

    // Lendo a resposta do servidor
    valread = read (sock, buffer, 1024);
    printf ("%s\n", buffer);

    return 0;
}
