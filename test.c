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

// int main()
// {
//     t_data data;
//     pthread_t p[10];
//     int  i = -1;
//     int n = 5;

//     pthread_mutex_t lock;
//     pthread_mutex_init(&lock, NULL);

//     data.lock = &lock;
//     data.n = &n;

//     printf("Before threads: %d\n", n);
//     while (++i < 10)
//     {
//         pthread_create(&p[i], NULL, add_five, &data);
//     }

//     i = -1;
//     while (++i < 10)
//         pthread_join(p[i], NULL);
    
//     printf("After joining threads, n = %d\n", n);

//     pthread_mutex_destroy(&lock);
//     return 0;
// }


#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

/*
** Returns the timestamp in milliseconds
*/

long    get_time(void)
{
    struct timeval  tp;
    long            milliseconds;

    gettimeofday(&tp, NULL);
    milliseconds = tp.tv_sec * 1000;
    milliseconds += tp.tv_usec / 1000;
    return (milliseconds);
}

/*
** Prints time, sleeps 200ms, repeats!
*/

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_time();
	while ((get_time() - start) < milliseconds)
		usleep(500);
	return (0);
}

int main(void)
{
    long start_time;
		
		// Remember when we started
    start_time = get_time();

    int i = -1;
    while (++i < 10 )
    {
				// Print time from start, in ms
        printf("%ld\n", get_time() - start_time);

				// Sleep 200 times 1000 microseconds (1 millisecond)
        usleep(200 * 1000);
    }

    start_time = get_time();

     i = -1;
    while (++i < 10 )
    {
				// Print time from start, in ms
        printf("%ld\n", get_time() - start_time);

				// Sleep 200 times 1000 microseconds (1 millisecond)
        ft_usleep(200);
    }
}