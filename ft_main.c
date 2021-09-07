/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoroko <ysoroko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 11:04:33 by ysoroko           #+#    #+#             */
/*   Updated: 2021/09/07 15:13:57 by ysoroko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/// Initializes the "display" mutex
/// Returns the mutex or a NULL pointer in case of an error
void	*ft_initialize_display_mutex(void)
{
	pthread_mutex_t	*displaying;

	if (!ft_malloc(sizeof(pthread_mutex_t), (void **)&displaying))
		return (NULL);
	if (pthread_mutex_init(displaying, NULL))
		return (ft_free(displaying, "Failed to init a display mutex", NULL));
	return (displaying);
}

/// Initializes the t_philo structure, and the mutexes
/// Returns 0 in case of success or -1 if an error
static int	ft_init(t_philo **ph, t_main_args *args, int i, pthread_mutex_t *d)
{
	ph[2] = ft_initialize_philo(args, i + 1, &(ph[0]));
	if (!ph[2])
		return (ft_puterr("Failed to initialize a t_philo"));
	if (ft_initialize_forks_mutex(ph[2], args->n_philos, ph[1], ph[0]))
		return (-1);
	ph[2]->displaying = d;
	return (0);
}

void	*ft_ph(t_philo **ph, t_main_args *arg, pthread_t *r, t_philo **p)
{
	int	i;

	i = -1;
	while (++i < arg->n_philos)
	{
		if (ft_init(ph, arg, i, arg->displaying))
			return (ft_free(r, NULL, NULL));
		(ph[2]->died = arg->dead);
		if (pthread_create(&(r[i]), NULL, &ft_thread_function, ph[2]))
			return (ft_free(r, "Failed to create a thread", NULL));
		ph[1] = ph[2];
		p[i] = ph[2];
	}
	*(p[0])->died = 0;
	return (p);
}

/// Initialize an array of threads of n_philos elements with malloc
/// Creates a thread for every philosopher and joins the threads
/// Returns a NULL pointer in case of an error
static pthread_t	*ft_initialize_threads(t_main_args *arg, t_philo **p)
{
	pthread_t		*ret;
	pthread_t		*death;
	t_philo			*ph[3];
	int				i;

	if (!ft_malloc(sizeof(*ret) * arg->n_philos, (void **)&ret))
		return (ft_puterr_ptr("Failed to malloc the philosophers"));
	i = arg->n_philos;
	ph[0] = NULL;
	if (!ft_ph(ph, arg, ret, p))
		return (NULL);
	death = ft_initialize_death_check_thread(p, arg->n_philos, arg->t_to_die);
	if (!death)
		return (NULL);
	while (--i >= 0)
		if (pthread_detach(ret[i]))
			return (ft_free(ret, "Failed to join a thread", NULL));
	if (pthread_join(*death, NULL))
		return (NULL);
	return (ret);
}

/*
** 1 - number of philosophers
** 2 - time to die
** 3 - time to eat
** 4 - time to sleep
** (optionnal) 5 - number of times they need to eat
*/

int	main(int argc, char **argv)
{
	t_main_args	*main_args;
	pthread_t	*threads;
	t_philo		**philos;

	if (ft_main_args_error(argc, argv))
		return (-1);
	philos = malloc(sizeof(t_philo *) * ft_atol(argv[1]));
	if (!philos)
		return (ft_puterr("Failed to malloc the philosophers array"));
	main_args = ft_initialize_main_args_struct(argc, argv);
	if (!main_args)
		return (ft_free_int_ret(philos, NULL, -1));
	threads = ft_initialize_threads(main_args, philos);
	if (!threads)
		return (-1);
	free(main_args);
	free(threads);
	free(philos);
	return (0);
}
