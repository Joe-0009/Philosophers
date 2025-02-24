
#include "philosophers.h"

static void	set_meal_time(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->number_of_meals++;
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->meal_mutex);
}

int death_check(t_philosopher *philo)
{
    long long current_time;
    long long last_meal_time;
    
	if (is_someone_dead(philo->program))
        return 1;
    current_time = get_current_time();
    pthread_mutex_lock(&philo->meal_mutex);
    last_meal_time = philo->last_meal;
    pthread_mutex_unlock(&philo->meal_mutex);
    
    if (current_time - last_meal_time >= philo->program->time_to_die)
    {
        if (!is_someone_dead(philo->program)) 
		{
        	pthread_mutex_lock(&philo->program->death_status);
            philo->program->someone_died = 1;
        	pthread_mutex_unlock(&philo->program->death_status);
            pthread_mutex_lock(&philo->program->print);
            current_time = get_current_time() - philo->program->start_time;
            printf("%lld %d %s\n", current_time, philo->id + 1, "died");
            pthread_mutex_unlock(&philo->program->print);
        }
        return (1);
    }
    return (0);
}

static void	take_forks(t_philosopher *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->id % 2)
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	else
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	pthread_mutex_lock(first_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(second_fork);
	print_status(philo, "has taken a fork");
}

int	eat(t_philosopher *philo)
{
	if (death_check(philo) || is_someone_dead(philo->program))
		return 1;
	if (!death_check(philo) && !is_someone_dead(philo->program))
		take_forks(philo);
	if (!death_check(philo) && !is_someone_dead(philo->program))
	{
		print_status(philo, "is eating");
		set_meal_time(philo);
		ft_usleep(philo->program->time_to_eat);
	}
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return 0;
}

int		sleep_think_actions(t_philosopher *philo)
{
	if (death_check(philo) || is_someone_dead(philo->program))
		return (1);
	print_status(philo, "is sleeping");
	ft_usleep(philo->program->time_to_sleep);
    if (death_check(philo) || is_someone_dead(philo->program))
		return (1);
	print_status(philo, "is thinking");
	if (philo->id % 2 == 0)
		usleep(1);
	return (0);
}


