#include "philo.h"

long	fn_current_time(void)
{
	struct timeval	tv;
	long			current_time;

	if (gettimeofday(&tv, NULL) == -1)
		exit(1);
	else
		current_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (current_time);
}

t_philo	*fn_init_philo(int ac, char **av)
{
	(void)ac;
	int			i;
	t_philo		*ph;
	long		current_time;

	ph = malloc(sizeof(t_philo));
	current_time = fn_current_time();
	ph->nb_philo = ft_atoi(av[1]);
	ph->thread = (pthread_t *)malloc(ph->nb_philo * sizeof(pthread_t));
	ph->forks = (pthread_mutex_t *)malloc(ph->nb_philo * sizeof(pthread_mutex_t));
	ph->meal = (pthread_mutex_t *)malloc(ph->nb_philo * sizeof(pthread_mutex_t));
	ph->tl_meal = (pthread_mutex_t *)malloc(ph->nb_philo * sizeof(pthread_mutex_t));
	ph->time_last_meal = (long *)malloc(ph->nb_philo * sizeof(long));
	i = 0;
	while (i < ph->nb_philo)
		ph->time_last_meal[i++] = current_time;
	ph->time_to_die = ft_atoi(av[2]);
	ph->time_to_eat = ft_atoi(av[3]);
	ph->time_to_sleep = ft_atoi(av[4]);
	ph->nb_must_eat = 0;
	if (ac == 6)
		ph->nb_must_eat = ft_atoi(av[5]);
	ph->nb_meal = ft_calloc(ph->nb_philo, sizeof(int));
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
	if (fn_current_time() - ph->time_last_meal[i] >= ph->time_to_die)
	{
		printf("\033[0;37m%ld %d died\n", fn_current_time(), i);
		exit(1);
	}
}

void	fn_usleep(long time)
{
	long	start;
	long	time_sleep;

	start = fn_current_time();
	time_sleep = time;
	while (1)
	{
		time_sleep = time_sleep / 2;
		if (fn_current_time() - start >= time)
			break;
		usleep(time_sleep);
	}
}

void	*fn_stat_philos(void *philo)
{
	t_philo		*ph;
	int			i;
	int			j;
	int			i_left;
	long		rest_time_eat;
	long		rest_time_die;
	long		current_time;

	ph = (t_philo *)philo;
	i = ph->i;
	j = (i + 1) % (ph->nb_philo);
	i_left = ((ph->nb_philo - 1) + i) % (ph->nb_philo);
	if (i % 2 == 0)
		usleep(2000);
	while (1)
	{
		fn_philo_die(ph, i);
		if (ph->nb_must_eat && fn_nb_philo_eat(ph))
			exit(0);
////////////////////////////////////////////////////////////////////////////////////
		rest_time_eat = (ph->time_to_die - (fn_current_time() - ph->time_last_meal[i]));
		pthread_mutex_lock(&(ph->tl_meal[i_left]));
		rest_time_die = (fn_current_time() - ph->time_last_meal[i_left]);
		pthread_mutex_unlock(&(ph->tl_meal[i_left]));
		// rest_time_die = (ph->time_to_eat - (fn_current_time() - ph->time_last_meal[i_left]));
		if (rest_time_die < ph->time_to_eat && rest_time_eat - (ph->time_to_eat - rest_time_die) < 0)
		{
			fn_usleep(ph->time_to_die - (fn_current_time() - ph->time_last_meal[i]));
			fn_philo_die(ph, i);
		}
		pthread_mutex_lock(&(ph->forks[i]));
		rest_time_eat = (ph->time_to_die - (fn_current_time() - ph->time_last_meal[i]));
		pthread_mutex_lock(&(ph->tl_meal[j]));
		rest_time_die = (fn_current_time() - ph->time_last_meal[j]);
		pthread_mutex_unlock(&(ph->tl_meal[j]));
		// rest_time_die = (ph->time_to_eat - (fn_current_time() - ph->time_last_meal[j]));
		printf("\033[0;32m%ld %d has taken a fork\n", fn_current_time(), i);
		if (rest_time_die < ph->time_to_eat && rest_time_eat - (ph->time_to_eat - rest_time_die) < 0)
		{
			fn_usleep(ph->time_to_die - (fn_current_time() - ph->time_last_meal[i]));
			fn_philo_die(ph, i);
		}
////////////////////////////////////////////////////////////////////////////////////	
		pthread_mutex_lock(&(ph->forks[j]));
		fn_philo_die(ph, i);
		current_time = fn_current_time();
		printf("\033[0;32m%ld %d has taken a fork\n", current_time, i);
		printf("%ld %d is eating\n", current_time, i);
		pthread_mutex_lock(&(ph->tl_meal[i]));
		ph->time_last_meal[i] = current_time;
		pthread_mutex_unlock(&(ph->tl_meal[i]));
////////////////////////////////////////////////////////////////////////////////////	
		pthread_mutex_lock(&(ph->meal[i]));
		ph->nb_meal[i] = ph->nb_meal[i] + 1;
		pthread_mutex_unlock(&(ph->meal[i]));
////////////////////////////////////////////////////////////////////////////////////	
		fn_usleep(ph->time_to_eat);
		printf("\033[0;33m%ld %d is sleeping\n", fn_current_time(), i);
		pthread_mutex_unlock(&(ph->forks[i]));
		pthread_mutex_unlock(&(ph->forks[j]));
////////////////////////////////////////////////////////////////////////////////////
		fn_usleep(ph->time_to_sleep);
		printf("\033[0;34m%ld %d is thinking\n", fn_current_time(), i);
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
			if (pthread_create(&ph_cpy->thread[ph_cpy->i], NULL, &fn_stat_philos, ph_cpy) != 0)
				return (ph_cpy->i);
			philos->i++;
		}
		philos->i = 0;
		while (philos->i < philos->nb_philo)
			if (pthread_join(philos->thread[philos->i++], NULL) != 0)
				return (philos->i);
		i = 0;
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