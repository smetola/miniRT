/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jllamas- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 20:12:56 by jllamas-          #+#    #+#             */
/*   Updated: 2024/03/20 19:05:47 by jllamas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	calculate_result(const char *str, unsigned int i)
{
	int	result;
	int	base;

	base = 10;
	result = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result *= base;
		result += str[i] - '0';
		i ++;
	}
	if (str[i])
		return (-1);
	return (result);
}

int	ft_pos_atoi(const char *str)
{
	unsigned int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			return (-1);
		i ++;
	}
	if (str[i] >= '0' && str[i] <= '9')
		return (calculate_result(str, i));
	return (-1);
}
