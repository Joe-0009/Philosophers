#include "philosophers.h"

int	death_check(t_philosopher *philo)
{
	long long	current_time;
	long long	last_meal_time;

	if (get_death_status(philo->program))
		return (1);
	last_meal_time = get_meal_time(philo);
	current_time = get_time();
	if (current_time - last_meal_time >= philo->program->time_to_die)
	{
		if (!get_death_status(philo->program))
		{
			//  printf("diffrence :%llu \n", current_time - last_meal_time);
			set_death_status(philo);
			print_status(philo, "died");
		}
		return (1);
	}
	return (0);
}

static void	take_forks(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
}

int	eat(t_philosopher *philo)
{
	int	dead;

	dead = death_check(philo) || get_death_status(philo->program);
	if (dead)
		return (1);
	take_forks(philo);
	dead = death_check(philo) || get_death_status(philo->program);
	if (dead)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	print_status(philo, "is eating");
	set_meal_time(philo);
	ft_usleep(philo->program->time_to_eat, philo->program);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (0);
}

int	sleep_think_actions(t_philosopher *philo)
{
	if (death_check(philo) || get_death_status(philo->program))
		return (1);
	print_status(philo, "is sleeping");
	ft_usleep(philo->program->time_to_sleep, philo->program);
	if (death_check(philo) || get_death_status(philo->program))
		return (1);
	print_status(philo, "is thinking");
	return (0);
}
