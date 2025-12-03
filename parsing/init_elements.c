/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_elements.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smetola <smetola@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 10:00:00 by smetola           #+#    #+#             */
/*   Updated: 2023/09/20 10:00:00 by smetola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

int	init_ambient(char *line, t_scene *scene)
{
	char	**split;

	if (scene->ambient.ratio != -1.0)
		return (ft_error("Multiple ambient light definitions"));
	split = ft_split(line, ' ');
	if (!split || !split[0] || !split[1] || split[2])
		return (free_split(split), ft_error("Invalid ambient light format"));
	if (!ft_atof(&scene->ambient.ratio, split[0])
		|| scene->ambient.ratio > 1 || scene->ambient.ratio < 0)
		return (free_split(split),
			ft_error("Invalid ambient light intensity value"));
	if (!parse_color(split[1], &scene->ambient.color))
	{
		free_split(split);
		return (0);
	}
	free_split(split);
	scene->ambient.r_ratio = scene->ambient.ratio
		* (scene->ambient.color.r / 255.0);
	scene->ambient.g_ratio = scene->ambient.ratio
		* (scene->ambient.color.g / 255.0);
	scene->ambient.b_ratio = scene->ambient.ratio
		* (scene->ambient.color.b / 255.0);
	return (1);
}

int	init_camera(char *line, t_scene *scene)
{
	char	**split;
	t_vec3	forward;

	if (scene->camera.fov != -1)
		return (ft_error("Multiple camera definitions"));
	split = ft_split(line, ' ');
	if (!split || !split[0] || !split[1] || !split[2] || split[3])
		return (free_split(split), ft_error("Invalid camera format"));
	if (!parse_vector(split[0], &scene->camera.coord, 0)
		|| !parse_vector(split[1], &scene->camera.orient, 1))
		return (free_split(split), ft_error("Invalid camera format"));
	scene->camera.fov = ft_pos_atoi(split[2]);
	free_split(split);
	if (scene->camera.fov < 0 || scene->camera.fov > 180)
		return (ft_error("Invalid camera FOV value"));
	scene->camera.orient = vec_normalize(scene->camera.orient);
	forward = (t_vec3){0, 0, 1};
	scene->camera.rotation_axis = vec_normalize(
			vec_prod(scene->camera.orient, forward));
	scene->camera.rotation_angle = acos(
			vec_dot(scene->camera.orient, forward));
	return (1);
}

int	init_light(char *line, t_scene *scene)
{
	char	**split;

	if (scene->has_light)
		return (ft_error("Multiple light definitions"));
	split = ft_split(line, ' ');
	if (!split || !split[0] || !split[1] || !split[2] || split[3])
		return (free_split(split), ft_error("Invalid light format"));
	if (!parse_vector(split[0], &scene->light.coord, 0))
	{
		free_split(split);
		return (0);
	}
	if (!ft_atof(&scene->light.bright, split[1])
		|| scene->light.bright > 1 || scene->light.bright < 0)
		return (free_split(split), ft_error("Invalid light brightness value"));
	scene->has_light = 1;
	if (!parse_color(split[2], &scene->light.color))
	{
		free_split(split);
		return (0);
	}
	free_split(split);
	return (1);
}
