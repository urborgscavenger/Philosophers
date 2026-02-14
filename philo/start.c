/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 20:26:54 by mbauer            #+#    #+#             */
/*   Updated: 2026/02/14 20:27:43 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	create_philo_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.num_philos)
	{
		if (pthread_create(&sim->philos[i].thread, NULL, philo_routine,
				&sim->philos[i]))
			return (1);
		i++;
	}
	return (0);
}

static void	join_philo_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.num_philos)
	{
		pthread_join(sim->philos[i].thread, NULL);
		i++;
	}
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
	if (create_philo_threads(sim))
		return (1);
	if (pthread_create(&monitor, NULL, monitor_routine, sim))
		return (1);
	join_philo_threads(sim);
	pthread_join(monitor, NULL);
	return (0);
}
