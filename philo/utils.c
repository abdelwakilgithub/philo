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

#include "philo.h"

long	fn_current_time(void)
{
	struct timeval	tv;
	long			current_time;

	gettimeofday(&tv, NULL);
	current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (current_time);
}

int	fn_nb_philo_eat(t_philo *ph)
{
	int	i;

	i = 0;
	while (i < ph->nb_philo)
	{
		pthread_mutex_lock(&(ph->meal[i]));
		if (ph->nb_must_eat > ph->nb_meal[i])
		{
			pthread_mutex_unlock(&(ph->meal[i]));
			return (0);
		}
		pthread_mutex_unlock(&(ph->meal[i]));
		i++;
	}
	pthread_mutex_lock(ph->mutex_print);
	return (1);
}

void	fn_philo_die(t_philo *ph, int i)
{
	if (fn_current_time() - ph->time_last_meal[i] >= ph->time_to_die)
	{
		pthread_mutex_lock(ph->mutex_print);
		printf("\033[0;37m%ld %d died\n",
			fn_current_time() - ph->start_simulation[0], i + 1);
		pthread_mutex_lock(&(ph->meal[ph->i]));
		ph->nb_meal[ph->i] = -1;
		pthread_mutex_unlock(&(ph->meal[ph->i]));
	}
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

	pthread_mutex_lock(ph->mutex_print);
	current_time = fn_current_time();
	printf(str, current_time - ph->start_simulation[0], i + 1);
	pthread_mutex_unlock(ph->mutex_print);
	return (current_time);
}
