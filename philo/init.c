/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 20:30:13 by mbauer            #+#    #+#             */
/*   Updated: 2026/02/14 20:31:01 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
			sim->philos[i].right_fork = &sim->forks[(i + 1)
				% sim->config.num_philos];
		}
		else
		{
			sim->philos[i].left_fork = &sim->forks[(i + 1)
				% sim->config.num_philos];
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
