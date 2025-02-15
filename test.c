#include <pthread.h>
#include <stdio.h>

typedef struct s_data
{
    int *n;
    pthread_mutex_t *lock;
} t_data;


void *add_five(void *n)
{
    t_data *data;

    data = (t_data *)n;
    pthread_mutex_lock(data->lock);
    printf("initial value  -> %d \n", *data->n);
    *(data->n) += 5;

    printf("after value  -> %d \n", *(data->n));
    pthread_mutex_unlock(data->lock);
    return NULL;
}

int main()
{
    t_data data;
    pthread_t p[10];
    int  i = -1;
    int n = 5;

    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);

    data.lock = &lock;
    data.n = &n;

    printf("Before threads: %d\n", n);
    while (++i < 10)
    {
        pthread_create(&p[i], NULL, add_five, &data);
    }

    i = -1;
    while (++i < 10)
        pthread_join(p[i], NULL);
    
    printf("After joining threads, n = %d\n", n);

    pthread_mutex_destroy(&lock);
    return 0;
}
