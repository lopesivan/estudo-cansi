#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

int main(void) {
    const char* dir1 = "a/b/c";
    const char* dir2 = "a/d/e";
    const char* file1 = "a/b/c/file1.txt";
    const char* file2 = "a/d/e/file2.txt";
    const char* mylink = "../../b/c/file1.txt";
    int res;

    // Cria os diretórios pai para a/b/c
    res = mkdir("a", 0777|S_IFDIR|S_IRWXU|S_IRWXG|S_IRWXO);
    if (res != 0 && errno != EEXIST) {
        perror("Erro ao criar o diretório a");
        exit(1);
    }
    res = mkdir("a/b", 0777|S_IFDIR|S_IRWXU|S_IRWXG|S_IRWXO);
    if (res != 0 && errno != EEXIST) {
        perror("Erro ao criar o diretório a/b");
        exit(1);
    }

    res = mkdir("a/d", 0777|S_IFDIR|S_IRWXU|S_IRWXG|S_IRWXO);
    if (res != 0 && errno != EEXIST) {
        perror("Erro ao criar o diretório a/d");
        exit(1);
    }
    // Cria o diretório a/b/c
    res = mkdir(dir1, 0777|S_IFDIR|S_IRWXU|S_IRWXG|S_IRWXO);
    if (res != 0 && errno != EEXIST) {
        perror("Erro ao criar o diretório a/b/c");
        exit(1);
    }

    // Cria o diretório a/d/e
    res = mkdir(dir2, 0777|S_IFDIR|S_IRWXU|S_IRWXG|S_IRWXO);
    if (res != 0 && errno != EEXIST) {
        perror("Erro ao criar o diretório a/d/e");
        exit(1);
    }

    // Cria o arquivo file1.txt em a/b/c
    FILE* fp = fopen(file1, "w");
    if (fp == NULL) {
        perror("Erro ao criar o arquivo file1.txt");
        exit(1);
    }
    fprintf(fp, "Conteudo do arquivo file1.txt\n");
    fclose(fp);

    // Cria o link simbolico entre file1.txt e file2.txt
    res = symlink(mylink, file2);
    if (res != 0) {
        perror("Erro ao criar o link simbolico");
        exit(1);
    }

    printf("Diretorios e arquivos criados com sucesso!\n");
    return 0;
}
