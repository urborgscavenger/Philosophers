/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 20:27:00 by mbauer            #+#    #+#             */
/*   Updated: 2026/02/14 20:27:47 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
