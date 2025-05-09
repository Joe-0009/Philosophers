NAME = philo
CC = cc 
CFLAGS = -Wall -Wextra -Werror -pthread -fsanitize=thread  -g3
SRCS = main.c \
       args_check.c \
       init_program.c \
	   philo_actions.c \
	   philo_routine.c \
	   philo_for_3.c \
	   init_philos.c \
	   time.c \
	   utils.c \



OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS) philosophers.h
	$(CC) $(CFLAGS) $(OBJS) -o $@
	
clean : 
	rm -rf $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all

.SECONDARY : $(OBJS)