#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

// Define uma estrutura para a tabela hash
typedef struct hash_table
{
    char* keys[TABLE_SIZE];
    char* values[TABLE_SIZE];
} hash_table;

// Função hash simples que calcula o índice para uma chave
unsigned int hash (const char* key)
{
    unsigned int hashval = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        hashval = key[i] + (hashval << 5) - hashval;
    }
    return hashval % TABLE_SIZE;
}

// Inicializa a tabela hash
void init_table (hash_table* table)
{
    memset (table->keys, 0, sizeof (table->keys));
    memset (table->values, 0, sizeof (table->values));
}

// Insere um par chave-valor na tabela hash
void insert (hash_table* table, const char* key, const char* value)
{
    unsigned int index = hash (key);
    table->keys[index] = strdup (key);
    table->values[index] = strdup (value);
}

// Obtém o valor associado a uma chave na tabela hash
const char* get (hash_table* table, const char* key)
{
    unsigned int index = hash (key);
    return table->values[index];
}

// Remove um par chave-valor da tabela hash
void remove_key (hash_table* table, const char* key)
{
    unsigned int index = hash (key);
    free (table->keys[index]);
    free (table->values[index]);
    table->keys[index] = NULL;
    table->values[index] = NULL;
}

int main (void)
{
    // Cria a tabela hash
    hash_table table;
    init_table (&table);

    // Insere pares chave-valor na tabela hash
    insert (&table, "chave1", "valor1");
    insert (&table, "chave2", "valor2");
    insert (&table, "chave3", "valor3");

    // Obtém o valor associado a uma chave
    printf ("Valor da chave 'chave2': %s\n", get (&table, "chave2"));
    printf ("Valor da chave 'chave3': %s\n", get (&table, "chave3"));

    // Remove um par chave-valor da tabela hash
    remove_key (&table, "chave1");

    // Libera a memória alocada pela tabela hash
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (table.keys[i] != NULL)
        {
            free (table.keys[i]);
            free (table.values[i]);
        }
    }

    return 0;
}
