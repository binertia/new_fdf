#include "../includes/fdf.h"

void	calc_init_draw(t_img *img, t_node *ref)
{
	int	box_size;
	int	temp;
	int	range_max;
	int	range_min;

	range_max = (((ref->max + ref->max_col) ) - ref->max_col) ;
	range_min = ((abs)(ref->min + ref->min_col) ) - (img->col - ref->min_col);
	if (range_max < 0)
		range_max = 0;
	if (range_min < 0)
		range_min = 0;
	box_size = 1;
	while (1000 > (img->col + img->row + range_max + range_min - 2 ) * box_size && box_size < 13)
		box_size++;
	if (box_size > 1)
		box_size--;
	range_max = (range_min - range_max) * box_size;
	img->box_size = box_size;
	img->init_x = 1280 + (int)((img->col * img->box_size * sqrt(3) - (img->row * img->box_size * sqrt(3))) / 2);
	img->init_y = 895 - (((img->col + img->row - 2) * box_size + range_max) / 2);
}

void	ft_init_color(t_data *res, int *prev_dent, int *dent)
{
	if (prev_dent[1] != 0)
		res->start_color = prev_dent[1];
	else if (prev_dent[0] >= 8)
		res->start_color = 0xFFFFFF;
	else if (prev_dent[0] >= 0 && prev_dent[0] < 8)
		res->start_color = 0xFFFF + (prev_dent[0] * 0x220000);
	else if (prev_dent[0] < 0 && prev_dent[0] > -8)
		res->start_color = 0xFFFF - (abs(prev_dent[0]) * 0x2200);
	else
		res->start_color = 0xFF;
	if (dent[1] != 0)
		res->end_color = dent[1];
	else if (dent[0] >= 0 && dent[0] < 8)
		res->end_color = 0xFFFF + (dent[0] * 0x220000);
	else if (dent[0] >= 8)
		res->end_color = 0xFFFFFF;
	else if (dent[0] < 0 && dent[0] > 8)
		res->end_color = 0xFFFF - (abs(dent[0]) * 0x2200);
	else
		res->end_color = 0xFF;
}

t_data	calc_pos(t_arg curr, t_img *img, int *prev_dent, int *dent)
{
	t_data	res;
	double	diff_x;
	int	diff_y;

	ft_init_color(&res, prev_dent, dent);
	diff_x = img->box_size * sqrt(3);
	diff_y = img->box_size;
	res.row = curr.row;
	res.col = curr.col;
	res.end_x = img->init_x + (curr.row * diff_x) - (curr.col * diff_x);
	res.start_x = img->init_x + (curr.row - 1) * diff_x - curr.col * diff_x;
	res.end_y = img->init_y + (curr.row + curr.col - (double)dent[0] ) * diff_y;
	res.start_y = img->init_y + ((curr.row - 1) + curr.col - (double)prev_dent[0] ) * diff_y;
	return (res);
}

t_data	calc_pos_col(t_arg curr, t_img *img, int *prev_dent, int *dent)
{
	t_data	res;
	double	diff_x;
	int	diff_y;


	ft_init_color(&res, dent, prev_dent);
	diff_x = img->box_size * sqrt(3);
	diff_y = img->box_size;
	res.row = curr.row;
	res.col = curr.col;
	res.start_x = img->init_x + (curr.row) * diff_x - curr.col * diff_x;
	res.end_x = img->init_x + ((curr.row + 1) * diff_x) - (curr.col * diff_x);
	res.start_y = img->init_y + ((curr.row) + curr.col ) * diff_y - ((double)dent[0]) * diff_y;
	res.end_y = img->init_y + (curr.row + (curr.col - 1) - (double)prev_dent[0]) * diff_y;
	return (res);
}

//================================================


void	ft_calc_curr_color(t_data *curr)
{
	if (curr->start_color < curr->end_color)
	{
		if (curr->start_color < 0xFFFF && curr->start_color >= 0xFF)
			curr->start_color += 0x100;
		else if (curr->start_color <= 0xFEFFFF && curr->start_color >= 0xFFFF)
			curr->start_color += 0x10000 ;
	}
	else if (curr->start_color > curr->end_color)
	{
		if (curr->start_color <= 0xFFFF && curr->start_color >= 0x1FF)
			curr->start_color -= 0x100;
		else if (curr->start_color <= 0xFFFFFF  && curr->start_color >= 0x1FFFF)
			curr->start_color -= 0x10000;
	}
}
