#include "frames.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Estrutura de tarefa associada a um frame */
typedef struct
{
    int frame_id;
    char descricao[64];
} Tarefa;

/* Lista de tarefas ativas (usando List do list.c) */
List tarefas;

/* Lista de frames disponíveis (gerenciada com frames.c) */
List pool_frames;

/* Inicializa lista de frames disponíveis */
void inicializar_pool_frames(int total)
{
    int i;
    list_init(&pool_frames, free);
    for (i = total - 1; i >= 0; i--)
    {
        int *num = malloc(sizeof(int));
        *num = i;
        list_ins_next(&pool_frames, NULL, num);
    }
}

/* Imprime os frames disponíveis */
void mostrar_frames_disponiveis(void)
{
    ListElmt *elmt = list_head(&pool_frames);
    printf("Frames disponíveis: ");
    while (elmt != NULL)
    {
        int *f = (int *)list_data(elmt);
        printf("%d ", *f);
        elmt = list_next(elmt);
    }
    printf("\n");
}

/* Cria uma nova tarefa e aloca um frame para ela */
int adicionar_tarefa(const char *descricao)
{
    int frame = alloc_frame(&pool_frames);
    if (frame == -1)
    {
        printf("Nenhum frame disponível para '%s'\n", descricao);
        return -1;
    }

    Tarefa *t = malloc(sizeof(Tarefa));
    t->frame_id = frame;
    strncpy(t->descricao, descricao, sizeof(t->descricao));
    t->descricao[sizeof(t->descricao) - 1] = '\0';

    if (list_ins_next(&tarefas, NULL, t) != 0)
    {
        printf("Erro ao adicionar tarefa.\n");
        free(t);
        free_frame(&pool_frames, frame);
        return -1;
    }

    printf("Tarefa '%s' alocada no frame %d\n", descricao, frame);
    return 0;
}

/* Processa e remove todas as tarefas, liberando os frames */
void processar_e_limpar_tarefas(void)
{
    ListElmt *elmt;

    elmt = list_head(&tarefas);
    while (elmt != NULL)
    {
        Tarefa *t = (Tarefa *)list_data(elmt);
        printf("Processando tarefa: '%s' no frame %d\n", t->descricao,
               t->frame_id);
        free_frame(&pool_frames, t->frame_id);
        elmt = list_next(elmt);
    }

    /* Libera a memória das tarefas */
    list_destroy(&tarefas);
}

int main(void)
{
    /* Inicializa listas */
    list_init(&tarefas, free);
    inicializar_pool_frames(5);

    mostrar_frames_disponiveis();

    /* Simula alocação de tarefas */
    adicionar_tarefa("Renderizar imagem");
    adicionar_tarefa("Executar shader");
    adicionar_tarefa("Transmitir pacote");
    mostrar_frames_disponiveis();

    printf("\n--- Executando tarefas ---\n");
    processar_e_limpar_tarefas();

    mostrar_frames_disponiveis();

    /* Limpa lista de frames */
    list_destroy(&pool_frames);
    return 0;
}
