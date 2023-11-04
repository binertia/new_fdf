#ifndef FDF_H
# define FDF_H

# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <math.h>
# include <sys/fcntl.h>
# include "get_next_line.h"
# include "mlx.h"

# define FIXED_SCALE 16

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

typedef struct s_var
{
	void	*win;
	void	*mlx;
	t_img	*img;
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

typedef struct s_calc_map
{
	int	i;
	int	j;
	int	k;
	int	max;
	int	**num_arr;
}			t_calc_map;

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

int		check_valid_hex(char *s);
int		check_valid_input(char *s, int i);
int		valid_color(char *s);
int		add_init(t_img **img, int row, int col, int fd);
t_node		*get_input(char *s, t_img *img, int fd, t_node *ref);

void	draw_point_to_point(t_img **img, t_data curr);
void	draw_from_prev_row(t_img **img, int **curr_set,int **prev_set ,int current_col);
void	draw_map_to_img(t_node *head,t_img *img);
void	draw_background(t_img *img, int x, int y, int color);
void	my_mlx_pixel_put(t_img *data, int x, int y, int color);
void	calc_init_draw(t_img *img, t_node *ref);

t_data	calc_pos(t_arg curr, t_img *img, int *prev_dent, int *dent);
t_data	calc_pos_col(t_arg curr, t_img *img, int *prev_dent, int *dent);

char	*get_next_line(int fd);
char	**ft_split(char const *s, char c);

int		ft_atoi(char const *s);
int		ft_atoi_hex(char const *s);
int		ft_is_number(char c);
int		ft_color_input(t_calc_map *c, char **arr);
int		ft_make_map(char **arr, t_node **node, t_node **ref);
int		ft_exit_invalid_input(char** temp, t_node **head, int fd);

void	ft_free_node(t_node **map);
void	*ft_new_list(int **content);
void	ft_new_list_add_back(int **content, t_node **node);
void	ft_free_char_set(char **arr);
void	ft_init_calc_map(t_calc_map *c, t_node **ref);
void	ft_add_max_dent(t_calc_map *c, t_node **ref, char **arr);
void	ft_init_color(t_data *res, int *prev_dent, int *dent);
void	ft_calc_curr_color(t_data *curr);

int		win_close(int keycode, t_var *vars);



#endif
