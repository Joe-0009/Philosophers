#include "philosophers.h"

static int	check_meals_count(t_philosopher *philo)
{
	int	enough_meals;

	enough_meals = 0;
	if (philo->program->must_eat_count != -1)
	{
		pthread_mutex_lock(&philo->meal_mutex);
		if (philo->number_of_meals >= philo->program->must_eat_count)
			enough_meals = 1;
		pthread_mutex_unlock(&philo->meal_mutex);
	}
	return (enough_meals);
}

static int	check_if_all_ate(t_program *prog)
{
	int	i;
	int	all_ate;

	if (prog->must_eat_count == -1)
		return (0);
	i = -1;
	all_ate = 1;
	while (++i < prog->number_of_philosophers)
	{
		if (!check_meals_count(&prog->philosophers[i]))
		{
			all_ate = 0;
			break ;
		}
	}
	return (all_ate);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (philo->id % 2 != 0)
		ft_usleep(10, philo->program);
	while (!death_check(philo) && !get_death_status(philo->program))
	{
		if (eat(philo))
			break ;
		if (philo->program->must_eat_count != -1
			&& check_if_all_ate(philo->program))
		{
			set_death_status(philo);
			break ;
		}
		if (sleep_think_actions(philo))
			break ;
	}
	return (NULL);
}

void	*philosopher_routine_3(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (!death_check(philo) && !get_death_status(philo->program))
	{
		if (eat_3(philo))
			break ;
		if (philo->program->must_eat_count != -1
			&& check_if_all_ate(philo->program))
		{
			set_death_status(philo);
			break ;
		}
		if (sleep_think_actions(philo))
			break ;
	}
	return (NULL);
}
