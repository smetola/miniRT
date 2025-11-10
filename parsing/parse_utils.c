#include "../miniRT.h"

void	init_scene(t_scene *scene)
{
	scene->ambient.ratio = -1.0;
	scene->ambient.color = (t_color){0, 0, 0};
	scene->camera.coord = (t_vec3){0, 0, 0};
	scene->camera.orient = (t_vec3){0, 0, 0};
	scene->camera.fov = -1;
	scene->light.coord = (t_vec3){0, 0, 0};
	scene->light.bright = 0.0;
	scene->has_light = 0;
	scene->spheres = NULL;
	scene->num_spheres = 0;
	scene->planes = NULL;
	scene->num_planes = 0;
	scene->cylinders = NULL;
	scene->num_cylinders = 0;
}

int	parse_vector(char *str, t_vec3 *vec, int is_vec)
{
	char	**split;
	float	x;
	float	y;
	float	z;

	split = ft_split(str, ',');
	if (!split || !split[0] || !split[1] || !split[2] || split[3])
	{
		free_split(split);
		return (ft_error("Invalid vector format"));
	}
	x = ft_atof(split[0]);
	y = ft_atof(split[1]);
	z = ft_atof(split[2]);
	free_split(split);
	if (is_vec
		&& (x < -1.0 || x > 1.0 || y < -1.0 || y > 1.0 || z < -1.0 || z > 1.0))
		return (ft_error("Vector component out of range [-1,1]")); //todo error case for null direction vector?
	vec->x = x;
	vec->y = y;
	vec->z = z;
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
	r = ft_atoi(split[0]);
	g = ft_atoi(split[1]);
	b = ft_atoi(split[2]);
	free_split(split);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (ft_error("RGB value out of range (0-255)"));
	color->r = r;
	color->g = g;
	color->b = b;
	return (1);
}

double	ft_atof(char *str)
{
	double	result;
	double	fraction;
	int		sign;

	result = 0.0;
	fraction = 0.1;
	sign = 1;
	if (*str == '-' && ++str)
		sign = -1;
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
	return (result * sign);
}
