/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 20:26:23 by mbauer            #+#    #+#             */
/*   Updated: 2026/02/14 20:27:29 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
