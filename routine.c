#include "philosophers.h"

void *eat(void *arg)
{
    t_philosopher *philo;

    philo = (t_philosopher *)arg; 
    pthread_mutex_lock(philo->left_fork);
    printf("%d has taken a fork\n", philo->id);
    pthread_mutex_lock(philo->right_fork);
    printf("%d has taken a fork\n", philo->id);

    printf("%d is eating\n", philo->id);
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}

static int	create_threads(t_program *program)
{
    int			i;

    i = 0;
    while (i < program->number_of_philosophers)
    {
        if (pthread_create(&program->threads[i], NULL, 
            eat, &program->philosophers[i]))
            return (1);
        i++;
    }
    return (0);
}

