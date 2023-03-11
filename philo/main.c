#include "philo.h"

long	fn_current_time(long start)
{
	struct timeval	tv;
	long			current_time;

	if (gettimeofday(&tv, NULL) == -1)
		exit(1);
	else
		current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (current_time - start);
}

t_philo	*fn_init_philo(int ac, char **av)
{
	(void)ac;
	int			i;
	t_philo		*ph;
	long		current_time;

	ph = malloc(sizeof(t_philo));
	ph->nb_philo = ft_atoi(av[1]);
	ph->thread = (pthread_t *)malloc(ph->nb_philo * sizeof(pthread_t));
	ph->forks = (pthread_mutex_t *)malloc(ph->nb_philo * sizeof(pthread_mutex_t));
	ph->meal = (pthread_mutex_t *)malloc(ph->nb_philo * sizeof(pthread_mutex_t));
	ph->tl_meal = (pthread_mutex_t *)malloc(ph->nb_philo * sizeof(pthread_mutex_t));
	ph->mutex_print = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	ph->time_last_meal = (long *)malloc(ph->nb_philo * sizeof(long));
	ph->time_to_die = ft_atoi(av[2]);	// miliseconds
	ph->time_to_eat = ft_atoi(av[3]);	// miliseconds
	ph->time_to_sleep = ft_atoi(av[4]);	// miliseconds
	ph->nb_must_eat = 0;
	if (ac == 6)
		ph->nb_must_eat = ft_atoi(av[5]);
	ph->nb_meal = ft_calloc(ph->nb_philo, sizeof(int));
	ph->start_simulation = (long *)malloc(sizeof(long));
	current_time = fn_current_time(0) + 400;	// miliseconds
	ph->start_simulation[0] = current_time; // miliseconds
	i = 0;
	while (i < ph->nb_philo)
		ph->time_last_meal[i++] = current_time; // miliseconds
	return (ph);
}

int	fn_nb_philo_eat(t_philo *ph)
{
	int	i;

	i = 0;
	while (i < ph->nb_philo)
	{
		pthread_mutex_lock(&(ph->meal[i]));
		if (ph->nb_must_eat > ph->nb_meal[i])
		{
			pthread_mutex_unlock(&(ph->meal[i]));
			return (0);
		}
		pthread_mutex_unlock(&(ph->meal[i]));
		i++;
	}
	return (1);
}

void	fn_philo_die(t_philo *ph, int i)
{
	if (fn_current_time(ph->start_simulation) - ph->time_last_meal[i] >= ph->time_to_die)
	{
		pthread_mutex_lock(ph->mutex_print);
		printf("\033[0;37m%ld %d died\n", fn_current_time(ph->start_simulation), i);
		exit(0);
	}
}

void	fn_usleep(long start,long time, t_philo *ph)
{
	while (1)
	{
		if (fn_current_time(ph->start_simulation) - start >= time)
			break;
		usleep(100);
	}
}

void	fn_printf(char *str, long current_time, int i, t_philo *ph)
{
	pthread_mutex_lock(ph->mutex_print);
	printf("%ld\n", current_time);
	printf(str, current_time, i);
	pthread_mutex_unlock(ph->mutex_print);
}

// void	*fn_stat_philos(void *philo)
// {
// 	t_philo		*ph;
// 	int			i;
// 	int			i_right;
// 	int			i_left;
// 	// long		rest_time_die;
// 	// long		pass_time_eat;
// 	long		current_time;
// 	long		start_time;

