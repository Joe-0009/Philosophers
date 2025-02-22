#include "philosophers_bonus.h"

static int	init_semaphores(t_program *program)
{
	sem_unlink("/philo_forks");
	sem_unlink("/philo_write");
	sem_unlink("/philo_death");
	program->forks = sem_open("/philo_forks", O_CREAT, 0644,
			program->number_of_philosophers);
	program->write_lock = sem_open("/philo_write", O_CREAT, 0644, 1);
	program->death_check = sem_open("/philo_death", O_CREAT, 0644, 1);
	if (program->forks == SEM_FAILED || program->write_lock == SEM_FAILED
		|| program->death_check == SEM_FAILED)
		return (1);
	return (0);
}

static void	init_philosopher(t_program *program, int i)
{
	program->philosophers[i].id = i;
	program->philosophers[i].program = program;
	program->philosophers[i].number_of_meals = 0;
	program->philosophers[i].last_meal = get_current_time();
}

int	init_program(t_program *program)
{
	int	i;

	if (init_semaphores(program))
		return (1);
	program->philosophers = malloc(sizeof(t_philosopher)
			* program->number_of_philosophers);
	if (!program->philosophers)
		return (1);
	program->start_time = get_current_time();
	i = -1;
	while (++i < program->number_of_philosophers)
	{
		init_philosopher(program, i);
		program->philosophers[i].process_id = fork();
		if (program->philosophers[i].process_id == -1)
			return (1);
		if (program->philosophers[i].process_id == 0)
		{
			handle_philosopher(&program->philosophers[i]);
			exit(0);
		}
	}
	return (0);
}

