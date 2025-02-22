#include "philosophers_bonus.h"

static void	eat_action(t_philosopher *philo)
{
	sem_wait(philo->program->forks);
	safe_print(philo->program, philo->id, "has taken a fork");
	sem_wait(philo->program->forks);
	safe_print(philo->program, philo->id, "has taken a fork");
	sem_wait(philo->program->death_check);
	philo->last_meal = get_current_time();
	sem_post(philo->program->death_check);
	safe_print(philo->program, philo->id, "is eating");
	ft_usleep(philo->program->time_to_eat);
	philo->number_of_meals++;
	sem_post(philo->program->forks);
	sem_post(philo->program->forks);
}

void	check_death(t_philosopher *philo)
{
	while (1)
	{
		sem_wait(philo->program->death_check);
		if (get_current_time() - philo->last_meal >= philo->program->time_to_die)
		{
			safe_print(philo->program, philo->id, "died");
			exit(1);
		}
		sem_post(philo->program->death_check);
		usleep(100);
	}
}

void	handle_philosopher(t_philosopher *philo)
{
	pid_t	death_checker;

	death_checker = fork();
	if (death_checker == 0)
	{
		check_death(philo);
		exit(0);
	}
	while (1)
	{
		eat_action(philo);
		if (philo->program->must_eat_count != -1 && 
			philo->number_of_meals >= philo->program->must_eat_count)
		{
			kill(death_checker, SIGKILL);
			exit(0);
		}
		safe_print(philo->program, philo->id, "is sleeping");
		ft_usleep(philo->program->time_to_sleep);
		safe_print(philo->program, philo->id, "is thinking");
	}
}

