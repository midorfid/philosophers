#include "../include/philosophers.h"

t_philo_thread *philo_eat(t_philo_thread *philo, float time_to_wait) {
    pthread_mutex_lock(philo->meal_lock);
    philo->is_eating = 1;
    philo->last_meal = timestamp();
    philo->times_ate++;
    philo->is_eating = 0;
    pthread_mutex_unlock(philo->meal_lock);
    print_message(philo, philo->philo_num, "is eating");
    usleep(time_to_wait * 1000);
    return(philo);
}

void    philo_sleep(t_philo_thread *philo, float time_to_wait) {
    print_message(philo, philo->philo_num, "is sleeping");
    usleep(time_to_wait * 1000);
}

void    philo_think(t_philo_thread *philo) {
    print_message(philo, philo->philo_num, "is thinking");
}