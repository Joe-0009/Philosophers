#include "philosophers_bonus.h"

long long	get_current_time(void)
{
	struct timeval	tp;
	long long		milliseconds;

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

void	safe_print(t_program *program, int id, char *msg)
{
	sem_wait(program->write_lock);
	printf("%lld %d %s\n", get_current_time() - program->start_time,
		id + 1, msg);
	sem_post(program->write_lock);
}
