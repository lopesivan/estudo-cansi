#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void buscar_arquivos_txt(const char* diretorio)
{
    DIR* dir;
    struct dirent* entrada;
    struct stat info;

    // Abrindo o diretório
    dir = opendir(diretorio);
    if (dir == NULL)
    {
        perror("Erro ao abrir o diretório");
        return;
    }

    // Percorrendo o conteúdo do diretório
    while ((entrada = readdir(dir)) != NULL)
    {
        char caminho[1024];
        snprintf(caminho, sizeof(caminho), "%s/%s", diretorio,
                 entrada->d_name);

        // Ignorando "." e ".."
        if (strcmp(entrada->d_name, ".") == 0 ||
            strcmp(entrada->d_name, "..") == 0)
        {
            continue;
        }

        // Obtendo informações do arquivo ou diretório
        if (stat(caminho, &info) == -1)
        {
            perror("Erro ao obter informações do arquivo");
            continue;
        }

        // Se for um diretório, chamamos a função recursivamente
        if (S_ISDIR(info.st_mode))
        {
            buscar_arquivos_txt(caminho);
        }
        // Se for um arquivo, verificamos a extensão
        else if (S_ISREG(info.st_mode))
        {
            const char* extensao = strrchr(entrada->d_name, '.');
            if (extensao && strcmp(extensao, ".txt") == 0)
            {
                printf("Arquivo encontrado: %s\n", caminho);
            }
        }
    }

    // Fechando o diretório
    closedir(dir);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s <diretório>\n", argv[0]);
        return 1;
    }

    printf("Buscando arquivos .txt no diretório: %s\n", argv[1]);
    buscar_arquivos_txt(argv[1]);

    return 0;
}
