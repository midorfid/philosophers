#include "../include/philosophers.h"

void    print_message(t_philo_thread *philo, size_t id, char *msg) {
    size_t time;

    pthread_mutex_lock(philo->print_lock);
    time = timestamp() - philo->data->t_start;
    if (!still_alive(philo))
        printf("%ldms P%ld %s\n", time, id, msg);
    pthread_mutex_unlock(philo->print_lock);
}

int     philo_dead(t_philo_thread *philo, size_t time_to_die) {
    long long   time;
    pthread_mutex_lock(philo->meal_lock);
    time = timestamp();
    if ((size_t)time >= philo->last_meal && (time - philo->last_meal) >= time_to_die && philo->is_eating == 0) {
        printf("last_meal: %ld\n", philo->last_meal);
        printf("timestamp: %lld\n", time);
        printf("time_to_die: %ld\n", time_to_die);
        return (pthread_mutex_unlock(philo->meal_lock), 1);
    }
    pthread_mutex_unlock(philo->meal_lock);
    return (0);
}

int     death_check(t_info *data) {
    size_t  i;

    i = 0;
    while (i < data->philo_count) {
        if (philo_dead(&data->philosopher[i], data->time_to_die)) {
            print_message(&data->philosopher[i], i + 1, "died");
            pthread_mutex_lock(&data->dead_status);
            *data->dead_flag = DEAD;
            pthread_mutex_unlock(&data->dead_status);
            return (1);
            }
        i++;
    }
    return (0);
}

int     meals_limit(t_info *data) {
    size_t      i;
    size_t      finished_eating;

    i = 0;
    finished_eating = 0;
    while (i < data->philo_count) {
        pthread_mutex_lock(data->philosopher[i].meal_lock);
        if (data->philosopher[i].times_ate >= data->times_to_eat_count)
            finished_eating++;
        pthread_mutex_unlock(data->philosopher[i].meal_lock);
        i++;
    }
    if (finished_eating == data->philo_count) {
        printf("All philos finished. They ate %ld meals in total!\n", finished_eating * data->times_to_eat_count);
        pthread_mutex_lock(&data->dead_status);
        *data->dead_flag = DEAD;
        pthread_mutex_unlock(&data->dead_status);
        return (1);
    }
    return (0);
}

void    *monitoring(void *_data) {
    t_info  *data;

    data = _data;
    while (1) {
        if (death_check(data) || meals_limit(data))
            break;
    }
    return (_data);
}
