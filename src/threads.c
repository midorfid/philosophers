#include "../include/philosophers.h"

int     still_alive(t_philo_thread  *philosopher) {
    pthread_mutex_lock(philosopher->dead_status);
    if (*philosopher->data->dead_flag == DEAD) {
        return (pthread_mutex_unlock(philosopher->dead_status), 1);
    }
    pthread_mutex_unlock(philosopher->dead_status);
    return (0);
}

// n-1 n   n+1 n+2
void  *routine(void *input) {
    t_philo_thread  *philosopher;

    philosopher = (t_philo_thread *)input;
    if (philosopher->philo_num % 2 == 0)
        usleep(1);
    while (!still_alive(philosopher)) {
        pthread_mutex_lock(&philosopher->l_fork);
        pthread_mutex_lock(philosopher->r_fork);
        philosopher = philo_eat(philosopher, philosopher->data->time_to_eat);
        pthread_mutex_unlock(&philosopher->l_fork);
        pthread_mutex_unlock(philosopher->r_fork);
        philo_sleep(philosopher, philosopher->data->time_to_sleep);
        philo_think(philosopher);
    }
    // free(input);
    return(NULL);
}
