#include <stdio.h>

void printBinary (unsigned int number)
{
    // Define o tamanho do inteiro em bits
    int bitSize = sizeof (unsigned int) * 8;

    // Percorre cada bit do número
    for (int i = bitSize - 1; i >= 0; i--)
    {
        // Verifica o valor do bit na posição i
        unsigned int mask = 1 << i;
        printf ("%d", (number & mask) ? 1 : 0);
    }
    printf ("\n");
}

int main()
{
    unsigned int number;

    printf ("Digite um número inteiro: ");
    scanf ("%u", &number);

    printf ("Representação binária: ");
    printBinary (number);

    return 0;
}
