#include "miniRT.h"

int	is_point_ahead(const t_ray ray, const t_vec3 hit) //comprueba si hit esta delante (1) o detras (0) del origen de ray
{
	//x
	if (ray.direction.x > 0 && ray.origin.x > hit.x)
		return (0);
	if (ray.direction.x < 0 && ray.origin.x < hit.x)
		return (0);
	//y
	if (ray.direction.y > 0 && ray.origin.y > hit.y)
		return (0);
	if (ray.direction.y < 0 && ray.origin.y < hit.y)
		return (0);
	//z
	if (ray.direction.z > 0 && ray.origin.z > hit.z)
		return (0);
	if (ray.direction.z < 0 && ray.origin.z < hit.z)
		return (0);
	return (1);
}

double	min_distance(double d1, double d2)
{
	if (d1 < 0 && d2 < 0) //both miss
		return (-1);
	if (d1 < 0)
		return (d2);
	if (d2 < 0)
		return (d1);
	if (d1 < d2)
		return (d1);
	return (d2);
}

double	get_ray_to_point_distance(t_ray	ray, t_vec3	point)
{
	t_vec3	hit;

	//x
	if (ray.direction.x == 0)
		hit.x = ray.origin.x;
	else 
		hit.x = (point.x - ray.origin.x) / ray.direction.x; //check for inf
	//y
	if (ray.direction.y == 0)
		hit.y = ray.origin.y;
	else 
		hit.y = (point.y - ray.origin.y) / ray.direction.y; //check for inf
	//z
	if (ray.direction.z == 0)
		hit.z = ray.origin.z;
	else 
		hit.z = (point.z - ray.origin.z) / ray.direction.z; //check for inf

	return (dot_distance(hit, point));
}

t_hit	get_sphere_hit(t_ray ray, const t_sphere sphere)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	t_out;

	// oc = O - C
	oc = vec_sub(ray.origin, sphere.coord);
	// coeficientes de la cuadrática
	a = vec_dot(ray.direction, ray.direction);
	b = 2.0 * vec_dot(oc, ray.direction);
	c = vec_dot(oc, oc) - sphere.radius_squared;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return ((t_hit){0}); //no choca
	discriminant = sqrt(discriminant);
	/* intentamos la solución más cercana */
	t_out = (-b - discriminant) / (2.0 * a);
	if (t_out <= 0.0)
	{
		t_out = (-b + discriminant) / (2.0 * a);
		if (t_out <= 0.0) //todo epsilon?
			return ((t_hit){0});
	}
	t_vec3	normal = vec_sub(vec_add(ray.origin, vec_scale(ray.direction, t_out)), sphere.coord); //normalized dividing by sphere radius later
	if (vec_dot(ray.direction, normal) > 0) //check if looking inside the sphere //todo testing
		normal = vec_reverse(normal);
	return ((t_hit){1, t_out, sphere.color, vec_scale(normal, 2/sphere.diam), (t_vec3){0}, (t_vec3){0}, (t_vec3){0}}); // choca
}

t_hit	get_plane_hit(t_ray ray, const t_plane plane)
{
	if (vec_dot(plane.ori, ray.direction) == 0) //parallel, no hit. even if ray is contained within plane
		return ((t_hit){0});
	double	dividend = vec_dot(vec_sub(plane.coord, ray.origin), plane.ori);
	double divisor = vec_dot(ray.direction, plane.ori);
	if ((dividend > 0) != (divisor > 0)) //result will be negative (behind camera), return empty hit
		return ((t_hit){0});
	double	d = dividend / divisor;
	t_vec3	normal = plane.ori;
	if (vec_dot(ray.direction, plane.ori) > 0)
		normal = vec_reverse(normal);
	return	((t_hit){1, d, plane.color, normal, (t_vec3){0}, (t_vec3){0}, (t_vec3){0}});
}

