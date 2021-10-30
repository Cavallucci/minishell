/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcavallu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:41:15 by lcavallu          #+#    #+#             */
/*   Updated: 2021/10/30 18:59:22 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sp	*init_sp()
{
	t_sp	*sp;

	sp = malloc(sizeof(t_sp));
	sp->line = 0;
	sp->count = 0;
	sp->j = -1;
	sp->k = 0;
	sp->new = NULL;
	return (sp);
}

int is_charset(char s)
{
    if (s == ' ' || s == '|' || s == '&' || s == '\'' || s == '"' || s == '<' || s == '>')
        return (1);
    return (0);
}

static int   count_charset(const char *s)
{
    int     i;
    int  words;

    words = 0;
    i = 0;
    while (s[i])
    {   
        if (is_charset(s[i]))
        {
			if (s[i] == '"')
			{
				i++;
				while (s[i] && s[i] != '"')
					i++;
				i++;
			}
			else if (s[i] == '\'')
			{
				i++;
				while (s[i] && s[i] != '\'')
					i++;
				i++;
			}
			else
            	while (s[i] && is_charset(s[i]) && s[i] != '"' && s[i] != '\'')
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

static size_t   count_char(const char *s, char c)
{
    size_t  i;

    i = 0;
    if (c == 'w')
        while (s[i] && !is_charset(s[i]))
            i++;
    else if (c == 'o') 
        while (s[i] && is_charset(s[i]))
            i++;
    else if (c == 'q')
        while (s[i] && s[i] != '"')
            i++;
    else if (c == 'u')
        while (s[i] && s[i] != '\'')
            i++;
    return (i);
}

static char *ft_free(char **dst)
{
    int i;

    i = 0;
    while (dst[i])
    {
        free(dst[i]);
        i++;
    }
    free(dst);
    return (NULL);
}

static char	*make_split_char(char *s, t_sp *sp)
{
	while (s[sp->line] && !is_charset(s[sp->line]))
	{
		sp->new[sp->j] = (char *)malloc(sizeof(char) * (count_char(s, 'w') + 1));
		if (!sp->new[sp->j])
			return (ft_free(sp->new));
		sp->k = 0;
		while (s[sp->line] && !is_charset(s[sp->line]))
			sp->new[sp->j][sp->k++] = s[sp->line++];
		sp->new[sp->j][sp->k++] = 0;
	}
	return (sp->new[sp->j]);
}

static char	*make_split_q(char *s, t_sp *sp)
{
	sp->line++;
	while (s[sp->line] && s[sp->line] != '"')
	{
		sp->new[sp->j] = (char *)malloc(sizeof(char) * (count_char(s, 'q') + 1));
		if (!sp->new[sp->j])
			return (ft_free(sp->new));
		sp->k = 0;
		while (s[sp->line] && s[sp->line] != '"')
		{
			sp->new[sp->j][sp->k] = s[sp->line];
			sp->k++;
			sp->line++;
		}
	//	sp->new[sp->j][sp->k++] = s[sp->line++];
		sp->new[sp->j][sp->k] = 0;
	}
	return (sp->new[sp->j]);
}

static char	*make_split_u(char *s, t_sp *sp)
{
	sp->line++;
	while (s[sp->line] && s[sp->line] != '\'')
	{
		sp->new[sp->j] = (char *)malloc(sizeof(char) * (count_char(s, 'u') + 1));
		if (!sp->new[sp->j])
			return (NULL);
		sp->k = 0;
		while (s[sp->line] && s[sp->line] != '\'')
		{
			sp->new[sp->j][sp->k] = s[sp->line];
			sp->k++;
			sp->line++;
		}
		sp->new[sp->j][sp->k++] = s[sp->line++];
		sp->new[sp->j][sp->k++] = 0;
	}
	return (sp->new[sp->j]);
}

static char	*make_split(char *s, t_sp *sp)
{
	while (s[sp->line] && is_charset(s[sp->line]) && s[sp->line] != '"' && s[sp->line] != '\'')
	{
		sp->new[sp->j] = (char *)malloc(sizeof(char) * (count_char(s, 'o') + 1));
		if (!sp->new[sp->j])
			return (NULL);
		sp->k = 0;
		while (s[sp->line] && is_charset(s[sp->line]) && s[sp->line] != '"' && s[sp->line] != '\'')
			sp->new[sp->j][sp->k++] = s[sp->line++];
		sp->new[sp->j][sp->k++] = 0;
	}
	return (sp->new[sp->j]);
}

char	**ft_split_parsing(char *s)
{
	t_sp	*sp;

	sp = init_sp();
	if (!sp)
		return (NULL);
	sp->j = -1;
	sp->count = count_charset(s);
	sp->k = 0;
	sp->new = (char **)malloc(sizeof(char *) * (sp->count + 1));
	if (!s || !sp->new)
		return (NULL);
	while (++sp->j < sp->count)
	{
		if (is_charset(s[sp->line]))
		{
			if (s[sp->line] == '"')
				sp->new[sp->j] = make_split_q(s, sp);
			else if (s[sp->line] == '\'')
				sp->new[sp->j] = make_split_u(s, sp);
			else
				sp->new[sp->j] = make_split(s, sp);
		}
		else if (!is_charset(s[sp->line]))
			sp->new[sp->j] = make_split_char(s, sp);
	}
	sp->new[sp->j] = 0;
	return (sp->new);
}
