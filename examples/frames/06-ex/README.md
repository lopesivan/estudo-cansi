## ✅ Objetivo

- **Capturar os frames do vídeo**
- **Armazená-los em memória** (como buffers de imagem RGB)
- Após o processamento completo:
  - **Salvar todos os frames armazenados em disco** (em um segundo loop)

---

## 🧠 Estratégia

1. Criamos uma **estrutura `ImagemRGB`** para armazenar:
   - Dados do frame RGB (em heap)
   - Dimensões (largura e altura)
   - ID lógico (`frame_id`)
2. Criamos uma `List imagens_em_memoria` usando `list.c`
3. Ao capturar o frame no intervalo desejado:
   - Alocamos memória, copiamos os dados RGB para `ImagemRGB`
   - Inserimos na lista
4. Após a leitura completa do vídeo:
   - Iteramos sobre a lista e salvamos cada imagem com `fwrite`

---

## ✅ Estrutura sugerida:

```c
typedef struct {
    int id;             /* ID do frame (para nome do arquivo) */
    int largura;
    int altura;
    uint8_t *dados;     /* buffer RGB mallocado (largura * altura * 3) */
} ImagemRGB;
```

---

## ✅ Quer que eu escreva agora o código completo com isso?
Inclui:

- Criação de `ImagemRGB`
- Armazenamento na lista
- Salvamento posterior
