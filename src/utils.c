/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 14:00:01 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/06 17:29:07 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putstr(const char *s)
{
	while (s && *s)
		write(1, s++, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	while (s && *s)
		write(fd, s++, 1);
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

int	ft_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	return (0);
}

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	if (!(*needle))
		return ((char *)haystack);
	i = 0;
	while (haystack[i] && len && i < len)
	{
		j = 0;
		while (haystack[i + j] == needle[j] && (i + j) < len)
		{
			j++;
			if (!needle[j])
				return ((char *)&haystack[i]);
		}
		i++;
	}
	return (NULL);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_strncmp(const char *s1, const char *s2, int n)
{
	int		i;
	int		d;

	i = 0;
	while (s1[i] && s1[i] == s2[i] && i + 1 < n)
		i++;
	d = (unsigned char)s1[i] - (unsigned char)s2[i];
	if (!d || !n)
		return (0);
	else if (d < 0)
		return (-1);
	else
		return (1);
}

int cmp_str(const char *s1, const char *s2)
{
	return (!ft_strncmp(s1, s2, ft_strlen(s1)) && !ft_strncmp(s1, s2, ft_strlen(s2)));
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*d;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	d = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (d == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		d[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		d[i + j] = s2[j];
		j++;
	}
	d[i + j] = 0;
//	free_str(&s1);
	return (d);
}

int	ft_atoi(const char *str)
{
	unsigned long long	value;
	int					sign;
	size_t				i;

	value = 0;
	sign = 1;
	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
		value = value * 10 + str[i++] - 48;
	if (value > 9223372036854775807 && sign == 1)
		return (-1);
	else if (value > 9223372036854775807 && sign == -1)
		return (0);
	return (value * sign);
}
