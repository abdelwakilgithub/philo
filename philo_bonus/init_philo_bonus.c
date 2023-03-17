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

#include "philo_bonus.h"

t_philo	*fn_init_philo(int ac, char **av, int i)
{
	t_philo		*ph;

	ph = malloc(sizeof(t_philo));
	ph->nb_philo = ft_atoi(av[1]);
	ph->pid = (pid_t *)malloc(ph->nb_philo * sizeof(pid_t));
	// ph->key = (sem_t *)malloc(sizeof(sem_t));
	// ph->forks = malloc(sizeof(sem_t));
	ph->tl_meal = malloc(ph->nb_philo * sizeof(sem_t *));
	ph->thread = (pthread_t *)malloc(ph->nb_philo * sizeof(pthread_t));
	// ph->sem_print = (sem_t *)malloc(sizeof(sem_t));
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

sem_t	*fn_sem_open(const char *str, int nb)
{
	sem_t	*sem;

	sem = sem_open(str, O_CREAT, 0644, nb);
	if (sem == SEM_FAILED)
		exit(1);
	return (sem);
}

void	fn_init_sem(t_philo *ph)
{
	int		i;
	char	*str;

	ph->key = fn_sem_open("/key", 1);
	ph->sem_print = fn_sem_open("/sem_print", 1);
	ph->forks = fn_sem_open("/forks", ph->nb_philo);
	i = 0;
	while (i < ph->nb_philo)
	{
		str = ft_strjoin("/tl_meal", ft_itoa(i));
		ph->tl_meal[i++] = fn_sem_open(str, 0);
		free(str);
	}
}

void	fn_close_sem(t_philo *ph)
{
	int		i;
	char	*str;

	sem_close(ph->key);
	sem_close(ph->sem_print);
	sem_close(ph->forks);
	i = 0;
	while (i < ph->nb_philo)
		sem_close(ph->tl_meal[i++]);
	i = 0;
	while (i < ph->nb_philo)
	{
		str = ft_strjoin("/tl_meal", ft_itoa(i++));
		sem_unlink(str);
		free(str);
	}
	sem_unlink("/key");
	sem_unlink("/sem_print");
	sem_unlink("/forks");
}
