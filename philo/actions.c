/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 20:28:04 by mbauer            #+#    #+#             */
/*   Updated: 2026/02/14 20:28:14 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
