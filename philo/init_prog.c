#include "philosophers.h"

long  long  get_current_time(void)
{
    struct timeval  tp;
    long            milliseconds;

    gettimeofday(&tp, NULL);
    milliseconds = tp.tv_sec * 1000;
    milliseconds += tp.tv_usec / 1000;
    return (milliseconds);
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(500);
	return (0);
}

int	is_someone_dead(t_program *prog)
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

	pthread_mutex_lock(&philo->program->print);
	if (!is_someone_dead(philo->program))
	{
		current_time = get_current_time() - philo->program->start_time;
		printf("%lld %d %s\n", current_time, philo->id + 1, status);
	}
	pthread_mutex_unlock(&philo->program->print);
}

long long	get_lastmeal_time(t_philosopher *philo)
{
	long long	last_meal;

	pthread_mutex_lock(&philo->meal_mutex);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (last_meal);
}

int	get_number_of_meals(t_philosopher *philo)
{
	int	num_of_meals;

	pthread_mutex_lock(&philo->meal_mutex);
	num_of_meals = philo->number_of_meals;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (num_of_meals);
}

void	*death_monitor(void *arg)
{
	t_program		*prog;
	t_philosopher	*philo;
	int				i;
	int				all_ate;

	prog = (t_program *)arg;
	while (1)
	{
		all_ate = 1;
		i = -1;
		while (++i < prog->number_of_philosophers)
		{
			philo = &prog->philosophers[i];
			if (prog->must_eat_count != -1
				&& get_number_of_meals(philo) < prog->must_eat_count)
				all_ate = 0;
			if (get_current_time()
				- get_lastmeal_time(philo) >= prog->time_to_die)
			{
				print_status(philo, "died");
				pthread_mutex_lock(&prog->death_status);
				prog->someone_died = 1;
				pthread_mutex_unlock(&prog->death_status);
				return (NULL);
			}
		}
		if (prog->must_eat_count != -1 && all_ate)
			return (write(1, "all ate \n", 10), NULL);
		ft_usleep(100);
	}
	return (NULL);
}

void	set_nofmeals_time(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_current_time();
	philo->number_of_meals++;
	pthread_mutex_unlock(&philo->meal_mutex);
}
void	eat(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
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
	set_nofmeals_time(philo);
	print_status(philo, "is eating");
	ft_usleep(philo->program->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (philo->id % 2)
		ft_usleep(philo->program->time_to_eat);
	while (!is_someone_dead(philo->program))
	{
		eat(philo);
		if (philo->program->must_eat_count != -1
			&& philo->number_of_meals >= philo->program->must_eat_count)
			break ;
		print_status(philo, "is sleeping");
		ft_usleep(philo->program->time_to_sleep);
		print_status(philo, "is thinking");
	}
	return (NULL);
}

static int	create_threads(t_program *program)
{
	int	i;

	program->start_time = get_current_time();
	i = 0;
	while (i < program->number_of_philosophers)
	{
		program->philosophers[i].last_meal = get_current_time();
		if (pthread_create(&program->threads[i], NULL, philosopher_routine,
				&program->philosophers[i]))
			return (1);
		i++;
	}
	if (pthread_create(&program->monitor, NULL, death_monitor, program))
		return (1);
	return (0);
}

int	init_program(t_program *program)
{
	int	i;

	program->someone_died = 0;
	program->forks = malloc(sizeof(pthread_mutex_t)
			* program->number_of_philosophers);
	program->threads = malloc(sizeof(pthread_t)
			* program->number_of_philosophers);
	program->philosophers = malloc(sizeof(t_philosopher)
			* program->number_of_philosophers);
	if (!program->forks || !program->threads || !program->philosophers)
	{
		if (program->forks)
			free(program->forks);
		if (program->threads)
			free(program->threads);
		if (program->philosophers)
			free(program->philosophers);
		return (1);
	}
	// if (program->number_of_philosophers == 1)
	//     return (handle_one_philosopher(program));
	if (pthread_mutex_init(&program->print, NULL)
		|| pthread_mutex_init(&program->death_status, NULL))
	{
		free(program->forks);
		free(program->threads);
		free(program->philosophers);
		return (1);
	}
	i = 0;
	while (i < program->number_of_philosophers)
	{
		if (pthread_mutex_init(&program->forks[i], NULL)
			|| pthread_mutex_init(&program->philosophers[i].meal_mutex, NULL))
		{
			while (--i >= 0)
			{
				pthread_mutex_destroy(&program->forks[i]);
				pthread_mutex_destroy(&program->philosophers[i].meal_mutex);
			}
			pthread_mutex_destroy(&program->print);
			pthread_mutex_destroy(&program->death_status);
			free(program->forks);
			free(program->threads);
			free(program->philosophers);
			return (1);
		}
		program->philosophers[i].id = i;
		program->philosophers[i].left_fork = &program->forks[i];
		program->philosophers[i].right_fork = &program->forks[(i + 1)
			% program->number_of_philosophers];
		program->philosophers[i].program = program;
		program->philosophers[i].number_of_meals = 0;
		i++;
	}
	return (create_threads(program));
}
void	clean_program(t_program *program)
{
	int	i;

	if (program->threads)
	{
		i = -1;
		while (++i < program->number_of_philosophers)
			pthread_join(program->threads[i], NULL);
		pthread_join(program->monitor, NULL);
		i = -1;
		while (++i < program->number_of_philosophers)
			pthread_mutex_destroy(&program->forks[i]);
		pthread_mutex_destroy(&program->print);
		pthread_mutex_destroy(&program->death_status);
		free(program->forks);
		free(program->threads);
		free(program->philosophers);
	}
}
