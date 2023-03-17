/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabdelo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 23:49:24 by moabdelo          #+#    #+#             */
/*   Updated: 2023/03/12 23:49:26 by moabdelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>

typedef struct s_philo
{
	int				i;
	int				nb_philo;
	pthread_t		*thread;
	pid_t			*pid;
	sem_t			*key;
	sem_t			*forks;
	sem_t			*sem_print;
	sem_t			**tl_meal;
	long			*time_last_meal;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				*nb_meal;
	int				nb_must_eat;
	long			*start_simulation;
}	t_philo;

int		ft_atoi(const char *str);
char	*ft_strjoin(char const *s1, char *s2);
char	*ft_itoa(int n);
void	*ft_calloc(size_t count, size_t size);
long	fn_current_time(void);
void	*fn_philo_die(void *ph);
void	fn_usleep(long start, long time);
long	fn_printf(char *str, int i, t_philo *ph);
t_philo	*fn_init_philo(int ac, char **av, int i);
long	fn_ph_eat(t_philo *ph);
void	*fn_stat_ph(void *philo);
void	fn_init_sem(t_philo *ph);
void	fn_close_sem(t_philo *ph);

#endif
