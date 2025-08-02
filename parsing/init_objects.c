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

	split = ft_split(line, ' ');
	if (!split || !split[0] || !split[1] || !split[2])
		return (0);
	new_spheres = realloc_array(scene->spheres, scene->num_spheres,
			scene->num_spheres + 1, sizeof(t_sphere));
	if (!new_spheres)
		return (0);
	new_spheres[scene->num_spheres].diam = ft_atof(split[1]);
	if (!parse_vector(split[0], &new_spheres[scene->num_spheres].shape.ori, 0)
		|| !parse_color(split[2], &new_spheres[scene->num_spheres].shape.color))
		return (0);
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
		return (ft_error("Invalid plane format"));
	new_planes = realloc_array(scene->planes, scene->num_planes,
			scene->num_planes + 1, sizeof(t_plane));
	if (!new_planes)
		return (0);
	pl = &new_planes[scene->num_planes];
	if (!parse_vector(split[0], &pl->shape.ori, 0)
		|| !parse_vector(split[1], &pl->normal, 1)
		|| !parse_color(split[2], &pl->shape.color))
		return (0);
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
		return (ft_error("Invalid cylinder format"));
	new_cylinders = realloc_array(scene->cylinders, scene->num_cylinders,
			scene->num_cylinders + 1, sizeof(t_cylinder));
	if (!new_cylinders)
		return (0);
	cy = &new_cylinders[scene->num_cylinders];
	if (!parse_vector(split[0], &cy->shape.ori, 0)
		|| !parse_vector(split[1], &cy->axis, 1))
		return (0);
	cy->diam = ft_atof(split[2]);
	cy->hgt = ft_atof(split[3]);
	if (!parse_color(split[4], &cy->shape.color))
		return (0);
	scene->cylinders = new_cylinders;
	scene->num_cylinders++;
	free_split(split);
	return (1);
}
