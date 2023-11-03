#include "includes/fdf.h"
#include "includes/mlx.h"
#include <math.h>
#include <sys/fcntl.h>

// setup
typedef struct s_img
{
	// default
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;

	// custom
	int		row;
	int		col;
	int		box_size;
	int		init_x;
	int		init_y;
}	t_img;

// first part == input
typedef struct s_var
{
	void	*win;
	void	*mlx;
}	t_var;

typedef struct s_node
{
	int **num_arr;
	
	int	max;
	int	min;
	int	max_col;
	int	min_col;
	struct s_node	*next;
}	t_node;

// free_time
void	ft_free_node(t_node **map)
{
	t_node	*temp;

	temp = *map;
	while (temp)
	{
		t_node *free_time = temp;
		temp = temp->next;
		int i = 0;
		while(free_time->num_arr[i])
		{
			free(free_time->num_arr[i]);
			i++;
		}
		free(free_time->num_arr[i]);
		free(free_time->num_arr);
		free(free_time);
		free_time = NULL;
	}
}

int	check_valid_hex(char *s)
{
	int	i;

	i = 0;
	if (*s == '\n')
		return (0);
	while (s[i] && ((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'f')
		|| (s[i] >= 'A' && s[i] <= 'F') || s[i] == '\n'))
		i++;
	if (!s[i] || s[i] == ' ')
		return (i);
	return (0);
}

int	check_valid_input(char *s)
{
	while (*s)
	{
		if (*s == ' ' || (*s >= '0' && *s <= '9') || *s == '\n')
			s++;
		else if (*s == '-' && s[1] && (s[1] >= '0' && s[1] <= '9'))
			s++;
		else if (*s == ',' && s[1] && s[1] == '0' && s[2] && s[2] == 'x' && s[3])
		{
			if (check_valid_hex(s + 3))
				s = s + 3 + (check_valid_hex(s + 3));
			else	
				return (0);
		}
		else if (*s == '0' && s[1] && s[1] == 'x' && s[2])
			s += 2;
		else
			break;
	}
	if (*s)
		return (0);
	return (1);
}

void	*ft_new_list(int **content)
{
	t_node	*new;

	new = (t_node *)malloc(sizeof(t_node));
	new->num_arr = content;
	new->next = NULL;
	return (new);
}


void	ft_new_list_add_back(int **content, t_node **node)
{
	t_node	*temp;

	if (!content)
		return ;
	printf("finish add\n");
	if (*node == NULL)
	{
		*node = ft_new_list(content);
		return ;
	}
	temp = *node;
	while (temp && temp -> next)
		temp = temp -> next;
	temp->next = ft_new_list(content);
}

int	valid_color(char *s)
{
	if (*s && s[1] && *s == '0' && s[1] == 'x')
		s += 2;
	else
		return (0);
	while (*s)
	{
		if (*s >= '0' && *s <= '9')
			s++;
		else if (*s >= 'a' && *s <= 'f')
			s++;
		else if (*s >= 'A' && *s <= 'F')
			s++;
		else
			return (0);
	}
	if (*s == 'x')
		return (0);
	return (1);
}

typedef struct s_calc_map
{
	int	i;
	int	j;
	int	k;
	int	max;
	int	**num_arr;
}			t_calc_map;

///  =========================
///
void	ft_init_calc_map(t_calc_map *c, t_node **ref)
{
	c->i = 0;
	c->j = 0;
	(*ref)->min = 322;
	(*ref)->max = -322;
}

void	ft_add_max_dent(t_calc_map *c, t_node **ref, char **arr)
{
		c->num_arr[c->j] = (int *)malloc(sizeof(int) * 2);
		c->num_arr[c->j][0] = ft_atoi(arr[c->j]);
		if (c->num_arr[c->j][0] - c->j> (*ref)->max)
		{
			(*ref)->max = c->num_arr[c->j][0] - c->j;
			(*ref)->max_col = c->j;
		}
		if (c->num_arr[c->j][0] < (*ref)->min)
		{
			(*ref)->min = c->num_arr[c->j][0] - c->j;
			(*ref)->min_col = c->j;
		}
		c->k = 0;
}

