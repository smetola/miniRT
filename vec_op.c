#include "miniRT.h"

t_vec3  vec_normalize(t_vec3 v)
{
	double length;

	length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length == 0)
		return (v);
	v.x /= length;
	v.y /= length;
	v.z /= length;
	return (v);
}

t_vec3 vec_sub(t_vec3 a, t_vec3 b)
{
	return (t_vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

t_vec3 vec_add(t_vec3 a, t_vec3 b)
{
	return (t_vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

t_vec3 vec_scale(t_vec3 v, double s)
{
	return (t_vec3){v.x * s, v.y * s, v.z * s};
}

t_vec3	vec_prod(t_vec3 a, t_vec3 b)
{
	t_vec3	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}

double vec_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

double vec_length(t_vec3 v)
{
    return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

double	dot_distance(t_vec3 a, t_vec3 b)
{
	//taken from https://stackoverflow.com/questions/14568249/finding-the-distance-between-2-3d-points
	// (math.h is allowed)
	return (hypot(hypot(a.x - b.x, a.y - b.y), a.z - b.z));
}
int	is_empty_vec(t_vec3 v)
{
	return (v.x == 0 && v.y == 0 && v.z == 0);
}