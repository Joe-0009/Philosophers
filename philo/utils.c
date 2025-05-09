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



void	print_status(t_philosopher *philo, char *status)
{
	long long	current_time;

	if (!death_check(philo))
	{
		if (!get_death_status(philo->program))
		{
			pthread_mutex_lock(&philo->program->print);
			current_time = get_time() - philo->program->start_time;
			printf("%lld %d %s\n", current_time, philo->id + 1, status);
			pthread_mutex_unlock(&philo->program->print);
		}
	}
}
