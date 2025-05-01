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

## ✅ Solução

Se você deseja **10 amostras** ao longo do vídeo, e o vídeo
tem `total_frames`, então:

```c
int intervalo = total_frames / 10;
```

Isso garante que você capture aproximadamente um frame a cada
`intervalo`, resultando em 10 capturas espaçadas.

---

## 🧠 Como obter `total_frames` com FFmpeg?

Você pode usar:

```c
int64_t total_frames = pFormatCtx->streams[videoStream]->nb_frames;
```

### ⚠️ Mas atenção:
- **`nb_frames` nem sempre é confiável ou definido**, dependendo do codec.
- Se for 0, você pode usar uma estimativa baseada na duração e FPS:

```c
double duration = pFormatCtx->duration / (double)AV_TIME_BASE; // em segundos
AVRational framerate = pFormatCtx->streams[videoStream]->r_frame_rate;

int64_t total_frames = (int64_t)(duration * av_q2d(framerate));
```

---

## ✅ Inserindo no seu código:

Logo **após abrir o arquivo** e descobrir `videoStream`, adicione:

```c
/* Estima o número total de frames */
int64_t total_frames = pFormatCtx->streams[videoStream]->nb_frames;
if (total_frames == 0) {
    double duration = pFormatCtx->duration / (double)AV_TIME_BASE;
    AVRational framerate = pFormatCtx->streams[videoStream]->r_frame_rate;
    total_frames = (int64_t)(duration * av_q2d(framerate));
}

int intervalo = total_frames / 10;
if (intervalo < 1) intervalo = 1;  /* evita divisão por zero ou intervalo 0 */
```

---

## 📌 Exemplo prático:

Suponha:
- Duração do vídeo = 25 segundos
- FPS = 30
→ total estimado ≈ 750 frames
→ `intervalo = 750 / 10 = 75`

Você então captura 1 frame a cada 75 frames.





