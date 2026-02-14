/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 15:54:21 by mbauer            #+#    #+#             */
/*   Updated: 2026/02/14 03:58:44 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_config
{
	int			num_philos;
	long long	time_to_die;
	long long	time_to_eat;
	long long	time_to_sleep;
	int			num_meals;
}	t_config;

typedef struct s_sim	t_sim;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	long long		last_meal;
	int				meals_eaten;
	pthread_mutex_t	meal_mutex;
	t_sim			*sim;
}	t_philo;

typedef struct s_sim
{
	t_config		config;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	long long		start_time;
	int				stop;
}	t_sim;

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	is_stopped(t_sim *sim)
{
	int	ret;

	pthread_mutex_lock(&sim->stop_mutex);
	ret = sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (ret);
}

void	set_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
}

void	precise_sleep(long long duration_ms, t_sim *sim)
{
	long long	start;
	long long	remaining;

	start = get_time_ms();
	while (!is_stopped(sim))
	{
		remaining = duration_ms - (get_time_ms() - start);
		if (remaining <= 0)
			break ;
		if (remaining > 2)
			usleep(500);
		else
			usleep(100);
	}
}

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->sim->print_mutex);
	if (!is_stopped(philo->sim))
		printf("%lld %d %s\n",
			get_time_ms() - philo->sim->start_time, philo->id, status);
	pthread_mutex_unlock(&philo->sim->print_mutex);
}

void	print_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->print_mutex);
	printf("%lld %d died\n",
		get_time_ms() - philo->sim->start_time, philo->id);
	pthread_mutex_unlock(&philo->sim->print_mutex);
}

int	ft_atoi_positive(const char *str)
{
	int		i;
	long	result;

	result = 0;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '+')
		i++;
	if (str[i] < '0' || str[i] > '9')
		return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		if (result > 2147483647)
			return (-1);
		i++;
	}
	if (str[i] != '\0')
		return (-1);
	return ((int)result);
}

int	parse_args(int argc, char **argv, t_config *config)
{
	if (argc < 5 || argc > 6)
	{
		printf("Usage: %s num_philos time_to_die time_to_eat"
			" time_to_sleep [num_meals]\n", argv[0]);
		return (1);
	}
	config->num_philos = ft_atoi_positive(argv[1]);
	config->time_to_die = ft_atoi_positive(argv[2]);
	config->time_to_eat = ft_atoi_positive(argv[3]);
	config->time_to_sleep = ft_atoi_positive(argv[4]);
	config->num_meals = -1;
	if (argc == 6)
		config->num_meals = ft_atoi_positive(argv[5]);
	if (config->num_philos <= 0 || config->time_to_die <= 0
		|| config->time_to_eat <= 0 || config->time_to_sleep <= 0
		|| (argc == 6 && config->num_meals <= 0))
	{
		printf("Error: Invalid arguments.\n");
		return (1);
	}
	return (0);
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, "has taken a fork");
}

void	eat(t_philo *philo)
{
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	precise_sleep(philo->sim->config.time_to_eat, philo->sim);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	think(t_philo *philo)
{
	long long	think_time;

	print_status(philo, "is thinking");
	if (philo->sim->config.num_philos % 2 != 0)
	{
		think_time = (philo->sim->config.time_to_eat * 2
				- philo->sim->config.time_to_sleep);
		if (think_time < 0)
			think_time = 0;
		precise_sleep(think_time, philo->sim);
	}
}

void	*solo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	print_status(philo, "is thinking");
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	precise_sleep(philo->sim->config.time_to_die, philo->sim);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->sim->config.num_philos == 1)
		return (solo_routine(arg));
	if (philo->id % 2 == 0)
		precise_sleep(philo->sim->config.time_to_eat / 2, philo->sim);
	while (!is_stopped(philo->sim))
	{
		take_forks(philo);
		if (is_stopped(philo->sim))
		{
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(philo->left_fork);
			break ;
		}
		eat(philo);
		if (is_stopped(philo->sim))
			break ;
		print_status(philo, "is sleeping");
		precise_sleep(philo->sim->config.time_to_sleep, philo->sim);
		if (is_stopped(philo->sim))
			break ;
		think(philo);
	}
	return (NULL);
}

int	check_death(t_sim *sim)
{
	int			i;
	long long	elapsed;

	i = 0;
	while (i < sim->config.num_philos)
	{
		pthread_mutex_lock(&sim->philos[i].meal_mutex);
		elapsed = get_time_ms() - sim->philos[i].last_meal;
		pthread_mutex_unlock(&sim->philos[i].meal_mutex);
		if (elapsed > sim->config.time_to_die)
		{
			set_stop(sim);
			print_death(&sim->philos[i]);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_all_ate(t_sim *sim)
{
	int	i;

	if (sim->config.num_meals == -1)
		return (0);
	i = 0;
	while (i < sim->config.num_philos)
	{
		pthread_mutex_lock(&sim->philos[i].meal_mutex);
		if (sim->philos[i].meals_eaten < sim->config.num_meals)
		{
			pthread_mutex_unlock(&sim->philos[i].meal_mutex);
			return (0);
		}
		pthread_mutex_unlock(&sim->philos[i].meal_mutex);
		i++;
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!is_stopped(sim))
	{
		if (check_death(sim))
			return (NULL);
		if (check_all_ate(sim))
		{
			set_stop(sim);
			return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}

int	init_forks(t_sim *sim)
{
	int	i;

	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->config.num_philos);
	if (!sim->forks)
		return (1);
	i = 0;
	while (i < sim->config.num_philos)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&sim->forks[i]);
			free(sim->forks);
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_philos(t_sim *sim)
{
	int	i;

	sim->philos = malloc(sizeof(t_philo) * sim->config.num_philos);
	if (!sim->philos)
		return (1);
	i = 0;
	while (i < sim->config.num_philos)
	{
		sim->philos[i].id = i + 1;
		sim->philos[i].sim = sim;
		sim->philos[i].meals_eaten = 0;
		sim->philos[i].last_meal = sim->start_time;
		if (i % 2 == 0)
		{
			sim->philos[i].left_fork = &sim->forks[i];
			sim->philos[i].right_fork
				= &sim->forks[(i + 1) % sim->config.num_philos];
		}
		else
		{
			sim->philos[i].left_fork
				= &sim->forks[(i + 1) % sim->config.num_philos];
			sim->philos[i].right_fork = &sim->forks[i];
		}
		if (pthread_mutex_init(&sim->philos[i].meal_mutex, NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&sim->philos[i].meal_mutex);
			free(sim->philos);
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_sim(t_sim *sim, t_config config)
{
	sim->config = config;
	sim->stop = 0;
	sim->start_time = get_time_ms();
	if (pthread_mutex_init(&sim->print_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&sim->stop_mutex, NULL))
	{
		pthread_mutex_destroy(&sim->print_mutex);
		return (1);
	}
	if (init_forks(sim))
	{
		pthread_mutex_destroy(&sim->print_mutex);
		pthread_mutex_destroy(&sim->stop_mutex);
		return (1);
	}
	if (init_philos(sim))
	{
		pthread_mutex_destroy(&sim->print_mutex);
		pthread_mutex_destroy(&sim->stop_mutex);
		free(sim->forks);
		return (1);
	}
	return (0);
}

void	cleanup_sim(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.num_philos)
	{
		pthread_mutex_destroy(&sim->philos[i].meal_mutex);
		pthread_mutex_destroy(&sim->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_mutex_destroy(&sim->stop_mutex);
	free(sim->philos);
	free(sim->forks);
}

int	start_sim(t_sim *sim)
{
	int			i;
	pthread_t	monitor;

	sim->start_time = get_time_ms();
	i = 0;
	while (i < sim->config.num_philos)
	{
		sim->philos[i].last_meal = sim->start_time;
		i++;
	}
	i = 0;
	while (i < sim->config.num_philos)
	{
		if (pthread_create(&sim->philos[i].thread, NULL,
				philo_routine, &sim->philos[i]))
			return (1);
		i++;
	}
	if (pthread_create(&monitor, NULL, monitor_routine, sim))
		return (1);
	i = 0;
	while (i < sim->config.num_philos)
	{
		pthread_join(sim->philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
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
	if (start_sim(&sim))
	{
		cleanup_sim(&sim);
		printf("Error: Failed to start simulation.\n");
		return (1);
	}
	cleanup_sim(&sim);
	return (0);
}
