Boa! Em C o caminho clássico é implementar um bigint como
um vetor dinâmico de limbs (palavras de 32 ou 64 bits). Vou
te mostrar um exemplo mínimo com soma e subtração em base .


---

Estrutura de dados

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint32_t *limbs; // array de palavras (32 bits cada)
    size_t size;    // número de limbs usados
    int sign;       // +1 ou -1
} BigInt;


---

Criação e liberação

BigInt *bigint_new(size_t n) {
    BigInt *x = malloc(sizeof(BigInt));
    x->limbs = calloc(n, sizeof(uint32_t));
    x->size = n;
    x->sign = 1;
    return x;
}

void bigint_free(BigInt *x) {
    if (x) {
        free(x->limbs);
        free(x);
    }
}


---

Soma de magnitudes (|a| + |b|)

BigInt *bigint_add_abs(const BigInt *a, const BigInt *b) {
    size_t n = (a->size > b->size ? a->size : b->size);
    BigInt *res = bigint_new(n + 1);

    uint64_t carry = 0;
    for (size_t i = 0; i < n; i++) {
        uint64_t ai = (i < a->size) ? a->limbs[i] : 0;
        uint64_t bi = (i < b->size) ? b->limbs[i] : 0;

        uint64_t s = ai + bi + carry;
        res->limbs[i] = (uint32_t)(s & 0xFFFFFFFFu);
        carry = s >> 32;
    }
    if (carry) {
        res->limbs[n] = (uint32_t)carry;
        res->size = n + 1;
    } else {
        res->size = n;
    }
    return res;
}


---

Subtração de magnitudes (|a| - |b|, assumindo |a| ≥ |b|)

BigInt *bigint_sub_abs(const BigInt *a, const BigInt *b) {
    BigInt *res = bigint_new(a->size);

    int64_t borrow = 0;
    for (size_t i = 0; i < a->size; i++) {
        int64_t ai = a->limbs[i];
        int64_t bi = (i < b->size) ? b->limbs[i] : 0;
        int64_t s = ai - bi - borrow;

        if (s < 0) {
            s += ((int64_t)1 << 32);
            borrow = 1;
        } else {
            borrow = 0;
        }
        res->limbs[i] = (uint32_t)(s & 0xFFFFFFFFu);
    }

    // remover zeros à esquerda
    size_t n = a->size;
    while (n > 1 && res->limbs[n-1] == 0) {
        n--;
    }
    res->size = n;
    return res;
}


---

Exemplo de uso

int main(void) {
    // Exemplo: 2^32 + 1
    BigInt *a = bigint_new(1);
    a->limbs[0] = 1; // número = 1

    BigInt *b = bigint_new(2);
    b->limbs[0] = 0; // número = 2^32
    b->limbs[1] = 1;

    BigInt *soma = bigint_add_abs(a, b);

    printf("Resultado soma: ");
    for (ssize_t i = soma->size - 1; i >= 0; i--) {
        printf("%08X ", soma->limbs[i]); // imprime em hex
    }
    printf("\n");

    bigint_free(a);
    bigint_free(b);
    bigint_free(soma);
    return 0;
}


---

Saída esperada

Resultado soma: 00000001 00000001

Isto representa o valor .


---

👉 Esse é só o esqueleto básico. Em um bigint “de verdade” você ainda precisaria:

Gerenciar o sinal (decidir quando usar add/sub dependendo dos sinais).

Implementar comparação de magnitudes (para decidir se faz |a|-|b| ou |b|-|a|).

Funções para converter de/para string decimal.

Multiplicação e divisão (algoritmos mais pesados).

