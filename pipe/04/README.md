Aqui está um exemplo de como um programa em C pode criar um pipe, executar um script Bash que escreve para esse pipe, e depois ler o resultado do pipe:

### Script Bash

Primeiro, vamos criar um script Bash chamado `script.sh` que será executado pelo programa em C e que escreve o resultado do comando `pwd` no pipe.

```bash
#!/bin/bash
# script.sh
pipe_fd=$1
pwd > /proc/self/fd/$pipe_fd
```

Este script Bash recebe o descritor do pipe como argumento e escreve o resultado do comando `pwd` nesse pipe.

### Programa C

Aqui está o programa C que cria um pipe, executa o script Bash e lê o resultado do pipe:

```c
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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
        // Fechando a extremidade de escrita do pipe no processo pai
        close(pipefds[1]);

        // Lendo a mensagem do pipe
        read(pipefds[0], read_buffer, sizeof(read_buffer));
        printf("Current directory: %s", read_buffer);

        // Fechando a extremidade de leitura do pipe no processo pai
        close(pipefds[0]);

        // Esperando o processo filho terminar
        wait(NULL);
    } else {
        // Código do processo filho
        // Fechando a extremidade de leitura do pipe no processo filho
        close(pipefds[0]);

        // Convertendo o descritor de arquivo para string
        char pipe_fd_str[10];
        snprintf(pipe_fd_str, 10, "%d", pipefds[1]);

        // Executando o script Bash
        execl("/bin/bash", "/bin/bash", "script.sh", pipe_fd_str, NULL);

        // Fechando a extremidade de escrita do pipe no processo filho
        close(pipefds[1]);

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
    - `read_buffer`: Buffer para armazenar a mensagem lida do pipe pelo processo pai.

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
        close(pipefds[1]);
        read(pipefds[0], read_buffer, sizeof(read_buffer));
        printf("Current directory: %s", read_buffer);
        close(pipefds[0]);
        wait(NULL);
    }
    ```

    - `close(pipefds[1])`: Fecha a extremidade de escrita do pipe no processo pai, pois o pai só lê.
    - `read(pipefds[0], read_buffer, sizeof(read_buffer))`: Lê a mensagem do pipe.
    - `printf("Current directory: %s", read_buffer)`: Imprime a mensagem lida do pipe (o diretório de trabalho atual).
    - `close(pipefds[0])`: Fecha a extremidade de leitura do pipe no processo pai após ler.
    - `wait(NULL)`: Espera que o processo filho termine.

5. **Código do Processo Filho:**
    ```c
    else {
        close(pipefds[0]);
        char pipe_fd_str[10];
        snprintf(pipe_fd_str, 10, "%d", pipefds[1]);
        execl("/bin/bash", "/bin/bash", "script.sh", pipe_fd_str, NULL);
        close(pipefds[1]);
        exit(EXIT_SUCCESS);
    }
    ```

    - `close(pipefds[0])`: Fecha a extremidade de leitura do pipe no processo filho, pois o filho só escreve.
    - `snprintf(pipe_fd_str, 10, "%d", pipefds[1])`: Converte o descritor de arquivo de escrita do pipe para uma string.
    - `execl("/bin/bash", "/bin/bash", "script.sh", pipe_fd_str, NULL)`: Executa o script Bash, passando o descritor de arquivo de escrita do pipe como argumento.
    - `close(pipefds[1])`: Fecha a extremidade de escrita do pipe no processo filho após escrever.
    - `exit(EXIT_SUCCESS)`: Finaliza o processo filho.

### Execução

1. Salve o script Bash como `script.sh` e torne-o executável:

    ```sh
    chmod +x script.sh
    ```

2. Compile e execute o programa C:

    ```sh
    gcc -o pipe_exec pipe_exec.c
    ./pipe_exec
    ```

O programa C criará um pipe, executará o script Bash e lerá o resultado do comando `pwd` do pipe, exibindo o diretório de trabalho atual.
