
#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>

typedef struct s_philosopher
{
    int             id;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    struct s_program *program;
} t_philosopher;


typedef struct s_program
{
    int             number_of_philosophers;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             must_eat_count;
    pthread_mutex_t *forks;
    pthread_t       *threads;
    t_philosopher   *philosophers;
} t_program;



void    ft_putstr(char *str);
int     check_args(int ac, char **av);
int     init_program_args(t_program *program, int ac, char **av);
void    eat(t_philosopher *philo);
int	init_program(t_program *program);
void clean_program(t_program *program);
#endif