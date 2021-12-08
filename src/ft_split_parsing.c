/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcavallu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:41:15 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/08 17:31:43 by lcavallu         ###   ########.fr       */
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
	sp->s_quote = 0;
	sp->d_quote = 0;
	sp->count_c = 0;
	sp->remember_mem = 0;
	return (sp);
}

int is_charset(char s)
{
	if (s == ' ' || s == '<' || s == '>')
		return (1);
	return (0);
}

static int   count_charset(const char *s)
{
    int	i;
    int	words;
	int	s_quote;
	int	d_quote;

    words = 0;
    i = 0;
	s_quote = 0;
	d_quote = 0;
    while (s[i])
    {  
        if (is_charset(s[i]))
		{
			if (s[i] == '<' || s[i] == '>')
			{
				while (s[i] == '<' || s[i] == '>')
					i++;
				words++;
			}
			else
				while (s[i] == ' ')
					i++;
		}
        else
        {
			if (s[i] == '"' && s_quote == 0)
			{
				d_quote = 1;
				i++;
				while (s[i] && s[i] != '"')
					i++;
				i++;
				if (s[i] && s[i] != ' ')
					words--;
			}
			else if (s[i] == '\'' && d_quote == 0)
			{
				s_quote = 1;
				i++;
				while (s[i] && s[i] != '\'')
					i++;
				i++;
				if (s[i] && s[i] != ' ')
					words--;
			}
			else
			{
				while (s[i] && !is_charset(s[i]) && s[i] != '"' && s[i] != '\'')
					i++;
				if (s[i] && s[i] != ' ')
					words--;
			}
			words++;
        }
    }
	return (words);
}

void    check_quote(char str, t_sp *sp)
{
    if (str == '\'' && sp->d_quote == 0)
    {
        if (sp->s_quote == 0)
            sp->s_quote = 1;
        else
            sp->s_quote = 0;
    }
    if (str == '"' && sp->s_quote == 0)
    {
        if (sp->d_quote == 0)
            sp->d_quote = 1;
        else
            sp->d_quote = 0;
    }
}

