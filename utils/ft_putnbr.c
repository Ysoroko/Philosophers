/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoroko <ysoroko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 14:58:04 by ysoroko           #+#    #+#             */
/*   Updated: 2021/09/07 15:05:47 by ysoroko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

static int	ft_abs(int n)
{
	if (n < 0)
	{
		return (-1 * n);
	}
	else
	{
		return (n);
	}
}

static int	ft_div(int n)
{
	int	d;
	int	count;

	d = abs(n);
	count = 1;
	if (d >= 1000000000)
	{
		return (1000000000);
	}
	while (d > 0)
	{
		d = d / 10;
		count = count * 10;
	}
	count = count / 10;
	return (count);
}

void	ft_putnbr(int n)
{
	int	d;
	int	c;

	c = ft_abs(n);
	if (n == 0)
		write(1, "0", 1);
	else if (n > 0)
	{
		d = ft_div(c);
		while (d >= 1)
		{
			ft_putchar_fd('0' + c / d, STDOUT);
			c = c % d;
			d = d / 10;
		}
	}
	else if (n == -2147483648)
		write(1, "-2147483648", 11);
	else
	{
		c = -n;
		ft_putchar_fd('-', STDOUT);
	}
}
