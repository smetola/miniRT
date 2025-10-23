#include "../miniRT.h"

int	init_ambient(char *line, t_scene *scene)
{
	char	**split;

	if (scene->ambient.ratio != -1.0)
		return (ft_error("Multiple ambient light definitions"));
	split = ft_split(line, ' ');
	if (!split || !split[0] || !split[1])
		return (ft_error("Invalid ambient light format"));
	scene->ambient.ratio = ft_atof(split[0]);
	if (!parse_color(split[1], &scene->ambient.color))
	{
		free_split(split);
		return (0);
	}
	free_split(split);
	return (1);
}

int	init_camera(char *line, t_scene *scene)
{
	char	**split;

	if (scene->camera.fov != -1)
		return (ft_error("Multiple camera definitions"));
	split = ft_split(line, ' ');
	if (!split || !split[0] || !split[1] || !split[2])
		return (ft_error("Invalid camera format"));
	if (!parse_vector(split[0], &scene->camera.coord, 0)
		|| !parse_vector(split[1], &scene->camera.orient, 1))
	{
		free_split(split);
		return (0);
	}
	scene->camera.fov = ft_atoi(split[2]);
	free_split(split);
	/*calculate rotation axis and angle in advance*/
	t_vec3	forward = {0, 0, 1};
	scene->camera.rotation_axis = vec_normalize(vec_prod(scene->camera.orient, forward)); //axis of rotation is perpendicular to both camera orientation and forward axis
	scene->camera.rotation_angle = acos(vec_dot(scene->camera.orient, forward));
	return (1);
}

int	init_light(char *line, t_scene *scene)
{
	char	**split;

	if (scene->has_light)
		return (ft_error("Multiple light definitions"));
	split = ft_split(line, ' ');
	if (!split || !split[0] || !split[1])
		return (ft_error("Invalid light format"));
	if (!parse_vector(split[0], &scene->light.coord, 0))
	{
		free_split(split);
		return (0);
	}
	scene->light.bright = ft_atof(split[1]);
	scene->has_light = 1;
	free_split(split);
	return (1);
}
