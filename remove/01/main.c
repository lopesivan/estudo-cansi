#include <stdio.h>

int main() {
    char *filename = "verde.png";

    if (remove(filename) == 0) {
        printf("Arquivo removido com sucesso.\n");
    } else {
        printf("Erro ao remover o arquivo.\n");
    }

    return 0;
}
