
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
