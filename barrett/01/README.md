Esse texto do Sean Barrett descreve uma técnica extremamente
simples, mas muito elegante, usada em computação gráfica
e geometria computacional para percorrer as arestas de um
polígono sem precisar usar:

* `%` (módulo/divisão),
* `if`,
* branches especiais.

A ideia parece pequena, mas ela aparece em código de:

* rasterização,
* clipping,
* triangulação,
* detecção de colisão,
* renderização 2D/3D,
* engines de jogos,
* bibliotecas gráficas.

E ela é muito “estilo C”: reorganizar os dados para evitar custo extra.

---

# 1. O problema

Imagine um polígono definido pelos vértices:

```text
v0, v1, v2, v3
```

Um quadrado, por exemplo.

As arestas são:

```text
(v0,v1)
(v1,v2)
(v2,v3)
(v3,v0)
```

Ou seja:

cada vértice conecta no próximo,
e o último conecta no primeiro.

---

# 2. A solução “óbvia”

Tradicionalmente:

```c
for (i = 0; i < n; ++i) {
    j = (i + 1) % n;
}
```

Para:

```text
n = 4
```

temos:

| i | j |
| - | - |
| 0 | 1 |
| 1 | 2 |
| 2 | 3 |
| 3 | 0 |

---

# 3. O problema escondido

A linha:

```c
(i + 1) % n
```

usa:

* divisão inteira,
* operação módulo.

Historicamente isso era caro em CPU.

Mesmo hoje:

* `%` ainda custa mais que soma/comparação,
* pode impedir otimizações,
* pode atrapalhar vetorização/SIMD,
* em loops gráficos isso importa.

Além disso, existe um caso especial:

```text
3 -> 0
```

Todos os outros são:

```text
i -> i+1
```

Só o último “dá a volta”.

---

# 4. A observação genial

Sean Barrett percebe:

> Se existe UM caso especial,
> coloque ele primeiro.

Em vez de enumerar:

```text
0->1
1->2
2->3
3->0
```

enumeramos:

```text
3->0
0->1
1->2
2->3
```

Agora tudo fica uniforme.

---

# 5. A nova ideia

Código:

```c
int i, j;

i = n - 1;

for (j = 0; j < n; ++j) {
    // edge = <i, j>
    i = j;
}
```

---

# 6. Simulação passo a passo

## Estado inicial

```text
i = 3
j = 0
```

Primeira aresta:

```text
3 -> 0
```

Depois:

```c
i = j;
```

fica:

```text
i = 0
```

---

## Segunda iteração

```text
j = 1
```

aresta:

```text
0 -> 1
```

---

## Terceira

```text
1 -> 2
```

---

## Quarta

```text
2 -> 3
```

Fim.

---

# 7. O que aconteceu matematicamente?

A variável:

```text
i
```

fica “atrasada” uma iteração.

Ela sempre guarda:

> o vértice anterior.

Enquanto:

```text
j
```

é o vértice atual.

---

# 8. Por que isso é elegante?

Porque elimina:

* `%`,
* `if`,
* branch,
* caso especial no final.

Tudo vira:

```c
i = j;
```

Que é extremamente barato.

---

# 9. Forma compacta

Em C:

```c
for (i=n-1, j=0; j<n; i=j++) {
    ...
}
```

Isso parece estranho à primeira vista.

Vamos destrinchar.

---

# 10. Entendendo `i=j++`

Lembre:

```c
j++
```

usa o valor antigo e depois incrementa.

Então:

```c
i = j++;
```

equivale a:

```c
i = j;
j = j + 1;
```

---

# 11. Execução real

## Inicialmente

```text
i = 3
j = 0
```

Loop:

```text
edge = (3,0)
```

fim da iteração:

```c
i = j++;
```

vira:

```text
i = 0
j = 1
```

Próxima:

```text
edge = (0,1)
```

e assim por diante.

---

# 12. A versão mais comum “na prática”

Muito código usa:

```c
for (j=n-1, i=0; i<n; j=i++) {
    edge = <j,i>;
}
```

Agora:

* `i` é o atual,
* `j` é o anterior.

Fica mais natural para leitura.

---

# 13. Visualização mental

Imagine um ponteiro “seguindo” o outro:

```text
j --> anterior
i --> atual
```

A cada iteração:

```text
j pega o valor antigo de i
i avança
```

É literalmente uma “bucket brigade”,
uma passagem de bastão.

---

# 14. Por que isso importa em computação gráfica?

Porque polígonos são percorridos o tempo inteiro:

* clipping de linhas,
* Sutherland-Hodgman,
* scanline rasterization,
* convex hull,
* triangulação,
* SAT collision,
* winding tests,
* edge functions.

Em loops de milhões de iterações:

remover `%` e branches pode ser relevante.

Especialmente em:

* CPUs antigas,
* embedded,
* SIMD,
* GPUs,
* software rasterizers.

---

# 15. O princípio geral

O texto termina com uma ideia profunda:

> Se existe apenas um caso excepcional,
> coloque ele no início.

Isso aparece MUITO em programação de alto desempenho.

Exemplos:

* listas encadeadas com nó sentinela,
* buffers circulares,
* parsing,
* alocadores,
* iteradores STL,
* pipelines SIMD.

A ideia é:

```text
transformar "N-1 casos normais + 1 especial"

em

"N casos iguais"
```

---

# 16. Extensão para 3 índices

Exemplo:

```c
int i,j,k;

j = n-1;
k = n-2;

for (i=0; i<n; k=j, j=i++) {
    triple = <i,j,k>;
}
```

Agora temos:

* atual,
* anterior,
* anterior do anterior.

Útil para:

* normais,
* curvatura,
* triangulação,
* suavização,
* filtros geométricos.

---

# 17. Relação com estruturas circulares

Isso é praticamente uma enumeração circular sem módulo.

O padrão aparece em:

* ring buffers,
* filas circulares,
* animações,
* pipelines,
* DSP,
* processamento de sinais.

---

# 18. Exemplo completo com vértices reais

```c
#include <stdio.h>

typedef struct {
    float x, y;
} Vec2;

int main(void)
{
    Vec2 poly[] = {
        {0,0},
        {1,0},
        {1,1},
        {0,1}
    };

    int n = 4;

    int i, j;

    for (j = n - 1, i = 0; i < n; j = i++)
    {
        printf(
            "edge (%f,%f) -> (%f,%f)\n",
            poly[j].x,
            poly[j].y,
            poly[i].x,
            poly[i].y
        );
    }
}
```

Saída:

```text
(0,1) -> (0,0)
(0,0) -> (1,0)
(1,0) -> (1,1)
(1,1) -> (0,1)
```

Observe:

* percorreu o polígono inteiro,
* fechou corretamente,
* sem `%`.

---

# 19. A essência da técnica

A ideia inteira pode ser resumida assim:

```text
em vez de calcular o próximo elemento circularmente,
carregue o anterior entre iterações
```

Ou ainda:

```text
estado incremental > recomputação circular
```

Isso é muito típico de código de engine, gráficos e DSP.

