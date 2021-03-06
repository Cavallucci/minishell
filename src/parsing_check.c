/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:20:28 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/22 21:24:11 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_sep(t_sep *sep, t_data *d)
{
	if (!check_pipe_space(d->line))
		return (1);
	if (sep->simple_quo % 2 == 1 || sep->double_quo % 2 == 1)
	{
		ft_putstr_fd("Syntax error : unmatched quote\n", 2);
		g_exit_status = 2;
		return (g_exit_status);
	}
	return (0);
}

int	check_chev_data(char **split_pipe, int i, int j, t_data *d)
{
	if (((split_pipe[i][j] == '<' && split_pipe[i][j + 1] == '<'
		&& split_pipe[i][j + 2] == '<') || (split_pipe[i][j] == '<'
		&& split_pipe[i][j + 1] == '>')) && (d->sp->s_quote == 0 || d->sp->d_quote == 0))
	{
		ft_putstr_fd("syntax error near unexpected token `<<<'\n", 2);
		g_exit_status = 2;
		return (g_exit_status);
	}
	else if (((split_pipe[i][j] == '>' && split_pipe[i][j + 1] == '>'
		&& split_pipe[i][j + 2] == '>') || (split_pipe[i][j] == '>'
		&& split_pipe[i][j + 1] == '<')) && (d->sp->s_quote == 0 || d->sp->d_quote == 0))
	{
		ft_putstr_fd("syntax error near unexpected token `>>>'\n", 2);
		g_exit_status = 2;
		return (g_exit_status);
	}
	else if (((split_pipe[i][j] == '>' || split_pipe[i][j] == '<')
		&& split_pipe[i + 1] == NULL) && (d->sp->s_quote == 0 && d->sp->d_quote == 0))
	{
		ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
		g_exit_status = 2;
		return (g_exit_status);
	}
	return (0);
}

int	check_chev(t_data *d, char **split_pipe)
{
	int	i;
	int	j;

	i = 0;
	d->sp = init_sp();
	d->sp->s_quote = 0;
	d->sp->d_quote = 0;
	while (split_pipe[i])
	{
		j = 0;
		while (split_pipe[i][j])
		{
			check_quote(split_pipe[i][j], d);
			if (check_chev_data(split_pipe, i, j, d))
			{
				return (g_exit_status);
			}
			j++;
		}
		i++;
	}
	free(d->sp);
	d->sp = NULL;
	return (0);
}

int	check_pipe_space(char *split_pipe)
{
	int	i;
	int	not_space;

	i = 0;
	not_space = 0;
	while (split_pipe[i])
	{
		if (split_pipe[i] != ' ')
			not_space++;
		i++;
	}
	return (not_space);
}

int	check_pipe(char **split_pipe, t_sep *sep)
{
	int	i;
	
	i = 0;
	if (split_pipe[i])
	{
		while (i <= sep->pipe)
		{
			if (!split_pipe[i] || !check_pipe_space(split_pipe[i]))
			{
				ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
				g_exit_status = 2;
				return (g_exit_status);
			}
			i++;
		}
	}
	else
	{
		ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
		g_exit_status = 2;
		return (g_exit_status);
	}
	return (0);
}
