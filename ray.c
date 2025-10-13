#include "miniRT.h"

/* generate_ray: create ray in camera-local coordinates and rotate by camera */
t_ray	generate_ray(int x, int y, t_camera cam)
{
	t_ray	ray;
	t_vec3	dir;
	double	u;
	double	v;
	double	viewport;

	viewport = 2.0 * tan(cam.fov * M_PI / 180.0 * 0.5);
	u = ((x + 0.5) / WIDTH - 0.5) * viewport * (WIDTH / (double)HEIGHT);
	v = (0.5 - (y + 0.5) / HEIGHT) * viewport;
	ray.origin = cam.coord;
	dir.x = u;
	dir.y = v;
	dir.z = 1.0;
	ray.direction = dir;
	ray = vec_camera_rotate(ray, cam);
	ray.direction = vec_normalize(ray.direction);
	return (ray);
}
