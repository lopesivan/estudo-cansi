#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef struct Pessoa {
    char nome[50];
    int idade;
} Pessoa;

// Função de destruição de Pessoa
void destruir_pessoa(void *data) {
    free(data);
}

// Função de comparação (opcional)
int comparar_pessoa(const void *a, const void *b) {
    const Pessoa *p1 = (const Pessoa *)a;
    const Pessoa *p2 = (const Pessoa *)b;
    return strcmp(p1->nome, p2->nome);
}

void imprimir_lista(const List *lista) {
    ListElmt *elmt = list_head(lista);
    while (elmt != NULL) {
        Pessoa *p = (Pessoa *)list_data(elmt);
        printf("Nome: %-10s | Idade: %d\n", p->nome, p->idade);
        elmt = list_next(elmt);
    }
}

int main(void) {
    List lista;
    list_init(&lista, destruir_pessoa);  // define destruidor

    // Criando e inserindo pessoas
    Pessoa *p1 = malloc(sizeof(Pessoa));
    strcpy(p1->nome, "Ana");
    p1->idade = 30;
    list_ins_next(&lista, NULL, p1);  // insere no início

    Pessoa *p2 = malloc(sizeof(Pessoa));
    strcpy(p2->nome, "Bruno");
    p2->idade = 25;
    list_ins_next(&lista, list_head(&lista), p2);  // insere após p1

    Pessoa *p3 = malloc(sizeof(Pessoa));
    strcpy(p3->nome, "Carlos");
    p3->idade = 40;
    list_ins_next(&lista, list_head(&lista), p3);  // insere após p1

    printf("Lista de pessoas:\n");
    imprimir_lista(&lista);

    // Libera todos os nós
    list_destroy(&lista);
    return 0;
}

