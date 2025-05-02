#include "philosophers.h"

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
}

int	is_someone_dead(t_program *prog)
{
	int	status;

	pthread_mutex_lock(&prog->death_status);
	status = prog->someone_died;
	pthread_mutex_unlock(&prog->death_status);
	return (status);
}

void	print_status(t_philosopher *philo, char *status)
{
	long long	current_time;

	if (!death_check(philo) && !is_someone_dead(philo->program))
	{
		pthread_mutex_lock(&philo->program->print);
		if (!is_someone_dead(philo->program))
		{
			current_time = get_time() - philo->program->start_time;
			printf("%lld %d %s\n", current_time, philo->id + 1, status);
		}
		pthread_mutex_unlock(&philo->program->print);
	}
}
