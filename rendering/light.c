/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntuser <ubuntuser@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 10:00:00 by smetola           #+#    #+#             */
/*   Updated: 2025/11/17 21:39:42 by ubuntuser        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_color	compute_ambient(const t_amb_light amb, t_color obj_color)
{
	t_color	result;

	result.r = (int)(obj_color.r * amb.ratio * (amb.color.r / 255.0));
	result.g = (int)(obj_color.g * amb.ratio * (amb.color.g / 255.0));
	result.b = (int)(obj_color.b * amb.ratio * (amb.color.b / 255.0));
	if (result.r > 255)
		result.r = 255;
	if (result.g > 255)
		result.g = 255;
	if (result.b > 255)
		result.b = 255;
	return (result);
}

static int	obj_shadows(t_ray r, const t_scene scene, double light_dist)
{
	t_hit	temp;
	size_t	i;

	i = 0;
	while (i < scene.num_spheres)
	{
		temp = get_sphere_hit(r, scene.spheres[i++]);
		if (temp.is_hit && temp.distance > 0 && temp.distance < light_dist)
			return (1);
	}
	i = 0;
	while (i < scene.num_planes)
	{
		temp = get_plane_hit(r, scene.planes[i++]);
		if (temp.is_hit && temp.distance > 0 && temp.distance < light_dist)
			return (1);
	}
	i = 0;
	while (i < scene.num_cylinders)
	{
		temp = get_cylinder_hit(r, scene.cylinders[i++]);
		if (temp.is_hit && temp.distance > 0 && temp.distance < light_dist)
			return (1);
	}
	return (0);
}

static int	is_in_shadow(const t_scene scene, t_vec3 origin, t_vec3 dir)
{
	t_ray	r;
	t_ray	temp;
	double	light_dist;

	light_dist = vec_length(vec_sub(scene.light.coord, origin));
	temp.origin = origin;
	temp.direction = dir;
	r.origin = get_hit_point(temp, EPSILON);
	r.direction = dir;
	return (obj_shadows(r, scene, light_dist));
}

t_color	compute_diffuse(const t_scene scene, t_color obj_color,
						t_vec3 normal, t_vec3 hit_point)
{
	t_vec3	l_dir;
	double	dot_nl;
	double	intensity;

	l_dir = vec_normalize(vec_sub(scene.light.coord, hit_point));
	dot_nl = vec_dot(normal, l_dir);
	if (dot_nl < 0)
		dot_nl = 0;
	if (is_in_shadow(scene, hit_point, l_dir))
		return ((t_color){0});
	intensity = scene.light.bright * dot_nl;
	return (color_scale(obj_color, intensity));
}

t_color	compute_specular(const t_scene scene, t_color obj_color,
					t_vec3 normal, t_vec3 hit_point)
{
	t_vec3	l_dir;
	t_vec3	c_dir;
	double	intensity;

	l_dir = vec_normalize(vec_sub(scene.light.coord, hit_point));
	if (is_in_shadow(scene, hit_point, l_dir))
		return ((t_color){0});
	l_dir = reflect_vector(l_dir, normal);
	c_dir = vec_normalize(vec_sub(hit_point, scene.camera.coord));
	intensity = scene.light.bright * pow(vec_dot(l_dir, c_dir),
			REFLECTION_INTENSITY);
	return (color_scale(obj_color, intensity));
}
