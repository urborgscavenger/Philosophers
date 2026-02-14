/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: generated <generated@student.42.fr>         +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 04:30:00 by gen               #+#    #+#             */
/*   Updated: 2026/02/14 04:30:00 by gen              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_config
{
	int					num_philos;
	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;
	int					num_meals;
}						t_config;

typedef struct s_sim	t_sim;

typedef struct s_philo
{
	int					id;
	pthread_t			thread;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	long long			last_meal;
	int					meals_eaten;
	pthread_mutex_t		meal_mutex;
	t_sim				*sim;
}						t_philo;

typedef struct s_sim
{
	t_config			config;
	t_philo				*philos;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		stop_mutex;
	long long			start_time;
	int					stop;
}						t_sim;

long long				get_time_ms(void);
int						is_stopped(t_sim *sim);
void					set_stop(t_sim *sim);
void					precise_sleep(long long duration_ms, t_sim *sim);

void					print_status(t_philo *philo, char *status);
void					print_death(t_philo *philo);

int						ft_atoi_positive(const char *str);
int						parse_args(int argc, char **argv, t_config *config);

void					take_forks(t_philo *philo);
void					eat(t_philo *philo);
void					think(t_philo *philo);

void					*solo_routine(void *arg);
void					*philo_routine(void *arg);

int						check_death(t_sim *sim);
int						check_all_ate(t_sim *sim);
void					*monitor_routine(void *arg);

int						init_forks(t_sim *sim);
int						init_philos(t_sim *sim);
int						init_sim(t_sim *sim, t_config config);
void					cleanup_sim(t_sim *sim);

int						start_sim(t_sim *sim);

#endif
