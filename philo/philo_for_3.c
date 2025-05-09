#include "philosophers.h"

void	take_forks_3(t_philosopher *philo, pthread_mutex_t **first_fork,
		pthread_mutex_t **second_fork)
{
	int	left_fork_id;
	int	right_fork_id;

	left_fork_id = philo->id;
	right_fork_id = (philo->id + 1) % philo->program->number_of_philosophers;
	if (left_fork_id < right_fork_id)
	{
		*first_fork = philo->left_fork;
		*second_fork = philo->right_fork;
	}
	else
	{
		*first_fork = philo->right_fork;
		*second_fork = philo->left_fork;
	}
	pthread_mutex_lock(*first_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(*second_fork);
	print_status(philo, "has taken a fork");
}

static int	wait_for_turn(t_philosopher *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->program->turn_mutex);
		if (philo->program->current_turn == philo->id)
			break ;
		pthread_mutex_unlock(&philo->program->turn_mutex);
		if (death_check(philo) || get_death_status(philo->program))
			return (1);
		ft_usleep(1);
	}
	return (0);
}

static int	release_resources_on_death(t_philosopher *philo,
		pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(&philo->program->turn_mutex);
	return (1);
}

static void	finish_eating_and_update_turn(t_philosopher *philo,
		pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	ft_usleep(philo->program->time_to_eat);
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
	philo->program->current_turn = (philo->program->current_turn + 1) % 3;
	pthread_mutex_unlock(&philo->program->turn_mutex);
}

int	eat_3(t_philosopher *philo)
{
	int				dead;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	dead = death_check(philo) || get_death_status(philo->program);
	if (dead)
		return (1);
	if (wait_for_turn(philo))
		return (1);
	take_forks_3(philo, &first_fork, &second_fork);
	dead = death_check(philo) || get_death_status(philo->program);
	if (dead)
		return (release_resources_on_death(philo, first_fork, second_fork));
	print_status(philo, "is eating");
	set_meal_time(philo);
	finish_eating_and_update_turn(philo, first_fork, second_fork);
	return (0);
}
