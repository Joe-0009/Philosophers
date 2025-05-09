#include "philosophers.h"

int	init_mutexes(t_program *program)
{
	int	i;

	if (pthread_mutex_init(&program->print, NULL)
		|| pthread_mutex_init(&program->death_status, NULL))
		return (1);
	if (program->number_of_philosophers % 2 != 0)
		pthread_mutex_init(&program->turn_mutex, NULL);
	i = -1;
	while (++i < program->number_of_philosophers)
	{
		if (pthread_mutex_init(&program->forks[i], NULL)
			|| pthread_mutex_init(&program->philosophers[i].meal_mutex, NULL))
		{
			while (--i >= 0)
			{
				pthread_mutex_destroy(&program->forks[i]);
				pthread_mutex_destroy(&program->philosophers[i].meal_mutex);
			}
			return (pthread_mutex_destroy(&program->print),
				pthread_mutex_destroy(&program->death_status), 1);
		}
	}
	return (0);
}

static int	create_threads(t_program *program)
{
	int	i;

	i = 0;
	program->start_time = get_time();
	while (i < program->number_of_philosophers)
	{
		program->philosophers[i].last_meal = get_time();
		if (program->number_of_philosophers % 2 != 0 && i < 3)
		{
			if (pthread_create(&program->threads[i], NULL,
					philosopher_routine_3, &program->philosophers[i]))
				return (1);
		}
		else
		{
			if (pthread_create(&program->threads[i], NULL, philosopher_routine,
					&program->philosophers[i]))
				return (1);
		}
		i++;
	}
	return (0);
}

int	init_program(t_program *program)
{
	program->someone_died = 0;
	program->forks = malloc(sizeof(pthread_mutex_t)
			* program->number_of_philosophers);
	program->threads = malloc(sizeof(pthread_t)
			* program->number_of_philosophers);
	program->philosophers = malloc(sizeof(t_philosopher)
			* program->number_of_philosophers);
	if (!program->forks || !program->threads || !program->philosophers)
	{
		if (program->forks)
			free(program->forks);
		if (program->threads)
			free(program->threads);
		if (program->philosophers)
			free(program->philosophers);
		return (1);
	}
	if (program->number_of_philosophers % 2 != 0)
		program->current_turn = 0;
	if (init_mutexes(program))
		return (free(program->forks), free(program->threads),
			free(program->philosophers), 1);
	return (init_philosophers(program), create_threads(program));
}

void	clean_program(t_program *program)
{
	int	i;

	if (program->threads)
	{
		i = -1;
		while (++i < program->number_of_philosophers)
			pthread_join(program->threads[i], NULL);
		i = -1;
		while (++i < program->number_of_philosophers)
		{
			pthread_mutex_destroy(&program->forks[i]);
			pthread_mutex_destroy(&program->philosophers[i].meal_mutex);
		}
		pthread_mutex_destroy(&program->print);
		pthread_mutex_destroy(&program->death_status);
		if (program->number_of_philosophers % 2 != 0)
		{
			pthread_mutex_destroy(&program->turn_mutex);
		}
		free(program->forks);
		free(program->threads);
		free(program->philosophers);
	}
}
