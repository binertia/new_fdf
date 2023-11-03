#include "get_next_line.h"

static int	ft_to_upper(char c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	else
		return (c);
}

int	ft_atoi_hex(const char *s)
{
	long	res;

	res = 0;
	if (*s && s[1] && *s == '0' && s[1] == 'x')
		s+=2;
	else
		return (0);
	while ((*s >= 'a' && *s <= 'f') || (*s >= 'A' && *s <= 'F') || (*s >= '0' && *s <= '9'))
	{
		if (*s >= '0' && *s <= '9')
			res = (res * 16) + (*s++ - '0');
		else
		{
			res = (res * 16) + (ft_to_upper(*s) + 10 - 'A');
			s++;
		}
	}
	return (res);
}
