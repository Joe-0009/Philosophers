#include "philosophers.h"

static void	set_meal_time(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->number_of_meals++;
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->meal_mutex);
}
static long long	get_meal_time(t_philosopher *philo)
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
static void    take_forks(t_philosopher *philo, pthread_mutex_t first_fork,
        pthread_mutex_t second_fork)
{
    if (philo->id % philo->program->number_of_philosophers < 
        (philo->id + 1) % philo->program->number_of_philosophers)
    {
        *first_fork = philo->right_fork;
        *second_fork = philo->left_fork;
    }
    else
    {
        *first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    }
    pthread_mutex_lock(first_fork);
    print_status(philo, "has taken a fork");
    pthread_mutex_lock(*second_fork);
    print_status(philo, "has taken a fork");
}

static void	take_forks_3(t_philosopher *philo, pthread_mutex_t **first_fork,
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
int    eat(t_philosopher *philo)
{
    int                dead;
    pthread_mutex_t    *first_fork;
    pthread_mutex_t    *second_fork;

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

int	eat_3(t_philosopher *philo)
{
	int				dead;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	dead = death_check(philo) || get_death_status(philo->program);
	if (dead)
		return (1);
	while (1)
	{
		pthread_mutex_lock(&philo->program->turn_mutex);
		if (philo->program->current_turn == philo->id)
		{
			
			break ;
		}
		pthread_mutex_unlock(&philo->program->turn_mutex);
		
		if (death_check(philo) || get_death_status(philo->program))
			return (1);
		ft_usleep(1); 
	}
	take_forks(philo, &first_fork, &second_fork);
	dead = death_check(philo) || get_death_status(philo->program);
	if (dead)
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(&philo->program->turn_mutex);
		return (1);
	}
	print_status(philo, "is eating");
	set_meal_time(philo);
	ft_usleep(philo->program->time_to_eat);
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
	philo->program->current_turn = (philo->program->current_turn + 1)
		% philo->program->number_of_philosophers;
	pthread_mutex_unlock(&philo->program->turn_mutex);
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
