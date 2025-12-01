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

t_color	compute_diffuse(const t_scene scene, t_hit hit)
{
	double	dot_nl;
	double	intensity;

	dot_nl = vec_dot(hit.surface_normal, hit.light_dir);
	if (dot_nl < 0)
		dot_nl = 0;
	intensity = scene.light.bright * dot_nl;
	return (color_scale(hit.color, intensity));
}

//todo revamp
t_color	compute_specular(const t_scene scene, t_hit hit)
{
	double	intensity;

	intensity = scene.light.bright * pow(
			vec_dot(hit.reflection_dir, hit.camera_dir), REFLECTION_INTENSITY);
	return (color_scale(hit.color, intensity));
}