int	ft_color_input(t_calc_map *c, char **arr)
{
	if (!valid_color(&arr[c->j][c->k + 1]))
		c->num_arr[c->j][1] = 0xFF0000;
	else
		c->num_arr[c->j][1] = ft_atoi_hex(&arr[c->j][c->k + 1]);
	return (1);
}

void	ft_free_char_set(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr[i]);
	free(arr);
}

int	ft_make_map(char **arr, t_node **node, t_node **ref)
{
	t_calc_map	c;

	ft_init_calc_map(&c, ref);
	while (arr[c.i])
		c.i++;
	c.num_arr = (int **)malloc(sizeof(int *) * (c.i + 1));
	while (c.i > c.j)
	{
		ft_add_max_dent(&c, ref, arr);
		while(arr[c.j][c.k])
		{
			if (arr[c.j][c.k] == ',' && ft_color_input(&c, arr))
				break;
			c.k++;
		}
		if (!arr[c.j][c.k])
			c.num_arr[c.j][1] = 0x0;
		c.j++;
	}
	c.num_arr[c.j] = NULL;
	ft_new_list_add_back(c.num_arr, node);
	ft_free_char_set(arr);
	return (c.j);
}

//=====================================

int	add_init(t_img **img, int row, int col)
{
	(*img)->row = row;
	(*img)->col = col;
	return (1);
}

//==================================

int	ft_exit_invalid_input(char** temp, t_node **head, int fd)
{
			printf("input_not_valid_baby");
			ft_free_node(head);
			free(temp);
			close(fd);
			return (0); // need to free :TODO:
}
int	get_input(char *s, t_node **head, t_img *img, int fd, t_node *ref)
{
	int		checker;
	int		num;
	int		count;
	char	*temp;

	if (fd == -1)
		return (0);
	checker = 0;
	count = 0;
	while (1)
	{
		temp = get_next_line(fd);
		if (!temp || *temp == '\n' || !*temp)
			break;
		if (!check_valid_input(temp))
			return (ft_exit_invalid_input(&temp, head, fd));
		num = ft_make_map(ft_split(temp, ' '), head, &ref);
		free(temp);
		count++;
	}
	free(temp);
	close(fd);
	return (add_init(&img, num, count));
}

// calc init_point :BUG:

void	calc_init_draw(t_img *img, t_node *ref)
{
	int	box_size;
	int	temp;
	int	range_max;
	int	range_min;

	range_max = (((ref->max + ref->max_col) ) - ref->max_col) ; //remove /3
	range_min = ((abs)(ref->min + ref->min_col) ) - (img->col - ref->min_col); // remove /3
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

// // for putr
 void	my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char *dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

// for drawing
typedef	struct s_arg
{
	int	row;
	int	col;
}	t_arg;

typedef	struct	s_data
{
	int	row;
	int	col;
	int	start_x;
	int	start_y;
	int	start_color;
	int	end_x;
	int	end_y;
	int	end_color;
}				t_data;

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

t_data	calc_pos(t_arg curr, t_img *img, int *prev_dent, int *dent) //:TODO:
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
#define FIXED_SCALE 16

typedef	struct	s_calc
{
	int	dif_x;
	int	dif_y;
	int	steps;
	int	x_inc;
	int	y_inc;
	int		i;
	int		x;
	int		y;
	int		color;
}				t_calc;

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
//  ======================================


// debug drawing :BUG:

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

//================================================
//hook
int win_close(int keycode, t_var *vars)
{
	if (keycode == 53)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		exit(2);
	}
	return (0);
}

int	main(int ac, char *av[])
{
	t_img img;
	t_var var;
	t_node *map;
	t_node ref;

	map = NULL;
	if (ac != 2)
		return (0);
	if (!get_input(av[1], &map, &img, open(av[1], O_RDONLY ), &ref))
		return (2);
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, 2560, 1390, "fdf"); // max 1680, 1020
	img.img = mlx_new_image(var.mlx, 2560, 1390);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
								 &img.endian);
	calc_init_draw(&img, &ref);
	draw_background(&img, 2560, 1390, 0x7C81AD);
	draw_map_to_img(map, &img);
	mlx_put_image_to_window(var.mlx, var.win, img.img, 0, 0);
	mlx_hook(var.win, 2, 0, win_close, &var);
	mlx_loop(var.mlx);
	ft_free_node(&map);
	return (0);
}
