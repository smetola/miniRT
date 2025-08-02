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

double	get_sphere_distance(const t_ray line, const t_sphere sphere)
{
	(void)line;
	(void)sphere;
	return -1;
}
double	get_plane_distance(const t_ray line, const t_plane plane)
{
	//taken from https://stackoverflow.com/questions/7168484/3d-line-segment-and-plane-intersection
	//@TODO: caso en el que el vector es parte del plano? supongo que se puede dejar como invisible al ser un plano perpendicular a la cámara y no hay curvatura de la luz?
	double	d;

	if (vec_dot(plane.normal, line.direction) == 0) //paralelo al plano, nunca intersecan
		return (-1);
	d = vec_dot(plane.normal, plane.shape.ori); //todos los puntos X del plano cumplen la ecuacion vec_dot(plane.normal, X) = d
	float x = (d - vec_dot(plane.normal, line.origin)) / vec_dot(plane.normal, line.direction);
	t_vec3	hit = vec_add(line.origin, vec_scale(line.direction, x));
	if (!is_point_ahead(line, hit)) //behind the screen, not valid
		return (-1);
	return	(dot_distance(hit, line.origin));
}

double	get_cylinder_body_hit_distance(const t_ray line, const t_cylinder cylinder, const t_vec3 hit)
{
	if (!is_point_ahead(line, hit))
		return (-1);
	double	body_distance = vec_dot(cylinder.axis, vec_sub(hit, cylinder.shape.ori)); //distance from cylinder origin to hit, aligned with cylinder axis
	if (body_distance < 0 || body_distance > cylinder.hgt) //not within cylinder height, miss
		return (-1);
	return (dot_distance(hit, line.origin));
}

double	get_cylinder_body_distance(const t_ray line, const t_cylinder cylinder)
{
	t_vec3	divisor_vec;
	double	divisor;
	double	prefix;
	double	incog;

	divisor_vec = vec_prod(line.direction, cylinder.axis);
	if (is_empty_vec(divisor_vec))
		return (-1); //no solutions
	divisor = vec_dot(divisor_vec, divisor_vec);
	prefix = vec_dot(divisor_vec, vec_prod(cylinder.shape.ori, cylinder.axis));
	incog = vec_dot(cylinder.axis, cylinder.axis) * pow(vec_dot(cylinder.shape.ori, divisor_vec), 2);
	incog = divisor * pow(cylinder.diam, 2) - incog;
	if (incog == 0) //one solution
		return get_cylinder_body_hit_distance(line, cylinder, vec_scale(line.direction, prefix / divisor));
	else
	{
		double	d1 = get_cylinder_body_hit_distance(line, cylinder, vec_scale(line.direction, (prefix + sqrt(incog)) / divisor));
		double	d2 = get_cylinder_body_hit_distance(line, cylinder, vec_scale(line.direction, (prefix - sqrt(incog)) / divisor));
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
}

double	get_cylinder_base_hit_distance(const t_ray line, const t_vec3 ori, const double diam, const t_vec3 hit)
{
	if (!is_point_ahead(line, hit))
		return (-1);
	t_vec3	aux = vec_sub(hit, ori);
	double	base_distance = vec_dot(aux, aux); //distance from cylinder base origin to hit, within base plane
	if (base_distance >= pow(diam, 2)) //not within base circumference, miss
		return (-1);
	return (dot_distance(hit, line.origin));
}

double	get_cylinder_base_distance(const t_ray line, const t_cylinder cylinder)
{
	t_vec3	ori2 = vec_add(cylinder.shape.ori, vec_scale(cylinder.axis, cylinder.hgt)); //cylinder.shape.ori is the center point of one of the bases, ori2 is the other
	double	divisor = vec_dot(line.direction, cylinder.axis);
	if (divisor == 0) //line is parallel to base planes, not solvable
		return (-1);
	double	d1 = get_cylinder_base_hit_distance(line, cylinder.shape.ori, cylinder.diam, vec_scale(line.direction, vec_dot(cylinder.shape.ori, cylinder.axis) / divisor));
	double	d2 = get_cylinder_base_hit_distance(line, ori2, cylinder.diam, vec_scale(line.direction, vec_dot(ori2, cylinder.axis) / divisor));
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

double	get_cylinder_distance(const t_ray line, const t_cylinder cylinder)
{
	double	d1;
	double	d2;

	d1 = get_cylinder_body_distance(line, cylinder);
	d2 = get_cylinder_base_distance(line, cylinder);
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