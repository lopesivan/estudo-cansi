#include <stdio.h>

typedef struct {
    float x, y;
} Vec2;

int main(void)
{
    Vec2 poly[] = {
        {0, 0},
        {1, 0},
        {1, 1},
        {0, 1}
    };
    
    int n = 4;
    
    int i, j;
    
    for (j = n - 1, i = 0; i < n; j = i++) {
        printf(
            "edge (%f,%f) -> (%f,%f)\n",
            poly[j].x,
            poly[j].y,
            poly[i].x,
            poly[i].y
        );
    }
}
