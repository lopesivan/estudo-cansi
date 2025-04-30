#include "frames.h"
#include <stdio.h>
#include <stdlib.h>

void print_lista(const List *lista)
{
    printf("Frames disponíveis: ");
    ListElmt *elmt = list_head(lista);
    while (elmt != NULL)
    {
        int *frame = (int *)list_data(elmt);
        printf("%d ", *frame);
        elmt = list_next(elmt);
    }
    printf("\n");
}

int main(void)
{
    List frames;
    list_init(&frames, free); /* Cada item é um ponteiro para int */

    /* Inicializar com 5 quadros disponíveis (0 a 4) */
    int i = 0;
    for (i = 4; i >= 0; --i)
    {
        int *num = malloc(sizeof(int));
        *num = i;
        list_ins_next(&frames, NULL, num);
    }

    print_lista(&frames);

    /* Alocar dois quadros */
    int f1 = alloc_frame(&frames);
    int f2 = alloc_frame(&frames);
    printf("Quadros alocados: %d e %d\n", f1, f2);
    print_lista(&frames);

    /* Liberar o quadro f1 de volta */
    free_frame(&frames, f1);
    printf("Quadro %d devolvido\n", f1);
    print_lista(&frames);

    /* Limpeza final */
    list_destroy(&frames);
    return 0;
}
