#include "../includes/fdf.h"

void	my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char *dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	draw_point_to_point(t_img **img, t_data curr)
{
	t_calc	c;

	c.dif_x = (curr.end_x - curr.start_x) << FIXED_SCALE;
	c.dif_y = (curr.end_y - curr.start_y) << FIXED_SCALE;
    if (abs(c.dif_x) > abs(c.dif_y))
        c.steps = abs(c.dif_x >> FIXED_SCALE);
    else
        c.steps = abs(c.dif_y >> FIXED_SCALE);
    c.x_inc = c.dif_x / c.steps;
    c.y_inc = c.dif_y / c.steps;
    c.x = curr.start_x << FIXED_SCALE;
    c.y = curr.start_y << FIXED_SCALE;
	if ((c.y >> FIXED_SCALE) > 0 && (c.y >> FIXED_SCALE) < 1390 && (c.x >> FIXED_SCALE) > 0 && (c.x >> FIXED_SCALE) < 2560)
		my_mlx_pixel_put(*img,  c.x >> FIXED_SCALE, c.y >> FIXED_SCALE, curr.start_color);
	c.i = 0;
	while(c.i < c.steps)
	{
		ft_calc_curr_color(&curr);
        c.x += c.x_inc;
        c.y += c.y_inc;
		if ((c.y >> FIXED_SCALE) > 0 && (c.y >> FIXED_SCALE) < 1390 && (c.x >> FIXED_SCALE) > 0 && (c.x >> FIXED_SCALE) < 2560)
			my_mlx_pixel_put(*img, c.x >> FIXED_SCALE, c.y >> FIXED_SCALE, curr.start_color);
		c.i++;
    }
}


void	draw_from_prev_row(t_img **img, int **curr_set,int **prev_set ,int current_col)
{
	int	i = 0;
	while (prev_set[i] && curr_set[i])
	{
		draw_point_to_point(img, calc_pos_col((t_arg){i, current_col},*img, prev_set[i], curr_set[i]));
		i++;
	}
}

void	draw_map_to_img(t_node *head,t_img *img)
{
	int	i;
	int	j;
	int	*prev;
	int	**previous_set;

	previous_set = NULL;
	j = 0;
	while (head)
	{
		i = 0;
		while (head->num_arr[i])
		{
			if (i != 0)
				draw_point_to_point(&img, calc_pos((t_arg){i, j}, img, prev, head->num_arr[i]));
			prev = head->num_arr[i];
			i++;
		}
		if (previous_set != NULL)
			draw_from_prev_row(&img, head->num_arr, previous_set, j);
		previous_set = head->num_arr;
		head = head -> next;
		j++;
	}
}

void	draw_background(t_img *img, int x, int y, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i != y)
	{
		j = 0;
		while (j != x)
		{
			my_mlx_pixel_put(img, j, i, color);
			j++;
		}
		i++;
		if (i % 8 == 0)
		{
		if (color % 0x100 > 0x11)
			color--;
		if (color % 0x10000 > 0x1111)
			color -= 0x100;
		if (color <= 0xff0000 && color > 0x111111)
			color-= 0x10000;
		}
	}
}
