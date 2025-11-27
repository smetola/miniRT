/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntuser <ubuntuser@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 10:00:00 by smetola           #+#    #+#             */
/*   Updated: 2025/11/17 21:39:44 by ubuntuser        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* generate_ray: create ray in camera-local coordinates and rotate by camera */
t_ray	generate_ray(int x, int y, t_camera cam)
{
	t_ray	ray;
	t_vec3	dir;
	double	u;
	double	v;
	double	viewport;

	viewport = 2.0 * tan(cam.fov * M_PI / (180.0 + EPSILON) * 0.5);
	u = ((x + 0.5) / WIDTH - 0.5) * viewport * (WIDTH / (double)HEIGHT);
	v = (0.5 - (y + 0.5) / HEIGHT) * viewport;
	ray.origin = cam.coord;
	dir.x = u;
	dir.y = v;
	dir.z = FOCAL_LENGTH;
	ray.direction = dir;
	ray = vec_camera_rotate(ray, cam);
	ray.direction = vec_normalize(ray.direction);
	return (ray);
}
