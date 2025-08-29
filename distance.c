#include "miniRT.h"

int	is_point_ahead(const t_ray line, const t_vec3 hit) //comprueba si hit esta delante (1) o detras (0) del origen de line
{
	//x
	if (line.direction.x > 0 && line.origin.x > hit.x)
		return (0);
	if (line.direction.x < 0 && line.origin.x < hit.x)
		return (0);
	//y
	if (line.direction.y > 0 && line.origin.y > hit.y)
		return (0);
	if (line.direction.y < 0 && line.origin.y < hit.y)
		return (0);
	//z
	if (line.direction.z > 0 && line.origin.z > hit.z)
		return (0);
	if (line.direction.z < 0 && line.origin.z < hit.z)
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

	return (point_distance(hit, point));
}

/*double	get_plane_distance(const t_ray line, const t_plane plane)
{
	//taken from https://stackoverflow.com/questions/7168484/3d-line-segment-and-plane-intersection
	double	d;

	if (vec_dot(plane.normal, line.direction) == 0) //paralelo al plano, nunca intersecan
		return (-1);
	d = vec_dot(plane.normal, plane.shape.ori); //todos los puntos X del plano cumplen la ecuacion vec_dot(plane.normal, X) = d
	float x = (d - vec_dot(plane.normal, line.origin)) / vec_dot(plane.normal, line.direction);
	t_vec3	hit = vec_add(line.origin, vec_scale(line.direction, x));
	if (!is_point_ahead(line, hit)) //behind the screen, not valid
		return (-1);
	return	(point_distance(hit, line.origin));
}*/

double	get_plane_distance(t_ray line, const t_plane plane)
{
	//@TODO idk why, but if the plane point.y is 0, the plane is never detected, there is still some tweaking to do here

	//transform line before calculating intersection
	line = vec_rotate_by_plane(line, plane.normal, (t_vec3){0, 1, 0});
	//@TODO rotate plane point also? idk
	line.origin.y -= plane.shape.ori.y; //with an xz plane, translation only matters in y axis? i think this is the wrong part, it cant be any point of the plane as reference
	if (line.direction.y == 0) //paralelo al plano, nunca intersecan
		return (-1);
	//si el plano está debajo y miras arriba, no se ve. lo mismo si inviertes el eje y
	if ((plane.shape.ori.y > line.origin.y && line.direction.y < 0) || (plane.shape.ori.y < line.origin.y && line.direction.y > 0))
		return (-1);
	double x = -line.origin.y/line.direction.y;
	t_vec3	hit = vec_add(line.origin, vec_scale(line.direction, x));
	if (!is_point_ahead(line, hit)) //behind the screen, not valid
		return (-1);
	return	(point_distance(hit, line.origin));
}

double	get_sphere_distance(t_ray line, const t_sphere sphere)
{
	line.origin = vec_sub(line.origin, sphere.shape.ori);
	line.origin = vec_scale(line.origin, 1 / sphere.diam);
	//scale direction vector as well so distance is properly calculated at the end, not multiplied by sphere size (this doesnt seem to work tho)
	//line.direction = vec_scale(line.direction, 1 / sphere.diam);
	//no rotation transform, as a one color sphere is gonna be the same from all angles
	double	divisor = vec_dot(line.direction, line.direction);
	if (divisor == 0) //divide by 0, no solution //todo epsilon compare
		return (-1);
	double	x = 2 * vec_dot(line.direction, line.origin);
	double	discriminant = x * x - 4 * divisor * (vec_dot(line.origin, line.origin) - 1);
	if (discriminant < 0) //root square of negative number, no solution
		return (-1);
	double	distance;
	if (discriminant == 0) //todo epsilon compareW
		distance = -x / (2 * divisor);
	else
	{
		double	d1 = (-x + sqrt(discriminant)) / (2 * divisor);
		double	d2 = (-x - sqrt(discriminant)) / (2 * divisor);
		distance = min_distance(d1, d2);
	}
	t_vec3	hit = vec_add(line.origin, vec_scale(line.direction, distance * sphere.diam)); //scale final hit to sphere scale
	if (!is_point_ahead(line, hit)) //behind the screen, not valid
		return (-1);
	return	(point_distance(hit, line.origin));
}

