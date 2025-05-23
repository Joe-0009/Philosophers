#include "philosophers.h"

static int	is_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	ft_atoi(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		if (result * sign > INT_MAX || result * sign < INT_MIN)
			return (0);
		str++;
	}
	return (result * sign);
}

int	check_args(int ac, char **av)
{
	int	i;

	if (ac != 5 && ac != 6)
	{
		printf("Error: Wrong number of arguments\n");
		printf("Usage: ./philo number_of_philosophers time_to_die ");
		printf("time_to_eat time_to_sleep [number_of_times_must_eat]\n");
		return (1);
	}
	i = 0;
	while (++i < ac)
	{
		if (!is_number(av[i]))
		{
			printf("Error: Arguments must be positive numbers\n");
			return (1);
		}
	}
	return (0);
}

int	init_program_args(t_program *program, int ac, char **av)
{
	program->number_of_philosophers = ft_atoi(av[1]);
	program->time_to_die = ft_atoi(av[2]);
	program->time_to_eat = ft_atoi(av[3]);
	program->time_to_sleep = ft_atoi(av[4]);
	program->must_eat_count = -1;
	if (ac == 6)
		program->must_eat_count = ft_atoi(av[5]);
	if (program->number_of_philosophers <= 0 || program->time_to_die <= 0
		|| program->time_to_eat <= 0 || program->time_to_sleep <= 0
		|| (ac == 6 && program->must_eat_count <= 0))
	{
		printf("Error: Invalid arguments (must be positive numbers)\n");
		return (1);
	}
	if (program->number_of_philosophers > 200)
	{
		printf("Error: Too many philosophers (maximum is 200)\n");
		return (1);
	}
	return (0);
}
#include "philosophers.h"

static void	init_group_of_three(t_program *program, int start_idx)
{
	int	i;
	int	philo_idx;

	i = 0;
	while (i < 3 && (start_idx + i) < program->number_of_philosophers)
	{
		philo_idx = start_idx + i;
		program->philosophers[philo_idx].left_fork = &program->forks[philo_idx];
		program->philosophers[philo_idx].right_fork = &program->forks[(philo_idx
				+ 1) % 3];
		i++;
	}
}

static void	init_group_of_two(t_program *program, int start_idx)
{
	int	i;
	int	philo_idx;
	int	other_idx;

	i = 0;
	while (i < 2 && (start_idx + i) < program->number_of_philosophers)
	{
		philo_idx = start_idx + i;
		other_idx = start_idx + ((i + 1) % 2);
		program->philosophers[philo_idx].left_fork = &program->forks[philo_idx];
		program->philosophers[philo_idx].right_fork
			= &program->forks[other_idx];
		i++;
	}
}

static void	init_basic_data(t_program *program)
{
	int	i;
	int	num_philos;

	num_philos = program->number_of_philosophers;
	program->current_turn = 0;
	i = 0;
	while (i < num_philos)
	{
		program->philosophers[i].id = i;
		program->philosophers[i].program = program;
		program->philosophers[i].number_of_meals = 0;
		i++;
	}
}

static void	init_groups(t_program *program)
{
	int	i;
	int	num_philos;

	num_philos = program->number_of_philosophers;
	if (num_philos % 2 != 0)
	{
		init_group_of_three(program, 0);
		i = 3;
		while (i < num_philos)
		{
			init_group_of_two(program, i);
			i += 2;
		}
	}
	else
	{
		i = 0;
		while (i < num_philos)
		{
			init_group_of_two(program, i);
			i += 2;
		}
	}
}

void	init_philosophers(t_program *program)
{
	init_basic_data(program);
	init_groups(program);
}
#include "philosophers.h"

int	init_mutexes(t_program *program)
{
	int	i;

	if (pthread_mutex_init(&program->print, NULL)
		|| pthread_mutex_init(&program->death_status, NULL))
		return (1);
	if (program->number_of_philosophers % 2 != 0)
		pthread_mutex_init(&program->turn_mutex, NULL);
	i = -1;
	while (++i < program->number_of_philosophers)
	{
		if (pthread_mutex_init(&program->forks[i], NULL)
			|| pthread_mutex_init(&program->philosophers[i].meal_mutex, NULL))
		{
			while (--i >= 0)
			{
				pthread_mutex_destroy(&program->forks[i]);
				pthread_mutex_destroy(&program->philosophers[i].meal_mutex);
			}
			return (pthread_mutex_destroy(&program->print),
				pthread_mutex_destroy(&program->death_status), 1);
		}
	}
	return (0);
}

static int	create_threads(t_program *program)
{
	int	i;

	i = 0;
	program->start_time = get_time();
	while (i < program->number_of_philosophers)
	{
		program->philosophers[i].last_meal = get_time();
		if (program->number_of_philosophers % 2 != 0 && i < 3)
		{
			if (pthread_create(&program->threads[i], NULL,
					philosopher_routine_3, &program->philosophers[i]))
				return (1);
		}
		else
		{
			if (pthread_create(&program->threads[i], NULL, philosopher_routine,
					&program->philosophers[i]))
				return (1);
		}
		i++;
	}
	return (0);
}

