#include <stdio.h>
#include <stdlib.h>

typedef unsigned char        uint8_t;

#define data_fmt             "%8f"
typedef double               Scalar;
typedef Scalar*              Array1d;
typedef Scalar**             Array2d;
typedef Scalar***            Array3d;
typedef Scalar****           Array4d;
typedef Scalar*****          Array5d;
typedef Scalar******         Array6d;
typedef Scalar*******        Array7d;
typedef Scalar********       Array8d;

typedef const unsigned long  Size;

#define INDEX_OF_ARRAY1D(i)                                                       ((i))
#define INDEX_OF_ARRAY2D(dim2, i, j)                                              (INDEX_OF_ARRAY1D(                                        (i))                            *(dim2)    + (j))
#define INDEX_OF_ARRAY3D(dim2, dim3, i, j, k)                                     (INDEX_OF_ARRAY2D((dim2),                                 (i), (j))                       *(dim3)    + (k))
#define INDEX_OF_ARRAY4D(dim2, dim3, dim4, i, j, k, l)                            (INDEX_OF_ARRAY3D((dim2), (dim3),                         (i), (j), (k))                  *(dim4)    + (l))
#define INDEX_OF_ARRAY5D(dim2, dim3, dim4, dim5, i, j, k, l, m)                   (INDEX_OF_ARRAY4D((dim2), (dim3), (dim4),                 (i), (j), (k), (l))             *(dim5)    + (m))
#define INDEX_OF_ARRAY6D(dim2, dim3, dim4, dim5, dim6, i, j, k, l, m, n)          (INDEX_OF_ARRAY5D((dim2), (dim3), (dim4), (dim5),         (i), (j), (k), (l), (m))        *(dim6)    + (n))
#define INDEX_OF_ARRAY7D(dim2, dim3, dim4, dim5, dim6, dim7, i, j, k, l, m, n, o) (INDEX_OF_ARRAY6D((dim2), (dim3), (dim4), (dim5), (dim6), (i), (j), (k), (l), (m), (n))   *(dim7)    + (o))

#define LINEARIZE2D(dim2, i)                                                       (INDEX_OF_ARRAY1D(                                                (i))                              *(dim2))
#define LINEARIZE3D(dim2, dim3, i, j)                                              (INDEX_OF_ARRAY2D((dim2),                                         (i), (j))                         *(dim3))
#define LINEARIZE4D(dim2, dim3, dim4, i, j, k)                                     (INDEX_OF_ARRAY3D((dim2), (dim3),                                 (i), (j), (k))                    *(dim4))
#define LINEARIZE5D(dim2, dim3, dim4, dim5, i, j, k, l)                            (INDEX_OF_ARRAY4D((dim2), (dim3), (dim4),                         (i), (j), (k), (l))               *(dim5))
#define LINEARIZE6D(dim2, dim3, dim4, dim5, dim6, i, j, k, l, m)                   (INDEX_OF_ARRAY5D((dim2), (dim3), (dim4), (dim5),                 (i), (j), (k), (l), (m))          *(dim6))
#define LINEARIZE7D(dim2, dim3, dim4, dim5, dim6, dim7, i, j, k, l, m, n)          (INDEX_OF_ARRAY6D((dim2), (dim3), (dim4), (dim5), (dim6),         (i), (j), (k), (l), (m),(n))      *(dim7))
#define LINEARIZE8D(dim2, dim3, dim4, dim5, dim6, dim7, dim8, i, j, k, l, m, n, o) (INDEX_OF_ARRAY7D((dim2), (dim3), (dim4), (dim5), (dim6), (dim7), (i), (j), (k), (l), (m),(n),(o))  *(dim8))

static
void print_array(Array1d array, Size size) {

    unsigned int i;

    for (i = 0; i < size; i++)
    {

        printf("array[%u] = "data_fmt"\n", i, array[i]);
    }

    return;

}

static
void populate_array2d_w_sequential_numbers(Size    dim1,
                                           Size    dim2,
                                           Array2d a2d)
{

    unsigned int i,
                 j;

    for (i = 0; i < dim1; ++i)
    {
        for (j = 0; j < dim2; ++j)
        {
            a2d[i][j] = (Scalar) INDEX_OF_ARRAY2D(dim2,i,j);
        }
    }

    return;

}


