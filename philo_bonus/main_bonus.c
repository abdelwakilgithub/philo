#include "philo_bonus.h"

t_philo	*fn_init(int ac, char **av)
{
	(void)ac;
	int		i;
	t_philo *ph;

	ph = (t_philo *)malloc(sizeof(t_philo));
	ph->nb_philo = ft_atoi(av[1]);
	ph->stat = malloc(4 * sizeof(int));
	i = 0;
	while (i < 4)
	{
		ph->stat[i] = i;
		i++;
	}
	// ph->time_to_die = ft_atoi(av[2]);
	// ph->time_to_eat = ft_atoi(av[3]);
	// ph->time_to_sleep = ft_atoi(av[4]);
	ph->pid = (pid_t *)malloc(ph->nb_philo * sizeof(pid_t));
	return (ph);
}

long	fn_current_time(void)
{
	struct timeval	tv;
	long			current_time;

	if (gettimeofday(&tv, NULL) == -1)
		exit(1);
	else
		current_time = tv.tv_sec * 1000 + tv.tv_usec / 1000; // miliseconds
	return (current_time); // miliseconds
}

t_philo	*fn_init_philo(int ac, char **av)
{
	(void)ac;
	int			i;
	t_philo		*ph;
	long		current_time;

	ph = malloc(sizeof(t_philo));
	ph->nb_philo = ft_atoi(av[1]);
	ph->pid = (pid_t *)malloc(ph->nb_philo * sizeof(pid_t));
	// ph->thread = (pthread_t *)malloc(ph->nb_philo * sizeof(pthread_t));
	ph->forks = (sem_t *)malloc(sizeof(sem_t));
	// ph->meal = (pthread_mutex_t *)malloc(ph->nb_philo * sizeof(pthread_mutex_t));
	// ph->tl_meal = (pthread_mutex_t *)malloc(ph->nb_philo * sizeof(pthread_mutex_t));
	// ph->mutex_print = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	ph->time_last_meal = (long *)malloc(ph->nb_philo * sizeof(long));
	ph->time_to_die = ft_atoi(av[2]);	// miliseconds
	ph->time_to_eat = ft_atoi(av[3]);	// miliseconds
	ph->time_to_sleep = ft_atoi(av[4]);	// miliseconds
	ph->nb_must_eat = 0;
	if (ac == 6)
		ph->nb_must_eat = ft_atoi(av[5]);
	ph->nb_meal = ft_calloc(ph->nb_philo, sizeof(int));
	ph->start_simulation = (long *)malloc(sizeof(long));
	current_time = fn_current_time() + 400;	// miliseconds
	ph->start_simulation[0] = current_time; // miliseconds
	i = 0;
	while (i < ph->nb_philo)
		ph->time_last_meal[i++] = current_time; // miliseconds
	return (ph);
}

void	fn_philo_die(t_philo *ph, int i)
{
	if (fn_current_time() - ph->time_last_meal[i] >= ph->time_to_die)
	{
		// pthread_mutex_lock(ph->mutex_print);
		printf("\033[0;37m%ld %d died\n", fn_current_time(), i);
		exit(0);
	}
}

void	fn_usleep(long start,long time)
{
	while (1)
	{
		if (fn_current_time() - start >= time)
			break;
			usleep(100);
	}
}

void	fn_printf(char *str, long current_time, int i, t_philo *ph)
{
	(void)ph;
	// pthread_mutex_lock(ph->mutex_print);
	printf(str, current_time, i);
	// pthread_mutex_unlock(ph->mutex_print);
}

void	*fn_stat_philos(void *philo)
{
	printf("hi");
	t_philo		*ph;
	int			i;
	int			i_right;
	int			i_left;
	long		current_time;
	long		start_time;

	ph = (t_philo *)philo;
	i = ph->i;
	i_right = (i + 1) % (ph->nb_philo);
	i_left = ((ph->nb_philo - 1) + i) % (ph->nb_philo);
	if (i % 2 == 0)
		fn_usleep(fn_current_time(), ph->start_simulation[0] - fn_current_time()); 
	if (i % 2 == 1)
		fn_usleep(fn_current_time(), ph->start_simulation[0] + 10 - fn_current_time()); 
	start_time = fn_current_time(); 
	// while(1)
	// {
		current_time = fn_current_time(); 
		fn_printf("\033[0;32m%ld %d is eating\n", current_time, i, ph);
		fn_printf("\033[0;32m%ld %d is eating\n", fn_current_time(), i, ph);
		fn_usleep(current_time, ph->time_to_eat); 
		fn_printf("\033[0;34m%ld %d is thinking\n", fn_current_time(), i, ph);
		fn_usleep(current_time + ph->time_to_eat, ph->time_to_sleep); 
	// // }

	return (0);	
}

// void	*fn_stat_philos(void *philo)
// {
// 	t_philo		*ph;
// 	int			i;
// 	int			i_right;
// 	int			i_left;
// 	long		rest_time_die;
// 	long		pass_time_eat;
// 	long		current_time;

