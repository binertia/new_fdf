#ifndef FDF_H
# define FDF_H

# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include "get_next_line.h"
# include "mlx.h"

char	*get_next_line(int fd);
char	**ft_split(char const *s, char c);
int		ft_atoi(char const *s);
int		ft_atoi_hex(char const *s);

#endif