static
void populate_array3d_w_sequential_numbers(Size    dim1,
                                           Size    dim2,
                                           Size    dim3,
                                           Array3d a3d)
{

    unsigned int i,
                 j,
                 k;

    for (i = 0; i < dim1; ++i)
    {
        for (j = 0; j < dim2; ++j)
        {
            for (k = 0; k < dim3; ++k)
            {
                a3d[i][j][k] = (Scalar) INDEX_OF_ARRAY3D(dim2, dim3, i, j, k);
            }
        }
    }

    return;

}


static
void populate_array4d_w_sequential_numbers(Size    dim1,
                                           Size    dim2,
                                           Size    dim3,
                                           Size    dim4,
                                           Array4d a4d)
{

    unsigned int i,
                 j,
                 k,
                 l;

    for (i = 0; i < dim1; ++i)
    {
        for (j = 0; j < dim2; ++j)
        {
            for (k = 0; k < dim3; ++k)
            {
                for (l = 0; l < dim4; ++l)
                {
                    a4d[i][j][k][l] = (Scalar) INDEX_OF_ARRAY4D(dim2, dim3, dim4, i, j, k, l);
                }
            }
        }
    }

    return;

}

static
void populate_array5d_w_sequential_numbers(Size    dim1,
                                           Size    dim2,
                                           Size    dim3,
                                           Size    dim4,
                                           Size    dim5,
                                           Array5d a5d)
{

    unsigned int i,
                 j,
                 k,
                 l,
                 m;

    for (i = 0; i < dim1; ++i)
    {
        for (j = 0; j < dim2; ++j)
        {
            for (k = 0; k < dim3; ++k)
            {
                for (l = 0; l < dim4; ++l)
                {
                    for (m = 0; m < dim5; ++m)
                    {
                        a5d[i][j][k][l][m] = (Scalar) INDEX_OF_ARRAY5D(dim2, dim3, dim4, dim5, i, j, k, l, m);
                    }

                }
            }
        }
    }

    return;

}

static
void populate_array6d_w_sequential_numbers(Size    dim1,
                                           Size    dim2,
                                           Size    dim3,
                                           Size    dim4,
                                           Size    dim5,
                                           Size    dim6,
                                           Array6d a6d)
{

    unsigned int i,
                 j,
                 k,
                 l,
                 m,
                 n;

    for (i = 0; i < dim1; ++i)
    {
        for (j = 0; j < dim2; ++j)
        {
            for (k = 0; k < dim3; ++k)
            {
                for (l = 0; l < dim4; ++l)
                {
                    for (m = 0; m < dim5; ++m)
                    {
                        for (n = 0; n < dim6; ++n)
                        {
                            a6d[i][j][k][l][m][n] = (Scalar) INDEX_OF_ARRAY6D(dim2, dim3, dim4, dim5, dim6, i, j, k, l, m, n);
                        }
                    }
                }
            }
        }
    }

    return;

}

static
void populate_array7d_w_sequential_numbers(Size    dim1,
                                           Size    dim2,
                                           Size    dim3,
                                           Size    dim4,
                                           Size    dim5,
                                           Size    dim6,
                                           Size    dim7,
                                           Array7d a7d)
{

    unsigned int i,
                 j,
                 k,
                 l,
                 m,
                 n,
                 o;

    for (i = 0; i < dim1; ++i)
    {
        for (j = 0; j < dim2; ++j)
        {
            for (k = 0; k < dim3; ++k)
            {
                for (l = 0; l < dim4; ++l)
                {
                    for (m = 0; m < dim5; ++m)
                    {
                        for (n = 0; n < dim6; ++n)
                        {
                            for (o = 0; o < dim7; ++o)
                            {
                                a7d[i][j][k][l][m][n][o] = (Scalar) INDEX_OF_ARRAY7D(dim2, dim3, dim4, dim5, dim6, dim7, i, j, k, l, m, n, o);
                            }
                        }
                    }
                }
            }
        }
    }

    return;

}


Array2d new_2d(Size dim1, Size dim2)
{

    /* layers */
    size_t     layer_1_size =  dim1 * sizeof(Array1d);

    size_t     layers_size  = layer_1_size;

    /* data */
    size_t     data_size   = dim1 * dim2 * sizeof(Scalar);

    /* raw of bytes */
    uint8_t    *raw_bytes = (uint8_t *) malloc(layers_size + data_size);

    if (raw_bytes == NULL)
        return NULL;

    Array2d array2d = (Array2d)raw_bytes;

    unsigned int i;

    for(i = 0; i < dim1; ++i)
    {
        array2d[i] = (Array1d)(array2d + dim1)
                     + LINEARIZE2D(dim2, i);
    }

    return array2d;

}