// 	ph = (t_philo *)philo;
// 	i = ph->i;
// 	i_right = (i + 1) % (ph->nb_philo);
// 	i_left = ((ph->nb_philo - 1) + i) % (ph->nb_philo);
// 	if (i % 2 == 0)
// 		fn_usleep(fn_current_time(), ph->start_simulation[0] - fn_current_time()); // miliseconds
// 	if (i % 2 == 1)
// 		fn_usleep(fn_current_time(), ph->start_simulation[0] + 10 - fn_current_time()); // miliseconds
// 	while (1)
// 	{
// 		fn_philo_die(ph, i);
// 		// if (ph->nb_must_eat && fn_nb_philo_eat(ph))
// 		if (ph->nb_must_eat && ph->nb_meal[i] >= ph->nb_must_eat)
// 			exit(0);
// ////////////////////////////////////////////////////////////////////////////////////
// 		rest_time_die = (ph->time_to_die - (fn_current_time() - ph->time_last_meal[i]));
// 		pthread_mutex_lock(&(ph->tl_meal[i_left]));
// 		pass_time_eat = (fn_current_time() - ph->time_last_meal[i_left]);
// 		pthread_mutex_unlock(&(ph->tl_meal[i_left]));
// 		// pass_time_eat = (ph->time_to_eat - (fn_current_time() - ph->time_last_meal[i_left]));
// 		if (pass_time_eat < ph->time_to_eat && rest_time_die - (ph->time_to_eat - pass_time_eat) < 0)
// 		{
// 			fn_usleep(fn_current_time(), ph->time_to_die - (fn_current_time() - ph->time_last_meal[i]));
// 			fn_philo_die(ph, i);
// 		}
// 		pthread_mutex_lock(&(ph->forks[i]));
// 		rest_time_die = (ph->time_to_die - (fn_current_time() - ph->time_last_meal[i]));
// 		pthread_mutex_lock(&(ph->tl_meal[i_right]));
// 		pass_time_eat = (fn_current_time() - ph->time_last_meal[i_right]);
// 		pthread_mutex_unlock(&(ph->tl_meal[i_right]));
// 		// pass_time_eat = (ph->time_to_eat - (fn_current_time() - ph->time_last_meal[i_right]));
// 		fn_printf("\033[0;32m%ld %d has taken a fork\n", fn_current_time(), i, ph);
// 		if (pass_time_eat < ph->time_to_eat && rest_time_die - (ph->time_to_eat - pass_time_eat) < 0)
// 		{
// 			fn_usleep(fn_current_time(), ph->time_to_die - (fn_current_time() - ph->time_last_meal[i]));
// 			fn_philo_die(ph, i);
// 		}
// ////////////////////////////////////////////////////////////////////////////////////	
// 		pthread_mutex_lock(&(ph->forks[i_right]));
// 		fn_philo_die(ph, i);
// 		current_time = fn_current_time();
// 		fn_printf("\033[0;32m%ld %d has taken a fork\n", current_time, i, ph);
// 		fn_printf("%ld %d is eating\n", current_time, i, ph);
// 		pthread_mutex_lock(&(ph->tl_meal[i]));
// 		ph->time_last_meal[i] = current_time;
// 		pthread_mutex_unlock(&(ph->tl_meal[i]));
// ////////////////////////////////////////////////////////////////////////////////////	
// 		// pthread_mutex_lock(&(ph->meal[i]));
// 		ph->nb_meal[i] = ph->nb_meal[i] + 1;
// 		// pthread_mutex_unlock(&(ph->meal[i]));
// ////////////////////////////////////////////////////////////////////////////////////	
// 		fn_usleep(current_time, ph->time_to_eat);
// 		fn_printf("\033[0;33m%ld %d is sleeping\n", fn_current_time(), i, ph);
// 		pthread_mutex_unlock(&(ph->forks[i]));
// 		pthread_mutex_unlock(&(ph->forks[i_right]));
// ////////////////////////////////////////////////////////////////////////////////////
// 		fn_usleep(current_time + ph->time_to_eat, ph->time_to_sleep);
// 		fn_printf("\033[0;34m%ld %d is thinking\n", fn_current_time(), i, ph);
// 	}
// 	free(ph);
// 	return (NULL);
// }

int	fn_philo_process(int ac, char **av)
{
	(void)ac;
	int		i;
	t_philo	*ph;

	ph = fn_init_philo(ac, av);

	i = 0;
	while (i < ph->nb_philo)
	{
		if((ph->pid[i] = fork()) && ph->pid[i] < 0)
			exit(1);
		if (ph->pid[i] == 0)
		{
			ph->i = i;
			fn_stat_philos(ph);
		}
		i++;
	}
	waitpid(ph->pid[0], NULL, 0);
	waitpid(ph->pid[1], NULL, 0);
	exit(0);
	return (0);
}

int	main(int ac, char **av)
{
	fn_philo_process(ac, av);
}