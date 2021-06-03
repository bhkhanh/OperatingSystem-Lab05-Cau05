#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
// w = x1 * x2;     (a)
// v = x3 * x4;     (b)
// y = v * x5;      (c)
// z = v * x6;      (d)
// y = w * y;       (e)
// z = w * z;       (f)
// ans = y + z;     (g)

int x1;
int x2;
int x3;
int x4;
int x5;
int x6;
int w, v, y, z;
int ans = 0;

sem_t c_Wait_b, d_Wait_b;
sem_t e_Wait_a, e_Wait_c; 
sem_t f_Wait_a, f_Wait_d; 
sem_t g_Wait_e, g_Wait_f;

void *process_a()
{
    w = x1 * x2;
    printf("w = x1 * x2 = %d * %d = %d \n", x1, x2, w);

    sem_post(&e_Wait_a);
    sem_post(&f_Wait_a);

    sleep(1);
}

void *process_b()
{
    v = x3 * x4;
    printf("v = x3 * x4 = %d * %d = %d \n", x3, x4, v);

    sem_post(&c_Wait_b);
    sem_post(&d_Wait_b);

    sleep(1);
}

void *process_c()
{
    sem_wait(&c_Wait_b);

    y = v * x5;
    printf("y = v * x5 = %d * %d = %d \n", v, x5, y);

    sem_post(&e_Wait_c);

    sleep(1);
}

void *process_d()
{
    sem_wait(&d_Wait_b);

    z = v * x6;
    printf("z = v * x6 = %d * %d = %d \n", v, x6, z);

    sem_post(&f_Wait_d);

    sleep(1);
}

void *process_e()
{
    sem_wait(&e_Wait_a);
    sem_wait(&e_Wait_c);

    int y_temp = y;
    y = w * y;
    printf("y = w * y = %d * %d = %d \n", w, y_temp, y);

    sem_post(&g_Wait_e);

    sleep(1);
}

void *process_f()
{
    sem_wait(&f_Wait_a);
    sem_wait(&f_Wait_d);

    int z_temp = z;
    z = w * z;
    printf("z = w * z = %d * %d = %d \n", w, z_temp, z);

    sem_post(&g_Wait_f);

    sleep(1);
}

void *process_g()
{
    sem_wait(&g_Wait_e);
    sem_wait(&g_Wait_f);

    ans = y + z;
    printf("ans = y + z = %d + %d = %d \n", y, z, ans);

    sleep(1);
}

void main()
{
    printf("Enter x1: ");
    scanf("%d", &x1);
    printf("Enter x2: ");
    scanf("%d", &x2);
    printf("Enter x3: ");
    scanf("%d", &x3);
    printf("Enter x4: ");
    scanf("%d", &x4);
    printf("Enter x5: ");
    scanf("%d", &x5);
    printf("Enter x6: ");
    scanf("%d", &x6);

    sem_init(&e_Wait_a, 0, 0);
    sem_init(&e_Wait_c, 0, 0);
    sem_init(&c_Wait_b, 0, 0);
    sem_init(&d_Wait_b, 0, 0);
    sem_init(&f_Wait_a, 0, 0);
    sem_init(&f_Wait_d, 0, 0);
    sem_init(&g_Wait_e, 0, 0);
    sem_init(&g_Wait_f, 0, 0);

    pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7;
    pthread_create(&thread1, NULL, process_a, NULL);
    pthread_create(&thread2, NULL, process_b, NULL);
    pthread_create(&thread3, NULL, process_c, NULL);
    pthread_create(&thread4, NULL, process_d, NULL);
    pthread_create(&thread5, NULL, process_e, NULL);
    pthread_create(&thread6, NULL, process_f, NULL);
    pthread_create(&thread7, NULL, process_g, NULL);

    while(1);
}