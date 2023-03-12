/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabdelo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 23:48:50 by moabdelo          #+#    #+#             */
/*   Updated: 2023/03/12 23:48:53 by moabdelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*fn_init_philo(int ac, char **av, int i)
{
	t_philo		*ph;

	ph = malloc(sizeof(t_philo));
	ph->nb_philo = ft_atoi(av[1]);
	ph->thread = (pthread_t *)malloc(ph->nb_philo * sizeof(pthread_t));
	ph->forks = malloc(ph->nb_philo * sizeof(pthread_mutex_t));
	ph->meal = malloc(ph->nb_philo * sizeof(pthread_mutex_t));
	ph->tl_meal = malloc(ph->nb_philo * sizeof(pthread_mutex_t));
	ph->mutex_print = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	ph->count = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	ph->cnt = (int *)malloc(sizeof(int));
	ph->cnt[0] = 0;
	ph->key = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	ph->time_last_meal = (long *)malloc(ph->nb_philo * sizeof(long));
	ph->time_to_die = ft_atoi(av[2]);
	ph->time_to_eat = ft_atoi(av[3]);
	ph->time_to_sleep = ft_atoi(av[4]);
	ph->nb_must_eat = 0;
	if (ac == 6)
		ph->nb_must_eat = ft_atoi(av[5]);
	ph->nb_meal = ft_calloc(ph->nb_philo, sizeof(int));
	ph->start_simulation = (long *)malloc(sizeof(long));
	while (i < ph->nb_philo)
		ph->time_last_meal[i++] = ph->start_simulation[0];
	return (ph);
}

void	fn_start_thread_init(t_philo *ph)
{
	pthread_mutex_lock(ph->count);
	ph->cnt[0] = ph->cnt[0] + 1;
	pthread_mutex_unlock(ph->count);
	pthread_mutex_lock(ph->key);
}

void	fn_start_thread(t_philo *ph, int i)
{
	fn_start_thread_init(ph);
	while (1)
	{
		pthread_mutex_lock(ph->count);
		if (ph->cnt[0] > ph->nb_philo)
		{
			pthread_mutex_unlock(ph->count);
			pthread_mutex_unlock(ph->key);
			break ;
		}
		else if (ph->cnt[0] == ph->nb_philo)
		{
			ph->cnt[0] = ph->cnt[0] + 1;
			ph->start_simulation[0] = fn_current_time();
			while (i < ph->nb_philo)
				ph->time_last_meal[i++] = ph->start_simulation[0];
			pthread_mutex_unlock(ph->count);
			pthread_mutex_unlock(ph->key);
			break ;
		}
		else
			pthread_mutex_unlock(ph->count);
	}
	if (ph->i % 2 == 1)
		fn_usleep(fn_current_time(), 50);
}
