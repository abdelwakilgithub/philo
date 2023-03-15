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

// void	fn_ph_can_eat()
// {
// 	long		rest_time_die;
// 	long		pass_time_eat;

// 	rest_time_die = (ph->time_to_die
// 			- (fn_current_time() - ph->time_last_meal[ph->i]));
// 	// sem_wait(&(ph->tl_meal[j]));
// 	pass_time_eat = (fn_current_time() - ph->time_last_meal[j]);
// 	// sem_post(&(ph->tl_meal[j]));
// 	if (pass_time_eat < ph->time_to_eat
// 		&& rest_time_die - (ph->time_to_eat - pass_time_eat) < 0)
// 	{
// 		fn_usleep(fn_current_time(), ph->time_to_die
// 			- (fn_current_time() - ph->time_last_meal[ph->i]));
// 		fn_philo_die(ph, ph->i);
// 	}
// }

long	fn_ph_eat(t_philo *ph)
{
	long		current_time;
	// long		v;

	sem_wait(ph->forks);
	fn_printf("\033[0;32m%ld %d has taken a fork\n", ph->i, ph);
	sem_wait(ph->forks);
	fn_printf("\033[0;32m%ld %d has taken a fork\n", ph->i, ph);
	current_time = fn_printf("%ld %d is eating\n", ph->i, ph);
	sem_wait(ph->tl_meal[ph->i]);
	// sem_wait(ph->sem_print);
	ph->time_last_meal[ph->i] = current_time;
	// v = ph->time_last_meal[ph->i];
	// sem_post(ph->sem_print);
	sem_post(ph->tl_meal[ph->i]);
	// sem_wait(ph->meal[ph->i]);
	// ph->nb_meal[ph->i] = ph->nb_meal[ph->i] + 1;
	// sem_post(ph->meal[ph->i]);
	fn_usleep(current_time, ph->time_to_eat);
	fn_printf("\033[0;33m%ld %d is sleeping\n", ph->i, ph);
	sem_post(ph->forks);
	sem_post(ph->forks);
	return (current_time);
}

void	*fn_stat_ph(void *philo)
{
	t_philo		*ph;
	// long		current_time;
	ph = (t_philo *)philo;
	sem_post(ph->meal[ph->i]);
	sem_wait(ph->key);
	sem_post(ph->key);
	if (ph->i >= ph->nb_philo / 2)
		fn_usleep(ph->start_simulation[0], 5000);
	// while (1)
	// {
		// current_time = fn_ph_eat(ph);
		// fn_usleep(current_time + ph->time_to_eat, ph->time_to_sleep);
		fn_printf("\033[0;34m%ld %d is thinking\n", ph->i, ph);
	// }
	free(ph);
	exit(0);
	return (NULL);
}