Array3d new_3d(Size dim1, Size dim2, Size dim3)
{

    /* layers */
    size_t     layer_1_size =  dim1 * sizeof(Array1d);
    size_t     layer_2_size =  dim1 * dim2 * sizeof(Array2d);

    size_t     layers_size = layer_1_size + layer_2_size;

    /* data */
    size_t     data_size   = dim1 * dim2 * dim3 * sizeof(Scalar);

    /* raw of bytes */
    uint8_t    *raw_bytes = (uint8_t *) malloc(layers_size + data_size);

    if (raw_bytes == NULL)
        return NULL;

    unsigned int i,
                 j;

    Array3d    array3d = (Array3d)raw_bytes;

    for(i = 0; i < dim1; ++i)
    {
        array3d[i] = (Array2d)(array3d + dim1)
                     + LINEARIZE2D(dim2, i);

        for(j = 0; j < dim2; ++j)
        {
            array3d[i][j] =
                    (Array1d)(array3d + dim1 + dim1 * dim2)
                    + LINEARIZE3D(dim2, dim3, i, j);
        }
    }

    return array3d;

}

Array4d new_4d(Size dim1, Size dim2, Size dim3, Size dim4)
{
    /* layers */
    size_t     layer_1_size =  dim1 * sizeof(Array1d);
    size_t     layer_2_size =  dim1 * dim2 * sizeof(Array2d);
    size_t     layer_3_size =  dim1 * dim2 * dim3 * sizeof(Array3d);

    size_t     layers_size  = layer_1_size + layer_2_size + layer_3_size;

    /* data */
    size_t     data_size    = dim1 * dim2 * dim3 * dim4 * sizeof(Scalar);

    /* raw of bytes */
    uint8_t    *raw_bytes = (uint8_t *) malloc(layers_size + data_size);

    if (raw_bytes == NULL)
        return NULL;

    Array4d    array4d = (Array4d)raw_bytes;

    unsigned int i,
                 j,
                 k;

    for(i = 0; i < dim1; ++i)
    {
        array4d[i] = (Array3d)(array4d + dim1)
                     + LINEARIZE2D(dim2, i);

        for(j = 0; j < dim2; ++j)
        {
            array4d[i][j] =
                    (Array2d)(array4d + dim1 + dim1 * dim2)
                    + LINEARIZE3D(dim2, dim3, i, j);

            for(k = 0; k < dim3; ++k)
            {
                array4d[i][j][k] =
                        (Array1d)(array4d + dim1 + dim1 * dim2 + dim1 * dim2 * dim3)
                        + LINEARIZE4D(dim2, dim3, dim4, i, j, k);
            }
        }
    }

    return array4d;

}


Array5d new_5d(Size dim1, Size dim2, Size dim3, Size dim4, Size dim5)
{
    /* layers */
    size_t     layer_1_size =  dim1 * sizeof(Array1d);
    size_t     layer_2_size =  dim1 * dim2 * sizeof(Array2d);
    size_t     layer_3_size =  dim1 * dim2 * dim3 * sizeof(Array3d);
    size_t     layer_4_size =  dim1 * dim2 * dim3 * dim4 * sizeof(Array4d);

    size_t     layers_size  = layer_1_size + layer_2_size + layer_3_size + layer_4_size;

    /* data */
    size_t     data_size    = dim1 * dim2 * dim3 * dim4 * dim5 * sizeof(Scalar);

    /* raw of bytes */
    uint8_t    *raw_bytes = (uint8_t *) malloc(layers_size + data_size);

    if (raw_bytes == NULL)
        return NULL;

    Array5d    array5d = (Array5d)raw_bytes;

    unsigned int i,
                 j,
                 k,
                 l;

    for(i = 0; i < dim1; ++i)
    {
        array5d[i] = (Array4d)(array5d + dim1)
                     + LINEARIZE2D(dim2, i);

        for(j = 0; j < dim2; ++j)
        {
            array5d[i][j] =
                    (Array3d)(array5d + dim1 + dim1 * dim2)
                    + LINEARIZE3D(dim2, dim3, i, j);

            for(k = 0; k < dim3; ++k)
            {
                array5d[i][j][k] =
                        (Array2d)(array5d + dim1 + dim1 * dim2 + dim1 * dim2 * dim3)
                        + LINEARIZE4D(dim2, dim3, dim4, i, j, k);

                for(l = 0; l < dim4; ++l)
                {
                    array5d[i][j][k][l] =
                            (Array1d)(array5d + dim1 + dim1 * dim2 + dim1 * dim2 * dim3 + dim1 * dim2 * dim3 * dim4)
                            + LINEARIZE5D(dim2, dim3, dim4, dim5, i, j, k, l);
                }
            }
        }

    }

    return array5d;

}


