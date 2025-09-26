/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntuser <ubuntuser@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 11:23:23 by sanmetol          #+#    #+#             */
/*   Updated: 2025/06/13 11:29:26 by ubuntuser        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	gnl_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

size_t	gnl_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	dst_l;
	size_t	src_l;

	dst_l = gnl_strlen(dst);
	src_l = gnl_strlen(src);
	if (dstsize == 0)
		return (src_l);
	if (dst_l >= dstsize)
		return (dstsize + src_l);
	i = 0;
	while (src[i] && i < dstsize - dst_l - 1)
	{
		dst[i + dst_l] = src[i];
		i++;
	}
	dst[i + dst_l] = '\0';
	return (dst_l + src_l);
}

size_t	gnl_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	while (i < dstsize - 1 && src[i] && dstsize != 0)
	{
		dst[i] = src[i];
		i++;
	}
	if (dstsize != 0)
		dst[i] = '\0';
	return (gnl_strlen(src));
}

char	*gnl_strjoin(char *s1, char *s2)
{
	char	*s3;
	size_t	s3_len;

	if (!s1)
	{
		s1 = (char *)malloc(1 * sizeof(char));
		s1[0] = '\0';
	}
	if (!s1 || !s2)
		return (NULL);
	s3_len = gnl_strlen (s1) + gnl_strlen (s2);
	s3 = malloc(s3_len + 1);
	if (!s3)
		return (NULL);
	gnl_strlcpy(s3, s1, s3_len + 1);
	gnl_strlcat(s3, s2, s3_len + 1);
	free(s1);
	return (s3);
}

char	*gnl_strchr(char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == c)
			return ((char *)(s));
		s++;
	}
	if (*s == c)
		return ((char *)(s));
	return (NULL);
}
