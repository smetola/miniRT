#include "miniRT.h"

/* 
O: origen rayo
D: dirección rayo
t: incognita, distancia recorrida dentro del rayo (hasta q choca)
P(t) = punto a distancia t del O

P(t) = O + t·D
|P(t) - C| = r -> |O + t·D - C|² = r²
dot(P(t) - C, P(t) - C) = r² ( |V|² = dot(V, V) = V.x² + V.y² + V.z², logitud del vector)
dot((O - C + t·D), (O - C + t·D)) = r²
oc = O - C
dot((oc + t·D), (oc + t·D)) = r²
dot(oc, oc) + 2t·dot(oc, D) + t²·dot(D, D) = r²
t²·dot(D, D) +  2t·dot(oc, D)+ dot(oc, oc) - r² = 0

A = dot(D, D)
B = 2·dot(oc, D)
C = dot(oc, oc) - r²
Solución: A·t² + B·t + C = 0
Δ = B² - 4·A·C
t = (-B ± sqrt(Δ)) / (2·A)
< 0 → No hay soluciones reales → no hay colisión
= 0 → Hay 1 punto → el rayo roza la esfera
> 0 → Hay 2 puntos → el rayo entra y sale de la esfera
 */

int hit_sphere(t_ray ray, t_sphere *sphere, double *t_out)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;
	double	discriminant;

	// oc = O - C
	oc = vec_sub(ray.origin, sphere->coord);
	// coeficientes de la cuadrática
	a = vec_dot(ray.direction, ray.direction);
	b = 2.0 * vec_dot(oc, ray.direction);
	c = vec_dot(oc, oc) - (sphere->diam / 2) * (sphere->diam / 2);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0); //no choca
	discriminant = sqrt(discriminant);
	/* intentamos la solución más cercana */
	*t_out = (-b - discriminant) / (2.0 * a);
	if (*t_out <= 0.0)
	{
		*t_out = (-b + discriminant) / (2.0 * a);
		if (*t_out <= 0.0)
			return (0);
	}
	return (1); // choca
}

static t_vec3	viewport_to_world(double u, double v, t_camera cam)
{
	t_vec3	forward;
	t_vec3	up;
	t_vec3	right;
	t_vec3	world_point;

	forward = vec_normalize(cam.orient);
	up = (t_vec3){0, 1, 0};
	if (fabs(forward.x) < 1e-6 && fabs(forward.z) < 1e-6) //prevenir gimbal lock
		up = (t_vec3){0, 0, 1};
	right = vec_normalize(vec_prod(up, forward));
	up = vec_prod(forward, right);
	// Transformar coordenadas del viewport al espacio real
	world_point = vec_add(forward, vec_add(vec_scale(right, u), vec_scale(up, v)));
	return (world_point);
}

/*
	WIDTH / HEIGHT				<- aspect ratio
	cam.fov * M_PI / 180.0		<- convertir grados a radianes
	viewport = ancho del plano  <- calculado desde el FOV horizontal
*/
t_ray	generate_ray(int x, int y, t_camera cam)
{
	t_ray	ray;
	t_vec3	pixel_dir;
	double	u;
	double	v;
	double	viewport;

	viewport = 2.0 * tan(cam.fov * M_PI / 180.0 / 2.0);
	// Convertir coordenadas de pixel a coordenadas del viewport
	u = ((x + 0.5) / WIDTH - 0.5) * viewport;
	v = (0.5 - (y + 0.5) / HEIGHT) * (viewport / (WIDTH / (double)HEIGHT));
	// Transformar usando la orientación real de la cámara
	pixel_dir = viewport_to_world(u, v, cam);
	ray.origin = cam.coord;
	ray.direction = vec_normalize(pixel_dir);
	return (ray);
}
