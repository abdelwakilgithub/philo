#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				i;
	int				nb_philo;
	pthread_t		*thread;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*mutex_print;
	pthread_mutex_t	*count;
	int				*cnt;
	pthread_mutex_t	*key;
	long			*time_last_meal;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	pthread_mutex_t	*meal;
	pthread_mutex_t	*tl_meal;
	int				*nb_meal;
	int				nb_must_eat;
	long			*start_simulation;
}	t_philo;

int		ft_atoi(const char *str);
void	*ft_calloc(size_t count, size_t size);
long	fn_current_time(void);
int	fn_nb_philo_eat(t_philo *ph);
void	fn_philo_die(t_philo *ph, int i);
void	fn_usleep(long start,long time);
void	fn_printf(char *str, long current_time, int i, t_philo *ph);

#endif