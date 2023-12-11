#include <stdio.h>

typedef struct {
    int numeroDeOlhos;
    void (*fala)(struct Mamifero*);
} Mamifero;

void falaMamifero(Mamifero *m) {
    printf("O mamífero com %d olhos diz: Olá!\n", m->numeroDeOlhos);
}

void inicializaMamifero(Mamifero *m, int numeroDeOlhos) {
    m->numeroDeOlhos = numeroDeOlhos;
    m->fala = falaMamifero;
}

int main() {
    Mamifero m;
    inicializaMamifero(&m, 2);
    m.fala(&m);

    return 0;
}

