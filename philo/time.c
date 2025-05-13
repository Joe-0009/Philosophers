#include "philosophers.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(long time_in_ms, t_program *prog)
{
	long	start;
	
	start = get_time();
	if (prog->number_of_philosophers > 100)
	{
		while ((get_time() - start) < time_in_ms)
			usleep(200);
	}
	else if (prog->number_of_philosophers > 50)
	{
		while ((get_time() - start) < time_in_ms)
			usleep(100);
	}
	else
	{
		while ((get_time() - start) < time_in_ms)
			usleep(50);
	}
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i] && i < n)
		i++;
	if (n == i)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
