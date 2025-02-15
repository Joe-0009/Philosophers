#include "philosophers.h"

void eat(t_philosopher *philo)
{
    pthread_mutex_lock(philo->left_fork);
    printf("%d has taken a fork\n", philo->id + 1);
    pthread_mutex_lock(philo->right_fork);
    printf("%d has taken a fork\n", philo->id + 1);

    printf("%d is eating\n", philo->id + 1);
    usleep(philo->program->time_to_eat * 1000);

    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}
void	*philosopher_routine(void *arg)
{
    t_philosopher	*philo;

    philo = (t_philosopher *)arg;
    if (philo->id % 2)
        usleep(1000);
    while (1)
    {
        eat(philo);
        printf("%d is sleeping\n", philo->id + 1);
        usleep(philo->program->time_to_sleep * 1000);
        printf("%d is thinking\n", philo->id + 1);
    }
    return (NULL);
}

static int	create_threads(t_program *program)
{
    int			i;

    i = 0;
    while (i < program->number_of_philosophers)
    {
        if (pthread_create(&program->threads[i], NULL, 
            philosopher_routine, &program->philosophers[i]))
            return (1);
        i++;
    }
    return (0);
}



int	init_program(t_program *program)
{
    int	i;

    program->forks = malloc(sizeof(pthread_mutex_t) * program->number_of_philosophers);
    program->threads = malloc(sizeof(pthread_t) * program->number_of_philosophers);
    program->philosophers = malloc(sizeof(t_philosopher) * program->number_of_philosophers);
    if (!program->forks || !program->threads || !program->philosophers)
        return (1);
    i = 0;
    while (i < program->number_of_philosophers)
    {
        if (pthread_mutex_init(&program->forks[i], NULL))
            return (1);
        program->philosophers[i].id = i;
        program->philosophers[i].left_fork = &program->forks[i];
        program->philosophers[i].right_fork = &program->forks[(i + 1) % 
            program->number_of_philosophers];
        program->philosophers[i].program = program; 
        i++;
    }
    return (create_threads(program));
}

void clean_program(t_program *program)
{
    int i;

    i = -1;
    while (++i < program->number_of_philosophers)
    {
        pthread_join(program->threads[i], NULL);
        pthread_mutex_destroy(&program->forks[i]);
    }
    free(program->forks);
    free(program->threads);
    free(program->philosophers);
}
