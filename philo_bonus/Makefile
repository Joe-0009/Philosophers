NAME = philo_bonus
CC = cc 
CFLAGS = -Wall -Wextra -Werror  
SRCS = main.c \
       args_check_bonus.c \
       init_bonus.c \
	   cleanup_bonus.c \
	   philosopher_bonus.c \
	   time_utils_bonus.c


OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS) philosophers_bonus.h
	$(CC) $(CFLAGS) $(OBJS) -o $@
	
clean : 
	rm -rf $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all

.SECONDARY : $(OBJS)