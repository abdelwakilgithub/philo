/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stat_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabdelo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 23:49:50 by moabdelo          #+#    #+#             */
/*   Updated: 2023/03/12 23:49:53 by moabdelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long	fn_ph_eat(t_philo *ph)
{
	long		current_time;

	sem_wait(ph->forks);
	fn_printf("\033[0;32m%ld %d has taken a fork\n", ph->i, ph);
	sem_wait(ph->forks);
	fn_printf("\033[0;32m%ld %d has taken a fork\n", ph->i, ph);
	current_time = fn_printf("%ld %d is eating\n", ph->i, ph);
	sem_wait(ph->tl_meal[ph->i]);
	ph->time_last_meal[ph->i] = current_time;
	ph->nb_meal[ph->i] = ph->nb_meal[ph->i] + 1;
	fn_usleep(current_time, ph->time_to_eat);
	fn_printf("\033[0;33m%ld %d is sleeping\n", ph->i, ph);
	sem_post(ph->forks);
	sem_post(ph->forks);
	sem_post(ph->tl_meal[ph->i]);
	return (current_time);
}

void	*fn_stat_ph(void *philo)
{
	t_philo		*ph;
	long		current_time;

	ph = (t_philo *)philo;
	sem_post(ph->tl_meal[ph->i]);
	sem_wait(ph->key);
	sem_post(ph->key);
	if (ph->i >= ph->nb_philo / 2)
		fn_usleep(ph->start_simulation[0], 50);
	while (1)
	{
		current_time = fn_ph_eat(ph);
		fn_usleep(current_time + ph->time_to_eat, ph->time_to_sleep);
		fn_printf("\033[0;34m%ld %d is thinking\n", ph->i, ph);
	}
	free(ph);
	exit(0);
	return (NULL);
}
