#include <stdio.h>

typedef struct {
  int numeroDeOlhos;
  void (*fala)(struct Mamifero *);
} Mamifero;

void falaMamifero(Mamifero *m) {
  printf("O mamífero com %d olhos diz: Olá!\n", m->numeroDeOlhos);
}

void inicializaMamifero(Mamifero *m, int numeroDeOlhos) {
  m->numeroDeOlhos = numeroDeOlhos;
  m->fala = falaMamifero;
}

typedef struct {
  Mamifero base;
  // Outros atributos específicos de Gato
} Gato;

void falaGato(Mamifero *m) {
  printf("O gato com %d olhos diz: Miau!\n", m->numeroDeOlhos);
}

void inicializaGato(Gato *g, int numeroDeOlhos) {
  inicializaMamifero(&g->base, numeroDeOlhos);
  g->base.fala = falaGato;
}

int main() {
  Gato g;
  inicializaGato(&g, 2);
  g.base.fala((Mamifero *)&g);

  return 0;
}
