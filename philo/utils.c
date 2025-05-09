#include "philosophers.h"

void	set_meal_time(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->number_of_meals++;
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->meal_mutex);
}

long long	get_meal_time(t_philosopher *philo)
{
	long long	last_meal_time;

	pthread_mutex_lock(&philo->meal_mutex);
	last_meal_time = philo->last_meal;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (last_meal_time);
}

void	set_death_status(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->program->death_status);
	philo->program->someone_died = 1;
	pthread_mutex_unlock(&philo->program->death_status);
}

int	get_death_status(t_program *prog)
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

	if (get_death_status(philo->program))
		return ;
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
