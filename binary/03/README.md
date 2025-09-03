
Ótima pergunta! Para soma e subtração de inteiros gigantes, a escolha da estrutura faz muita diferença.

Recomendação prática (99% dos casos)

Use um vetor/array dinâmico de “limbs” (palavras de máquina):

Cada limb é um inteiro de 32 ou 64 bits (ex.: base ou ).

Guarde os limbs em ordem little-endian (o menos significativo primeiro).

Mantenha um sinal separado (signo + magnitude).

Remova zeros à esquerda (normalização).


Por quê?

Soma/subtração são O(n) com propagação de carry/borrow linear.

Arrays são cache-friendly e rápidos (acesso contíguo na memória).

É exatamente o que usam bibliotecas como GMP/Java BigInteger/Python int.


Esqueleto (pseudocódigo) para soma

function add(a[], b[], B):
    n = max(len(a), len(b))
    carry = 0
    res = new array of size n or n+1
    for i in 0..n-1:
        ai = (i < len(a)) ? a[i] : 0
        bi = (i < len(b)) ? b[i] : 0
        s = ai + bi + carry
        res[i] = s mod B
        carry = s div B
    if carry > 0: append carry
    trim_leading_zeros(res)
    return res

(Subtração é idêntica com borrow; escolha a ordem para sempre subtrair o menor do maior e ajustar o sinal.)

Quando não usar listas ligadas

Listas ligadas (uma célula por dígito/chunk) tornam o acesso disperso:

Pior localidade de cache → mais lento.

Sobrecarga de ponteiros. Úteis só se você precisar de inserções no topo o tempo todo (pouco comum em bigints) ou estiver em linguagem puramente funcional sem vetores mutáveis.


Árvores/estruturas avançadas — casos especiais

Considere árvores balanceadas / B-trees de blocos (ou “segmentos/chunks”) apenas se:

Os números são esparsos (muitos zeros no meio) e você só mexe em blocos localizados.

Precisa concatenar/particionar números gigantes frequentemente (operações estruturais).

O tamanho ultrapassa o que você consegue alocar contiguamente (ambientes muito restritos).


Mesmo assim, para soma/subtração puras, o custo de percorrer a árvore geralmente mata o benefício. Arrays continuam vencendo.

Detalhes que ajudam na prática

Base: use (portável) ou (rápido em 64-bits); para exibição em decimal, converta em base ou só na hora de imprimir.

Endianness: little-endian simplifica propagação de carry/borrow.

Normalização: corte zeros no topo após cada operação.

Sinal: mantenha sign ∈ {−1, +1} e magnitude separada (nada de complemento de dois para arbitrário).

Testes de fronteira: zero, resultados com “underflow” (borrow que vai além), e tamanhos diferentes.


TL;DR

Use vetor dinâmico de palavras (limbs) para soma/subtração de números muito grandes.

Listas: evite (lentas).

Árvores/chunked B-trees: só em casos muito específicos (esparsidade/extremos de memória/edições estruturais).


Se quiser, eu te mostro um mini-exemplo em código (C/C++/Python) com add/sub de bigints em base .

