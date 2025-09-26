#include "miniRT.h"

t_vec3	get_hit_point(t_ray ray, double t)
{
	t_vec3 hit;

	hit.x = ray.origin.x + ray.direction.x * t;
	hit.y = ray.origin.y + ray.direction.y * t;
	hit.z = ray.origin.z + ray.direction.z * t;

	return (hit);
}

t_vec3 get_normal_at_sphere(t_vec3 point, t_sphere *sphere)
{
	t_vec3 normal;

	normal = vec_sub(point, sphere->coord);
	return (vec_normalize(normal));
}

t_color compute_ambient(t_amb_light *amb, t_color obj_color)
{
	t_color	result;

	result.r = (int)(obj_color.r * amb->ratio * (amb->color.r / 255.0));
	result.g = (int)(obj_color.g * amb->ratio * (amb->color.g / 255.0));
	result.b = (int)(obj_color.b * amb->ratio * (amb->color.b / 255.0));
	// Clamp a [0,255]
	if (result.r > 255) result.r = 255;
	if (result.g > 255) result.g = 255;
	if (result.b > 255) result.b = 255;
	return (result);
}

static int is_in_shadow(t_scene *scene, t_vec3 origin, t_vec3 dir)
{
	t_ray	r;
	int		i;
	double	t;
	double	light_dist;

	light_dist = vec_length(vec_sub(scene->light.coord, origin));
	r.origin.x = origin.x + dir.x * 0.0001;
	r.origin.y = origin.y + dir.y * 0.0001;
	r.origin.z = origin.z + dir.z * 0.0001;
	r.direction = dir;
	i = 0;
	while (i < scene->num_spheres)
	{
		if (hit_sphere(r, &scene->spheres[i], &t))
		{
			if (t > 0.0001 && t < light_dist)
				return (1);
		}
		i++;
	}
	return (0);
}


t_color compute_diffuse(t_scene *scene, t_color obj_color,
						t_vec3 normal, t_vec3 hit_point)
{
	t_vec3	l_dir;
	double	dot_nl;
	double	intensity;
	t_color	result;

	// dirección normalizada a la luz
	l_dir = vec_normalize(vec_sub(scene->light.coord, hit_point));
	// ángulo
	dot_nl = vec_dot(normal, l_dir);
	if (dot_nl < 0)
		dot_nl = 0;
	if (is_in_shadow(scene, hit_point, l_dir))
	{
		result.r = 0;
		result.g = 0;
		result.b = 0;
		return (result);
	}
	// intensida brillo de la luz * dot_nl
	intensity = scene->light.bright * dot_nl;
	result.r = (int)(obj_color.r * intensity);
	result.g = (int)(obj_color.g * intensity);
	result.b = (int)(obj_color.b * intensity);
	if (result.r > 255) result.r = 255;
	if (result.g > 255) result.g = 255;
	if (result.b > 255) result.b = 255;
	return (result);
}
