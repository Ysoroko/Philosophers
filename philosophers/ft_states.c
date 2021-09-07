/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_states.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoroko <ysoroko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 12:03:02 by ysoroko           #+#    #+#             */
/*   Updated: 2021/09/07 13:45:38 by ysoroko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

/* /// This function allows to print debug messages while in thread (mutexed)
** /// without the output being scrambled with other philosophers
** int	ft_print_mutexed(t_philo *philo, char *msg, int n, int ph_number)
** {
** 	if (pthread_mutex_lock(philo->displaying))
** 		return (ft_puterr("Failed to lock display mutex"));
** 	ft_putchar_fd('\n', STDOUT);
** 	ft_putstr_fd("MSG: ", STDOUT);
** 	ft_putendl_fd(msg, STDOUT);
** 	ft_putstr_fd("N: ", STDOUT);
** 	ft_putnbr_fd(n, STDOUT);
** 	ft_putchar_fd('\n', STDOUT);
** 	ft_putstr_fd("PH Number: ", STDOUT);
** 	ft_putnbr_fd(ph_number, STDOUT);
** 	ft_putchar_fd('\n', STDOUT);
** 	ft_putchar_fd('\n', STDOUT);
** 	if (pthread_mutex_unlock(philo->displaying))
** 		return (ft_puterr("Failed to unlock display mutex"));
** 	return (0);
** }
*/

/// The special eating state in a case where there's only one philosopher
/// He will have one fork only (because one fork per philosopher)
/// He will grab the fork, wait for the second one, but since there is none
/// He will eventually die
static int	ft_eat_alone(t_philo *philo)
{
	if (pthread_mutex_lock(philo->left_fork))
		return (ft_puterr("Failed to lock left fork"));
	if (ft_print_status(philo, FORK))
	{
		pthread_mutex_unlock(philo->left_fork);
		return (-1);
	}
	if (ft_msleep(philo->t_to_die + 10))
	{
		pthread_mutex_unlock(philo->left_fork);
		return (-1);
	}
	if (pthread_mutex_unlock(philo->left_fork))
		return (ft_puterr("Failed to unlock left fork"));
	return (0);
}

/// This function will lock forks differently depending on the philo's number
/// Odd philosophers will take left fork first, then right (and wait a bit)
/// Even philosophers will take right fork first, then left
/// This is done to avoid the death lock
static int	ft_lock_forks(t_philo *ph)
{
	if (ph->philo_number % 2)
	{
		if (pthread_mutex_lock(ph->left_fork) || ft_print_status(ph, FORK))
			return (ft_puterr("Failed to lock left fork"));
		usleep(500);
		if (pthread_mutex_lock(ph->right_fork) || ft_print_status(ph, FORK))
			return (ft_puterr("Failed to lock right fork"));
	}
	else if (!(ph->philo_number % 2))
	{
		if (pthread_mutex_lock(ph->right_fork) || ft_print_status(ph, FORK))
			return (ft_puterr("Failed to lock left fork"));
		if (pthread_mutex_lock(ph->left_fork) || ft_print_status(ph, FORK))
			return (ft_puterr("Failed to lock right fork"));
	}
	return (0);
}

/// Philosopher's "eating" state
/// The philosopher will take his 2 forks if available and start eating
/// Displays a message for each fork taken and when he starts to eat
/// Pauses the thread for t_to_eat milliseconds.
/// At the end updates the last time when the philosopher ate
/// Returns 0 in case of success, -1 in case of error
int	ft_eat(t_philo *philo)
{
	if (philo->n_philos == 1)
		return (ft_eat_alone(philo));
	if (ft_lock_forks(philo))
		return (-1);
	if (ft_print_status(philo, EATING))
		return (-1);
	if (ft_update_last_time_ate(philo))
		return (ft_puterr("Couldn't update last time the philosopher ate"));
	if (ft_msleep(philo->t_to_eat) == -1)
		return (ft_puterr("Failed to call usleep function"));
	if (pthread_mutex_unlock(philo->right_fork)
		|| pthread_mutex_unlock(philo->left_fork))
		return (ft_puterr("Failed to unlock the forks"));
	philo->n_times_ate++;
	return (0);
}

/// Philosopher's "sleeping" status he enters after he finished eating
/// Since the sleeping status is right after eating, we check in philosopher
/// ate enough times. If so, we quit.
/// Displays a message that the philosopher has started to sleep
/// Waits for t_to_sleep milliseconds
/// Returns 0 in case of success, -1 in case of error
int	ft_sleep(t_philo *philo)
{
	if (philo->n_to_eat && philo->n_times_ate >= philo->n_to_eat)
		return (1);
	if (ft_print_status(philo, SLEEPING) || ft_msleep(philo->t_to_sleep))
		return (-1);
	return (0);
}

/// Philosopher's "thinking" status he enters after he finished sleeping
/// Displays a message that the philosopher has started to think
/// Returns 0 in case of success, -1 in case of error
int	ft_think(t_philo *philo)
{
	if (ft_print_status(philo, THINKING))
		return (-1);
	return (0);
}
