/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_params.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jllamas- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 22:17:31 by jllamas-          #+#    #+#             */
/*   Updated: 2024/02/24 22:18:17 by jllamas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*result;

	if (!s)
		return (NULL);
	if (ft_strlen(s) < start)
		len = 0;
	if (len)
	{
		if (ft_strlen(s + start) < len)
			len = ft_strlen(s + start);
	}
	result = malloc((len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	if (!len)
		result[0] = '\0';
	else
		ft_strlcpy(result, s + start, len + 1);
	return (result);
}