t_hit	get_infinite_cylinder_hit(t_ray ray, const t_cylinder cylinder)
{
	double	a;
	double	b;
	double	c;
	double	delta;
	double	t1;
	double	t2;
	double	h1;
	double	h2;
	t_vec3		u;
	t_vec3		v;
	t_hit		hit;

	if (!vec_dot(ray.direction, cylinder.ori)) //todo epsilon compare
		return ((t_hit){0});
	
	hit.is_hit = 0;
	hit.color = cylinder.color;

	v = vec_sub(ray.origin, cylinder.coord); //X = O-C
	a = vec_dot(ray.direction, ray.direction) - pow(vec_dot(ray.direction, cylinder.ori), 2);
	b = vec_dot(ray.direction, v) - vec_dot(ray.direction, cylinder.ori) * vec_dot(v, cylinder.ori); //this is b/2
	b *= 2;
	c = vec_dot(v, v) - pow(vec_dot(v, cylinder.ori), 2) - cylinder.radius_squared;

	delta = b * b - 4 * a * c;
	if (delta < 0)
		return ((t_hit){0});
	t1 = (-b - sqrt(delta)) / (a * 2);
	t2 = (-b + sqrt(delta)) / (a * 2);
	if (t1 <= EPSILON && t2 <= EPSILON)
		return ((t_hit){0});
	if (t1 <= EPSILON)
		h1 = -1;
	else
		h1 = vec_dot(ray.direction, cylinder.ori)*t1 + vec_dot(v, cylinder.ori);
	if (t2 <= EPSILON)
		h2 = -1;
	else
		h2 = vec_dot(ray.direction, cylinder.ori)*t2 + vec_dot(v, cylinder.ori);
	if ((h2 < 0 || h2 > cylinder.hgt) && (h1 < 0 || h1 > cylinder.hgt))
		return ((t_hit){0});
	hit.is_hit = 1;
	if ((h1 < 0 || h1 > cylinder.hgt) || (h2 >= 0 && h2 <= cylinder.hgt && (t2 < t1)))
	{
		t1 = t2; //make sure hit is saved in t1/h1
		h1 = h2;
	}
	hit.distance = t1;
	u = ray_distance(ray.origin, ray.direction, t1);
	hit.surface_normal = vec_sub(u, vec_sub(cylinder.coord, vec_scale(cylinder.ori, h1)));
	if (vec_dot(ray.direction, vec_sub(u, ray_distance(cylinder.coord, cylinder.ori, h1))) > 0) //todo fully check if this works for camera inside cylinder
		hit.surface_normal = vec_reverse(hit.surface_normal); //reverse normal if inside the cylinder
	hit.surface_normal = vec_scale(hit.surface_normal, 2/cylinder.diam); //normalized dividing by cylinder radius
	return (hit);
}

t_hit	get_cylinder_hit(t_ray ray, const t_cylinder cylinder)
{
	t_hit	result;
	t_hit	temp_hit;
	result.is_hit = 0;
	result.distance = INFINITY;
	temp_hit = get_infinite_cylinder_hit(ray, cylinder);
	if (temp_hit.is_hit && temp_hit.distance < result.distance)
		result = temp_hit;
	temp_hit = get_plane_hit(ray, (t_plane){cylinder.coord, vec_reverse(cylinder.ori), cylinder.color});
	if (temp_hit.is_hit && temp_hit.distance < result.distance && dot_distance(cylinder.coord, ray_distance(ray.origin, ray.direction, temp_hit.distance)) < cylinder.diam * 0.5)
		result = temp_hit;
	temp_hit = get_plane_hit(ray, (t_plane){cylinder.coord2, cylinder.ori, cylinder.color});
	if (temp_hit.is_hit && temp_hit.distance < result.distance && dot_distance(ray_distance(cylinder.coord, cylinder.ori, cylinder.hgt), ray_distance(ray.origin, ray.direction, temp_hit.distance)) < cylinder.diam * 0.5)
		result = temp_hit;
	return result;
}
