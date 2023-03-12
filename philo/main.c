#include "philo.h"

t_philo	*fn_init_philo(int ac, char **av)
{
	(void)ac;
	int			i;
	t_philo		*ph;

	ph = malloc(sizeof(t_philo));
	ph->nb_philo = ft_atoi(av[1]);
	ph->thread = (pthread_t *)malloc(ph->nb_philo * sizeof(pthread_t));
	ph->forks = (pthread_mutex_t *)malloc(ph->nb_philo * sizeof(pthread_mutex_t));
	ph->meal = (pthread_mutex_t *)malloc(ph->nb_philo * sizeof(pthread_mutex_t));
	ph->tl_meal = (pthread_mutex_t *)malloc(ph->nb_philo * sizeof(pthread_mutex_t));
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
	// ph->start_simulation[0] = fn_current_time() + 500;
	i = 0;
	while (i < ph->nb_philo)
		ph->time_last_meal[i++] = ph->start_simulation[0];
	return (ph);
}

void	fn_start_thread(t_philo *ph)
{
	int	i;

	pthread_mutex_lock(ph->count);
	ph->cnt[0] = ph->cnt[0] + 1;
	pthread_mutex_unlock(ph->count);
	pthread_mutex_lock(ph->key);
	while(1)
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
			i = 0;
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
void	*fn_stat_ph(void *philo)
{
	t_philo		*ph;
	int			i;
	int			i_right;
	int			i_left;
	long		rest_time_die;
	long		pass_time_eat;
	long		current_time;

	ph = (t_philo *)philo;
	i = ph->i;
	i_right = (i + 1) % (ph->nb_philo);
	i_left = ((ph->nb_philo - 1) + i) % (ph->nb_philo);
	fn_start_thread(ph);
////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
	while (1)
	{
		// if (ph->nb_must_eat && fn_nb_philo_eat(ph))
		if (ph->nb_must_eat && ph->nb_meal[i] >= ph->nb_must_eat)
			break ;
////////////////////////////////////////////////////////////////////////////////////
		rest_time_die = (ph->time_to_die - (fn_current_time() - ph->time_last_meal[i]));
		pthread_mutex_lock(&(ph->tl_meal[i_left]));
		pass_time_eat = (fn_current_time() - ph->time_last_meal[i_left]);
		pthread_mutex_unlock(&(ph->tl_meal[i_left]));
		if (pass_time_eat < ph->time_to_eat && rest_time_die - (ph->time_to_eat - pass_time_eat) < 0)
		{
			fn_usleep(fn_current_time(), ph->time_to_die - (fn_current_time() - ph->time_last_meal[i]));
			fn_philo_die(ph, i);
		}
		pthread_mutex_lock(&(ph->forks[i]));
		rest_time_die = (ph->time_to_die - (fn_current_time() - ph->time_last_meal[i]));
		pthread_mutex_lock(&(ph->tl_meal[i_right]));
		pass_time_eat = (fn_current_time() - ph->time_last_meal[i_right]);
		pthread_mutex_unlock(&(ph->tl_meal[i_right]));
		fn_printf("\033[0;32m%ld %d has taken a fork\n", fn_current_time(), i, ph);
		if (pass_time_eat < ph->time_to_eat && rest_time_die - (ph->time_to_eat - pass_time_eat) < 0)
		{
			fn_usleep(fn_current_time(), ph->time_to_die - (fn_current_time() - ph->time_last_meal[i]));
			fn_philo_die(ph, i);
		}
////////////////////////////////////////////////////////////////////////////////////	
		pthread_mutex_lock(&(ph->forks[i_right]));
		fn_philo_die(ph, i);
		current_time = fn_current_time();
		fn_printf("\033[0;32m%ld %d has taken a fork\n", current_time, i, ph);
		fn_printf("%ld %d is eating\n", current_time, i, ph);
		pthread_mutex_lock(&(ph->tl_meal[i]));
		ph->time_last_meal[i] = current_time;
		pthread_mutex_unlock(&(ph->tl_meal[i]));
////////////////////////////////////////////////////////////////////////////////////	
		pthread_mutex_lock(&(ph->meal[i]));
		ph->nb_meal[i] = ph->nb_meal[i] + 1;
		pthread_mutex_unlock(&(ph->meal[i]));
////////////////////////////////////////////////////////////////////////////////////	
		fn_usleep(current_time, ph->time_to_eat);
		fn_printf("\033[0;33m%ld %d is sleeping\n", fn_current_time(), i, ph);
		pthread_mutex_unlock(&(ph->forks[i]));
		pthread_mutex_unlock(&(ph->forks[i_right]));
////////////////////////////////////////////////////////////////////////////////////
		fn_usleep(current_time + ph->time_to_eat, ph->time_to_sleep);
		fn_printf("\033[0;34m%ld %d is thinking\n", fn_current_time(), i, ph);
		fn_philo_die(ph, i);
	}
	free(ph);
	return (NULL);
}

int	fn_creat_thread_cal(t_philo *ph)
{
	t_philo		*ph_cpy;

	ph->i = 0;
	while (ph->i < ph->nb_philo)
	{
		ph_cpy = malloc(sizeof(t_philo));
		*ph_cpy = *ph;
		if (pthread_create(&ph_cpy->thread[ph_cpy->i], NULL, &fn_stat_ph, ph_cpy) != 0)
			return (ph_cpy->i);
		ph->i = ph->i + 2;
	}
	ph->i = 1;
	while (ph->i < ph->nb_philo)
	{
		ph_cpy = malloc(sizeof(t_philo));
		*ph_cpy = *ph;
		if (pthread_create(&ph_cpy->thread[ph_cpy->i], NULL, &fn_stat_ph, ph_cpy) != 0)
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

	ph = fn_init_philo(ac, av);
	if (ph->time_to_die > ph->time_to_eat + ph->time_to_sleep && ph->nb_philo > 1)
	{
		fn_init_mutex(ph);
		nb_return = fn_creat_thread_cal(ph);
		if (nb_return)
			return nb_return;
		ph->i = 0;
		while (ph->i < ph->nb_philo)
			if (pthread_join(ph->thread[ph->i++], NULL) != 0)
				return (ph->i);
		while (1)
			if (ph->nb_must_eat && fn_nb_philo_eat(ph))
				exit(0);
		fn_destroy_mutex(ph);
	}
	return (0);
}


int	main(int ac, char **av)
{
	if (ac == 5 || ac == 6)
		return (fn_creat_thread(ac, av));
	return (0);
}	