#include "includes/fdf.h"
#include "includes/mlx.h"

int	win_exit(int keycode, t_var *var)
{
	exit(0);
}

int win_close(int keycode, t_var *var)
{
	if (keycode == 53)
	{
		mlx_destroy_image(var->mlx, var->img->img);
		mlx_destroy_window(var->mlx, var->win);
		free(var->mlx);
		exit(2);
	}
	return (0);
}

int	dear_user()
{
	write(2, "Usage : ./fdf (... map_file_name)\nbetter version is : valgrind ./fdf (... map_filename)", 87);
	return (2);
}

int main(int ac, char *av[])
{
	t_img	img;
	t_var	var;
	t_node	*map;
	t_node	ref;

	if (ac != 2)
		return (dear_user());
	map = get_input(av[1], &img, open(av[1], O_RDONLY), &ref);
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, 2560, 1390, "fdf");
	img.img = mlx_new_image(var.mlx, 2560, 1390);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
								 &img.endian);
	var.img = &img;
	calc_init_draw(&img, &ref);
	draw_background(&img, 2560, 1390, 0x7C81AD);
	draw_map_to_img(map, &img);
	ft_free_node(&map);
	mlx_put_image_to_window(var.mlx, var.win, img.img, 0, 0);
	mlx_hook(var.win, 2, 0, win_close, &var);
	mlx_hook(var.win, 17, 0, win_exit, &var);
	mlx_loop(var.mlx);
	return (0);
}

/*	from leaks
 * NO STACK
====
    2 (96.1K) ROOT LEAK: <non-object with no stack backtrace 0x600000f8b240> [64]
       1 (96.0K) <non-object with no stack backtrace 0x130008000> [98304]
*/
