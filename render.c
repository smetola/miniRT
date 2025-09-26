#include "miniRT.h"

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

static int	find_closest_sphere(t_ray ray, t_scene *scene, int *out_index, double *out_t)
{
	int		i;
	double	t;
	double	closest_t;
	int		found;

	i = 0;
	found = 0;
	closest_t = INFINITY;
	while (i < scene->num_spheres)
	{
		if (hit_sphere(ray, &scene->spheres[i], &t))
		{
			if (t > 0.0001 && t < closest_t)
			{
				closest_t = t;
				*out_index = i;
				found = 1;
			}
		}
		i++;
	}
	if (found)
		*out_t = closest_t;
	return (found);
}

t_color color_add(t_color a, t_color b)
{
    t_color r;

    r.r = a.r + b.r;
    r.g = a.g + b.g;
    r.b = a.b + b.b;
    if (r.r > 255)
        r.r = 255;
    if (r.g > 255)
        r.g = 255;
   if (r.b > 255)
       r.b = 255;
   return (r);
}


static int32_t	shade_pixel(t_scene *scene, t_ray ray, int index, double t)
{
	t_vec3	hit;
	t_vec3	normal;
	t_color	obj;
	t_color	amb;
	t_color	diff;

	hit = get_hit_point(ray, t);
	normal = get_normal_at_sphere(hit, &scene->spheres[index]);
	obj = scene->spheres[index].color;
	amb = compute_ambient(&scene->ambient, obj);
	diff = compute_diffuse(scene, obj, normal, hit);
	amb.r += diff.r;
	if (amb.r > 255)
		amb.r = 255;
	amb.g += diff.g;
	if (amb.g > 255)
		amb.g = 255;
	amb.b += diff.b;
	if (amb.b > 255)
		amb.b = 255;
	return (ft_pixel(amb.r, amb.g, amb.b, 255));
}

void	render_scene(t_scene *scene)
{
	int		x;
	int		y;
	int		hit_index;
	double	t;
	t_ray	ray;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			ray = generate_ray(x, y, scene->camera);
			hit_index = -1;
			if (find_closest_sphere(ray, scene, &hit_index, &t))
				mlx_put_pixel(image, x, y, shade_pixel(scene, ray, hit_index, t));
			else
				mlx_put_pixel(image, x, y, ft_pixel(0, 0, 0, 255));
			x++;
		}
		y++;
	}
}