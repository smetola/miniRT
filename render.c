#include "miniRT.h"
#include <float.h> /* DBL_MAX */

int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

/* get_ray_hit: cast ray for (x,y) and return t_hit populated (spheres, planes, cylinders) */
t_hit	get_ray_hit(const t_ray r, const t_scene scene)
{
	t_hit	result;
	t_hit	temp;
	double	min_dist;
	int		i;
	t_vec3	hitp;

	result.is_hit = 0;
	min_dist = INFINITY;
	/* spheres */
	i = 0;
	while (i < scene.num_spheres)
	{
		temp = get_sphere_hit(r, scene.spheres[i]);
		if (temp.is_hit)
		{
			if (temp.distance > 0 && temp.distance < min_dist)
			{
				min_dist = temp.distance;
				result = temp;
				result.is_hit = 1;
			}
		}
		i++;
	}
	/*planes*/
	i = 0;
	while (i < scene.num_planes)
	{
		temp = get_plane_hit(r, scene.planes[i]);
		if (temp.is_hit)
		{
			if (temp.distance > 0 && temp.distance < min_dist) //&& dist = 0?
			{
				min_dist = temp.distance;
				result = temp;
				result.is_hit = 1;
			}
		}
		i++;
	}
	/*cylinders*/
	i = 0;
	while (i < scene.num_cylinders)
	{
		temp = get_cylinder_hit(r, scene.cylinders[i]);
		if (temp.is_hit)
		{
			if (temp.distance > 0 && temp.distance < min_dist) //&& dist = 0?
			{
				min_dist = temp.distance;
				result = temp;
				result.is_hit = 1;
			}
		}
		i++;
	}
	/* fill camera_dir, light_dir, reflection_dir if hit */
	if (result.is_hit)
	{
		hitp = get_hit_point(r, result.distance);
		result.camera_dir = vec_normalize(vec_sub(scene.camera.coord, hitp)); //remove these extra properties from hit? redo structs in .h
		result.light_dir = vec_normalize(vec_sub(scene.light.coord, hitp));
		result.reflection_dir = reflect_vector(vec_reverse(result.light_dir), result.surface_normal);
	}
	return (result);
}

static int32_t	shade_hit(const t_scene scene, t_hit hit, t_ray ray)
{
	t_color	amb;
	t_color	diff;
	//t_color	spec; specular reflection, its bonus and doesnt seem to work too well right now so idk
	t_color	accum;

	amb = compute_ambient(scene.ambient, hit.color);
	diff = compute_diffuse(scene, hit.color, hit.surface_normal, get_hit_point(ray, hit.distance));
	//spec = compute_specular(scene, hit.color, hit.surface_normal, get_hit_point(ray, hit.distance));
	//accum = color_add(color_add(amb, diff), spec);
	accum = color_add(amb, diff);
	return (ft_pixel(accum.r, accum.g, accum.b, 255));
}

void	render_scene(const t_scene scene)
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
			mlx_put_pixel(image, x, y, color);
			x++;
		}
		y++;
	}
}
