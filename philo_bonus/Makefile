NAME = philo

CC = cc -pthread -fsanitize=thread

CFLAGS = -Wall -Werror -Wextra

SRC = main.c\
	stat_philo.c\
	init_philo.c\
	utils.c\
	ft_atoi.c\
	ft_calloc.c\
	ft_strjoin.c\
	ft_itoa.c\

OBJ = ${SRC:.c=.o}

all : ${NAME}

${NAME} : ${OBJ}
	${CC} -o $@ $^

clean :
	rm -rf *.o

fclean : clean
	rm -rf ${NAME}
re : fclean all