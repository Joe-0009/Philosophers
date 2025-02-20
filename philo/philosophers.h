
#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
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
	pthread_t			monitor;
	t_philosopher		*philosophers;
}						t_program;


long long	get_current_time(void);
void	print_status(t_philosopher *philo, char *status);

void					ft_putstr(char *str);
int						check_args(int ac, char **av);
int						init_program_args(t_program *program, int ac,
							char **av);
void					eat(t_philosopher *philo);
int						init_program(t_program *program);
void					clean_program(t_program *program);
#endif