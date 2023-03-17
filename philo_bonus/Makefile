NAME = philo_bonus

CC = cc -pthread 
# -fsanitize=thread

CFLAGS = -Wall -Werror -Wextra

SRC = main_bonus.c\
	stat_philo_bonus.c\
	init_philo_bonus.c\
	utils_bonus.c\
	ft_atoi_bonus.c\
	ft_calloc_bonus.c\
	ft_strjoin_bonus.c\
	ft_itoa_bonus.c\

OBJ = ${SRC:.c=.o}

all : ${NAME}

${NAME} : ${OBJ}
	${CC} -o $@ $^

clean :
	rm -rf *.o

fclean : clean
	rm -rf ${NAME}
re : fclean all