static size_t   count_char(const char *s, char c, t_sp *sp)
{
	size_t  i;
	int		count;

	i = sp->line;
	count = 0;
	if (c == 'w')
	{
		while (s[i] && !is_charset(s[i]))
		{
			check_quote(s[i], sp);
			while (s[i] == ' ')
				i++;
			if (s[i] == ' ' && sp->s_quote == 0 && sp->d_quote == 0)
				break ;
			if (s[i] == '"' && sp->s_quote == 0)
				i++;
			else if (s[sp->line] == '\'' && sp->d_quote == 0)
				i++;
			i++;
			count++;
		}
	}
	else if (c == 'c')
		while (s[i] && (s[i] == '<' || s[i] == '>'))
		{
			count++;
			i++;
		}
	sp->s_quote = 0;
	sp->d_quote = 0;
	return (count);
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

char *make_change(char *s, t_sp *sp, t_data *d)
{
	char	*new;
	int		size_new;
	int		i;
	t_env	*tmp;
	char	*bis;

	size_new = 0;
	while (s[sp->line] && !is_charset(s[sp->line]) && ft_isalpha_parsing(s[sp->line]))
	{
		size_new++;
		sp->line++;
	}
	new = (char *)malloc(sizeof(char) * (size_new + 1));
	sp->line -= size_new;
	i = 0;
	while (size_new > 0)
	{
		new[i] = s[sp->line];
		sp->line++;
		i++;
		size_new--;
	}
	new[i] = 0;
	tmp = d->env;
	while (tmp->next && !cmp_str(tmp->key, new))
		tmp = tmp->next;
	if (cmp_str(tmp->key, new))
	{
		sp->new[sp->j][sp->k] = 0;
		bis = (char *)malloc(sizeof(char) * (ft_strlen(sp->new[sp->j]) + 1));
		ft_strcpy(bis, sp->new[sp->j]);
		free(sp->new[sp->j]);
		sp->new[sp->j] = NULL;
		if (sp->remember_mem == 0)
			sp->new[sp->j] = (char *)malloc(sizeof(char) * ((sp->count_c - i) + ft_strlen(tmp->value) + 1));
		else
			sp->new[sp->j] = (char *)malloc(sizeof(char) * (sp->remember_mem + ft_strlen(tmp->value) + 1));
		sp->remember_mem += ft_strlen(tmp->value);
	printf("malloc = %li\n", (sp->count_c - i) + ft_strlen(tmp->value));
		i = 0;
		sp->k = 0;
		while (bis[i])
		{
			sp->new[sp->j][sp->k] = bis[i];
			sp->k++;
			i++;
		}
		i = 0;
		while (tmp->value[i])
		{
			sp->new[sp->j][sp->k] = tmp->value[i];
			sp->k++;
			i++;
		}
	}
	else
		return (NULL);
	return (sp->new[sp->j]);
}

static char	*make_split_char(char *s, t_sp *sp, t_data *d)
{
	sp->count_c = count_char(s, 'w', sp);
	sp->new[sp->j] = (char *)malloc(sizeof(char) * (sp->count_c + 1));
	if (!sp->new[sp->j])
		return (ft_free(sp->new));
	sp->k = 0;
	while (s[sp->line])
	{
		check_quote(s[sp->line], sp);
        if (is_charset(s[sp->line]) && sp->s_quote == 0 && sp->d_quote == 0)
			break;
		if (s[sp->line] == '"' && sp->s_quote == 0)
			sp->line++;
		else if (s[sp->line] == '\'' && sp->d_quote == 0)
			sp->line++;
		else if ((s[sp->line] == '$' && ft_isalpha_parsing(s[sp->line + 1])) && sp->s_quote == 0)
		{
			sp->line++;
			make_change(s, sp, d);
		}
		else
		{
			sp->new[sp->j][sp->k] = s[sp->line];
			sp->k++;
			sp->line++;
		}
	}
	sp->new[sp->j][sp->k] = 0;
	return (sp->new[sp->j]);
}
/*
static char	*make_split_q(char *s, t_sp *sp)
{
//	sp->d_quote = 1;
	sp->new[sp->j] = (char *)malloc(sizeof(char) * (count_char(s, 'q') + 1));
	if (!sp->new[sp->j])
		return (ft_free(sp->new));
	sp->k = 0;
	while (s[sp->line])
	{
		check_quote(s[sp->line], sp);
		if (s[sp->line] == ' ' && sp->s_quote == 0 && sp->d_quote == 0)
			break ;
		if (s[sp->line] == '"' && sp->s_quote == 0)
			sp->line++;
		else if (s[sp->line] == '\'' && sp->d_quote == 0)
			sp->line++;
		sp->new[sp->j][sp->k] = s[sp->line];
		sp->k++;
		sp->line++;
	}
	sp->new[sp->j][sp->k] = s[sp->line];
	sp->k++;
	sp->line++;
	sp->new[sp->j][sp->k] = 0;
	return (sp->new[sp->j]);
}

static char	*make_split_u(char *s, t_sp *sp)
{
//	sp->s_quote = 1;
	sp->new[sp->j] = (char *)malloc(sizeof(char) * (count_char(s, 'u') + 1));
	if (!sp->new[sp->j])
		return (NULL);
	sp->k = 0;
	while (s[sp->line])
	{
		check_quote(s[sp->line], sp);
		if (s[sp->line] == ' ' && sp->s_quote == 0 && sp->d_quote == 0)
			break ;
		if (s[sp->line] == '"' && sp->s_quote == 0)
			sp->line++;
		else if (s[sp->line] == '\'' && sp->d_quote == 0)
			sp->line++;
		sp->new[sp->j][sp->k] = s[sp->line];
		sp->k++;
		sp->line++;
	}
	sp->new[sp->j][sp->k] = s[sp->line];
	sp->k++;
	sp->line++;
	sp->new[sp->j][sp->k] = 0;
	return (sp->new[sp->j]);
}
*/
static char	*make_split_chev(char *s, t_sp *sp)
{
	sp->new[sp->j] = (char *)malloc(sizeof(char) * (count_char(s, 'c', sp) + 1));
	if (!sp->new[sp->j])
		return (ft_free(sp->new));
	sp->k = 0;
	while (s[sp->line] && (s[sp->line] == '<' || s[sp->line] == '>'))
	{
		sp->new[sp->j][sp->k] = s[sp->line];
		sp->k++;
		sp->line++;
	}
	sp->new[sp->j][sp->k] = 0;
	return (sp->new[sp->j]);
}

char	**ft_split_parsing(char *s, t_data *d)
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
			if (s[sp->line] == '<' || s[sp->line] == '>')
					make_split_chev(s, sp);
			else
			{
				while (s[sp->line] == ' ')
					sp->line++;
				sp->j--;
			}
		}
		else
			make_split_char(s, sp, d);
	}
	sp->new[sp->j] = 0;
	return (sp->new);
}
