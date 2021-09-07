/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_death.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoroko <ysoroko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/02 11:40:54 by ysoroko           #+#    #+#             */
/*   Updated: 2021/09/07 13:33:00 by ysoroko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

/// This function checks for philosopher's death
/// Returns 1 in case of death or error, 0 otherwise
static int	ft_check_philo_death(t_philo **philos, int i)
{
	useconds_t		current_time;
	int				time_without_eating;

	if (ft_get_current_time(philos[i]))
		return (-1);
	current_time = (philos[i])->current_time - philos[i]->start_time;
	time_without_eating = current_time - (int)(philos[i])->time_last_time_ate;
	if ((int)time_without_eating > philos[i]->t_to_die)
	{
		*(philos[i]->died) = 1;
		ft_print_status(philos[i], DIED);
		return (1);
	}
	return (0);
}

/// This function checks both whether a philo has died and if he has eaten
/// enough times
/// Returns 1 if he has died/in case of error, 0 otherwise
static int	ft_check_philo_death_and_n_ate(t_philo **philos, int n_philos)
{
	int	i;
	int	j;
	int	n_to_eat;
	int	n_times_ate;

	i = -1;
	n_to_eat = philos[0]->n_to_eat;
	while (++i < n_philos && (!(*philos[i]->died)))
	{	
		if (ft_check_philo_death(philos, i))
			return (1);
		j = -1;
		while (++j < n_philos)
		{
			n_times_ate = philos[j]->n_times_ate;
			if (n_to_eat && n_times_ate < n_to_eat)
				break ;
			if (n_to_eat && j == n_philos - 1 && n_times_ate >= n_to_eat)
				return (1);
		}
	}
	return (0);
}

/// Function called to clean up the memory
/// To clean up:
/// 1) Free the address of left fork
/// 2) Destroy the mutex of the same left fork
///	3) Free the address of the "displaying" mutex (once for all the philos)
/// 4) Destroy the mutex of "displaying" mutex (once for all the philos)
/// 5) Free int *died (once for all the philos)
/// 5) Free t_philo structure itself
static int	ft_cleanup_threads_and_mutexes(t_philo **philos, int n_philos)
{
	int	i;

	i = -1;
	if (pthread_mutex_destroy(philos[0]->displaying))
		return (-1);
	free(philos[0]->displaying);
	free(philos[0]->died);
	while (++i < n_philos)
	{
		if (pthread_mutex_destroy(philos[i]->left_fork))
			return (-1);
		free(philos[i]->left_fork);
		free(philos[i]->ph_num);
		free(philos[i]);
	}
	return (0);
}

/// This is a thread function which will check the current time for every
/// philosopher and check if the philosopher has died
/// Returns 
static void	*ft_check_time_and_death(void *arg)
{
	t_death_struct	*death_struct;
	t_philo			**philos;
	int				n_philos;

	death_struct = (t_death_struct *)arg;
	philos = death_struct->philos;
	n_philos = death_struct->n_philos;
	while (1)
	{
		ft_msleep(1);
		if (ft_check_philo_death_and_n_ate(philos, n_philos))
		{
			ft_cleanup_threads_and_mutexes(philos, n_philos);
			free(death_struct);
			return (NULL);
		}
	}
	return (arg);
}

pthread_t	*ft_initialize_death_check_thread(t_philo **p, int n_ph, int t_die)
{
	pthread_t		*d_thread;
	t_death_struct	*d_struct;

	d_struct = malloc(sizeof(t_death_struct));
	if (!d_struct)
		return ((pthread_t *)ft_puterr_ptr("Failed to malloc t_death_struct"));
	d_thread = malloc(sizeof(pthread_t));
	if (!d_thread)
		return (ft_free(d_struct, "Failed to malloc death_thread", NULL));
	d_struct->n_philos = n_ph;
	d_struct->philos = p;
	d_struct->t_to_die = t_die;
	if (pthread_create(d_thread, NULL, &ft_check_time_and_death, d_struct))
		return ((pthread_t *)ft_free(d_struct, "D Thread not created", NULL));
	return (d_thread);
}
