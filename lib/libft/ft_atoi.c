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

int	calculate_result(const char *str, unsigned int i)
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
	return (result);
}

int	ft_atoi(const char *str)
{
	int				sign;
	unsigned int	i;

	sign = 1;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || \
		str[i] == '\f' || str[i] == '\v' || str[i] == '\r')
		i ++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -sign;
		i ++;
	}
	if (str[i] >= '0' && str[i] <= '9')
		return (calculate_result(str, i) * sign);
	return (0);
}
