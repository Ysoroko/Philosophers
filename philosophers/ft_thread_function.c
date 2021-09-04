/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_thread_function.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoroko <ysoroko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 14:32:41 by ysoroko           #+#    #+#             */
/*   Updated: 2021/09/04 11:18:55 by ysoroko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

/// Prints the message status as required per subject
/// Ex: "2000 1 is sleeping"
/// Returns 0 in case of success, -1 in case of error
int	ft_print_status(t_philo *philo, int state)
{
	if (pthread_mutex_lock(philo->displaying))
		return (ft_puterr("Failed to lock display mutex"));
	if (ft_get_current_time(philo) == -1)
	{
		pthread_mutex_unlock(philo->displaying);
		return (ft_puterr("Failed to get current time"));
	}
	ft_putnbr_fd(philo->current_time - philo->start_time, STDOUT);
	ft_putchar_fd('\t', STDOUT);
	ft_putnbr_fd(philo->philo_number, STDOUT);
	ft_putchar_fd('\t', STDOUT);
	if (state == EATING)
		ft_putendl_fd("is eating", STDOUT);
	else if (state == FORK)
		ft_putendl_fd("has taken a fork", STDOUT);
	else if (state == SLEEPING)
		ft_putendl_fd("is sleeping", STDOUT);
	else if (state == THINKING)
		ft_putendl_fd("is thinking", STDOUT);
	else if (state == DIED)
		ft_putendl_fd("died", STDOUT);
	if (state != DIED)
		if (pthread_mutex_unlock(philo->displaying))
			return (ft_puterr("Failed to unlock display mutex"));
	return (0);
}

/// The routine where each philosopher will eat, sleep and think
/// 1: They eat if both their forks are available
/// 2: They sleep after they finished eating
/// 3: They think after they finished sleeping
/// !!! If at any moment they didn't eat for t_to_die milliseconds, they die
static int	ft_philo_routine(t_philo *philo)
{
	while (1)
	{
		if (*(philo->died) || ft_eat(philo))
			return (-1);
		if (*(philo->died) || ft_sleep(philo))
			return (-1);
		if (*(philo->died) || ft_think(philo))
			return (-1);
	}
	return (0);
}

/// Function called when creating a new thread
/// Sets up the starting time, enters the philopher's routine (eat-sleep-think)
/// At the end once out of the routine, cleans up the memory and the mutexes.
void	*ft_thread_function(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (ft_setup_start_time(philo) == -1)
		return (NULL);
	ft_philo_routine(philo);
	free(philo);
	return (NULL);
}
