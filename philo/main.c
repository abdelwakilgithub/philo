/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabdelo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 23:48:31 by moabdelo          #+#    #+#             */
/*   Updated: 2023/03/12 23:48:42 by moabdelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	fn_creat_thread_cal(t_philo *ph)
{
	t_philo		*ph_cpy;

	ph->i = 0;
	while (ph->i < ph->nb_philo)
	{
		ph_cpy = malloc(sizeof(t_philo));
		*ph_cpy = *ph;
		if (pthread_create(&ph_cpy->thread[ph_cpy->i],
				NULL, &fn_stat_ph, ph_cpy) != 0)
			return (ph_cpy->i);
		ph->i = ph->i + 2;
	}
	ph->i = 1;
	while (ph->i < ph->nb_philo)
	{
		ph_cpy = malloc(sizeof(t_philo));
		*ph_cpy = *ph;
		if (pthread_create(&ph_cpy->thread[ph_cpy->i],
				NULL, &fn_stat_ph, ph_cpy) != 0)
			return (ph_cpy->i);
		ph->i = ph->i + 2;
	}
	return (0);
}

void	fn_init_mutex(t_philo *ph)
{
	int	i;

	i = 0;
	pthread_mutex_init(ph->mutex_print, NULL);
	pthread_mutex_init(ph->count, NULL);
	pthread_mutex_init(ph->key, NULL);
	while (i < ph->nb_philo)
	{
		pthread_mutex_init(&(ph->forks[i]), NULL);
		pthread_mutex_init(&(ph->tl_meal[i]), NULL);
		pthread_mutex_init(&(ph->meal[i++]), NULL);
	}
}

void	fn_destroy_mutex(t_philo *ph)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(ph->mutex_print);
	pthread_mutex_destroy(ph->count);
	pthread_mutex_destroy(ph->key);
	while (i < ph->nb_philo)
	{
		pthread_mutex_destroy(&(ph->forks[i]));
		pthread_mutex_destroy(&(ph->tl_meal[i]));
		pthread_mutex_destroy(&(ph->meal[i++]));
	}
}

int	fn_creat_thread(int ac, char **av)
{
	t_philo		*ph;
	int			nb_return;

	ph = fn_init_philo(ac, av, 0);
	fn_init_mutex(ph);
	nb_return = fn_creat_thread_cal(ph);
	if (nb_return)
		return (nb_return);
	while (1)
		if (ph->nb_must_eat && fn_nb_philo_eat(ph))
			exit(0);
	ph->i = 0;
	while (ph->i < ph->nb_philo)
		if (pthread_join(ph->thread[ph->i++], NULL) != 0)
			return (ph->i);
	fn_destroy_mutex(ph);
	return (0);
}

int	main(int ac, char **av)
{
	if (ac == 5 || ac == 6)
		return (fn_creat_thread(ac, av));
	return (0);
}	
