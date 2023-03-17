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

#include "philo_bonus.h"

void	fn_waitpids(t_philo *ph, int i)
{
	int	status;

	while (i < ph->nb_philo)
	{
		waitpid(-1, &status, 0);
		if (status / 256 != 0)
			break ;
		i++;
	}
	i = 0;
	while (i < ph->nb_philo)
		kill(ph->pid[i++], SIGTERM);
}

int	fn_creat_processes_cal(t_philo *ph)
{
	t_philo		*ph_cpy;

	ph->i = 0;
	while (ph->i < ph->nb_philo)
	{
		ph->pid[ph->i] = fork();
		if (ph->pid[ph->i] < 0)
			exit(1);
		if (ph->pid[ph->i] == 0)
		{
			ph_cpy = malloc(sizeof(t_philo));
			*ph_cpy = *ph;
			if (pthread_create(&ph_cpy->thread[ph_cpy->i],
					NULL, &fn_philo_die, ph_cpy) != 0)
				return (ph_cpy->i);
			fn_stat_ph(ph_cpy);
			pthread_join(ph_cpy->thread[ph_cpy->i], NULL);
		}
		ph->i = ph->i + 1;
	}
	return (0);
}

int	fn_creat_processes(int ac, char **av)
{
	int			i;
	t_philo		*ph;
	int			nb_return;

	ph = fn_init_philo(ac, av, 0);
	fn_close_sem(ph);
	fn_init_sem(ph);
	sem_wait(ph->key);
	ph->start_simulation[0] = fn_current_time() + 1000;
	i = 0;
	while (i < ph->nb_philo)
		ph->time_last_meal[i++] = ph->start_simulation[0];
	nb_return = fn_creat_processes_cal(ph);
	i = 0;
	while (i < ph->nb_philo)
	{
		sem_wait(ph->tl_meal[i]);
		sem_wait(ph->tl_meal[i]);
		sem_post(ph->tl_meal[i++]);
	}
	fn_usleep(ph->start_simulation[0] - 1000, 1000);
	sem_post(ph->key);
	fn_waitpids(ph, 0);
	fn_close_sem(ph);
	return (0);
}

int	main(int ac, char **av)
{
	if (ac == 5 || ac == 6)
		return (fn_creat_processes(ac, av));
	return (0);
}	
