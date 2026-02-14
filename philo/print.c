/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 20:26:32 by mbauer            #+#    #+#             */
/*   Updated: 2026/02/14 20:27:34 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->sim->print_mutex);
	if (!is_stopped(philo->sim))
		printf("%lld %d %s\n", get_time_ms() - philo->sim->start_time,
			philo->id, status);
	pthread_mutex_unlock(&philo->sim->print_mutex);
}

void	print_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->print_mutex);
	printf("%lld %d died\n", get_time_ms() - philo->sim->start_time, philo->id);
	pthread_mutex_unlock(&philo->sim->print_mutex);
}
