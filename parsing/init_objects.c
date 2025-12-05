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
	if (!split || !split[0] || !split[1] || !split[2] || split[3])
		return (free_split(split), ft_error("Invalid sphere format"));
	new_spheres = realloc_array(scene->spheres, scene->num_spheres,
			scene->num_spheres + 1, sizeof(t_sphere));
	if (!new_spheres)
		return (free_split(split), 0);
	scene->spheres = new_spheres;
	sp = &new_spheres[scene->num_spheres];
	if (!ft_atof(&sp->diam, split[1]) || sp->diam <= 0
		|| !parse_vector(split[0], &sp->coord, 0)
		|| !parse_color(split[2], &sp->color))
		return (free_split(split), ft_error("Invalid sphere declaration"));
	sp->radius_squared = sp->diam * sp->diam * 0.25;
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
	if (!split || !split[0] || !split[1] || !split[2] || split[3])
		return (free_split(split), ft_error("Invalid plane format"));
	new_planes = realloc_array(scene->planes, scene->num_planes,
			scene->num_planes + 1, sizeof(t_plane));
	if (!new_planes)
		return (free_split(split), 0);
	scene->planes = new_planes;
	pl = &new_planes[scene->num_planes];
	if (!parse_vector(split[0], &pl->coord, 0)
		|| !parse_vector(split[1], &pl->ori, 1)
		|| !parse_color(split[2], &pl->color))
		return (free_split(split),
			ft_error("Invalid plane declaration"));
	pl->ori = vec_normalize(pl->ori);
	scene->num_planes++;
	free_split(split);
	return (1);
}

static void	init_cylinder_aux(t_cylinder *cy)
{
	cy->ori = vec_normalize(cy->ori);
	cy->coord2 = ray_distance(cy->coord, cy->ori, cy->hgt);
	cy->radius_squared = cy->diam * cy->diam * 0.25;
}

int	init_cylinder(char *line, t_scene *scene)
{
	char		**split;
	t_cylinder	*new_cylinders;
	t_cylinder	*cy;

	split = ft_split(line, ' ');
	if (!split || !split[0] || !split[1] || !split[2]
		|| !split[3] || !split[4] || split[5])
		return (free_split(split), ft_error("Invalid cylinder format"));
	new_cylinders = realloc_array(scene->cylinders, scene->num_cylinders,
			scene->num_cylinders + 1, sizeof(t_cylinder));
	if (!new_cylinders)
		return (free_split(split), 0);
	scene->cylinders = new_cylinders;
	cy = &new_cylinders[scene->num_cylinders];
	if (!parse_vector(split[0], &cy->coord, 0)
		|| !parse_vector(split[1], &cy->ori, 1)
		|| !ft_atof(&cy->diam, split[2]) || cy->diam <= 0
		|| !ft_atof(&cy->hgt, split[3]) || cy->hgt <= 0
		|| !parse_color(split[4], &cy->color))
		return (free_split(split), ft_error("Invalid cylinder declaration"));
	init_cylinder_aux(cy);
	return (scene->num_cylinders++,
		free_split(split), 1);
}
