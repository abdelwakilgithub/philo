/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabdelo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 12:59:51 by moabdelo          #+#    #+#             */
/*   Updated: 2022/10/26 13:00:25 by moabdelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo_bonus.h"

int	ft_atoi(const char *str)
{
	int				i;
	unsigned long	result;
	int				signe;

	i = 0;
	result = 0;
	signe = 1;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			signe = -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] + -48;
		i++;
	}
	if (result > 9223372036854775807 && signe > 0)
		return (-1);
	if (result > 9223372036854775807 && signe < 0)
		return (0);
	return (signe * result);
}
