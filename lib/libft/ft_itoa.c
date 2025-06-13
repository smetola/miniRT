/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jllamas- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 15:07:49 by jllamas-          #+#    #+#             */
/*   Updated: 2024/03/20 19:08:28 by jllamas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_nsize(int n)
{
	size_t	size;

	if (!n)
		return (1);
	size = 0;
	while (n)
	{
		n /= 10;
		size ++;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	size_t		size;
	long int	num;
	char		*result;

	num = n;
	size = ft_nsize(n);
	if (num < 0)
	{
		num = -num;
		size ++;
	}
	result = malloc((size + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result[size] = '\0';
	while (size)
	{
		size --;
		result[size] = (num % 10) + '0';
		num /= 10;
	}
	if (n < 0)
		result[0] = '-';
	return (result);
}
