NAME = philo_bonus

CC = cc -pthread

CFLAGS = -Wall -Werror -Wextra

SRC = main_bonus.c\
	philo_bonus.c\
	ft_atoi_bonus.c\
	ft_calloc_bonus.c\

OBJ = ${SRC:.c=.o}

all : ${NAME}

${NAME} : ${OBJ}
	${CC} -o $@ $^

clean :
	rm -rf *.o

fclean : clean
	rm -rf ${NAME}
re : fclean all