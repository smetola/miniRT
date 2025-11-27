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

t_vec3	reflect_vector(t_vec3 target, t_vec3 normal)
{
	double	dot_prod;

	dot_prod = vec_dot(target, normal);
	return (vec_normalize(vec_sub(target, vec_scale(normal, 2 * dot_prod))));
}

int32_t	shade_hit(const t_scene scene, t_hit hit, t_ray ray)
{
	t_color	amb;
	t_color	diff;
	t_color	accum;
	t_color	spec;

	amb = compute_ambient(scene.ambient, hit.color);
	diff = compute_diffuse(scene, hit.color, hit.surface_normal,
			get_hit_point(ray, hit.distance));
	spec = compute_specular(scene, hit.color, hit.surface_normal,
			get_hit_point(ray, hit.distance));
	accum = color_add(color_add(amb, diff), spec);
	return (ft_pixel(accum.r, accum.g, accum.b, 255));
}
