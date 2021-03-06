/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_fill.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:20:28 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/22 21:22:12 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_sep_init(t_data *d, t_sep *sep, int i)
{
	if (d->line[i] == '<' && d->line[i + 1] != '<')
		sep->simple_raft_left++;
	if (d->line[i] == '>' && d->line[i + 1] != '>')
		sep->simple_raft_right++;
	if (d->line[i] == '<' && d->line[i + 1] == '<')
		sep->double_raft_left++;
	if (d->line[i] == '>' && d->line[i + 1] == '>')
		sep->double_raft_right++;
	if (d->line[i] == '&' && d->line[i + 1] == '&')
		sep->double_and++;
}

void	fill_sep(t_data *d, t_sep *sep)
{
	int	i;
	int	s_quote;
	int	d_quote;

	i = 0;
	s_quote = 0;
	d_quote = 0;
	while (d->line[i])
	{
		if (d->line[i] == '\'' && d_quote == 0)
		{
			s_quote = 1;
			sep->simple_quo++;
		}
		if (d->line[i] == '"' && s_quote == 0)
		{
			d_quote = 1;
			sep->double_quo++;
		}
		if (d->line[i] == '|' && d_quote == 0 && s_quote == 0)
			sep->pipe++;
		fill_sep_init(d, sep, i);
		i++;
	}
}

t_lst	*fill_in_out_file(t_data *d, t_sep *sep, t_lst *cell)
{
	int	p_r;
	int	fd[0];

	p_r = found_place_raft(d->split, 0, d);
	while (p_r != -1)
	{
		if (d->split[p_r][0] == '<' && d->split[p_r][1] != '<')
			cell = create_new_int(cell, 'i', open(sep->infile, O_RDONLY));
		else if (d->split[p_r][0] == '>' && d->split[p_r][1] != '>')
			cell = create_new_int(cell, 'o', open(sep->outfile, O_CREAT
						| O_WRONLY | O_TRUNC, 0644));
		else if (d->split[p_r][0] == '>' && d->split[p_r][1] == '>')
			cell = create_new_int(cell, 'i', open(sep->infile, O_CREAT
						| O_WRONLY | O_APPEND, 0644));
		else if (d->split[p_r][0] == '<' && d->split[p_r][1] == '<')
		{
			if (cell->input > 0)
				close(cell->input);
			fd[0] = heredoc(d, d->split[p_r + 1]);
			cell->input = fd[0];
		}
		p_r = found_place_raft(d->split, p_r + 1, d);
	}
	return (cell);
}

int	fill_arg_data(t_data *d, int place_cmd)
{
	while (d->split[place_cmd] && (d->split[place_cmd][0] != '|' || (d->split[place_cmd][0] == '|' && (d->sp->d_quote == 1 || d->sp->s_quote == 1))))
		place_cmd++;
	return (place_cmd);
}

t_lst	*fill_arg(t_data *d, t_lst *cell)
{
	int	place_cmd;
	int	i;

	i = 0;
	place_cmd = found_cmd(d, cell);
	if (place_cmd != -1)
	{
		d->argo = malloc(sizeof(char *) * (fill_arg_data(d, place_cmd) + 1));
		if (!d->argo)
			return (NULL);
		while (d->split[place_cmd] && d->split[place_cmd][0] != '|')
		{
			if (d->split[place_cmd][0] == '<' || d->split[place_cmd][0] == '>')
				break ;
			d->argo[i] = ft_strdup(d->split[place_cmd]);
			i++;
			place_cmd++;
		}
		d->argo[i] = NULL;
		cell = create_new_char(cell, NULL, d->argo, 'a');
	}
	return (cell);
}
