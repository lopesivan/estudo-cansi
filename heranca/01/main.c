
#include <stdio.h>

// Definindo a classe mãe abstrata Mamifero
typedef struct Mamifero {
    void (*falar)(void); // Método abstrato
} Mamifero;

// Definindo a classe filha Gato
typedef struct Gato {
    Mamifero mamifero;
} Gato;

void gato_falar(void) {
    printf("Miau\n");
}

// Definindo a classe filha Cachorro
typedef struct Cachorro {
    Mamifero mamifero;
} Cachorro;

void cachorro_falar(void) {
    printf("Au Au\n");
}

int main() {
    Gato gato = { .mamifero.falar = gato_falar };
    Cachorro cachorro = { .mamifero.falar = cachorro_falar };

    gato.mamifero.falar();     // Saída: Miau
    cachorro.mamifero.falar(); // Saída: Au Au

    return 0;
}

