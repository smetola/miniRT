#include "miniRT.h"

t_vec3	get_hit_point(t_ray ray, double t)
{
	t_vec3	hit;

	hit.x = ray.origin.x + ray.direction.x * t;
	hit.y = ray.origin.y + ray.direction.y * t;
	hit.z = ray.origin.z + ray.direction.z * t;
	return (hit);
}

t_vec3	get_normal_at_sphere(t_vec3 point, t_sphere *sphere)
{
	t_vec3	normal;

	normal = vec_sub(point, sphere->coord);
	return (vec_normalize(normal));
}

t_color	compute_ambient(const t_amb_light *amb, t_color obj_color)
{
	t_color	result;

	result.r = (int)(obj_color.r * amb->ratio * (amb->color.r / 255.0));
	result.g = (int)(obj_color.g * amb->ratio * (amb->color.g / 255.0));
	result.b = (int)(obj_color.b * amb->ratio * (amb->color.b / 255.0));
	if (result.r > 255)
		result.r = 255;
	if (result.g > 255)
		result.g = 255;
	if (result.b > 255)
		result.b = 255;
	return (result);
}

static int	is_in_shadow(const t_scene *scene, t_vec3 origin, t_vec3 dir)
{
	t_ray	r;
	int		i;
	t_hit	temp;
	double	light_dist;

	light_dist = vec_length(vec_sub(scene->light.coord, origin));
	r.origin.x = origin.x + dir.x * 0.0001;
	r.origin.y = origin.y + dir.y * 0.0001;
	r.origin.z = origin.z + dir.z * 0.0001;
	r.direction = dir;
	i = 0;
	while (i < scene->num_spheres)
	{
		temp = get_sphere_hit(r, scene->spheres[i]);
		if (temp.is_hit)
		{
			if (temp.distance > 0 && temp.distance < light_dist)
				return (1);
		}
		i++;
	}
	i = 0;
	while (i < scene->num_planes)
	{
		temp = get_plane_hit(r, scene->planes[i]);
		if (temp.is_hit)
		{
			if (temp.distance > 0 && temp.distance < light_dist)
				return (1);
		}
		i++;
	}
	i = 0;
	while (i < scene->num_cylinders)
	{
		temp = get_cylinder_hit(r, scene->cylinders[i]);
		if (temp.is_hit)
		{
			if (temp.distance > 0 && temp.distance < light_dist)
				return (1);
		}
		i++;
	}
	return (0);
}

t_color	compute_diffuse(const t_scene *scene, t_color obj_color,
						t_vec3 normal, t_vec3 hit_point)
{
	t_vec3	l_dir;
	double	dot_nl;
	double	intensity;
	t_color	result;

	l_dir = vec_normalize(vec_sub(scene->light.coord, hit_point));
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
	intensity = scene->light.bright * dot_nl;
	result.r = (int)(obj_color.r * intensity);
	result.g = (int)(obj_color.g * intensity);
	result.b = (int)(obj_color.b * intensity);
	if (result.r > 255)
		result.r = 255;
	if (result.g > 255)
		result.g = 255;
	if (result.b > 255)
		result.b = 255;
	return (result);
}

t_color	compute_specular(const t_scene *scene, t_color obj_color, t_vec3 normal, t_vec3 hit_point)
{
	t_vec3	l_dir;
	t_vec3	c_dir;
	double	intensity;
	t_color	result;

	if (is_in_shadow(scene, hit_point, vec_normalize(vec_sub(scene->light.coord, hit_point))))
	{
		result.r = 0;
		result.g = 0;
		result.b = 0;
		return (result);
	}
	l_dir = vec_normalize(vec_sub(hit_point, scene->light.coord));
	l_dir = reflect_vector(l_dir, normal);
	c_dir = vec_normalize(vec_sub(hit_point, scene->camera.coord));
	intensity = scene->light.bright * pow(vec_dot(l_dir, c_dir), REFLECTION_INTENSITY);
	result.r = (int)(obj_color.r * intensity);
	result.g = (int)(obj_color.g * intensity);
	result.b = (int)(obj_color.b * intensity);
	if (result.r > 255) //todo limit to 0 too
		result.r = 255;
	if (result.g > 255)
		result.g = 255;
	if (result.b > 255)
		result.b = 255;
	return (result);
}
