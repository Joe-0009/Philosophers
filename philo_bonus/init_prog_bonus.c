#include "philosophers_bonus.h"

long long	get_current_time(void)
{
	struct timeval	tp;
	long			milliseconds;

	gettimeofday(&tp, NULL);
	milliseconds = tp.tv_sec * 1000;
	milliseconds += tp.tv_usec / 1000;
	return (milliseconds);
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(500);
	return (0);
}

void	eat(t_philosopher *philo)
{
	philo->last_meal = get_current_time();
	philo->number_of_meals++;
	printf("%lld %d %s\n", get_current_time() - philo->program->start_time,
		philo->id + 1, "is eating");
	ft_usleep(philo->program->time_to_eat);
}

void	*philosopher_routine(t_philosopher *philo)
{
	while (1)
	{
		eat(philo);
		if (philo->program->must_eat_count != -1
			&& philo->number_of_meals >= philo->program->must_eat_count)
			break ;
		printf("%lld %d %s\n", get_current_time() - philo->program->start_time,
			philo->id + 1, "is sleeping");
		ft_usleep(philo->program->time_to_sleep);
		printf("%lld %d %s\n", get_current_time() - philo->program->start_time,
			philo->id + 1, "is thinking");
	}
	return (NULL);
}
int	run(t_program *program)
{
	int	i;

	i = -1;
	while (++i < program->number_of_philosophers)
	{
		program->philosophers[i].id = i;
		program->philosophers[i].process_id = fork();
		program->start_time = get_current_time();
		program->philosophers[i].program = program;
		program->philosophers[i].number_of_meals = 0;
		program->philosophers[i].last_meal = get_current_time();
		if (program->philosophers[i].process_id == 0)
		{
			philosopher_routine(&program->philosophers[i]);
			exit(EXIT_SUCCESS);
		}
		if (get_current_time()
			- program->philosophers[i].last_meal >= program->time_to_die)
		{
			printf("%lld %d %s\n", get_current_time() - program->start_time,
				program->philosophers[i].id + 1, "died");
			program->someone_died = 1;
			return (1);
		}
	}
	return (0);
}

int	init_program(t_program *program)
{
	program->someone_died = 0;
	program->philosophers = malloc(sizeof(t_philosopher)
			* program->number_of_philosophers);
	if (!program->philosophers)
	{
		if (program->philosophers)
			free(program->philosophers);
		return (1);
	}
	return (run(program));
}

void	clean_program(t_program *program)
{
	int	i;

	i = -1;
	while (++i < program->number_of_philosophers)
		waitpid(program->philosophers[i].process_id, NULL, 0);
	free(program->philosophers);
}
