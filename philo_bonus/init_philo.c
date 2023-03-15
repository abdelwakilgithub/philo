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
	ph->pid = (pid_t *)malloc(ph->nb_philo * sizeof(pid_t));
	ph->key = (sem_t *)malloc(sizeof(sem_t));
	ph->forks = malloc(sizeof(sem_t));
	ph->tl_meal = malloc(ph->nb_philo * sizeof(sem_t *));
	ph->meal = malloc(ph->nb_philo * sizeof(sem_t *));
	ph->thread1 = (pthread_t *)malloc(ph->nb_philo * sizeof(pthread_t));
	ph->thread2 = (pthread_t *)malloc(ph->nb_philo * sizeof(pthread_t));
	// i = 0;
	// while (i < ph->nb_philo)
	// 	ph->thread[i] = (pthread_t *)malloc(2 * sizeof(pthread_t));
	ph->sem_print = (sem_t *)malloc(sizeof(sem_t));
	ph->time_last_meal = (long *)malloc(ph->nb_philo * sizeof(long));
	ph->time_to_die = ft_atoi(av[2]);
	ph->time_to_eat = ft_atoi(av[3]);
	ph->time_to_sleep = ft_atoi(av[4]);
	ph->nb_must_eat = 0;
	if (ac == 6)
		ph->nb_must_eat = ft_atoi(av[5]);
	ph->nb_meal = ft_calloc(ph->nb_philo, sizeof(int));
	ph->start_simulation = (long *)ft_calloc(1, sizeof(long));
	i = 0;
	while (i < ph->nb_philo)
		ph->time_last_meal[i++] = ph->start_simulation[0];
	return (ph);
}