Array6d new_6d(Size dim1, Size dim2, Size dim3, Size dim4, Size dim5, Size dim6)
{
    /* layers */
    size_t     layer_1_size =  dim1 * sizeof(Array1d);
    size_t     layer_2_size =  dim1 * dim2 * sizeof(Array2d);
    size_t     layer_3_size =  dim1 * dim2 * dim3 * sizeof(Array3d);
    size_t     layer_4_size =  dim1 * dim2 * dim3 * dim4 * sizeof(Array4d);
    size_t     layer_5_size =  dim1 * dim2 * dim3 * dim4 * dim5 * sizeof(Array5d);

    size_t     layers_size  = layer_1_size + layer_2_size + layer_3_size + layer_4_size + layer_5_size;

    /* data */
    size_t     data_size    = dim1 * dim2 * dim3 * dim4 * dim5 * dim6 * sizeof(Scalar);

    /* raw of bytes */
    uint8_t    *raw_bytes = (uint8_t *) malloc(layers_size + data_size);

    if (raw_bytes == NULL)
        return NULL;

    Array6d    array6d = (Array6d)raw_bytes;

    unsigned int i,
                 j,
                 k,
                 l,
                 m;

    for(i = 0; i < dim1; ++i)
    {
        array6d[i] = (Array5d)(array6d + dim1)
                     + LINEARIZE2D(dim2, i);

        for(j = 0; j < dim2; ++j)
        {
            array6d[i][j] =
                    (Array4d)(array6d + dim1 + dim1 * dim2)
                    + LINEARIZE3D(dim2, dim3, i, j);

            for(k = 0; k < dim3; ++k)
            {
                array6d[i][j][k] =
                        (Array3d)(array6d + dim1 + dim1 * dim2 + dim1 * dim2 * dim3)
                        + LINEARIZE4D(dim2, dim3, dim4, i, j, k);

                for(l = 0; l < dim4; ++l)
                {
                    array6d[i][j][k][l] =
                            (Array2d)(array6d + dim1 + dim1 * dim2 + dim1 * dim2 * dim3 + dim1 * dim2 * dim3 * dim4)
                            + LINEARIZE5D(dim2, dim3, dim4, dim5, i, j, k, l);

                    for(m = 0; m < dim5; ++m)
                    {
                        array6d[i][j][k][l][m] =
                                (Array1d)(array6d + dim1 + dim1 * dim2 + dim1 * dim2 * dim3 + dim1 * dim2 * dim3 * dim4 + dim1 * dim2 * dim3 * dim4 * dim5)
                                + LINEARIZE6D(dim2, dim3, dim4, dim5, dim6, i, j, k, l, m);
                    }
                }
            }
        }

    }

    return array6d;

}


