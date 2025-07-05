#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
    float *a, *b, *c, temp;
    long int i, j, k, size, m;
    struct timeval time1, time2;

    if (argc < 2)
    {
        printf("\n\tUsage:%s <Row of square matrix>\n", argv[0]);
        exit(-1);
    } // if

    size = atoi(argv[1]);
    m = size * size;
    a = (float *)malloc(sizeof(float) * m);
    b = (float *)malloc(sizeof(float) * m);
    c = (float *)malloc(sizeof(float) * m);

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            a[i * size + j] = (float)(rand() % 1000 / 100.0);
            b[i * size + j] = (float)(rand() % 1000 / 100.0);
        }
    }

    gettimeofday(&time1, NULL);
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            c[i * size + j] = 0;
            for (k = 0; k < size; k++)
                c[i * size + j] += a[i * size + k] * b[k * size + j];
        }
    }
    // c[i][j]+=a[i][k]*b[k][j] a按行访问，b按列访问
    // a[0][0]*b[0][0] -> a[0][1]*b[1][0] -> ... -> a[0][n-1]*b[n-1][0]
    // a[1][0]*b[0][1] -> a[1][1]*b[1][1] -> ... -> a[1][n-1]*b[n-1][1]
    
    gettimeofday(&time2, NULL);

    time2.tv_sec -= time1.tv_sec;
    time2.tv_usec -= time1.tv_usec;
    if (time2.tv_usec < 0L)
    {
        time2.tv_usec += 1000000L;
        time2.tv_sec -= 1;
    }

    printf("Executiontime=%ld.%06ld seconds\n", time2.tv_sec, time2.tv_usec);
    return (0);
} // main
