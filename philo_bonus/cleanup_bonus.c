#include "philosophers_bonus.h"

void	clean_program(t_program *program)
{
	int	i;
	int	status;

	i = -1;
	while (++i < program->number_of_philosophers)
	{
		waitpid(-1, &status, 0);
		if (WEXITSTATUS(status) == 1)
		{
			i = -1;
			while (++i < program->number_of_philosophers)
				kill(program->philosophers[i].process_id, SIGKILL);
			break ;
		}
	}
	sem_close(program->forks);
	sem_close(program->write_lock);
	sem_close(program->death_check);
	sem_unlink("/philo_forks");
	sem_unlink("/philo_write");
	sem_unlink("/philo_death");
	free(program->philosophers);
}