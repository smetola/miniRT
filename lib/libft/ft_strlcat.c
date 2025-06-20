/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strlcat.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jllamas- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 19:33:22 by jllamas-          #+#    #+#             */
/*   Updated: 2024/02/19 19:33:25 by jllamas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	ret;
	size_t	i;

	ret = ft_strlen(src);
	i = 0;
	while (*dst != '\0')
	{
		dst ++;
		i ++;
	}
	if (i >= size)
		ret += size;
	else
		ret += i;
	while (*src != '\0' && (i + 1) < size)
	{
		*dst = *src;
		dst ++;
		src ++;
		i ++;
	}
	*dst = '\0';
	return (ret);
}
