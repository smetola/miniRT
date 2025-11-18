/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jllamas- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 19:10:41 by jllamas-          #+#    #+#             */
/*   Updated: 2024/03/24 18:25:22 by jllamas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strlen_cond(const char *str, char c)
{
	int	size;

	if (!*str)
		return (-1);
	size = 0;
	while (str[size] != c && str[size])
		size ++;
	return (size);
}

char	*ft_strdup_cond(const char *src, char c)
{
	char			*result;
	unsigned int	i;

	result = malloc(ft_strlen_cond(src, c) + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (src[i] && src[i] != c && !(src[i] == '\n' && src[i + 1] == '\0'))
	{
		result[i] = src[i];
		i ++;
	}
	result[i] = '\0';
	return (result);
}

int	ft_count_words(const char *str, char c)
{
	int	size;
	int	toggle;
	int	i;

	size = 0;
	toggle = 1;
	i = 0;
	while (str[i])
	{
		if (str[i] != c)
		{
			if (toggle)
				size ++;
			toggle = 0;
		}
		else
			toggle = 1;
		i ++;
	}
	return (size);
}

void	*free_array(char **array)
{
	size_t	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i ++;
	}
	free(array);
	return (NULL);
}

char	**ft_split(const char *s, char c)
{
	char	**result;
	int		length;
	size_t	i;

	length = ft_count_words(s, c);
	result = malloc((length + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	length = 0;
	while (s[i] == c && s[i])
		i ++;
	while (i < ft_strlen(s))
	{
		result[length] = ft_strdup_cond(s + i, c);
		if (!result[length])
			return (free_array(result));
		i += ft_strlen_cond(s + i, c);
		while (s[i] == c && s[i])
			i ++;
		length ++;
	}
	result[length] = NULL;
	return (result);
}
