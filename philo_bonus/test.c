# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <semaphore.h>

#define MAX_COUNT 10

int main()
{
    sem_t *sem;
	int *sval;
    pid_t pid;
    int i, count = 0;
    pthread_t	*thread;
    
    sem = sem_open("/my_semaphore", O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }
    
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) {
        // Child process
        while (count < MAX_COUNT) {
			// sem_getvalue(sem, sval);
            if (pthread_create(thread,
                NULL, &fn_philo_die, &count) != 0)
                return (ph_cpy->i);
            sem_wait(sem);
            printf("Child: %d\n", count++);
            
            sem_post(sem);
        }
    } else {
        // Parent process
        while (count < MAX_COUNT) {
            sem_wait(sem);
            printf("Parent: %d\n", count++);
            usleep(1000);
			//  printf("Child: %d ,%d\n", count++, *sem);
            sem_post(sem);
        }
    }
    wait(NULL);
    sem_close(sem);
    sem_unlink("/my_semaphore");
    return 0;
}