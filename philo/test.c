# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>

void *foo(void *i) 
{
	int a = *((int *) i);
	printf("i = %d\n", a);
	// free(i);
	return NULL;
}

void   fn_creat_thread()
{
	 pthread_t thread;
	for ( int i = 0; i < 10; ++i ) {
		int *arg = malloc(sizeof(*arg));
		if ( arg == NULL ) {
			fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
			exit(EXIT_FAILURE);
		}

		*arg = i;
		pthread_create(&thread, 0, foo, arg);
	}

	/*  Wait for threads, etc  */
}

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

void	fn

int main() {
	int i;
	int	j;

	i = 0;
	while (i < 20)
	{
		printf("\033[0;37m%ld %d died\n", fn_current_time(), i++);
		j = 0;
		usleep(100 * 1000);
	}
}