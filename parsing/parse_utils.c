/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smetola <smetola@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 10:00:00 by smetola           #+#    #+#             */
/*   Updated: 2023/09/20 10:00:00 by smetola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

int	parse_vector(char *str, t_vec3 *vec, int is_vec)
{
	char	**split;

	split = ft_split(str, ',');
	if (!split || !split[0] || !split[1] || !split[2] || split[3])
	{
		free_split(split);
		return (ft_error("Invalid vector format"));
	}
	if (!ft_atof(&vec->x, split[0]) || !ft_atof(&vec->y, split[1]) || !ft_atof(&vec->z, split[2])) {
		free_split(split);
		return (ft_error("Invalid vector format"));
	}
	free_split(split);
	if (is_vec
		&& (vec->x < -1.0 || vec->x > 1.0 || vec->y < -1.0 || vec->y > 1.0 || vec->z < -1.0 || vec->z > 1.0))
		return (ft_error("Vector component out of range [-1,1]"));
	if (is_vec && is_empty_vec(*vec))
		return (ft_error("Direction vector cannot be null"));
	return (1);
}

int	parse_color(char *str, t_color *color)
{
	char	**split;
	int		r;
	int		g;
	int		b;

	split = ft_split(str, ',');
	if (!split || !split[0] || !split[1] || !split[2] || split[3])
	{
		free_split(split);
		return (ft_error("Invalid RGB format"));
	}
	r = ft_pos_atoi(split[0]);
	g = ft_pos_atoi(split[1]);
	b = ft_pos_atoi(split[2]);
	free_split(split);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (ft_error("RGB value out of range (0-255)"));
	color->r = r;
	color->g = g;
	color->b = b;
	return (1);
}

int	ft_atof(double *ret, char *str)
{
	double	result;
	double	fraction;
	int		sign;

	result = 0.0;
	fraction = 0.1;
	sign = 1;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10.0 + (*str - '0');
		str++;
	}
	if (*str == '.')
	{
		str++;
		while (*str >= '0' && *str <= '9')
		{
			result += (*str - '0') * fraction;
			fraction /= 10.0;
			str++;
		}
	}
	if (*str)
		return (0);
	*ret = result * sign;
	return (1);
}
