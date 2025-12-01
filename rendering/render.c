/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntuser <ubuntuser@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 10:00:00 by smetola           #+#    #+#             */
/*   Updated: 2025/11/17 21:39:26 by ubuntuser        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

static void	update_closest_hit(t_hit *result, t_hit temp, double *min_dist)
{
	if (temp.is_hit && temp.distance > 0 && temp.distance < *min_dist)
	{
		*min_dist = temp.distance;
		*result = temp;
	}
}

static void	check_all_objects(t_ray r, const t_scene scene,
				t_hit *result, double *min_dist)
{
	t_hit	temp;
	size_t	i;

	i = 0;
	while (i < scene.num_spheres)
	{
		temp = get_sphere_hit(r, scene.spheres[i]);
		update_closest_hit(result, temp, min_dist);
		i++;
	}
	i = 0;
	while (i < scene.num_planes)
	{
		temp = get_plane_hit(r, scene.planes[i]);
		update_closest_hit(result, temp, min_dist);
		i++;
	}
	i = 0;
	while (i < scene.num_cylinders)
	{
		temp = get_cylinder_hit(r, scene.cylinders[i]);
		update_closest_hit(result, temp, min_dist);
		i++;
	}
}

t_hit	get_ray_hit(const t_ray r, const t_scene scene)
{
	t_hit	result;
	double	min_dist;
	t_vec3	hitp;

	result.is_hit = 0;
	min_dist = INFINITY;
	check_all_objects(r, scene, &result, &min_dist);
	if (result.is_hit)
	{
		hitp = get_hit_point(r, result.distance); //todo add to hit struct?
		result.camera_dir = vec_normalize(vec_sub(scene.camera.coord, hitp)); //todo full_hit struct? only calculate after confirming its not in shadow
		result.light_dir = vec_normalize(vec_sub(scene.light.coord, hitp));
		result.reflection_dir = reflect_vector(vec_reverse(result.light_dir),
				result.surface_normal);
	}
	return (result);
}

void	render_scene(const t_scene scene, mlx_image_t *g_image)
{
	int			x;
	int			y;
	t_hit		hit;
	t_ray		r;
	uint32_t	color;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			r = generate_ray(x, y, scene.camera);
			hit = get_ray_hit(r, scene);
			if (!hit.is_hit)
				color = ft_pixel(0, 0, 0, 255);
			else
				color = shade_hit(scene, hit, r);
			mlx_put_pixel(g_image, x, y, color);
			x++;
		}
		y++;
	}
}
