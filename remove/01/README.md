! Aqui está um exemplo simples de como criar três processos
para realizar um cálculo em paralelo e obter o resultado mais
rapidamente:

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int result = 0;

    // Criando o primeiro processo filho
    pid_t pid1 = fork();

    if (pid1 == 0) {
        // Código do primeiro processo filho
        for (int i = 1; i <= 1000; i++) {
            result += i;
        }
        return result;
    }

    // Criando o segundo processo filho
    pid_t pid2 = fork();

    if (pid2 == 0) {
        // Código do segundo processo filho
        for (int i = 1001; i <= 2000; i++) {
            result += i;
        }
        return result;
    }

    // Criando o terceiro processo filho
    pid_t pid3 = fork();

    if (pid3 == 0) {
        // Código do terceiro processo filho
        for (int i = 2001; i <= 3000; i++) {
            result += i;
        }
        return result;
    }

    // Aguardando o término dos processos filhos
    int status;
    waitpid(pid1, &status, 0);
    result += WEXITSTATUS(status);

    waitpid(pid2, &status, 0);
    result += WEXITSTATUS(status);

    waitpid(pid3, &status, 0);
    result += WEXITSTATUS(status);

    printf("O resultado final é: %d\n", result);

    return 0;
}
```

Neste exemplo, três processos filhos são criados para calcular a
soma dos números de três faixas diferentes (1 a 1000, 1001 a
2000 e 2001 a 3000). Cada processo filho retorna o resultado
parcial para o processo pai, que realiza a soma final dos
resultados dos três processos filhos.

Observe que o cálculo é dividido em três partes para que cada
processo filho possa realizar a tarefa em paralelo. Ao utilizar
múltiplos processos, o tempo total para executar o cálculo é
reduzido, já que cada processo está trabalhando em uma parte
diferente do problema.

Ao compilar e executar o código acima, você obterá o resultado
final da soma mais rapidamente do que se apenas um único
processo estivesse realizando todo o cálculo.

É importante ressaltar que o uso de processos para paralelizar
tarefas depende da natureza do problema e das características do
sistema em que está sendo executado. Em alguns casos, outras
abordagens, como threads, podem ser mais adequadas.
