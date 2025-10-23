#include "../miniRT.h"

static int	validate_scene(t_scene *scene)
{
	if (scene->ambient.ratio == -1.0)
		return (ft_error("Missing ambient light definition"));
	if (scene->camera.fov == -1)
		return (ft_error("Missing camera definition"));
	if (!scene->has_light)
		return (ft_error("Missing light definition"));
	if (scene->ambient.ratio < 0.0 || scene->ambient.ratio > 1.0)
		return (ft_error("Ambient ratio must be between 0.0 and 1.0"));
	if (scene->camera.fov < 0 || scene->camera.fov > 180)
		return (ft_error("Camera FOV must be between 0 and 180"));
	return (1);
}

static int	parse_line(char *line, t_scene *scene)
{
	if (!line || !*line || *line == '\n' || *line == '\r') //delete \r condition??
		return (1);
	if (line[0] == 'A' && line[1] == ' ')
		return (init_ambient(line + 2, scene));
	else if (line[0] == 'C' && line[1] == ' ')
		return (init_camera(line + 2, scene));
	else if (line[0] == 'L' && line[1] == ' ')
		return (init_light(line + 2, scene));
	else if (line[0] == 's' && line[1] == 'p' && line[2] == ' ')
		return (init_sphere(line + 3, scene));
	else if (line[0] == 'p' && line[1] == 'l' && line[2] == ' ')
		return (init_plane(line + 3, scene));
	else if (line[0] == 'c' && line[1] == 'y' && line[2] == ' ')
		return (init_cylinder(line + 3, scene));
	else
		return (ft_error("Unknown element type"));
}

int	parse_rt_file(char *filename, t_scene *scene)
{
	int		fd;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (ft_error("Cannot open file"));
	init_scene(scene);
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (!parse_line(line, scene))
		{
			free(line);
			close(fd);
			return (0);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (validate_scene(scene));
}

int	check_args(int argc, char *argv)
{
	int		len;

	if (argc != 2)
		return (ft_error("Usage: ./miniRT <scene.rt>"));
	len = 0;
	while (argv[len])
		len++;
	if (len < 3)
		return (ft_error("Invalid file name"));
	if (argv[len - 3] != '.' || argv[len - 2] != 'r' || argv[len - 1] != 't')
		return (ft_error("File must have .rt extension"));
	return (1);
}