/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strcmp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jllamas- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 18:49:20 by jllamas-          #+#    #+#             */
/*   Updated: 2024/02/17 18:49:25 by jllamas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *src, size_t n)
{
	size_t	i;
	char	*result;

	if (n < 1)
		return (NULL);
	i = ft_strlen(src);
	if (i > n)
		result = malloc((n + 1) * sizeof(char));
	else
		result = malloc((i + 1) * sizeof(char));
	i = 0;
	while (src[i] && i < n)
	{
		result[i] = src[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
