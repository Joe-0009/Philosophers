#include "philosophers.h"

int	death_check(t_philosopher *philo)
{
	long long	current_time;
	long long	last_meal_time;

	if (get_death_status(philo->program))
		return (1);
	last_meal_time = get_meal_time(philo);
	current_time = get_time();
	if (current_time - last_meal_time > philo->program->time_to_die)
	{
		if (!get_death_status(philo->program))
		{
			set_death_status(philo);
			pthread_mutex_lock(&philo->program->print);
			current_time = get_time() - philo->program->start_time;
			printf("%lld %d %s\n", current_time, philo->id + 1, "died");
			pthread_mutex_unlock(&philo->program->print);
		}
		return (1);
	}
	return (0);
}

static void	take_forks(t_philosopher *philo, pthread_mutex_t **first_fork,
		pthread_mutex_t **second_fork)
{
	if (philo->id % philo->program->number_of_philosophers < (philo->id + 1)
		% philo->program->number_of_philosophers)
	{
		*first_fork = philo->right_fork;
		*second_fork = philo->left_fork;
	}
	else
	{
		*first_fork = philo->left_fork;
		*second_fork = philo->right_fork;
	}
	pthread_mutex_lock(*first_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(*second_fork);
	print_status(philo, "has taken a fork");
}

int	eat(t_philosopher *philo)
{
	int				dead;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	dead = death_check(philo) || get_death_status(philo->program);
	if (dead)
		return (1);
	take_forks(philo, &first_fork, &second_fork);
	dead = death_check(philo) || get_death_status(philo->program);
	if (dead)
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return (1);
	}
	print_status(philo, "is eating");
	set_meal_time(philo);
	ft_usleep(philo->program->time_to_eat);
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
	return (0);
}

int	sleep_think_actions(t_philosopher *philo)
{
	if (death_check(philo) || get_death_status(philo->program))
		return (1);
	print_status(philo, "is sleeping");
	ft_usleep(philo->program->time_to_sleep);
	if (death_check(philo) || get_death_status(philo->program))
		return (1);
	print_status(philo, "is thinking");
	return (0);
}
