#include "philosophers.h"

long long get_current_time(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void print_status(t_philosopher *philo, char *status)
{
    long long current_time;

    current_time = get_current_time() - philo->program->start_time;
    printf("%lld %d %s\n", current_time, philo->id + 1, status);
}

 void	eat(t_philosopher *philo)
{
    pthread_mutex_lock(philo->left_fork);
    print_status(philo, "has taken a fork");
    pthread_mutex_lock(philo->right_fork);
    print_status(philo, "has taken a fork");
    
    print_status(philo, "is eating");
    philo->last_meal = get_current_time();
    usleep(philo->program->time_to_eat * 1000);
    philo->number_of_meals++;
    
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
        if (philo->program->must_eat_count != -1 && 
            philo->number_of_meals >= philo->program->must_eat_count)
            break ;
        print_status(philo, "is sleeping");
        usleep(philo->program->time_to_sleep * 1000);
        print_status(philo, "is thinking");
    }
    return (NULL);
}

static int	create_threads(t_program *program)
{
    int			i;

    program->start_time = get_current_time();
    i = 0;
    while (i < program->number_of_philosophers)
    {
        program->philosophers[i].last_meal = get_current_time();
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
        program->philosophers[i].number_of_meals = 0;
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
