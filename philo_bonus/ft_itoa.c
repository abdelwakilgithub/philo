/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabdelo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 14:46:56 by moabdelo          #+#    #+#             */
/*   Updated: 2022/10/25 14:46:59 by moabdelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static long	abs_nb(long nb)
{
	if (nb < 0)
		return (-nb);
	else
		return (nb);
}

static int	len_nbcal(long n)
{
	if (n < 10)
		return (1);
	else
		return (len_nbcal(n / 10) + 1);
}

static int	len_nb(long n)
{
	if (n < 0)
		return (len_nbcal(abs_nb(n)) + 1);
	return (len_nbcal(abs_nb(n)));
}

char	*ft_itoa(int n)
{
	int		i;
	int		len;
	long	nb;
	char	*nb_str;

	nb = n;
	len = len_nb(nb);
	nb = abs_nb(nb);
	nb_str = (char *)malloc(len_nb(n) * sizeof(char) + 1);
	if (!nb_str)
		return (0);
	i = len - 1;
	while (nb >= 10)
	{
		nb_str[i--] = nb % 10 + 48;
		nb = nb / 10;
	}
	nb_str[i] = nb + 48;
	if (n < 0)
		nb_str[0] = '-';
	nb_str[len] = '\0';
	return (nb_str);
}
