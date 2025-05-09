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
