Nesse exemplo, a função "determinant" recebe como argumento uma matriz de float de tamanho n x n e um inteiro n, que indica a ordem da matriz. A função utiliza o algoritmo de eliminação de Gauss para reduzir a matriz a uma forma triangular superior, e em seguida calcula o determinante como o produto dos elementos da diagonal principal.

No exemplo do programa principal, é criada uma matriz "mat" de tamanho 4x4, e a função "determinant" é chamada passando essa matriz e o valor 4 como argumentos. O resultado é armazenado na variável "det" e impresso no console.

Lembre-se de que o tamanho máximo da matriz é limitado pelo tamanho máximo permitido pelo compilador. No exemplo acima, o tamanho máximo da matriz é 10, mas isso pode variar dependendo do compilador e da máquina em que o programa é executado. Se você precisar trabalhar com matrizes maiores, pode ser necessário utilizar alocação dinâmica de memória.
