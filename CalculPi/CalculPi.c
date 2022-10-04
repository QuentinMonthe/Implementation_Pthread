#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *start_routine(void *arg);
float power(int exp, float value);
double calcul_pi(int order);

int n, a = 0;
double value_pi = 0;

//  int flag = 0;

// pthread_cond_t cond_var;
pthread_mutex_t mutex;
int counter = 0;

int main(int argc, char *argv[])
{
    long i;
    void *result;

    if (argc > 1)
    {
        n = atoi(argv[1]);
        pthread_t ids[n];
        do
        {
            if (a != 0 && a < n)
            {
                printf("Erreur ! L'ordre de grandeur du calcul est inferieur au nombre de trhead !\n\tVeuillez entrer l'ordre de grandeur : (? >= %d) : ", n);
            }
            else if (a < n)
            {
                printf("Veuillez entrez l'ordre de grandeur du calcul (? >= %d) : ", n);
            }
            scanf("%d", &a);
        } while (a < n);

        pthread_mutex_init(&mutex, NULL);
        for (i = 0; i < n; i++)
        {
            pthread_create(&ids[i], NULL, start_routine, (void *)i);
        }

        for (i = 0; i < n; i++)
        {
            pthread_join(ids[i], &result);
        }

        double value = calcul_pi(a);
        double erreur = value - value_pi;
        printf("Valeur de Pi par calcul parallelise :  %lf\n", value_pi);
        printf("Valeur de Pi par calcul sequentiel :   %lf\n", value);
        printf("L'erreur obtenu a l'ordre %d est de :  %lf\n", a, erreur);
    }

    return 0;
}

float power(int exp, float value)
{
    if (exp == 0 || value == 1)
    {
        return 1;
    }

    int i;
    double reault_power = 1;
    for (i = 1; i <= exp; i++)
    {
        reault_power *= value;
    }
    return reault_power;
}

void *start_routine(void *arg)
{
    long val = (long)arg;
    int i;
    float result = 0;
    int index_start = val == 0 ? 0 : (val * a) / n + 1;
    int index_end = (val + 1) * a / n;

    for (i = index_start; i <= index_end; i++)
    {
        result += power(i, -1) / (2 * i + 1);
    }

    // Barriere de synchronisation avec variable condition
    // pthread_mutex_lock(&mutex);
    // counter++;
    // if (counter == n)
    // {
    //     counter = 0;
    //     pthread_cond_broadcast(&cond_var);
    // }
    // else
    // {
    //     while (pthread_cond_wait(&cond_var, &mutex) != 0)
    //     {
    //         /* code */
    //     }
    // }
    // pthread_mutex_unlock(&mutex);
    // value_pi += result * 4;
    // printf("Result thread %ld : %f \n\n", val, value_pi);

    // Barriere de synchronisation avec les mutex
    pthread_mutex_lock(&mutex);
    counter++;
    value_pi += result * 4;
    pthread_mutex_unlock(&mutex);
    while (counter < n)
    {
        /* code */
    }
    printf("Result thread %ld : %f \n\n", val, value_pi);

    // Gestion de la synchronisation avec les mutex
    // pthread_mutex_lock(&mutex);
    // value_pi += result * 4;
    // printf("Result thread %ld : %f \n\n", val, value_pi);
    // pthread_mutex_unlock(&mutex);

    // Gestion de la synchronisation avec attente active
    //  while (flag != 0);
    //  flag = 1;
    // value_pi += result * 4;
    // // printf("Result thread %ld : %f \n\n", val, value_pi);
    // flag = 0;

    pthread_exit(&result);
}

double calcul_pi(int order)
{
    int i;
    double value = 0;
    for (i = 0; i <= order; i++)
    {
        value += power(i, -1) / (2 * i + 1);
    }
    return value * 4;
}