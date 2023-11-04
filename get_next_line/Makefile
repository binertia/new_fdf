SRCS	=	get_next_line.c get_next_line_utils_bonus.c ft_split.c ft_atoi.c ft_atoi_hex.c
OBJS	=	$(SRCS:%.c=%.o)
CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror
RM		=	rm -rf
NAME	=	libgetn.a

.PHONY:		all clean fclean re

all:		$(NAME)

$(NAME):	$(OBJS)
			ar rc $(NAME) $(OBJS)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

