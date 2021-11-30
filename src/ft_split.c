/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:03:18 by mkralik           #+#    #+#             */
/*   Updated: 2021/11/23 18:04:42 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split(char **args)
{
	int	size;
	int	i;

	i = 0;
	size = 0;
	while (args[size])
		size++;
	while (i < size)
		free(args[i++]);
	free(args);
}

static size_t	count_words(const char *s, char c)
{
	size_t	words;
	int		state;

	words = 0;
	state = 1;
	while (s && *s)
	{
		if (*s == c)
			state = 1;
		else if (state == 1)
		{
			words++;
			state = 0;
		}
		s++;
	}
	return (words);
}

static size_t	sizeof_word(const char *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] && (s[i] != c))
		i++;
	return (i);
}

static	char	**free_tab(char **tab, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
		free(tab[i++]);
	free(tab);
	return (0);
}

char	**ft_split(const char *s, char c)
{
	char	**tab;
	size_t	index;
	size_t	i;

	if (!s)
		return (NULL);
	tab = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!tab)
		return (NULL);
	index = 0;
	while (count_words(s, c))
	{
		while (*s == c)
			s++;
		tab[index] = malloc(sizeof(char) * (sizeof_word(s, c) + 1));
		if (!tab[index])
			return (free_tab(tab, index));
		i = 0;
		while (*s && (*s != c))
			tab[index][i++] = *s++;
		tab[index][i] = 0;
		index++;
	}
	tab[index] = 0;
	return (tab);
}

char	**ft_split_env(const char *s, char c)
{
	char	**tab;
	int		index;
	int		i;

	if (!s)
		return (NULL);
	tab = malloc(sizeof(char *) * 3);
	if (!tab)
		return (NULL);
	index = 0;
	tab[index] = malloc(sizeof(char) * (sizeof_word(s, c) + 1));
	if (!tab[index])
		return (free_tab(tab, index));
	i = 0;
	while (*s && (*s != c))
		tab[index][i++] = *s++;
	tab[index][i] = 0;
	index++;
	tab[index] = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!tab[index])
		return (free_tab(tab, index));
	i = 0;
	s++;
	while (*s)
		tab[index][i++] = *s++;
	tab[index][i] = 0;
	index++;
	tab[index] = 0;
	return (tab);
}

char	*ft_strdup(const char *s1)
{
	char	*dst;
	size_t	i;

	dst = malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!dst)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		dst[i] = s1[i];
		i++;
	}
	dst[i] = 0;
	return (dst);
}
