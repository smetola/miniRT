/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_math.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntuser <ubuntuser@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 10:00:00 by smetola           #+#    #+#             */
/*   Updated: 2025/11/21 18:09:09 by ubuntuser        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	solve_cylinder_quadratic(t_ray ray, t_cylinder cylinder,
		double *t1, double *t2)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;

	oc = vec_sub(ray.origin, cylinder.coord);
	a = vec_dot(ray.direction, ray.direction)
		- pow(vec_dot(ray.direction, cylinder.ori), 2);
	b = 2 * (vec_dot(ray.direction, oc)
			- vec_dot(ray.direction, cylinder.ori) * vec_dot(oc, cylinder.ori));
	c = vec_dot(oc, oc) - pow(vec_dot(oc, cylinder.ori), 2)
		- cylinder.radius_squared;
	c = b * b - 4 * a * c;
	if (c < 0)
		return (0);
	*t1 = (-b - sqrt(c)) / (a * 2);
	*t2 = (-b + sqrt(c)) / (a * 2);
	if (*t1 <= EPSILON && *t2 <= EPSILON)
		return (0);
	return (1);
}

double	get_cylinder_body_distance(t_ray ray, t_cylinder cylinder, double *h1)
{
	t_vec3	oc;
	double	t1;
	double	t2;
	double	h2;

	if (!solve_cylinder_quadratic(ray, cylinder, &t1, &t2))
		return (-1);
	oc = vec_sub(ray.origin, cylinder.coord);
	*h1 = -1;
	if (t1 > EPSILON)
		*h1 = vec_dot(ray.direction, cylinder.ori) * t1
			+ vec_dot(oc, cylinder.ori);
	h2 = -1;
	if (t2 > EPSILON)
		h2 = vec_dot(ray.direction, cylinder.ori) * t2
			+ vec_dot(oc, cylinder.ori);
	if ((h2 < 0 || h2 > cylinder.hgt) && (*h1 < 0 || *h1 > cylinder.hgt))
		return (-1);
	if ((*h1 < 0 || *h1 > cylinder.hgt)
		|| (h2 >= 0 && h2 <= cylinder.hgt && (t2 < t1)))
	{
		*h1 = h2;
		return (t2);
	}
	return (t1);
}
