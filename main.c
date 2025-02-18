#include "philosophers.h"

int main(int ac, char **av)
{
    t_program program;
    if (check_args(ac, av))
    {
        exit(EXIT_FAILURE);
    }
    if (init_program_args(&program, ac, av))
    {
        exit(EXIT_FAILURE);
    }
    init_program(&program);
    clean_program(&program);
}