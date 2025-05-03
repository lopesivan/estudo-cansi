O código que aplica uma **faixa preta horizontal** no topo
da imagem — e como você pode mudar a cor ou a posição.

---

## 📷 Contexto

Sua imagem está armazenada como um **buffer linear** de bytes RGB:

```
img->dados = [ R G B R G B R G B ... ]   // linha por linha
```

Cada pixel ocupa **3 bytes** (R, G, B).
A imagem está organizada em **modo "row-major"**, ou seja:
os pixels da **linha 0 vêm primeiro**, depois os da linha 1, etc.

---

## 🔍 Análise do código:

```c
for (int y = 0; y < 20 && y < img->altura; y++) {
```

- Percorre as **primeiras 20 linhas da imagem** (ou menos,
se a imagem tiver altura < 20)
- Isso define a **altura da faixa** (20 pixels)

```c
    for (int x = 0; x < img->largura; x++) {
```

- Para cada linha `y`, percorre todas as colunas da imagem (`x = 0 até largura - 1`)
- Ou seja, percorre **cada pixel da faixa horizontal**

```c
        int offset = (y * img->largura + x) * 3;
```

- Calcula a **posição do pixel no buffer linear**
- Fórmula:
  - `y * largura`: salta `y` linhas
  - `+ x`: vai até a coluna `x` na linha atual
  - `* 3`: pois cada pixel tem 3 bytes (RGB)
- O `offset` aponta para o **início do pixel (R)**

```c
        img->dados[offset + 0] = 0;   /* R */
        img->dados[offset + 1] = 0;   /* G */
        img->dados[offset + 2] = 0;   /* B */
```

- Define o pixel na posição `(x, y)` como **preto**:
  - R = 0
  - G = 0
  - B = 0

---

## 🎨 Se quiser mudar a cor

### Por exemplo, para **vermelho**:

```c
img->dados[offset + 0] = 255;   /* R */
img->dados[offset + 1] = 0;     /* G */
img->dados[offset + 2] = 0;     /* B */
```

### Ou um cinza claro:

```c
img->dados[offset + 0] = 200;
img->dados[offset + 1] = 200;
img->dados[offset + 2] = 200;
```

---

## 🔁 E se eu quiser a faixa no **meio** da imagem?

Troque o início e fim do `y`:

```c
int inicio = img->altura / 2 - 10;
int fim = inicio + 20;

for (int y = inicio; y < fim && y < img->altura; y++) {
    for (int x = 0; x < img->largura; x++) {
        int offset = (y * img->largura + x) * 3;
        // cor desejada aqui
    }
}
```

---

## ✅ Conclusão

- O buffer RGB é um vetor linear: linha após linha, pixel após pixel
- Você acessa cada pixel com o cálculo: `(linha * largura + coluna) * 3`
- Cada pixel são 3 bytes: R, G, B
- Modificando os bytes, você desenha na imagem

---

