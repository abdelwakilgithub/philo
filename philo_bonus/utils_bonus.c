/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabdelo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 23:49:05 by moabdelo          #+#    #+#             */
/*   Updated: 2023/03/12 23:49:07 by moabdelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long	fn_current_time(void)
{
	struct timeval	tv;
	long			current_time;

	if (gettimeofday(&tv, NULL) == -1)
		exit(1);
	else
		current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (current_time);
}

void	*fn_philo_die(void *philo)
{
	t_philo		*ph;

	ph = (t_philo *)philo;
	sem_post(ph->tl_meal[ph->i]);
	sem_wait(ph->key);
	sem_post(ph->key);
	while (1)
	{
		sem_wait(ph->tl_meal[ph->i]);
		if (ph->nb_must_eat && ph->nb_meal[ph->i] >= ph->nb_must_eat)
		{
			free(ph);
			exit(0);
		}
		if (fn_current_time() - ph->time_last_meal[ph->i] >= ph->time_to_die)
		{
			sem_post(ph->tl_meal[ph->i]);
			sem_wait(ph->sem_print);
			printf("\033[0;37m%ld %d died\n",
				fn_current_time() - ph->start_simulation[0], ph->i + 1);
			free(ph);
			exit(1);
		}
		sem_post(ph->tl_meal[ph->i]);
		fn_usleep(fn_current_time(), 1);
	}
	return (0);
}

void	fn_usleep(long start, long time)
{
	while (1)
	{
		if (fn_current_time() - start >= time)
			break ;
		usleep(100);
	}
}

long	fn_printf(char *str, int i, t_philo *ph)
{
	long	current_time;

	sem_wait(ph->sem_print);
	current_time = fn_current_time();
	printf(str, current_time - ph->start_simulation[0], i + 1);
	sem_post(ph->sem_print);
	return (current_time);
}
