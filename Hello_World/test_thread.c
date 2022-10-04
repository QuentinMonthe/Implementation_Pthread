#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *start_routine(void *arg);

int main(int argc, char *argv[])
{
    int n;
    long i;
    void *result;
    if (argc > 1)
    {
        n = atoi(argv[1]);
        pthread_t ids[n];
        for (i = 0; i < n; i++)
        {
            pthread_create(&ids[i], NULL, start_routine, (void *)i);
        }

        for (i = 0; i < n; i++)
        {
            pthread_join(ids[i], &result);
        }
    }

    return 0;
}

void *start_routine(void *arg)
{
    long val = (long)arg;
    printf("Hello World ! Execution thread %ld \n", val);
    pthread_exit(&val);
}
