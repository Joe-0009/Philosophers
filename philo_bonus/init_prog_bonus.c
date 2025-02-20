#include "philosophers_bonus.h"

long  long  get_current_time(void)
{
    struct timeval  tp;
    long            milliseconds;

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
	printf("%lld %d %s\n", get_current_time(), philo->id + 1, "is eating");
	ft_usleep(philo->program->time_to_eat);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (1)
	{
		eat(philo);
		if (philo->program->must_eat_count != -1
			&& philo->number_of_meals >= philo->program->must_eat_count)
			break ;
		printf("%lld %d %s\n", get_current_time(), philo->id + 1, "is sleeping");
		ft_usleep(philo->program->time_to_sleep);
		printf("%lld %d %s\n", get_current_time(), philo->id + 1, "is thinking");
	}
	return (NULL);
}

int	init_program(t_program *program)
{
	int	i;

	program->someone_died = 0;
	program->philosophers = malloc(sizeof(t_philosopher)
			* program->number_of_philosophers);
	if (!program->philosophers)
	{
		if (program->philosophers)
			free(program->philosophers);
		return (1);
	}
	i = -1;
	while (++i < program->number_of_philosophers)
	{
		program->philosophers[i].id = fork();
		program->philosophers[i].program = program;
		program->philosophers[i].number_of_meals = 0;
	}
	return (0);
}

void run()
{
	int i;

	i = -1;
	while (++i)
	{
		while ()
	}
}
void	clean_program(t_program *program)
{
	// int	i;

	// i = -1;

	free(program->philosophers);
}