// 	ph = (t_philo *)philo;
// 	i = ph->i;
// 	i_right = (i + 1) % (ph->nb_philo);
// 	i_left = ((ph->nb_philo - 1) + i) % (ph->nb_philo);
// 	if (i % 2 == 0)
// 		fn_usleep(fn_current_time(), ph->start_simulation[0] - fn_current_time()); // miliseconds
// 	if (i % 2 == 1)
// 		fn_usleep(fn_current_time(), ph->start_simulation[0] + 10 - fn_current_time()); // miliseconds
// 	start_time = fn_current_time(); // miliseconds
// 	// while(1)
// 	// {
// 		current_time = fn_current_time(); // miliseconds
// 		fn_printf("\033[0;32m%ld %d is eating\n", current_time, i, ph);
// 		fn_printf("\033[0;32m%ld %d is eating\n", fn_current_time(), i, ph);
// 		fn_usleep(current_time, ph->time_to_eat); // miliseconds
// 		fn_printf("\033[0;34m%ld %d is thinking\n", fn_current_time(), i, ph);
// 		fn_usleep(current_time + ph->time_to_eat, ph->time_to_sleep); // miliseconds
// 	// }
// 	return (0);	
// }
void	*fn_stat_philos(void *philo)
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
	if (i % 2 == 0)
		fn_usleep(fn_current_time(ph->start_simulation), ph->start_simulation[0] - fn_current_time(ph->start_simulation), ph); // miliseconds
	if (i % 2 == 1)
		fn_usleep(fn_current_time(ph->start_simulation), ph->start_simulation[0] + 10 - fn_current_time(ph->start_simulation), ph); // miliseconds
	while (1)
	{
		fn_philo_die(ph, i);
		// if (ph->nb_must_eat && fn_nb_philo_eat(ph))
		if (ph->nb_must_eat && ph->nb_meal[i] >= ph->nb_must_eat)
			break ;
////////////////////////////////////////////////////////////////////////////////////
		rest_time_die = (ph->time_to_die - (fn_current_time(ph->start_simulation) - ph->time_last_meal[i]));
		pthread_mutex_lock(&(ph->tl_meal[i_left]));
		pass_time_eat = (fn_current_time(ph->start_simulation) - ph->time_last_meal[i_left]);
		pthread_mutex_unlock(&(ph->tl_meal[i_left]));
		// pass_time_eat = (ph->time_to_eat - (fn_current_time(ph->start_simulation) - ph->time_last_meal[i_left]));
		if (pass_time_eat < ph->time_to_eat && rest_time_die - (ph->time_to_eat - pass_time_eat) < 0)
		{
			fn_usleep(fn_current_time(ph->start_simulation), ph->time_to_die - (fn_current_time(ph->start_simulation) - ph->time_last_meal[i]), ph);
			fn_philo_die(ph, i);
		}
		pthread_mutex_lock(&(ph->forks[i]));
		rest_time_die = (ph->time_to_die - (fn_current_time(ph->start_simulation) - ph->time_last_meal[i]));
		pthread_mutex_lock(&(ph->tl_meal[i_right]));
		pass_time_eat = (fn_current_time(ph->start_simulation) - ph->time_last_meal[i_right]);
		pthread_mutex_unlock(&(ph->tl_meal[i_right]));
		// pass_time_eat = (ph->time_to_eat - (fn_current_time(ph->start_simulation) - ph->time_last_meal[i_right]));
		fn_printf("\033[0;32m%ld %d has taken a fork\n", fn_current_time(ph->start_simulation), i, ph);
		if (pass_time_eat < ph->time_to_eat && rest_time_die - (ph->time_to_eat - pass_time_eat) < 0)
		{
			fn_usleep(fn_current_time(ph->start_simulation), ph->time_to_die - (fn_current_time(ph->start_simulation) - ph->time_last_meal[i]), ph);
			fn_philo_die(ph, i);
		}
////////////////////////////////////////////////////////////////////////////////////	
		pthread_mutex_lock(&(ph->forks[i_right]));
		fn_philo_die(ph, i);
		current_time = fn_current_time(ph->start_simulation);
		fn_printf("\033[0;32m%ld %d has taken a fork\n", current_time, i, ph);
		fn_printf("%ld %d is eating\n", current_time, i, ph);
		pthread_mutex_lock(&(ph->tl_meal[i]));
		ph->time_last_meal[i] = current_time;
		pthread_mutex_unlock(&(ph->tl_meal[i]));
////////////////////////////////////////////////////////////////////////////////////	
		// pthread_mutex_lock(&(ph->meal[i]));
		ph->nb_meal[i] = ph->nb_meal[i] + 1;
		// pthread_mutex_unlock(&(ph->meal[i]));
////////////////////////////////////////////////////////////////////////////////////	
		fn_usleep(current_time, ph->time_to_eat, ph);
		fn_printf("\033[0;33m%ld %d is sleeping\n", fn_current_time(ph->start_simulation), i, ph);
		pthread_mutex_unlock(&(ph->forks[i]));
		pthread_mutex_unlock(&(ph->forks[i_right]));
////////////////////////////////////////////////////////////////////////////////////
		fn_usleep(current_time + ph->time_to_eat, ph->time_to_sleep, ph);
		fn_printf("\033[0;34m%ld %d is thinking\n", fn_current_time(ph->start_simulation), i, ph);
	}
	free(ph);
	return (NULL);
}

int	fn_creat_thread(int ac, char **av)
{
	int			i;
	t_philo		*philos;
	t_philo		*ph_cpy;

	philos = fn_init_philo(ac, av);
	if (philos->time_to_die > philos->time_to_eat + philos->time_to_sleep && philos->nb_philo > 1)
	{
		i = 0;
		pthread_mutex_init(philos->mutex_print, NULL);
		while (i < philos->nb_philo)
		{
			pthread_mutex_init(&(philos->forks[i]), NULL);
			pthread_mutex_init(&(philos->tl_meal[i]), NULL);
			pthread_mutex_init(&(philos->meal[i++]), NULL);
		}
		philos->i = 0;
		while (philos->i < philos->nb_philo)
		{
			ph_cpy = malloc(sizeof(t_philo));
			*ph_cpy = *philos;
			// usleep(100);
			if (pthread_create(&ph_cpy->thread[ph_cpy->i], NULL, &fn_stat_philos, ph_cpy) != 0)
				return (ph_cpy->i);
			philos->i = philos->i + 2;
		}
		philos->i = 1;
		while (philos->i < philos->nb_philo)
		{
			ph_cpy = malloc(sizeof(t_philo));
			*ph_cpy = *philos;
			// usleep(100);
			if (pthread_create(&ph_cpy->thread[ph_cpy->i], NULL, &fn_stat_philos, ph_cpy) != 0)
				return (ph_cpy->i);
			philos->i = philos->i + 2;
		}
		philos->i = 0;
		while (philos->i < philos->nb_philo)
			if (pthread_join(philos->thread[philos->i++], NULL) != 0)
				return (philos->i);
		i = 0;
		pthread_mutex_destroy(philos->mutex_print);
		while (i < philos->nb_philo)
		{
			pthread_mutex_destroy(&(philos->forks[i]));
			pthread_mutex_destroy(&(philos->tl_meal[i]));
			pthread_mutex_destroy(&(philos->meal[i++]));
		}
	}
	return (0);
}


int	main(int ac, char **av)
{
	if (ac == 5 || ac == 6)
		return (fn_creat_thread(ac, av));
	return (0);
}	