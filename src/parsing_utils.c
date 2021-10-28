/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcavallu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:07:05 by lcavallu          #+#    #+#             */
/*   Updated: 2021/10/28 15:02:49 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lst   *create_cell(char *cmd)
{
	t_lst *cell;

	cell = malloc(sizeof(t_lst));
	if (!(cell))
		return (NULL);
	cell->cmd = cmd;
	cell->opt = NULL;
	cell->path = NULL;
	return (cell);
} 

t_lst   *add_cell_pos(t_lst *list, char *cmd, int pos)
{ 
	t_lst *prec;
	t_lst *cur;
	t_lst *cell;
	int       i;

	cur = list;
	i = 0; 
	cell = create_cell(cmd);
	if (list == NULL)
		return (cell);
	while (i < pos)
	{
		i++;
		prec = cur;
		cur = cur->next;
	} 
	prec->next = cell;
	cell->next = cur;
	return (list);
}   

void    print_list(t_lst *list)
{   
	int       i;

	i = 0;
	while (list)
	{
		printf("-----------------------------------\n");
		printf("| i = %d                            \n", i);
		printf("| cmd = %s            \n", list->cmd);
		printf("| option = %s            \n", list->opt);
		printf("-----------------------------------\n");
		list = list->next;
		i++;
	}
}

int	is_charset(char s)
{
	if (s == ' ' || s == '|' || s == '&' || s == '\'' || s == '"' || s == '<' || s == '>')
		return (1);
	return (0);
}

static size_t	count_charset(const char *s)
{
	int		i;
	size_t	words;

	words = 0;
	i = 0;
	while (s[i])
	{
		if (is_charset(s[i]))
		{
			while (s[i] && is_charset(s[i]))
				i++;
			words++;
		}
		else
		{
			while (s[i] && !is_charset(s[i]))
				i++;
			words++;
		}
	}
	return (words);
}

static size_t	count_char(const char *s, char c)
{
	size_t	i;

	i = 0;
	if (c == 'w')
		while (s[i] && !is_charset(s[i]))
			i++;
	else if (c == 'o')
		while (s[i] && is_charset(s[i]))
			i++;
	return (i);
}

static char	**ft_free(char **dst)
{
	int	i;

	i = 0;
	while (dst[i])
	{
		free(dst[i]);
		i++;
	}
	free(dst);
	return (NULL);
}

char	*ft_make_split(char **s, char **new, int k, int j)
{
	while (is_charset(**s))
	{
		*new[j] = (char *)malloc(sizeof(char) * (count_char(*s, 'o') + 1));
		if (!(*new[j]))
			return (ft_free(*new));
		k = 0;
		while (**s && is_charset(**s))
			*new[j][k++] = **s++;
		*new[j][k] = 0;
	}
}

char	*ft_make_split_char(char **s, char **new, int k, int j)
{
	while (**s && !is_charset(**s))
	{
		new[j] = (char *)alloc(1, sizeof(char) * (count_char(*s, 'w') + 1));
		if (!(*new[j]))
			return (ft_free(*new));
		k = 0;
		while (*(*s) && !is_charset(**s))
			new[j][k++] = *(*s)++;
		new[j][k] = 0;
	}
	return (NULL);
}


char	**ft_split_parsing(char *s)
	{
		char	**new;
		size_t	count;
		size_t	j;
		size_t	k;

		j = -1;
		count = count_charset(s);
		k = 0;
		new = (char **)malloc(sizeof(char *) * (count + 1));
		if (!s || !new)
			return (NULL);
		while (++j < count)
		{
			if (is_charset(**s))
				new[j] = ft_make_split(&s, new, k, j);
			else
				new[j] = ft_make_split_char(&s, new, k, j);
		}
		new[j] = 0;
		return (new);
	}

	t_lst	*create_new(char *split, char **arg, char what)
	{
		t_lst   *cell;

		cell = malloc(sizeof(t_lst));
		if (!cell)
			return (NULL);
		if (what == 'o')	//option
			cell->opt = split;
		if (what == 'c')	//commande
			cell->cmd = split;	
		if (what == 'a')	//argument
			cell->arg = arg;
		if (what == 'p')	//path
			cell->path = split;
		if (what == 'i')	//infile
			cell->infile = split;
		if (what == 't')	//outfile
			cell->outfile = split;
		cell->next = NULL;
		printf("cell = %s\n", cell->cmd);
		return (cell);
	}

	int	found_place_raft(char **split)
	{
		int	i;
		int	j;

		i = 0;
		while (split[i])
		{
			j = 0;
			while (split[i][j])
			{
				if (split[i][j] == '<' || split[i][j] == '>')
					return (i);
				j++;
			}
			i++;
		}
		return (-1);
	}
