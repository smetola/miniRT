#include "../miniRT.h"

int	init_ambient(char *line, t_scene *scene)
{
	char	**split;

	if (scene->ambient.ratio != -1.0)
		return (ft_error("Multiple ambient light definitions"));
	split = ft_split(line, ' ');
	if (!split || !split[0] || !split[1])
		return (ft_error("Invalid ambient light format"));//todo free_split?
	if (!ft_atof(&scene->ambient.ratio, split[0]) || scene->ambient.ratio > 1 || scene->ambient.ratio < 0)
		return (ft_error("Invalid ambient light intensity value")); //todo free_split?
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
	scene->camera.fov = ft_pos_atoi(split[2]); //modified atoi for positive numbers only, returns -1 on error instead of 0
	free_split(split);
	if (scene->camera.fov < 0 || scene->camera.fov > 180)
		return (ft_error("Invalid camera FOV value"));
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
	if (!split || !split[0] || !split[1] || !split[2])
		return (ft_error("Invalid light format"));
	if (!parse_vector(split[0], &scene->light.coord, 0))
	{
		free_split(split);
		return (0);
	}
	if (!ft_atof(&scene->light.bright, split[1]) || scene->light.bright > 1 || scene->light.bright < 0)
		return (ft_error("Invalid light brightness value"));
	scene->has_light = 1;
	if (!parse_color(split[2], &scene->light.color)) //unused, though the light color is still part of the file definition and probably needs to be included
	{
		free_split(split);
		return (0);
	}
	free_split(split);
	return (1);
}
