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

#include "philo.h"

int	fn_end_simulation(t_philo *ph)
{
	int	i;

	if (ph->nb_must_eat && fn_nb_philo_eat(ph))
		return (1);
	i = 0;
	while (i < ph->nb_philo)
	{
		pthread_mutex_lock(&(ph->meal[i]));
		if (ph->nb_meal[i] < 0)
		{
			pthread_mutex_unlock(&(ph->meal[i]));
			return (1);
		}
		pthread_mutex_unlock(&(ph->meal[i]));
		i++;
	}
	return (0);
}

void	fn_ph_can_eat(t_philo *ph, int j)
{
	long		rest_time_die;
	long		pass_time_eat;

	rest_time_die = (ph->time_to_die
			- (fn_current_time() - ph->time_last_meal[ph->i]));
	pthread_mutex_lock(&(ph->tl_meal[j]));
	pass_time_eat = (fn_current_time() - ph->time_last_meal[j]);
	pthread_mutex_unlock(&(ph->tl_meal[j]));
	if (pass_time_eat < ph->time_to_eat
		&& rest_time_die - (ph->time_to_eat - pass_time_eat) < 0)
	{
		fn_usleep(fn_current_time(), ph->time_to_die
			- (fn_current_time() - ph->time_last_meal[ph->i]));
		fn_philo_die(ph, ph->i);
	}
}

long	fn_ph_eat(t_philo *ph, int i_left, int i_right)
{
	long		current_time;

	fn_ph_can_eat(ph, i_left);
	pthread_mutex_lock(&(ph->forks[ph->i]));
	fn_printf("\033[0;32m%ld %d has taken a fork\n", ph->i, ph);
	fn_ph_can_eat(ph, i_right);
	pthread_mutex_lock(&(ph->forks[i_right]));
	fn_philo_die(ph, ph->i);
	fn_printf("\033[0;32m%ld %d has taken a fork\n", ph->i, ph);
	current_time = fn_printf("%ld %d is eating\n", ph->i, ph);
	pthread_mutex_lock(&(ph->tl_meal[ph->i]));
	ph->time_last_meal[ph->i] = current_time;
	pthread_mutex_unlock(&(ph->tl_meal[ph->i]));
	pthread_mutex_lock(&(ph->meal[ph->i]));
	ph->nb_meal[ph->i] = ph->nb_meal[ph->i] + 1;
	pthread_mutex_unlock(&(ph->meal[ph->i]));
	fn_usleep(current_time, ph->time_to_eat);
	fn_printf("\033[0;33m%ld %d is sleeping\n", ph->i, ph);
	pthread_mutex_unlock(&(ph->forks[ph->i]));
	pthread_mutex_unlock(&(ph->forks[i_right]));
	return (current_time);
}

void	*fn_stat_ph(void *philo)
{
	t_philo		*ph;
	long		current_time;

	ph = (t_philo *)philo;
	if (ph->nb_philo == 1)
	{
		ph->start_simulation[0] = fn_current_time();
		fn_printf("\033[0;32m%ld %d has taken a fork\n", ph->i, ph);
		ph->time_last_meal[0] = ph->start_simulation[0];
		while (1)
			fn_philo_die(ph, ph->i);
	}
	ph->i_right = (ph->i + 1) % (ph->nb_philo);
	ph->i_left = ((ph->nb_philo - 1) + ph->i) % (ph->nb_philo);
	fn_start_thread(ph, 0);
	while (1)
	{
		current_time = fn_ph_eat(ph, ph->i_left, ph->i_right);
		if (ph->time_to_sleep > ph->time_to_die - ph->time_to_eat)
			(fn_usleep(fn_current_time(), ph->time_to_die - ph->time_to_eat),
				fn_philo_die(ph, ph->i));
		fn_usleep(current_time + ph->time_to_eat, ph->time_to_sleep);
		fn_printf("\033[0;34m%ld %d is thinking\n", ph->i, ph);
	}
	return (free(ph), NULL);
}
