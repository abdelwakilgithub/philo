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

void	*fn_stat_ph2(void *philo)
{
	t_philo		*ph;
	long		current_time;
	ph = (t_philo *)philo;
	sem_post(ph->meal[ph->i]);
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

int	fn_creat_thread_cal(t_philo *ph)
{
	t_philo		*ph_cpy;

	ph->i = 0;
	while (ph->i < ph->nb_philo)
	{
		if((ph->pid[ph->i] = fork()) && ph->pid[ph->i] < 0)
			exit(1);
		if (ph->pid[ph->i] == 0)
		{
			ph_cpy = malloc(sizeof(t_philo));
			*ph_cpy = *ph;
			if (pthread_create(&ph_cpy->thread1[ph_cpy->i],
					NULL, &fn_philo_die, ph_cpy) != 0)
				return (ph_cpy->i);
			fn_stat_ph2(ph_cpy);
			pthread_join(ph_cpy->thread1[ph_cpy->i], NULL);
		}
		ph->i = ph->i + 1;
	}
	return (0);
}

void	fn_init_sem(t_philo *ph)
{
	int		i;
	char	*str;

	ph->key = sem_open("/key", O_CREAT, 0644, 1);
	ph->sem_print = sem_open("/sem_print", O_CREAT, 0644, 1);
	ph->forks = sem_open("/forks", O_CREAT, 0644, ph->nb_philo);
	i = 0;
	while(i < ph->nb_philo)
	{
		str = ft_strjoin("/meal", ft_itoa(i));
		ph->meal[i] = sem_open(str, O_CREAT, 0644, 0);
		free(str);
		str = ft_strjoin("/tl_meal", ft_itoa(i));
		ph->tl_meal[i++] = sem_open(str, O_CREAT, 0644, 1);
		free(str);
	}
    // if (ph->key == SEM_FAILED || ph->sem_print == SEM_FAILED || ph->forks == SEM_FAILED)
	// {
    //     perror("sem_open failed");
    //     exit(EXIT_FAILURE);
    // }
}

void	fn_close_sem(t_philo *ph)
{
	int		i;
	char	*str;

	sem_close(ph->key);
	sem_close(ph->sem_print);
	sem_close(ph->forks);
	i = 0;
	while(i < ph->nb_philo)
	{
		sem_close(ph->meal[i]);
		sem_close(ph->tl_meal[i++]);
	}
	i = 0;
	while(i < ph->nb_philo)
	{
		str = ft_strjoin("/meal", ft_itoa(i));
		sem_unlink(str);
		free(str);
		str = ft_strjoin("/tl_meal", ft_itoa(i++));
		sem_unlink(str);
		free(str);
	}
	sem_unlink("/key");
	sem_unlink("/sem_print");
	sem_unlink("/forks");
}

int	fn_creat_thread(int ac, char **av)
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
	nb_return = fn_creat_thread_cal(ph);
	i = 0;
	while (i < ph->nb_philo)
	{
		sem_wait(ph->meal[i]);
		sem_wait(ph->meal[i]);
		sem_post(ph->meal[i++]);
	}
	fn_usleep(ph->start_simulation[0] - 1000, 1000);
	sem_post(ph->key);
	if (nb_return)
		return (nb_return);
	fn_waitpids(ph, 0);
	fn_close_sem(ph);
	return (0);
}

int	main(int ac, char **av)
{
	if (ac == 5 || ac == 6)
		return (fn_creat_thread(ac, av));
	return (0);
}	
