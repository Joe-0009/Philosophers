#include "philosophers.h"

static void	init_group_of_three(t_program *program, int start_idx)
{
	int	i;
	int	philo_idx;

	i = 0;
	while (i < 3 && (start_idx + i) < program->number_of_philosophers)
	{
		philo_idx = start_idx + i;
		program->philosophers[philo_idx].left_fork = &program->forks[philo_idx];
		program->philosophers[philo_idx].right_fork = &program->forks[(philo_idx
				+ 1) % 3];
		i++;
	}
}

static void	init_group_of_two(t_program *program, int start_idx)
{
	int	i;
	int	philo_idx;
	int	other_idx;

	i = 0;
	while (i < 2 && (start_idx + i) < program->number_of_philosophers)
	{
		philo_idx = start_idx + i;
		other_idx = start_idx + ((i + 1) % 2);
		program->philosophers[philo_idx].left_fork = &program->forks[philo_idx];
		program->philosophers[philo_idx].right_fork
			= &program->forks[other_idx];
		i++;
	}
}

static void	init_basic_data(t_program *program)
{
	int	i;
	int	num_philos;

	num_philos = program->number_of_philosophers;
	program->current_turn = 0;
	i = 0;
	while (i < num_philos)
	{
		program->philosophers[i].id = i;
		program->philosophers[i].program = program;
		program->philosophers[i].number_of_meals = 0;
		i++;
	}
}

static void	init_groups(t_program *program)
{
	int	i;
	int	num_philos;

	num_philos = program->number_of_philosophers;
	if (num_philos % 2 != 0)
	{
		init_group_of_three(program, 0);
		i = 3;
		while (i < num_philos)
		{
			init_group_of_two(program, i);
			i += 2;
		}
	}
	else
	{
		i = 0;
		while (i < num_philos)
		{
			init_group_of_two(program, i);
			i += 2;
		}
	}
}

void	init_philosophers(t_program *program)
{
	init_basic_data(program);
	init_groups(program);
}
