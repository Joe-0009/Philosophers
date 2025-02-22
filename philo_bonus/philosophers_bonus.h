#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>

typedef struct s_philosopher
{
	int				id;
	pid_t			process_id;
	int				number_of_meals;
	long long		last_meal;
	struct s_program	*program;
}					t_philosopher;

typedef struct s_program
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	long long		start_time;
	t_philosopher	*philosophers;
	sem_t			*forks;
	sem_t			*write_lock;
	sem_t			*death_check;
}					t_program;

/* Function prototypes */
long long	get_current_time(void);
int			ft_usleep(size_t milliseconds);
int			check_args(int ac, char **av);
int			init_program_args(t_program *program, int ac, char **av);
int			init_program(t_program *program);
void		clean_program(t_program *program);
void		safe_print(t_program *program, int id, char *msg);
void		handle_philosopher(t_philosopher *philo);
void		check_death(t_philosopher *philo);

#endif

