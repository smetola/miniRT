#include "miniRT.h"

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

static int	find_closest_sphere(t_ray ray, const t_scene scene, int *out_index, double *out_t)
{
	int		i;
	double	t;
	double	closest_t;
	int		found;

	i = 0;
	found = 0;
	closest_t = INFINITY;
	while (i < scene.num_spheres)
	{
		if (hit_sphere(ray, scene.spheres[i], &t))
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
	t_color	r;

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


static int32_t	shade_pixel(const t_scene scene, t_ray ray, int index, double t)
{
	t_vec3	hit;
	t_vec3	normal;
	t_color	obj;
	t_color	amb;
	t_color	diff;
	t_color	specular;

	hit = get_hit_point(ray, t);
	normal = get_normal_at_sphere(hit, &scene.spheres[index]);
	obj = scene.spheres[index].color;
	amb = compute_ambient(scene.ambient, obj);
	diff = compute_diffuse(scene, obj, normal, hit);
	specular = compute_specular(scene, (t_color) {150, 150, 150}, normal, hit); //usar scene.light.color si tuviera, si no se usa color blanco
	amb = color_add(color_add(amb, diff), specular);
	return (ft_pixel(amb.r, amb.g, amb.b, 255));
}

void	render_scene(const t_scene scene)
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
			ray = generate_ray(x, y, scene.camera);
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

/*old render_scene
t_hit	get_ray_hit(const int x, const int y, const t_scene scene)
{
	t_hit	result;
	t_hit	temp;
	double	min_dist;
	int		i;

	result.is_hit = 0;
	min_dist = __DBL_MAX__;
	i = 0;
	while (i < scene.num_spheres)
	{
		temp = get_sphere_hit(generate_ray(x, y, scene.camera), scene.spheres[i]);
		if (temp.distance < min_dist && temp.distance > 0)
		{
			min_dist = temp.distance;
			result = temp;
		}
		i++;
	}
	*i = 0;
	while (i < scene.num_planes)
	{
		dist = get_plane_hit(generate_ray(x, y, scene.camera), scene.planes[i]);
		if (dist < min && dist > 0) //&& dist = 0?
		{
			min = dist;
			hit = &(scene.planes + i)->shape;
		}
		i++;
	}
	i = 0;
	while (i < scene.num_cylinders)
	{
		dist = get_cylinder_hit(generate_ray(x, y, scene.camera), scene.cylinders[i]);
		if (dist < min && dist > 0)
		{
			min = dist;
			hit = &(scene.cylinders + i)->shape;
		}
		i++;
	}*

	return (result);
}

void render_scene(const t_scene scene)
{
	int 		x;
	int			y;
	t_hit 		hit;
	uint32_t	color;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			hit = get_ray_hit(x, y, scene);
			if (!hit.is_hit) //no collision, use ambient light
			{
				//double	d = get_ray_to_point_distance(generate_ray(x, y, scene.camera), scene.light.coord); //max of about 35700 (40k?) maybe max(min) brightness should be 10k, and anything above that, map to int 255
				//color = ft_pixel(scene.ambient.color, ft_map(d, 5000, 255));
				color = ft_pixel(scene.ambient.color, 25); //map scene.ambient.ratio to 0~255 in second argument
			}
			else
			{
				//hit.color = color_scale(hit.color, sin(...)); //scale by sine of angle of light hitting the surface
				//hit.color = color_prod(hit.color, color_scale(scene.ambient.color, 0.01)); merge with ambient color (this should be add and not prod i think)
				color = ft_pixel(hit.color, 255);
				//color = hit->color.b;
				//printf("Printing %d-%d-%d\n", hit->color.b, hit->color.g, hit->color.r);
			}
			mlx_put_pixel(image, x, y, color);
			//ray = generate_ray_simple(x, y, scene->camera);
			*if (hit_sphere(ray, sphere))
				mlx_put_pixel(image, x, y, ft_pixel(255, 0, 0, 255));
			else
				mlx_put_pixel(image, x, y, ft_pixel(0, 0, 0, 255));*
			x++;
		}
		y++;
	}
}*/