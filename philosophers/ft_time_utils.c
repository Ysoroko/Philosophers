/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_time_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoroko <ysoroko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 10:12:48 by ysoroko           #+#    #+#             */
/*   Updated: 2021/09/07 13:10:41 by ysoroko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

/// Working in milliseconds!
/// 1ms = 1000us (microseconds, usleep +)
int	ft_setup_start_time(t_philo *philo)
{
	struct timeval	time_value;
	suseconds_t		in_milliseconds;

	if (gettimeofday(&time_value, NULL) == -1)
		return (-1);
	in_milliseconds = time_value.tv_usec / 1000 + time_value.tv_sec * 1000;
	philo->start_time = in_milliseconds;
	return (0);
}

/// Stores the current time (in microseconds) in philo->current_time
/// Returns 0 in case of success or -1 if a call to gettimeofday function fails
int	ft_get_current_time(t_philo *philo)
{
	struct timeval	time_value;
	suseconds_t		in_milliseconds;
	suseconds_t		start_time;

	if (gettimeofday(&time_value, NULL) == -1)
		return (-1);
	start_time = philo->start_time;
	in_milliseconds = time_value.tv_usec / 1000 + time_value.tv_sec * 1000;
	philo->current_time = in_milliseconds;
	return (0);
}

/// A replacement of usleep function which works with microseconds.
/// Pauses the execution of the calling thread for n_milliseconds milliseconds.
/// Returns -1 in case of error or 0 in case of success.
int	ft_msleep(int n_milliseconds)
{
	int	in_microseconds;

	in_microseconds = n_milliseconds * 1000;
	return (usleep(in_microseconds));
}

/// This function will update philo->t_without_eating component
/// If a philosopher doesn't eat for t_to_die milliseconds, he will die.
int	ft_update_last_time_ate(t_philo *philo)
{
	struct timeval	time_value;
	useconds_t		in_milliseconds;

	if (gettimeofday(&time_value, NULL) == -1)
		return (-1);
	in_milliseconds = time_value.tv_usec / 1000 + time_value.tv_sec * 1000;
	philo->time_last_time_ate = in_milliseconds - philo->start_time;
	return (0);
}

/// Returns 1 if a philo has died, 0 otherwise
int	ft_check_if_philo_died(t_philo *philo)
{
	useconds_t	time_without_eating;

	time_without_eating = philo->current_time - philo->time_last_time_ate;
	if (time_without_eating >= ((useconds_t)philo->t_to_die))
		return (1);
	else
		return (0);
}