Array7d new_7d(Size dim1, Size dim2, Size dim3, Size dim4, Size dim5, Size dim6, Size dim7)
{
    /* layers */
    size_t     layer_1_size =  dim1 * sizeof(Array1d);
    size_t     layer_2_size =  dim1 * dim2 * sizeof(Array2d);
    size_t     layer_3_size =  dim1 * dim2 * dim3 * sizeof(Array3d);
    size_t     layer_4_size =  dim1 * dim2 * dim3 * dim4 * sizeof(Array4d);
    size_t     layer_5_size =  dim1 * dim2 * dim3 * dim4 * dim5 * sizeof(Array5d);
    size_t     layer_6_size =  dim1 * dim2 * dim3 * dim4 * dim5 * dim6 * sizeof(Array5d);

    size_t     layers_size  = layer_1_size + layer_2_size + layer_3_size + layer_4_size + layer_5_size + layer_6_size;

    /* data */
    size_t     data_size    = dim1 * dim2 * dim3 * dim4 * dim5 * dim6 * dim7 * sizeof(Scalar);

    /* raw of bytes */
    uint8_t    *raw_bytes = (uint8_t *) malloc(layers_size + data_size);

    if (raw_bytes == NULL)
        return NULL;

    Array7d    array7d = (Array7d)raw_bytes;

    unsigned int i,
                 j,
                 k,
                 l,
                 m,
                 n;

    for(i = 0; i < dim1; ++i)
    {
        array7d[i] = (Array6d)(array7d + dim1)
                     + LINEARIZE2D(dim2, i);

        for(j = 0; j < dim2; ++j)
        {
            array7d[i][j] =
                    (Array5d)(array7d + dim1 + dim1 * dim2)
                    + LINEARIZE3D(dim2, dim3, i, j);

            for(k = 0; k < dim3; ++k)
            {
                array7d[i][j][k] =
                        (Array4d)(array7d + dim1 + dim1 * dim2 + dim1 * dim2 * dim3)
                        + LINEARIZE4D(dim2, dim3, dim4, i, j, k);

                for(l = 0; l < dim4; ++l)
                {
                    array7d[i][j][k][l] =
                            (Array3d)(array7d + dim1 + dim1 * dim2 + dim1 * dim2 * dim3 + dim1 * dim2 * dim3 * dim4)
                            + LINEARIZE5D(dim2, dim3, dim4, dim5, i, j, k, l);

                    for(m = 0; m < dim5; ++m)
                    {
                        array7d[i][j][k][l][m] =
                                (Array2d)(array7d + dim1 + dim1 * dim2 + dim1 * dim2 * dim3 + dim1 * dim2 * dim3 * dim4 + dim1 * dim2 * dim3 * dim4 * dim5)
                                + LINEARIZE6D(dim2, dim3, dim4, dim5, dim6, i, j, k, l, m);

                        for(n = 0; n < dim6; ++n)
                        {
                            array7d[i][j][k][l][m][n] =
                                    (Array1d)(array7d + dim1 + dim1 * dim2 + dim1 * dim2 * dim3 + dim1 * dim2 * dim3 * dim4 + dim1 * dim2 * dim3 * dim4 * dim5 + dim1 * dim2 * dim3 * dim4 * dim5 * dim6)
                                    + LINEARIZE7D(dim2, dim3, dim4, dim5, dim6, dim7, i, j, k, l, m, n);
                        }
                    }
                }
            }
        }

    }

    return array7d;

}

static
void test() {

    Size    dim1 = 5,
            dim2 = 6,
            dim3 = 6,
            dim4 = 8,
            dim5 = 6,
            dim6 = 5,
            dim7 = 6;

    Array2d a2d = new_2d(dim1, dim2);

    populate_array2d_w_sequential_numbers(dim1, dim2, a2d);

    printf("-- 2d -- \n");
    print_array(*a2d, dim1*dim2);

    free(a2d);


    Array3d a3d = new_3d(dim1, dim2, dim3);

    populate_array3d_w_sequential_numbers(dim1, dim2, dim3, a3d);

    printf("-- 3d -- \n");
    print_array(**a3d, dim1*dim2*dim3);

    free(a3d);

    Array4d a4d = new_4d(dim1, dim2, dim3, dim4);

    populate_array4d_w_sequential_numbers(dim1, dim2, dim3, dim4, a4d);

    printf("-- 4d -- \n");
    print_array(***a4d, dim1*dim2*dim3*dim4);

    free(a4d);

    Array5d a5d = new_5d(dim1, dim2, dim3, dim4, dim5);

    populate_array5d_w_sequential_numbers(dim1, dim2, dim3, dim4, dim5, a5d);

    printf("-- 5d -- \n");
    print_array(****a5d, dim1*dim2*dim3*dim4*dim5);

    free(a5d);

    Array6d a6d = new_6d(dim1, dim2, dim3, dim4, dim5, dim6);

    populate_array6d_w_sequential_numbers(dim1, dim2, dim3, dim4, dim5, dim6, a6d);

    printf("-- 6d -- \n");
    print_array(*****a6d, dim1*dim2*dim3*dim4*dim5*dim6);

    free(a6d);


    Array7d a7d = new_7d(dim1, dim2, dim3, dim4, dim5, dim6, dim7);

    populate_array7d_w_sequential_numbers(dim1, dim2, dim3, dim4, dim5, dim6, dim7, a7d);

    printf("-- 7d -- \n");
    print_array(******a7d, dim1*dim2*dim3*dim4*dim5*dim6*dim7);

    free(a7d);

    printf("-- fim --\n");

    return;

}

/*****************************************************************************
*                                                                            *
*  --------------------------------- main ---------------------------------  *
*                                                                            *
*****************************************************************************/

int main(int argc, char **argv)
{

    test();

    return EXIT_SUCCESS;
}
