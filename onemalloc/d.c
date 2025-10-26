#include <stdio.h>
#include <stdlib.h>

typedef double Scalar;

void* criar_array(size_t num_dim, size_t dims[])
{
    if (num_dim < 2) return NULL;

    size_t total_pointers = 0;
    size_t data_elements = 1;

    size_t pointers_at_level = 1;
    for (size_t i = 0; i < num_dim - 1; ++i) {
        pointers_at_level *= dims[i];
        total_pointers += pointers_at_level;
    }

    for (size_t i = 0; i < num_dim; ++i)
        data_elements *= dims[i];

    size_t pointers_bytes = total_pointers * sizeof(void*);
    size_t data_bytes = data_elements * sizeof(Scalar);

    char *mem = malloc(pointers_bytes + data_bytes);
    if (!mem) return NULL;

    void **layers = (void **)mem;
    Scalar *data = (Scalar *)(mem + pointers_bytes);

    void **current_layer = layers;
    void **next_layer = layers + dims[0];

    size_t offset = 0;

    for (size_t i = 0; i < dims[0]; ++i) {
        current_layer[i] = next_layer;
        next_layer += dims[1];

        for (size_t j = 0; j < dims[1]; ++j) {
            if (num_dim == 2) {
                ((Scalar**)current_layer)[i][j] = data + offset;
                offset += dims[num_dim-1];
            } else {
                current_layer[i][j] = data + offset;
                offset += dims[2];
            }
        }
    }

    return layers;
}

int main()
{
    size_t dims3D[] = {3, 3, 3};

    Scalar ***matriz3d = (Scalar ***) criar_array(3, dims3D);

    if (!matriz3d) {
        printf("Erro ao alocar matriz3d\n");
        return 1;
    }

    for (size_t i = 0; i < dims3D[0]; ++i)
        for (size_t j = 0; j < dims3D[1]; ++j)
            for (size_t k = 0; k < dims3D[2]; ++k)
                matriz3d[i][j][k] = (Scalar)(i * 100 + j * 10 + k);

    printf("matriz3d[1][1][1] = %f\n", matriz3d[1][1][1]); // Deve imprimir 111.000000

    free(matriz3d);
    return 0;
}

