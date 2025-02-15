#include "philosophers.h"


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
