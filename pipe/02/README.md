Aqui está um exemplo mais completo de uso de pipes em C com
comentários explicativos:

```c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void main() {
    int pipefds[2];
    char write_msg[256] = "Hello from parent!";
    char read_buffer[256] = {0};

    // Criando o pipe
    if (pipe(pipefds) == -1) {
        perror("pipe");
        return;
    }

    // Imprimindo o PID do processo pai
    printf("Parent PID: %d\n", getpid());

    // Criando o processo filho
    pid_t pid = fork();

    if (pid == -1) {
        // Se a criação do processo filho falhar
        perror("fork");
        return;
    }

    if (pid > 0) {
        // Código do processo pai

        // Fechando a extremidade de leitura do pipe no processo pai
        close(pipefds[0]);

        // Escrevendo mensagem no pipe
        write(pipefds[1], write_msg, strlen(write_msg));

        // Fechando a extremidade de escrita do pipe no processo pai
        close(pipefds[1]);

        // Esperando o processo filho terminar
        wait(NULL);
    } else {
        // Código do processo filho

        // Fechando a extremidade de escrita do pipe no processo filho
        close(pipefds[1]);

        // Lendo a mensagem do pipe
        read(pipefds[0], read_buffer, sizeof(read_buffer));

        // Imprimindo a mensagem lida
        printf("Child PID: %d\n", getpid());
        printf("Got from pipe: %s\n", read_buffer);

        // Fechando a extremidade de leitura do pipe no processo filho
        close(pipefds[0]);
    }
}
```

### Comentários Explicativos

1. **Declaração e Inicialização de Variáveis:**

   ```c
   int pipefds[2];
   char write_msg[256] = "Hello from parent!";
   char read_buffer[256] = {0};
   ```

   - `pipefds[2]`: Array que armazenará os descritores de arquivo do pipe (um para leitura e outro para escrita).
   - `write_msg`: Mensagem que será escrita no pipe pelo processo pai.
   - `read_buffer`: Buffer para armazenar a mensagem lida do pipe pelo processo filho.

2. **Criação do Pipe:**

   ```c
   if (pipe(pipefds) == -1) {
       perror("pipe");
       return;
   }
   ```

   - `pipe(pipefds)`: Cria um pipe e armazena os descritores de arquivo em `pipefds`. Retorna -1 em caso de erro.

3. **Imprimir o PID do Processo Pai:**

   ```c
   printf("Parent PID: %d\n", getpid());
   ```

   - `getpid()`: Retorna o ID do processo atual.

4. **Criação do Processo Filho:**

   ```c
   pid_t pid = fork();

   if (pid == -1) {
       perror("fork");
       return;
   }
   ```

   - `fork()`: Cria um novo processo. Retorna o PID do filho no processo pai e 0 no processo filho. Retorna -1 em caso de erro.

5. **Código do Processo Pai:**

   ```c
   if (pid > 0) {
       close(pipefds[0]);
       write(pipefds[1], write_msg, strlen(write_msg));
       close(pipefds[1]);
       wait(NULL);
   }
   ```

   - `close(pipefds[0])`: Fecha a extremidade de leitura do pipe no processo pai, pois o pai só escreve.
   - `write(pipefds[1], write_msg, strlen(write_msg))`: Escreve a mensagem no pipe.
   - `close(pipefds[1])`: Fecha a extremidade de escrita do pipe no processo pai após escrever.
   - `wait(NULL)`: Espera o processo filho terminar.

6. **Código do Processo Filho:**

   ```c
   else {
       close(pipefds[1]);
       read(pipefds[0], read_buffer, sizeof(read_buffer));
       printf("Child PID: %d\n", getpid());
       printf("Got from pipe: %s\n", read_buffer);
       close(pipefds[0]);
   }
   ```

   - `close(pipefds[1])`: Fecha a extremidade de escrita do pipe no processo filho, pois o filho só lê.
   - `read(pipefds[0], read_buffer, sizeof(read_buffer))`: Lê a mensagem do pipe.
   - `printf("Child PID: %d\n", getpid())`: Imprime o PID do processo filho.
   - `printf("Got from pipe: %s\n", read_buffer)`: Imprime a mensagem lida do pipe.
   - `close(pipefds[0])`: Fecha a extremidade de leitura do pipe no processo filho após ler.

### Conclusão

Este exemplo mostra como um processo pai pode criar um pipe,
escrever uma mensagem nele e como o processo filho pode ler
essa mensagem do pipe. O código inclui tratamento de erros
e gerenciamento adequado de descritores de arquivo.
