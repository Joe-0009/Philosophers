#include "philosophers.h"

void take_forks_3(t_philosopher *philo)
{
    if (philo->left_fork < philo->right_fork)
    {
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");
    }
}

static int	wait_for_turn(t_philosopher *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->program->turn_mutex);
		if (philo->program->current_turn == philo->id)
			break ;
		pthread_mutex_unlock(&philo->program->turn_mutex);
		if (get_death_status(philo->program))
			return (1);
	}
	return (0);
}

static int	release_resources_on_death(t_philosopher *philo)
{
	if (philo->left_fork)
		pthread_mutex_unlock(philo->left_fork);
	if (philo->right_fork)
		pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(&philo->program->turn_mutex);
	return (1);
}

static void	finish_eating_and_update_turn(t_philosopher *philo)
{
	ft_usleep(philo->program->time_to_eat, philo->program);
	if (philo->left_fork)
		pthread_mutex_unlock(philo->left_fork);
	if (philo->right_fork)
		pthread_mutex_unlock(philo->right_fork);
	if (!get_death_status(philo->program))
		philo->program->current_turn = (philo->program->current_turn + 1) % 3;
	pthread_mutex_unlock(&philo->program->turn_mutex);
}

int	eat_3(t_philosopher *philo)
{
	if (get_death_status(philo->program))
		return (1);
	if (wait_for_turn(philo))
		return (1);
	take_forks_3(philo);
	if (get_death_status(philo->program))
		return (release_resources_on_death(philo));
	print_status(philo, "is eating");
	set_meal_time(philo);
	finish_eating_and_update_turn(philo);
	return (0);
}
