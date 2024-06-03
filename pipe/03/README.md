Aqui está um exemplo de código que usa pipes para executar o comando `pwd` e passar o resultado do comando para um processo filho que irá ler e imprimir o diretório de trabalho atual.

### Código

```c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main() {
    int pipefds[2];
    char read_buffer[256] = {0};

    // Criando o pipe
    if (pipe(pipefds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Criando o processo filho
    pid_t pid = fork();

    if (pid == -1) {
        // Se a criação do processo filho falhar
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Código do processo pai
        // Fechando a extremidade de leitura do pipe no processo pai
        close(pipefds[0]);

        // Redirecionando stdout para a extremidade de escrita do pipe
        dup2(pipefds[1], STDOUT_FILENO);

        // Executando o comando `pwd`
        execlp("pwd", "pwd", NULL);

        // Fechando a extremidade de escrita do pipe no processo pai
        close(pipefds[1]);
    } else {
        // Código do processo filho
        // Fechando a extremidade de escrita do pipe no processo filho
        close(pipefds[1]);

        // Lendo a mensagem do pipe
        read(pipefds[0], read_buffer, sizeof(read_buffer));

        // Imprimindo a mensagem lida
        printf("Current directory: %s", read_buffer);

        // Fechando a extremidade de leitura do pipe no processo filho
        close(pipefds[0]);

        // Finalizando o processo filho
        exit(EXIT_SUCCESS);
    }
}
```

### Comentários Explicativos

1. **Declaração e Inicialização de Variáveis:**
    ```c
    int pipefds[2];
    char read_buffer[256] = {0};
    ```

    - `pipefds[2]`: Array que armazenará os descritores de arquivo do pipe (um para leitura e outro para escrita).
    - `read_buffer`: Buffer para armazenar a mensagem lida do pipe pelo processo filho.

2. **Criação do Pipe:**
    ```c
    if (pipe(pipefds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    ```

    - `pipe(pipefds)`: Cria um pipe e armazena os descritores de arquivo em `pipefds`. Retorna -1 em caso de erro.

3. **Criação do Processo Filho:**
    ```c
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    ```

    - `fork()`: Cria um novo processo. Retorna o PID do filho no processo pai e 0 no processo filho. Retorna -1 em caso de erro.

4. **Código do Processo Pai:**
    ```c
    if (pid > 0) {
        close(pipefds[0]); // Fecha a extremidade de leitura do pipe no processo pai
        dup2(pipefds[1], STDOUT_FILENO); // Redireciona stdout para a extremidade de escrita do pipe
        execlp("pwd", "pwd", NULL); // Executa o comando `pwd`
        close(pipefds[1]); // Fecha a extremidade de escrita do pipe no processo pai
    }
    ```

    - `close(pipefds[0])`: Fecha a extremidade de leitura do pipe no processo pai, pois o pai só escreve.
    - `dup2(pipefds[1], STDOUT_FILENO)`: Redireciona a saída padrão (stdout) para a extremidade de escrita do pipe.
    - `execlp("pwd", "pwd", NULL)`: Executa o comando `pwd`, que escreve o diretório de trabalho atual no stdout (agora redirecionado para o pipe).
    - `close(pipefds[1])`: Fecha a extremidade de escrita do pipe no processo pai após escrever.

5. **Código do Processo Filho:**
    ```c
    else {
        close(pipefds[1]); // Fecha a extremidade de escrita do pipe no processo filho
        read(pipefds[0], read_buffer, sizeof(read_buffer)); // Lê a mensagem do pipe
        printf("Current directory: %s", read_buffer); // Imprime a mensagem lida
        close(pipefds[0]); // Fecha a extremidade de leitura do pipe no processo filho
        exit(EXIT_SUCCESS); // Finaliza o processo filho
    }
    ```

    - `close(pipefds[1])`: Fecha a extremidade de escrita do pipe no processo filho, pois o filho só lê.
    - `read(pipefds[0], read_buffer, sizeof(read_buffer))`: Lê a mensagem do pipe.
    - `printf("Current directory: %s", read_buffer)`: Imprime a mensagem lida do pipe (o diretório de trabalho atual).
    - `close(pipefds[0])`: Fecha a extremidade de leitura do pipe no processo filho após ler.
    - `exit(EXIT_SUCCESS)`: Finaliza o processo filho.

Este exemplo demonstra como um processo pai pode executar um comando `pwd` e passar o resultado para um processo filho usando um pipe.
