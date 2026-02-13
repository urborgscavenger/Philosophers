/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 15:54:21 by mbauer            #+#    #+#             */
/*   Updated: 2026/02/13 19:46:41 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

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

long long	get_current_time(struct timeval start)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec - start.tv_sec) * 1000 + (now.tv_usec - start.tv_usec) / 1000);
}

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->sim->print_mutex);
	printf("%lld %d %s\n", get_current_time(philo->sim->start_time), philo->id, status);
	pthread_mutex_unlock(&philo->sim->print_mutex);
}

int	take_forks(t_philo *philo)
{
	t_sim *sim = philo->sim;

	if (!philo->right_fork)
		return (0); // Can't take both forks
	if (philo->id % 2 == 1)
	{
		// Odd: take left first
		while (!sim->someone_died)
		{
			if (pthread_mutex_trylock(philo->left_fork) == 0)
			{
				if (sim->someone_died)
				{
					pthread_mutex_unlock(philo->left_fork);
					return (0);
				}
				print_status(philo, "has taken a fork");
				while (!sim->someone_died)
				{
					if (pthread_mutex_trylock(philo->right_fork) == 0)
					{
						if (sim->someone_died)
						{
							pthread_mutex_unlock(philo->left_fork);
							pthread_mutex_unlock(philo->right_fork);
							return (0);
						}
						print_status(philo, "has taken a fork");
						return (1);
					}
					usleep(100);
				}
				pthread_mutex_unlock(philo->left_fork);
				return (0);
			}
			usleep(100);
		}
		return (0);
	}
	else
	{
		// Even: take right first
		while (!sim->someone_died)
		{
			if (pthread_mutex_trylock(philo->right_fork) == 0)
			{
				if (sim->someone_died)
				{
					pthread_mutex_unlock(philo->right_fork);
					return (0);
				}
				print_status(philo, "has taken a fork");
				while (!sim->someone_died)
				{
					if (pthread_mutex_trylock(philo->left_fork) == 0)
					{
						if (sim->someone_died)
						{
							pthread_mutex_unlock(philo->right_fork);
							pthread_mutex_unlock(philo->left_fork);
							return (0);
						}
						print_status(philo, "has taken a fork");
						return (1);
					}
					usleep(100);
				}
				pthread_mutex_unlock(philo->right_fork);
				return (0);
			}
			usleep(100);
		}
		return (0);
	}
}

void	eat(t_philo *philo)
{
	print_status(philo, "is eating");
	philo->last_meal = get_current_time(philo->sim->start_time);
	usleep(philo->sim->config.time_to_eat * 1000);
	philo->meals_eaten++;
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	sleep_philo(t_philo *philo)
{
	print_status(philo, "is sleeping");
	usleep(philo->sim->config.time_to_sleep * 1000);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	// usleep(philo->id * 1000); // No stagger
	while (!philo->sim->someone_died)
	{
		print_status(philo, "is thinking");
		if (take_forks(philo))
		{
			eat(philo);
			put_forks(philo);
		}
		sleep_philo(philo);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;
	int		all_eaten;

	sim = (t_sim *)arg;
	while (!sim->someone_died)
	{
		if (sim->config.num_meals != -1)
		{
			all_eaten = 1;
			i = 0;
			while (i < sim->config.num_philos)
			{
				if (sim->philos[i].meals_eaten < sim->config.num_meals)
					all_eaten = 0;
				i++;
			}
			if (all_eaten)
			{
				sim->someone_died = 1; // Stop simulation
				return (NULL);
			}
		}
		i = 0;
		while (i < sim->config.num_philos)
		{
			if (get_current_time(sim->start_time) - sim->philos[i].last_meal > sim->config.time_to_die)
			{
				sim->someone_died = 1;
				print_status(&sim->philos[i], "died");
				return (NULL);
			}
			i++;
		}
		usleep(1000); // check every 1ms
	}
	return (NULL);
}

int	init_sim(t_sim *sim, t_config config)
{
	int	i;

	sim->config = config;
	sim->philos = malloc(sizeof(t_philo) * config.num_philos);
	if (!sim->philos)
		return (1);
	sim->forks = malloc(sizeof(pthread_mutex_t) * config.num_philos);
	if (!sim->forks)
	{
		free(sim->philos);
		return (1);
	}
	i = 0;
	while (i < config.num_philos)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL))
		{
			// Cleanup previous mutexes
			while (--i >= 0)
				pthread_mutex_destroy(&sim->forks[i]);
			free(sim->forks);
			free(sim->philos);
			return (1);
		}
		i++;
	}
	if (pthread_mutex_init(&sim->print_mutex, NULL))
	{
		i = 0;
		while (i < config.num_philos)
			pthread_mutex_destroy(&sim->forks[i++]);
		free(sim->forks);
		free(sim->philos);
		return (1);
	}
	gettimeofday(&sim->start_time, NULL);
	sim->someone_died = 0;
	i = 0;
	while (i < config.num_philos)
	{
		sim->philos[i].id = i + 1;
		sim->philos[i].sim = sim;
		sim->philos[i].left_fork = &sim->forks[i];
		if (config.num_philos == 1)
			sim->philos[i].right_fork = NULL; // Only one fork
		else
			sim->philos[i].right_fork = &sim->forks[(i + 1) % config.num_philos];
		sim->philos[i].last_meal = get_current_time(sim->start_time); // Start from now
		sim->philos[i].meals_eaten = 0;
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_config	config;
	t_sim		sim;

	if (parse_args(argc, argv, &config))
		return (1);
	if (init_sim(&sim, config))
	{
		printf("Error: Failed to initialize simulation.\n");
		return (1);
	}
	// Start threads
	int	i;
	i = 0;
	while (i < sim.config.num_philos)
	{
		if (pthread_create(&sim.philos[i].thread, NULL, philo_routine, &sim.philos[i]))
		{
			printf("Error: Failed to create philosopher thread.\n");
			// Cleanup
			while (--i >= 0)
				pthread_join(sim.philos[i].thread, NULL);
			free(sim.philos);
			free(sim.forks);
			return (1);
		}
		i++;
	}
	pthread_t	monitor_thread;
	if (pthread_create(&monitor_thread, NULL, monitor_routine, &sim))
	{
		printf("Error: Failed to create monitor thread.\n");
		i = 0;
		while (i < sim.config.num_philos)
			pthread_join(sim.philos[i++].thread, NULL);
		free(sim.philos);
		free(sim.forks);
		return (1);
	}
	// Wait for threads
	i = 0;
	while (i < sim.config.num_philos)
	{
		pthread_join(sim.philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	// Cleanup
	i = 0;
	while (i < sim.config.num_philos)
		pthread_mutex_destroy(&sim.forks[i++]);
	pthread_mutex_destroy(&sim.print_mutex);
	free(sim.philos);
	free(sim.forks);
	return (0);
}
