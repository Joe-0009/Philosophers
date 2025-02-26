
#include "philosophers.h"

static int	handle_one_philo(t_program *prog)
{
	long long	start_time;

	prog->start_time = get_current_time();
	start_time = get_current_time() - prog->start_time;
	printf("%lld %d %s\n", start_time, 1, "has taken a fork");
	ft_usleep(prog->time_to_die);
	start_time = get_current_time() - prog->start_time;
	printf("%lld %d %s\n", start_time, 1, "is dead");
	return (0);
}

int main(int ac, char **av)
{
    t_program program;

    if (check_args(ac, av))
        return (EXIT_FAILURE);
    if (init_program_args(&program, ac, av))
        return (EXIT_FAILURE);
    if (program.number_of_philosophers == 1)
        return (handle_one_philo(&program));
    if (init_program(&program))
        return (EXIT_FAILURE);
    
    // Monitor philosophers to detect when simulation is complete
    while (!is_someone_dead(&program))
    {
        // Check if all philosophers have eaten enough
        if (program.must_eat_count != -1)
        {
            int all_ate = 1;
            int i = 0;
            
            while (i < program.number_of_philosophers)
            {
                pthread_mutex_lock(&program.philosophers[i].meal_mutex);
                if (program.philosophers[i].number_of_meals < program.must_eat_count)
                {
                    all_ate = 0;
                    pthread_mutex_unlock(&program.philosophers[i].meal_mutex);
                    break;
                }
                pthread_mutex_unlock(&program.philosophers[i].meal_mutex);
                i++;
            }
            
            if (all_ate)
            {
                pthread_mutex_lock(&program.death_status);
                program.someone_died = 1; // Signal to end the simulation
                pthread_mutex_unlock(&program.death_status);
                break;
            }
        }
        
        // Sleep briefly to avoid high CPU usage
        usleep(1000);
    }
    
    clean_program(&program);
    return (0);
}