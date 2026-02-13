/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 15:54:21 by mbauer            #+#    #+#             */
/*   Updated: 2026/02/13 16:07:00 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_config
{
	int	num_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	num_meals; // optional, -1 if not provided
}	t_config;

typedef struct s_sim	t_sim; // forward declaration

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	long long		last_meal;
	int				meals_eaten;
	t_sim			*sim;
}	t_philo;

typedef struct s_sim
{
	t_config		config;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	struct timeval	start_time;
	int				someone_died;
}	t_sim;

int	ft_atoi(const char *str)
{
	int	result;
	int	i;

	result = 0;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' ||
		   str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '+')
		i++;
	if (str[i] < '0' || str[i] > '9')
		return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] != '\0')
		return (-1);
	return (result);
}

int	parse_args(int argc, char **argv, t_config *config)
{
	if (argc < 5 || argc > 6)
	{
		printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
		return (1);
	}
	config->num_philos = ft_atoi(argv[1]);
	config->time_to_die = ft_atoi(argv[2]);
	config->time_to_eat = ft_atoi(argv[3]);
	config->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		config->num_meals = ft_atoi(argv[5]);
	else
		config->num_meals = -1;
	if (config->num_philos == -1 || config->time_to_die == -1 ||
		config->time_to_eat == -1 || config->time_to_sleep == -1 ||
		(config->num_meals == -1 && argc == 6))
	{
		printf("Error: Invalid arguments. Must be positive integers.\n");
		return (1);
	}
	if (config->num_philos <= 0 || config->time_to_die <= 0 ||
		config->time_to_eat <= 0 || config->time_to_sleep <= 0 ||
		(config->num_meals != -1 && config->num_meals <= 0))
	{
		printf("Error: All arguments must be positive integers.\n");
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_config	config;

	if (parse_args(argc, argv, &config))
		return (1);
	// Hier kommt später der Rest des Programms
	printf("Parsed successfully: philos=%d, die=%d, eat=%d, sleep=%d, meals=%d\n",
		   config.num_philos, config.time_to_die, config.time_to_eat, config.time_to_sleep, config.num_meals);
	return (0);
}

