#include <stdio.h>

int main()
{
    unsigned int x = 0x12345678;
    unsigned char* p = (unsigned char*)&x;

    printf ("Valor: 0x%X\n", x);
    printf ("Bytes em memória:\n");

    for (int i = 0; i < sizeof (x); i++)
    {
        printf ("byte[%d] = 0x%02X\n", i, p[i]);
    }

    return 0;
}
