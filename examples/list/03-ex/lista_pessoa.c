#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pessoa {
  char nome[50];
  int idade;
} Pessoa;

/* Função de destruição de Pessoa */
void destruir_pessoa(void *data) { free(data); }

/* Função de comparação (opcional) */
int comparar_pessoa(const void *a, const void *b) {
  const Pessoa *p1 = (const Pessoa *)a;
  const Pessoa *p2 = (const Pessoa *)b;
  return strcmp(p1->nome, p2->nome);
}

int match_pessoa(const void *key1, const void *key2) {
  const Pessoa *p1 = (const Pessoa *)key1;
  const Pessoa *p2 = (const Pessoa *)key2;
  return strcmp(p1->nome, p2->nome) == 0;
}

void imprimir_lista(const List *lista) {
  ListElmt *elmt = list_head(lista);
  while (elmt != NULL) {
    Pessoa *p = (Pessoa *)list_data(elmt);
    printf("Nome: %-10s | Idade: %d\n", p->nome, p->idade);
    elmt = list_next(elmt);
  }
}

/* Função para adicionar uma pessoa à lista */
void adicionar_pessoa(List *lista, const char *nome, int idade) {
  Pessoa *nova_pessoa = malloc(sizeof(Pessoa));
  if (nova_pessoa == NULL) {
    fprintf(stderr, "Erro ao alocar memória para Pessoa.\n");
    exit(EXIT_FAILURE);
  }
  strcpy(nova_pessoa->nome, nome);
  nova_pessoa->idade = idade;
  list_ins_next(lista, NULL, nova_pessoa); /* insere no início */
}

int main(void) {
  List lista;
  list_init(&lista, destruir_pessoa); /* define destruidor */

  /* Criando e inserindo pessoas */
  Pessoa *p1 = malloc(sizeof(Pessoa));
  strcpy(p1->nome, "Ana");
  p1->idade = 30;
  list_ins_next(&lista, NULL, p1); /* insere no início */

  Pessoa *p2 = malloc(sizeof(Pessoa));
  strcpy(p2->nome, "Bruno");
  p2->idade = 25;
  list_ins_next(&lista, list_head(&lista), p2); /* insere após p1 */

  Pessoa *p3 = malloc(sizeof(Pessoa));
  strcpy(p3->nome, "Carlos");
  p3->idade = 40;
  list_ins_next(&lista, list_head(&lista), p3); /* insere após p2 */

  Pessoa *p4 = malloc(sizeof(Pessoa));
  strcpy(p4->nome, "Ivan");
  p4->idade = 47;
  list_ins_next(&lista, list_head(&lista), p4); /* insere após p3 */

  printf("Lista de pessoas:\n");
  imprimir_lista(&lista);

  /* Agora procurar a pessoa */
  Pessoa chave;
  strcpy(chave.nome, "Ivan"); /* Procuramos pelo nome */
  chave.idade = 0;            /* idade não importa na comparação */

  ListElmt *elemento = list_head(&lista);
  while (elemento != NULL) {
    if (match_pessoa(&chave, list_data(elemento))) {
      Pessoa *achado = (Pessoa *)list_data(elemento);
      printf("Pessoa encontrada: %s, %d anos\n", achado->nome, achado->idade);
      break;
    }
    elemento = list_next(elemento);
  }

  /* Libera todos os nós */
  list_destroy(&lista);
  return 0;
}
