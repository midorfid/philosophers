#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define ALIVE 0
#define DEAD 1

struct  s_philo;

typedef struct s_philo_thread {
    size_t                  philo_num;
    size_t                  times_ate;
    int                     is_eating;
    size_t                     last_meal;
    struct s_info          *data;
    pthread_mutex_t     l_fork;
    pthread_mutex_t     *r_fork;
    pthread_mutex_t     *meal_lock;
    pthread_mutex_t     *dead_status;
    pthread_mutex_t     *print_lock; 
} t_philo_thread;

typedef struct  s_info {
    size_t  philo_count;
    size_t  times_to_eat_count;
    int         *dead_flag;

    long long               t_start;
    float   time_to_die;
    float   time_to_eat;
    float   time_to_sleep;
    pthread_mutex_t     dead_status;
    pthread_mutex_t     meal_lock;
    pthread_mutex_t     print_lock;
    t_philo_thread  *philosopher;

} t_info;


t_info             *init(char **input);
int                 destroy_threads(size_t  thread_count, t_philo_thread philosophers[]);

void                *routine(void *input);
t_philo_thread      *philo_eat(t_philo_thread *philo, float time_to_wait);
void                philo_sleep(t_philo_thread *philo, float time_to_wait);
void                philo_think(t_philo_thread *philo);

int                 still_alive(t_philo_thread  *philosopher);
void                *monitoring(void *_data);
void                print_message(t_philo_thread *philo, size_t id, char *msg);

int                 ft_atoi(const char *str);
long long           timestamp();
#endif