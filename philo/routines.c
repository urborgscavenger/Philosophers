/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 20:26:43 by mbauer            #+#    #+#             */
/*   Updated: 2026/02/14 20:27:39 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
