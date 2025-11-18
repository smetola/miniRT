/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_objects.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smetola <smetola@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 10:00:00 by smetola           #+#    #+#             */
/*   Updated: 2023/09/20 10:00:00 by smetola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	*realloc_array(
	void *old, size_t old_count, size_t new_count, size_t elem_size)
{
	void	*new_array;

	new_array = malloc(elem_size * new_count);
	if (!new_array)
		return (NULL);
	if (old)
	{
		ft_memcpy(new_array, old, elem_size * old_count);
		free(old);
	}
	return (new_array);
}

int	init_sphere(char *line, t_scene *scene)
{
	char		**split;
	t_sphere	*new_spheres;
	t_sphere	*sp;

	split = ft_split(line, ' ');
	if (!split || !split[0] || !split[1] || !split[2])
		return (free_split(split), 0);
	new_spheres = realloc_array(scene->spheres, scene->num_spheres,
			scene->num_spheres + 1, sizeof(t_sphere));
	if (!new_spheres)
		return (free_split(split), 0);
	sp = &new_spheres[scene->num_spheres];
	sp->diam = ft_atof(split[1]);
	if (!parse_vector(split[0], &sp->coord, 0)
		|| !parse_color(split[2], &sp->color))
		return (free(new_spheres), free_split(split), 0);
	sp->radius_squared = sp->diam * sp->diam * 0.25;
	scene->spheres = new_spheres;
	scene->num_spheres++;
	free_split(split);
	return (1);
}

int	init_plane(char *line, t_scene *scene)
{
	char	**split;
	t_plane	*new_planes;
	t_plane	*pl;

	split = ft_split(line, ' ');
	if (!split || !split[0] || !split[1] || !split[2])
		return (free_split(split), ft_error("Invalid plane format"));
	new_planes = realloc_array(scene->planes, scene->num_planes,
			scene->num_planes + 1, sizeof(t_plane));
	if (!new_planes)
		return (free_split(split), 0);
	pl = &new_planes[scene->num_planes];
	if (!parse_vector(split[0], &pl->coord, 0)
		|| !parse_vector(split[1], &pl->ori, 1)
		|| !parse_color(split[2], &pl->color))
		return (free(new_planes), free_split(split), 0);
	pl->ori = vec_normalize(pl->ori);
	scene->planes = new_planes;
	scene->num_planes++;
	free_split(split);
	return (1);
}

int	init_cylinder(char *line, t_scene *scene)
{
	char		**split;
	t_cylinder	*new_cylinders;
	t_cylinder	*cy;

	split = ft_split(line, ' ');
	if (!split || !split[0] || !split[1] || !split[2] || !split[3] || !split[4])
		return (free_split(split), ft_error("Invalid cylinder format"));
	new_cylinders = realloc_array(scene->cylinders, scene->num_cylinders,
			scene->num_cylinders + 1, sizeof(t_cylinder));
	if (!new_cylinders)
		return (free_split(split), 0);
	cy = &new_cylinders[scene->num_cylinders];
	if (!parse_vector(split[0], &cy->coord, 0)
		|| !parse_vector(split[1], &cy->ori, 1))
		return (free(new_cylinders), free_split(split), 0);
	cy->diam = ft_atof(split[2]);
	cy->hgt = ft_atof(split[3]);
	cy->ori = vec_normalize(cy->ori);
	if (!parse_color(split[4], &cy->color))
		return (free(new_cylinders), free_split(split), 0);
	cy->coord2 = ray_distance(cy->coord, cy->ori, cy->hgt);
	cy->radius_squared = cy->diam * cy->diam * 0.25;
	return (scene->cylinders = new_cylinders, scene->num_cylinders++,
		free_split(split), 1);
}
