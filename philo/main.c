#include "philosophers.h"

int handle_one_philo(t_program *prog)
{
    long long start_time;

    prog->start_time = get_current_time();
    start_time = get_current_time() - prog->start_time ;
    printf("%lld %d %s\n", start_time, 1, "has taken a fork");
    usleep(prog->time_to_die * 1000);
    start_time = get_current_time() - prog->start_time ;
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
        return (handle_one_philo (&program));
    init_program(&program);
    clean_program(&program);
}