int	init_program(t_program *program)
{
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
	if (program->number_of_philosophers % 2 != 0)
		program->current_turn = 0;
	if (init_mutexes(program))
		return (free(program->forks), free(program->threads),
			free(program->philosophers), 1);
	return (init_philosophers(program), create_threads(program));
}

void	clean_program(t_program *program)
{
	int	i;

	if (program->threads)
	{
		i = -1;
		while (++i < program->number_of_philosophers)
			pthread_join(program->threads[i], NULL);
		i = -1;
		while (++i < program->number_of_philosophers)
		{
			pthread_mutex_destroy(&program->forks[i]);
			pthread_mutex_destroy(&program->philosophers[i].meal_mutex);
		}
		pthread_mutex_destroy(&program->print);
		pthread_mutex_destroy(&program->death_status);
		if (program->number_of_philosophers % 2 != 0)
		{
			pthread_mutex_destroy(&program->turn_mutex);
		}
		free(program->forks);
		free(program->threads);
		free(program->philosophers);
	}
}
#include "philosophers.h"

static int	handle_one_philo(t_program *prog)
{
	long long	start_time;

	prog->start_time = get_time();
	start_time = get_time() - prog->start_time;
	printf("%lld %d %s\n", start_time, 1, "has taken a fork");
	usleep(1000 * (prog->time_to_die, prog);
	start_time = get_time() - prog->start_time;
	printf("%lld %d %s\n", start_time, 1, "is dead");
	return (0);
}

int	main(int ac, char **av)
{
	t_program	program;

	if (check_args(ac, av))
		return (EXIT_FAILURE);
	if (init_program_args(&program, ac, av))
		return (EXIT_FAILURE);
	if (program.number_of_philosophers == 1)
		return (handle_one_philo(&program));
	if (init_program(&program))
		return (EXIT_FAILURE);
	clean_program(&program);
	return (0);
}
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
	if (get_death_status(philo->program))
	{
		pthread_mutex_unlock(*first_fork);
		return ;
	}
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
	usleep(1000 * (philo->program->time_to_eat, philo->program);
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
	return (0);
}

int	sleep_think_actions(t_philosopher *philo)
{
	if (death_check(philo) || get_death_status(philo->program))
		return (1);
	print_status(philo, "is sleeping");
	usleep(1000 * (philo->program->time_to_sleep, philo->program);
	if (death_check(philo) || get_death_status(philo->program))
		return (1);
	print_status(philo, "is thinking");
	return (0);
}
#include "philosophers.h"

void	take_forks_3(t_philosopher *philo, pthread_mutex_t **first_fork,
		pthread_mutex_t **second_fork)
{
	int	left_fork_id;
	int	right_fork_id;

	left_fork_id = philo->id;
	right_fork_id = (philo->id + 1) % 3;
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
		usleep(1000 * (1, philo->program);
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
	usleep(1000 * (philo->program->time_to_eat, philo->program);
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
	if (!get_death_status(philo->program))
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
		usleep(1000 * (10, philo->program);
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
#include "philosophers.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	usleep(1000 * (long time_in_ms, t_program *prog)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < time_in_ms)
	{
		if (get_death_status(prog))
			break ;
		usleep(100);
	}
}
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
		pthread_mutex_lock(&philo->program->print);
		if (!get_death_status(philo->program))
		{
			current_time = get_time() - philo->program->start_time;
			printf("%lld %d %s\n", current_time, philo->id + 1, status);
		}
		pthread_mutex_unlock(&philo->program->print);
	}
}
#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philosopher
{
	int					id;
	int					number_of_meals;
	long long			last_meal;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		meal_mutex;
	struct s_program	*program;
}						t_philosopher;

typedef struct s_program
{
	int					number_of_philosophers;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					must_eat_count;
	long long			start_time;
	int					someone_died;
	pthread_mutex_t		death_status;
	pthread_mutex_t		print;
	pthread_mutex_t		*forks;
	pthread_t			*threads;
	t_philosopher		*philosophers;
	pthread_mutex_t		turn_mutex;
	int					current_turn;
}						t_program;

/* time functions */
long					get_time(void);
void					usleep(1000 * (long time_in_ms, t_program *prog);

/* utility functions */
void					print_status(t_philosopher *philo, char *status);
int						get_death_status(t_program *prog);
void					set_death_status(t_philosopher *philo);
long long				get_meal_time(t_philosopher *philo);
void					set_meal_time(t_philosopher *philo);
/* initialization functions */
int						check_args(int ac, char **av);
int						init_program_args(t_program *program, int ac,
							char **av);
int						init_program(t_program *program);
int						init_mutexes(t_program *program);
void					init_philosophers(t_program *program);

/* philosopher actions */
void					take_forks_3(t_philosopher *philo,
							pthread_mutex_t **first_fork,
							pthread_mutex_t **second_fork);
int						eat(t_philosopher *philo);
int						eat_3(t_philosopher *philo);
int						sleep_think_actions(t_philosopher *philo);
int						death_check(t_philosopher *philo);
void					*philosopher_routine(void *arg);
void					*philosopher_routine_3(void *arg);

/* cleanup functions */
void					clean_program(t_program *program);

#endif