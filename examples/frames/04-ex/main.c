#include "frames.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Quantidade total de frames disponíveis */
#define TOTAL_FRAMES 5

/* Lista de tarefas em execução */
List tarefas_ativas;

/* Lista de frames disponíveis (gerenciado por frames.c) */
List pool_frames;

/* Representa uma tarefa com tempo e frame associado */
typedef struct
{
    int frame_id;       /* Número do frame alocado */
    char descricao[64]; /* Descrição da tarefa */
    int tempo;          /* Simulação de "tempo" de execução */
} Tarefa;

/* Imprime os frames disponíveis */
void mostrar_frames(void)
{
    ListElmt *elmt = list_head(&pool_frames);
    printf("Frames disponíveis: ");
    while (elmt != NULL)
    {
        int *num = (int *)list_data(elmt);
        printf("%d ", *num);
        elmt = list_next(elmt);
    }
    printf("\n");
}

/* Simula a criação e alocação de uma nova tarefa */
void criar_tarefa(const char *descricao, int tempo)
{
    int frame_id = alloc_frame(&pool_frames);
    if (frame_id == -1)
    {
        printf("Não há frame disponível para a tarefa '%s'\n", descricao);
        return;
    }

    Tarefa *t = malloc(sizeof(Tarefa));
    t->frame_id = frame_id;
    t->tempo = tempo;
    strncpy(t->descricao, descricao, sizeof(t->descricao));
    t->descricao[sizeof(t->descricao) - 1] = '\0';

    if (list_ins_next(&tarefas_ativas, NULL, t) != 0)
    {
        printf("Erro ao adicionar tarefa\n");
        free(t);
        free_frame(&pool_frames, frame_id);
        return;
    }

    printf("Tarefa '%s' alocada no frame %d (tempo=%d)\n", t->descricao,
           frame_id, tempo);
}

/* Simula execução das tarefas */
void processar_tarefas(void)
{
    ListElmt *elmt = list_head(&tarefas_ativas);
    ListElmt *prev = NULL;

    while (elmt != NULL)
    {
        Tarefa *t = (Tarefa *)list_data(elmt);
        t->tempo--;

        if (t->tempo <= 0)
        {
            printf("Tarefa '%s' concluída. Liberando frame %d\n",
                   t->descricao, t->frame_id);
            free_frame(&pool_frames, t->frame_id);
            free(t);

            if (prev == NULL)
            {
                list_rem_next(&tarefas_ativas, NULL, (void **)&t);
                elmt = list_head(&tarefas_ativas);
            }
            else
            {
                list_rem_next(&tarefas_ativas, prev, (void **)&t);
                elmt = list_next(prev);
            }
        }
        else
        {
            prev = elmt;
            elmt = list_next(elmt);
        }
    }
}

int main(void)
{
    int tick;

    /* Inicializa as listas */
    list_init(&pool_frames, free);
    list_init(&tarefas_ativas, free);

    /* Cria os frames disponíveis (0 a TOTAL_FRAMES-1) */
    int i = 0;
    for (i = TOTAL_FRAMES - 1; i >= 0; i--)
    {
        int *num = malloc(sizeof(int));
        *num = i;
        list_ins_next(&pool_frames, NULL, num);
    }

    printf("Sistema iniciado com %d frames disponíveis.\n", TOTAL_FRAMES);

    /* Loop de simulação (ticks de tempo) */
    for (tick = 0; tick < 10; tick++)
    {
        printf("\n==== TICK %d ====\n", tick);

        /* A cada 2 ticks, criar nova tarefa */
        if (tick % 2 == 0)
        {
            char nome[64];
            snprintf(nome, sizeof(nome), "tarefa_tick_%d", tick);
            criar_tarefa(nome, 2 + (tick % 3)); /* tempo variável */
        }

        /* Processar tarefas em andamento */
        processar_tarefas();

        /* Mostrar frames disponíveis */
        mostrar_frames();

        /* Pequena pausa simulada */
        usleep(500000); /* 0.5 segundos */
    }

    /* Limpeza final */
    list_destroy(&tarefas_ativas);
    list_destroy(&pool_frames);
    return 0;
}
