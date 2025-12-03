/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntuser <ubuntuser@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 10:00:00 by smetola           #+#    #+#             */
/*   Updated: 2025/11/17 21:39:07 by ubuntuser        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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

t_vec3	reflect_vector(t_vec3 target, t_vec3 normal)
{
	double	dot_prod;

	dot_prod = vec_dot(target, normal);
	return (vec_normalize(vec_sub(vec_scale(normal, 2 * dot_prod), target)));
}

int32_t	shade_hit(const t_scene scene, t_hit hit, t_ray ray)
{
	t_color	amb;
	t_color	diff;
	t_color	spec;
	//t_vec3	hitp; //todo add to hit struct? make a second struct with this and the other 3 properties? also add them only after checking hit point is not in shadow

	amb = compute_ambient(scene.ambient, hit.color);
	if (!is_in_shadow(scene, get_hit_point(ray, hit.distance), hit.light_dir))
	{
		if (BONUS)
		{
			diff = compute_diffuse(scene, hit);
			spec = compute_specular(scene, hit);
			amb = color_add(amb, color_add(diff, spec));
		}
		else
			amb = color_add(amb, compute_diffuse(scene, hit));
	}
	return (ft_pixel(amb.r, amb.g, amb.b, 255));
}
