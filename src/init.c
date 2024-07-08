#include "../include/philosophers.h"

// get milliseconds
long long    timestamp() {
    struct timeval      tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);    
}

static t_info     *init_input(char **input) {
    t_info     *info;

    info = malloc(sizeof(t_info));
    info->dead_flag = malloc(sizeof(int));
    *info->dead_flag = ALIVE;
    info->philo_count = (size_t)ft_atoi(*(++input));
    info->time_to_die = (float)ft_atoi(*(++input));
    info->time_to_eat = (float)ft_atoi(*(++input));
    info->time_to_sleep = (float)ft_atoi(*(++input));
    if (*(++input) != NULL) {
        info->times_to_eat_count = (size_t)ft_atoi(*(input));
        if (info->times_to_eat_count != 0) {
            pthread_mutex_init(&info->meal_lock, NULL);
        }
    }
    else
        info->times_to_eat_count = 0;
    return(info);
}

void    init_philos(t_info *info) {
    size_t  i;

    info->philosopher = malloc(sizeof(t_philo_thread) * info->philo_count);
    if (info->philosopher == NULL)
        return;
    info->philosopher->r_fork = malloc(sizeof(pthread_mutex_t) * info->philo_count);
    if (info->philosopher->r_fork == NULL)
        return;
    info->philosopher->meal_lock = malloc(sizeof(pthread_mutex_t) * info->philo_count);
    if (info->philosopher->meal_lock == NULL)
        return;
    i = 0;
    info->t_start = timestamp();
    while (i < info->philo_count) {
        info->philosopher[i].times_ate = 0;
        info->philosopher[i].philo_num = i + 1;
        info->philosopher[i].is_eating = 0;
        info->philosopher[i].last_meal = timestamp();
        info->philosopher[i].data = info;
        info->philosopher[i].meal_lock = &info->meal_lock;
        info->philosopher[i].dead_status = &info->dead_status;
        info->philosopher[i].print_lock = &info->print_lock;
        pthread_mutex_init(&info->philosopher[i].l_fork, NULL);
        if (i == info->philo_count - 1)
            info->philosopher[i].r_fork = &info->philosopher[0].l_fork;
        else
            info->philosopher[i].r_fork = &info->philosopher[i + 1].l_fork;
        i++;
    }
    return;
}

static int    init_threads(t_philo_thread *philosopher) {
    size_t  i;
    pthread_t   th[4];
    pthread_t   monitor;

    i = 0;
    pthread_mutex_init(&philosopher->data->dead_status, NULL);
    pthread_mutex_init(&philosopher->data->print_lock, NULL);
    // pthread_mutex_init(&philosopher->data->meal_lock, NULL);
    if (pthread_create(&monitor, NULL, &monitoring, (void *)&(*philosopher->data)) != 0)
        return(EXIT_FAILURE);
    while (i < philosopher->data->philo_count) {
        if (pthread_create(th + i, NULL, &routine, (void *)&philosopher[i]) != 0)
            return(EXIT_FAILURE);
        print_message(philosopher, i + 1, "created");
        i++;
    }
    i = 0;
    while (i < philosopher->data->philo_count) {
        if (pthread_join(th[i], NULL) != 0)
            return(EXIT_FAILURE);
        print_message(philosopher, i + 1, "deleted");
        i++;
    }
    if (pthread_join(monitor, NULL) != 0)
        return(EXIT_FAILURE);
    return (0);
}

void    destroy_philos(void) {
    exit(EXIT_FAILURE);
}

t_info     *init(char **input) {
    t_info         *info;
    t_philo_thread  *philosopher;
    size_t          i;

    info = init_input(input);
    init_philos(info);
    philosopher = info->philosopher;
    if (init_threads(philosopher) != 0) {
        destroy_philos();
        return (NULL);
    }
    // usleep(100000);
    i = 0;
    while(i < info->philo_count) {
        if (pthread_mutex_destroy(&info->philosopher[i].l_fork) != 0)
            printf("failed");
        i++;
    }
    pthread_mutex_destroy(&info->dead_status);
    pthread_mutex_destroy(&info->meal_lock);
    pthread_mutex_destroy(&info->print_lock);
    return(info);
}
