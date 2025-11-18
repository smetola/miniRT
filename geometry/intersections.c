/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntuser <ubuntuser@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 10:00:00 by smetola           #+#    #+#             */
/*   Updated: 2025/11/17 21:39:51 by ubuntuser        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static t_vec3	calculate_sphere_normal(t_ray ray, double t, t_sphere sphere)
{
	t_vec3	normal;

	normal = vec_sub(vec_add(ray.origin, vec_scale(ray.direction, t)),
			sphere.coord);
	if (vec_dot(ray.direction, normal) > 0)
		normal = vec_reverse(normal);
	return (vec_scale(normal, 2 / sphere.diam));
}

t_hit	get_sphere_hit(t_ray ray, const t_sphere sphere)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	discriminant;
	double	t_out;

	oc = vec_sub(ray.origin, sphere.coord);
	a = vec_dot(ray.direction, ray.direction);
	b = 2.0 * vec_dot(oc, ray.direction);
	discriminant = b * b - 4 * a * (vec_dot(oc, oc) - sphere.radius_squared);
	if (discriminant < 0)
		return ((t_hit){0});
	discriminant = sqrt(discriminant);
	t_out = (-b - discriminant) / (2.0 * a);
	if (t_out <= 0.0)
	{
		t_out = (-b + discriminant) / (2.0 * a);
		if (t_out <= 0.0)
			return ((t_hit){0});
	}
	return ((t_hit){1, t_out, sphere.color,
		calculate_sphere_normal(ray, t_out, sphere),
		(t_vec3){0}, (t_vec3){0}, (t_vec3){0}});
}

t_hit	get_plane_hit(t_ray ray, const t_plane plane)
{
	double	dividend;
	double	divisor;
	double	d;
	t_vec3	normal;

	if (vec_dot(plane.ori, ray.direction) == 0)
		return ((t_hit){0});
	dividend = vec_dot(vec_sub(plane.coord, ray.origin), plane.ori);
	divisor = vec_dot(ray.direction, plane.ori);
	if ((dividend > 0) != (divisor > 0))
		return ((t_hit){0});
	d = dividend / divisor;
	normal = plane.ori;
	if (vec_dot(ray.direction, plane.ori) > 0)
		normal = vec_reverse(normal);
	return ((t_hit){1, d, plane.color, normal, (t_vec3){0}, (t_vec3){0},
		(t_vec3){0}});
}

t_hit	get_cylinder_body_hit(t_ray ray, const t_cylinder cylinder)
{
	double	height;
	t_vec3	u;
	t_hit	hit;

	if (!vec_dot(ray.direction, cylinder.ori))
		return ((t_hit){0});
	hit.is_hit = 0;
	hit.color = cylinder.color;
	hit.distance = get_cylinder_body_distance(ray, cylinder, &height);
	if (hit.distance < 0)
		return (hit);
	hit.is_hit = 1;
	u = ray_distance(ray.origin, ray.direction, hit.distance);
	hit.surface_normal = vec_sub(u, vec_sub(cylinder.coord,
				vec_scale(cylinder.ori, height)));
	if (vec_dot(ray.direction, vec_sub(u,
				ray_distance(cylinder.coord, cylinder.ori, height))) > 0)
		hit.surface_normal = vec_reverse(hit.surface_normal);
	hit.surface_normal = vec_scale(hit.surface_normal, 2 / cylinder.diam);
	return (hit);
}

t_hit	get_cylinder_hit(t_ray ray, const t_cylinder cylinder)
{
	t_hit	result;
	t_hit	temp_hit;

	result.is_hit = 0;
	result.distance = INFINITY;
	temp_hit = get_cylinder_body_hit(ray, cylinder);
	if (temp_hit.is_hit && temp_hit.distance < result.distance)
		result = temp_hit;
	temp_hit = get_plane_hit(ray, (t_plane){cylinder.coord,
			vec_reverse(cylinder.ori), cylinder.color});
	if (temp_hit.is_hit && temp_hit.distance < result.distance
		&& dot_distance(cylinder.coord, ray_distance(ray.origin,
				ray.direction, temp_hit.distance)) < cylinder.diam * 0.5)
		result = temp_hit;
	temp_hit = get_plane_hit(ray, (t_plane){cylinder.coord2, cylinder.ori,
			cylinder.color});
	if (temp_hit.is_hit && temp_hit.distance < result.distance
		&& dot_distance(ray_distance(cylinder.coord, cylinder.ori,
				cylinder.hgt), ray_distance(ray.origin, ray.direction,
				temp_hit.distance)) < cylinder.diam * 0.5)
		result = temp_hit;
	return (result);
}
