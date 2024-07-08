#include "../include/philosophers.h"

int     main(int argc, char *argv[]) {
    t_info             *info;

    if (argc != 5 && argc != 6)
        return(EXIT_FAILURE);
    info = init(argv);
    printf("philo_count:%ld\n", info->philo_count);
    printf("time_to_die:%f\n", info->time_to_die);
    printf("time_to_eat:%f\n", info->time_to_eat);
    printf("time_to_sleep:%f\n", info->time_to_sleep);
    printf("times_to_eat_count:%ld\n", info->times_to_eat_count);
    return(0);
}
