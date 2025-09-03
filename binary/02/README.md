Show 😃 vou te passar um exemplo simples em **C** que
imprime byte a byte a representação de um número inteiro
na memória. Assim você consegue ver se sua máquina está
em **little-endian** ou **big-endian**.

---

### Código C

```c
#include <stdio.h>

int main() {
    unsigned int x = 0x12345678;
    unsigned char *p = (unsigned char*) &x;

    printf("Valor: 0x%X\n", x);
    printf("Bytes em memória:\n");

    for (int i = 0; i < sizeof(x); i++) {
        printf("byte[%d] = 0x%02X\n", i, p[i]);
    }

    return 0;
}
```

---

### O que acontece

1. `x = 0x12345678` → é um inteiro de 4 bytes.
2. Convertemos o endereço de `x` para `unsigned char*` (`p`), para acessar byte a byte.
3. O `for` imprime cada byte em ordem de memória (do endereço mais baixo para o mais alto).

---

### Saída em uma máquina **little-endian** (x86/x86-64):

```
Valor: 0x12345678
Bytes em memória:
byte[0] = 0x78
byte[1] = 0x56
byte[2] = 0x34
byte[3] = 0x12
```

👉 O **LSB (`0x78`) aparece primeiro**.

---

### Saída em uma máquina **big-endian**:

```
Valor: 0x12345678
Bytes em memória:
byte[0] = 0x12
byte[1] = 0x34
byte[2] = 0x56
byte[3] = 0x78
```

👉 O **MSB (`0x12`) aparece primeiro**.

---


