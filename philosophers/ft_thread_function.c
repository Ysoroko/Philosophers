/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_thread_function.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoroko <ysoroko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 14:32:41 by ysoroko           #+#    #+#             */
/*   Updated: 2021/09/07 17:03:57 by ysoroko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

static void	ft_print_aid(t_philo *philo, char *state_msg)
{
	int	current_time;

	current_time = philo->current_time - philo->start_time;
	printf("%d %s %s\n", current_time, philo->ph_num, state_msg);
}

/// Prints the message status as required per subject
/// Ex: "2000 1 is sleeping"
/// Returns 0 in case of success, -1 in case of error
int	ft_print_status(t_philo *philo, int state)
{
	char	*state_msg;

	state_msg = NULL;
	pthread_mutex_lock(philo->displaying);
	ft_get_current_time(philo);
	if (state == EATING)
		state_msg = "is eating";
	else if (state == FORK)
		state_msg = "has taken a fork";
	else if (state == SLEEPING)
		state_msg = "is sleeping";
	else if (state == THINKING)
		state_msg = "is thinking";
	else if (state == DIED)
		state_msg = "died";
	ft_print_aid(philo, state_msg);
	if (state != DIED)
		pthread_mutex_unlock(philo->displaying);
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
	while (*(philo->died))
		usleep(50);
	ft_setup_start_time(philo);
	if (philo->n_philos == 1)
		ft_eat_alone(philo);
	ft_philo_routine(philo);
	return (philo);
}
