/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 15:54:21 by mbauer            #+#    #+#             */
/*   Updated: 2026/02/14 20:28:57 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi_positive(const char *str)
{
	int		i;
	long	result;

	result = 0;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
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
			" time_to_sleep [num_meals]\n",
			argv[0]);
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
		|| config->time_to_eat <= 0 || config->time_to_sleep <= 0 || (argc == 6
			&& config->num_meals <= 0))
	{
		printf("Error: Invalid arguments.\n");
		return (1);
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
	if (start_sim(&sim))
	{
		cleanup_sim(&sim);
		printf("Error: Failed to start simulation.\n");
		return (1);
	}
	cleanup_sim(&sim);
	return (0);
}
