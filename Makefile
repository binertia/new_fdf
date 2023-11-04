NAME		=	fdf
GETN		=	get_next_line/
MLX			=	minilibx/
GETN_A		=	get_next_line/libgetn.a
MLX_A		=	minilibx/libmlx.a

CC			=	cc
CFLAG		=	-Iincludes
RM			=	rm -rf
SRCS		=	fdf.c						\
				srcs/ft_calc.c				\
				srcs/ft_draw.c				\
				srcs/ft_get_input.c			\
				srcs/ft_list.c				\
				srcs/ft_make_map.c			\
				srcs/ft_valid_and_free.c	\

OBJS		=	$(SRCS:%.c=%.o)

.PHONY:			all clean fclean re localclean bonus

all:			$(NAME)

$(NAME):		$(OBJS)  $(GETN_A) $(MLX_A)
				@$(CC) $(CFLAG) $(OBJS) -L$(GETN) -lgetn -L$(MLX) -lmlx -lm -o $(NAME) -framework OpenGL -framework AppKit
				@echo "Hello FDF"

$(GETN_A):
				@$(MAKE) -s -C $(GETN)
				@echo "GET_NEXT_LINE is ready"

$(MLX_A):
				@$(MAKE) -s -C $(MLX)
				@echo "MLX has join the chat"


.c.o:
				@$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)
				@echo "Compiling with Love."

del:
				@$(RM) $(OBJS)
				@echo "Kill object."

clean:			del
				@$(MAKE) clean -s -C $(GETN)
				@$(MAKE) clean -s -C $(MLX)

fclean:			del
				@$(MAKE) fclean -s -C $(GETN)
				@$(MAKE) clean -s -C $(MLX)
				@$(RM) $(NAME)
				@echo "Execution file has gone."

re:				fclean all