double	get_cylinder_body_hit_distance(const t_ray line, const t_cylinder cylinder, const t_vec3 hit)
{
	if (!is_point_ahead(line, hit))
		return (-1);
	double	body_distance = vec_dot(cylinder.axis, vec_sub(hit, cylinder.shape.ori)); //distance from cylinder origin to hit, aligned with cylinder axis
	if (body_distance < 0 || body_distance > cylinder.hgt) //not within cylinder height, miss
		return (-1);
	return (point_distance(hit, line.origin));
}

double	get_cylinder_body_distance(const t_ray line, const t_cylinder cylinder)
{
	t_vec3	divisor_vec;
	double	divisor;
	double	prefix;
	double	incog;

	divisor_vec = vec_prod(line.direction, cylinder.axis);
	if (is_empty_vec(divisor_vec))
		return (-1); //divison by zero, no solutions
	divisor = vec_dot(divisor_vec, divisor_vec);
	prefix = vec_dot(divisor_vec, vec_prod(cylinder.shape.ori, cylinder.axis));
	incog = vec_dot(cylinder.axis, cylinder.axis) * pow(vec_dot(cylinder.shape.ori, divisor_vec), 2);
	incog = divisor * pow(cylinder.diam/2, 2) - incog;
	if (incog < 0) //sqrt of negative, no solutions
		return (-1);
	else if (incog == 0) //one solution
		return get_cylinder_body_hit_distance(line, cylinder, vec_scale(line.direction, prefix / divisor));
	else
	{
		double	d1 = get_cylinder_body_hit_distance(line, cylinder, vec_scale(line.direction, (prefix + sqrt(incog)) / divisor));
		double	d2 = get_cylinder_body_hit_distance(line, cylinder, vec_scale(line.direction, (prefix - sqrt(incog)) / divisor));
		return (min_distance(d1, d2));
	}
}

double	get_cylinder_base_hit_distance(const t_ray line, const t_vec3 ori, const double diam, const t_vec3 hit)
{
	if (!is_point_ahead(line, hit))
		return (-1);
	t_vec3	aux = vec_sub(hit, ori);
	double	base_distance = vec_dot(aux, aux); //distance from cylinder base origin to hit, within base plane
	if (base_distance >= pow(diam/2, 2.12)) //not within base circumference, miss (should be 2 and not 2.12, but idk)
		return (-1);
	return (point_distance(hit, line.origin));
}

double	get_cylinder_base_distance(const t_ray line, const t_cylinder cylinder)
{
	t_vec3	ori2 = vec_add(cylinder.shape.ori, vec_scale(cylinder.axis, cylinder.hgt)); //cylinder.shape.ori is the center point of one of the bases, ori2 is the other
	double	divisor = vec_dot(line.direction, cylinder.axis);
	if (divisor == 0) //line is parallel to base planes, not solvable
		return (-1);
	double	d1 = get_cylinder_base_hit_distance(line, cylinder.shape.ori, cylinder.diam, vec_scale(line.direction, vec_dot(cylinder.shape.ori, cylinder.axis) / divisor));
	double	d2 = get_cylinder_base_hit_distance(line, ori2, cylinder.diam, vec_scale(line.direction, vec_dot(ori2, cylinder.axis) / divisor));
	return (min_distance(d1, d2));
}

double	get_cylinder_distance(const t_ray line, const t_cylinder cylinder)
{
	double	d1;
	double	d2;

	d1 = get_cylinder_body_distance(line, cylinder);
	d2 = get_cylinder_base_distance(line, cylinder);
	return (min_distance(d1, d2));
}