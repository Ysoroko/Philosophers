/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_thread_function.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoroko <ysoroko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 14:32:41 by ysoroko           #+#    #+#             */
/*   Updated: 2021/09/07 13:42:54 by ysoroko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

/// Joins pref and suff and returns the result of the malloc'd copy
static char	*ft_strjoin(char const *pref, char const *suff)
{
	char	*ret;
	char	*my_pref;
	char	*my_suff;
	size_t	i;
	size_t	j;

	if (pref == 0 || suff == 0)
		return (0);
	i = -1;
	j = -1;
	my_pref = (char *)(pref);
	my_suff = (char *)(suff);
	ret = malloc(sizeof(char) * (ft_strlen(my_pref) + ft_strlen(my_suff) + 2));
	if (ret == 0)
		return (0);
	while (pref[++i] != '\0')
		ret[i] = pref[i];
	ret[i++] = '\t';
	while (suff[++j] != '\0')
	{
		ret[i] = suff[j];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

/// Creates a temporary buffer with the message and prints it to the output.
/// Is more optimal than using printf or multiple calls to write.
static int	ft_message_to_print(t_philo *philo, char *state_msg)
{
	char	*msg1;
	char	*msg2;
	char	*time;

	time = ft_itoa((int)(philo->current_time - philo->start_time));
	if (!time)
		return (-1);
	msg1 = ft_strjoin(time, philo->ph_num);
	if (!msg1)
		return (ft_free_int_ret(time, "Failed to strjoin philo to time", -1));
	msg2 = ft_strjoin(msg1, state_msg);
	if (!msg2)
	{
		ft_free(time, NULL, NULL);
		return (ft_free_int_ret(msg1, "Malloc error", -1));
	}
	ft_putendl_fd(msg2, STDOUT);
	free(msg1);
	free(msg2);
	free(time);
	return (0);
}

/// Prints the message status as required per subject
/// Ex: "2000 1 is sleeping"
/// Returns 0 in case of success, -1 in case of error
int	ft_print_status(t_philo *philo, int state)
{
	char	*state_msg;

	state_msg = NULL;
	if (pthread_mutex_lock(philo->displaying))
		return (ft_puterr("Failed to lock display mutex"));
	if (ft_get_current_time(philo) == -1)
		return (ft_puterr("Failed to get current time"));
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
	ft_message_to_print(philo, state_msg);
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
	while (*(philo->died))
		usleep(1);
	if (ft_setup_start_time(philo) == -1)
		return (NULL);
	ft_philo_routine(philo);
	return (philo);
}
