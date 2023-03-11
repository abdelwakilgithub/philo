/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabdelo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 14:01:34 by moabdelo          #+#    #+#             */
/*   Updated: 2022/10/25 14:02:36 by moabdelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	i;
	char	*value;

	i = 0;
	if (count * size >= SIZE_MAX)
		return (0);
	value = (char *)malloc(count * size);
	if (!value)
		return (0);
	while (i < (count * size))
		value[i++] = 0;
	return (value);
}
