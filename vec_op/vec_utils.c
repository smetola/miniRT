/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntuser <ubuntuser@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 10:00:00 by smetola           #+#    #+#             */
/*   Updated: 2025/11/17 21:39:11 by ubuntuser        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	is_empty_vec(t_vec3 v)
{
	return (v.x == 0 && v.y == 0 && v.z == 0);
}

t_vec3	ray_distance(t_vec3 start, t_vec3 direction, double distance)
{
	return (vec_add(start, vec_scale(direction, distance)));
}

t_vec3	get_hit_point(t_ray ray, double t)
{
	t_vec3	hit;

	hit.x = ray.origin.x + ray.direction.x * t;
	hit.y = ray.origin.y + ray.direction.y * t;
	hit.z = ray.origin.z + ray.direction.z * t;
	return (hit);
}

t_vec3	rotate_rodrigues(t_vec3 target, t_vec3 axis, double angle)
{
	t_vec3	term1;
	t_vec3	term2;
	t_vec3	term3;

	term1 = vec_scale(target, cos(angle));
	term2 = vec_scale(vec_prod(axis, target), sin(angle));
	term3 = vec_scale(axis, vec_dot(axis, target) * (1 - cos(angle)));
	return (vec_add(term1, vec_add(term2, term3)));
}

t_ray	vec_camera_rotate(t_ray target, t_camera camera)
{
	t_vec3	offset;

	offset = vec_sub(target.origin, camera.coord);
	target.origin = rotate_rodrigues(offset,
			camera.rotation_axis, camera.rotation_angle);
	target.direction = rotate_rodrigues(target.direction,
			camera.rotation_axis, camera.rotation_angle);
	return (target);
}
