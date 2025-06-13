/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jllamas- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 15:36:15 by jllamas-          #+#    #+#             */
/*   Updated: 2024/02/13 15:36:18 by jllamas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_checkchar(char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if ((unsigned char) c == ((unsigned char *) set)[i])
			return (1);
		i ++;
	}
	return (0);
}

void	fill_result(const char *s1, char *result, int start, int end)
{
	int	i;

	i = 0;
	while (i <= end - start)
	{
		result[i] = s1[start + i];
		i ++;
	}
	result[i] = '\0';
}

int	get_start(int start, int end)
{
	if (end < start)
		return (1);
	return (end - start + 2);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	int		start;
	int		end;
	int		i;
	char	*result;

	start = 0;
	end = ft_strlen(s1) - 1;
	while (ft_checkchar(s1[start], set))
		start ++;
	if (end >= 0)
	{
		while (ft_checkchar(s1[end], set))
			end --;
	}
	i = get_start(start, end);
	result = malloc(i * sizeof(char));
	if (!result)
		return (NULL);
	if (i == 1)
	{
		result[0] = '\0';
		return (result);
	}
	fill_result(s1, result, start, end);
	return (result);
}
