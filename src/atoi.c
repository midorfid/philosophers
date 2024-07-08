#include "../include/philosophers.h"

int  ft_atoi(const char *str) {
    int     result;

    result = 0;
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return (result);
}
