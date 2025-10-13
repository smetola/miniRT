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

	return (dot_distance(hit, point));
}

t_hit	get_sphere_hit(t_ray line, const t_sphere sphere)
{
	double	divisor;
	double	x;
	double	discriminant;
	double	distance;
	double	world_distance;
	double	d1;
	double	d2;
	t_vec3	hit_local;
	t_vec3	normal;

	line.origin = vec_sub(line.origin, sphere.coord);
	line.origin = vec_scale(line.origin, 2.0 / sphere.diam);
	divisor = vec_dot(line.direction, line.direction);
	if (fabs(divisor) < EPSILON) // divide by 0, no solution
		return ((t_hit){0});
	x = 2 * vec_dot(line.direction, line.origin);
	discriminant = x * x - 4 * divisor * (vec_dot(line.origin, line.origin) - 1);
	if (discriminant < 0)
		return ((t_hit){0});
	if (discriminant < EPSILON)
		distance = -x / (2 * divisor);
	else
	{
		d1 = (-x + sqrt(discriminant)) / (2 * divisor);
		d2 = (-x - sqrt(discriminant)) / (2 * divisor);
		distance = min_distance(d1, d2);
	}
	if (distance <= 0)
		return ((t_hit){0});
	hit_local = vec_add(line.origin, vec_scale(line.direction, distance));
	normal = vec_normalize(hit_local);
	world_distance = distance * sphere.diam / 2.0;
	return ((t_hit){1, world_distance, sphere.color, normal, (t_vec3){0}, (t_vec3){0}, (t_vec3){0}});
}

double	get_plane_hit(t_ray line, const t_plane plane)
{

	//transform line before calculating intersection
	double	height = point_to_plane_distance(line.origin, plane); //y origin coordinate of ray is equal to distance of origin to the plane
	line = vec_rotate_by_plane(line, plane.ori, (t_vec3){0, 1, 0});
	if (line.direction.y == 0) //paralelo al plano, nunca intersecan
		return (-1);
	//si el plano está debajo y miras arriba, no se ve. lo mismo si inviertes el eje y
	//if ((height < 0 && line.direction.y < 0) || (height > 0 && line.direction.y > 0))
	if (height > 0 && line.direction.y > 0)
		return (-1);
	if (height < 0 && line.direction.y < 0)
		return (-1);
	//line.origin.y -= height;
	double x = -height/line.direction.y;
	t_vec3	hit = vec_add(line.origin, vec_scale(line.direction, x));
	if (!is_point_ahead(line, hit)) //behind the screen, not valid
		return (-1);
	return	(dot_distance(hit, line.origin));
}

double	check_cylinder_height(t_ray line, double distance, double height)
{ //constrain final result to finite cylinder height
	double	hit_height = line.origin.y + distance * line.direction.y;
	if (hit_height < height && hit_height > 0) //todo epsilon compare
		return (hit_height);
	return (-1);
}

double	check_cap(t_ray line, double t, double radius)
{
	double	x = line.origin.x + t * line.direction.x;
	double	z = line.origin.z + t * line.direction.z;
	return (pow(x, 2) + pow(z, 2) <= radius); //if the line distance traveled in plane xz is higher than radius within cylinder height, its not a hit
}

double	get_cylinder_caps_distance(const t_ray line, const t_cylinder cylinder)
{
	//check lower cap
	double	t_low = (- line.origin.y) / line.direction.y; //cylinder height is doubled? (value of 1 means it extends 1 unit in both +y and -y, or that higher cap is y=height and lower cap is y=-height)
	//check higher cap
	double	t_high = (cylinder.hgt - line.origin.y) / line.direction.y; //cylinder is treated as starting at 0,0,0 and ending at 0,height,0 instead of 0,-height/2,0 and 0,height/2,0
	if (check_cap(line, t_low, cylinder.diam))
	{//lower cap is hit
		if (check_cap(line, t_high, cylinder.diam)) //both hit
			return (min_distance(t_low, t_high));
		else //no hit on higher cap
			return (t_low);
	}
	else
	{//no hit on lower cap
		if (check_cap(line, t_high, cylinder.diam))
			return (t_high);
	} //neither hit
	return (-1);
}

double	get_cylinder_hit(t_ray line, const t_cylinder cylinder)
{
	//line.origin = vec_sub(line.origin, cylinder.shape.ori); //included in rotation???
	//line.origin = vec_scale(line.origin, 1 / cylinder.diam);
	//scale direction vector as well so distance is properly calculated at the end, not multiplied by cylinder size (this doesnt seem to work tho)
	//line.direction = vec_scale(line.direction, 1 / cylinder.diam);
	line = vec_cylinder_rotate(line, cylinder);
	double	divisor = pow(line.direction.x, 2) + pow(line.direction.z, 2);
	if (divisor == 0) //divide by 0, no solution //todo epsilon compare
		return (-1);
	double	x = 2 * line.origin.x * line.direction.x + 2 * line.origin.z * line.direction.z;
	double	discriminant = x * x - 4 * divisor * (pow(line.origin.x, 2) + pow(line.origin.z, 2) - cylinder.diam);
	if (discriminant < 0) //root square of negative number, no solution
		return (-1);
	double	distance;
	if (discriminant == 0) //todo epsilon compare
		distance = check_cylinder_height(line, (-x / (2 * divisor)), cylinder.hgt);
	else
	{
		//double	d1 = check_cylinder_height(line, ((-x + sqrt(discriminant)) / (2 * divisor)), cylinder.hgt);
		//double	d2 = check_cylinder_height(line, ((-x - sqrt(discriminant)) / (2 * divisor)), cylinder.hgt);
		double	d1 = check_cylinder_height(line, ((-x + sqrt(discriminant)) / (2 * divisor)), cylinder.hgt);
		double	d2 = check_cylinder_height(line, ((-x - sqrt(discriminant)) / (2 * divisor)), cylinder.hgt);
		distance = min_distance(d1, d2);
	}
	distance = min_distance(distance, get_cylinder_caps_distance(line, cylinder));
	t_vec3	hit = vec_add(line.origin, vec_scale(line.direction, distance));
	//t_vec3	hit = vec_add(line.origin, vec_scale(line.direction, distance * cylinder.diam)); //scale final hit to cylinder scale
	if (!is_point_ahead(line, hit)) //behind the screen, not valid
		return (-1);
	return	(dot_distance(hit, line.origin));
}
