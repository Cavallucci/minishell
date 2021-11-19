/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcavallu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:20:28 by lcavallu          #+#    #+#             */
/*   Updated: 2021/11/19 17:11:35 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (d->line[i] == '|')
			sep->pipe++;
		if (d->line[i] == '&' && d->line[i + 1] == '&')
			sep->double_and++;
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
		if (d->line[i] == '<' && d->line[i + 1] != '<')
			sep->simple_raft_left++;
		if (d->line[i] == '>' && d->line[i + 1] != '>')
			sep->simple_raft_right++;
		if (d->line[i] == '<' && d->line[i + 1] == '<')
			sep->double_raft_left++;
		if (d->line[i] == '>' && d->line[i + 1] == '>')
			sep->double_raft_right++;	
		i++;
	}
}

int	check_sep(t_sep *sep)
{
	if (sep->simple_quo % 2 == 1 || sep->double_quo % 2 == 1)
	{
		ft_putstr("Syntax error\n");
		return (1);
	}
	return (0);
}

void	init_sep(t_sep *sep)
{
	sep->pipe = 0;
	sep->double_and = 0;
    sep->simple_quo = 0;
    sep->double_quo = 0;
    sep->simple_raft_left = 0;
    sep->simple_raft_right = 0;
    sep->double_raft_left = 0;
    sep->double_raft_right = 0;
}

void	print_sep(t_sep *sep, char **split)
{
	printf("| = %i\n&& = %i\n' = %i\n\" = %i\n< = %i\n> = %i\n<< = %i\n>> = %i\n", sep->pipe, sep->double_and, sep->simple_quo, sep->double_quo, sep->simple_raft_left, sep->simple_raft_right, sep->double_raft_left, sep->double_raft_right);
	for(int q=0; split[q]; q++)
		printf("split[%i] = %s\n", q, split[q]);
}

void	check_infile_outfile(char **split, t_sep *sep)
{
	int	place_raft;

	if (sep->simple_raft_left > 0 || sep->simple_raft_right > 0)
	{
		place_raft = found_place_raft(split, 0);
		if (place_raft != -1)
		{
			if (split[place_raft][0] == '<')
			{
				if (place_raft != 0)
					create_new(split[place_raft - 1], NULL, 'c');
				else
					create_new(split[place_raft + 2], NULL, 'c');
			}
			else if (split[place_raft][0] == '>')
			{
				create_new(split[place_raft - 1], NULL, 'c');
			}
		}
	}
}

int	check_chev(char **split)
{
	int	i;
	int	j;

	i = 0;
	while (split[i])
	{
		j = 0;
		if (split[i][0] != '"' && split[i][0] != '\'')
		{
			while (split[i][j])
			{
				if ((split[i][j] == '<' && split[i][j + 1] == '<' && split[i][j + 2] == '<' && split[i][j + 3] == '<') || (split[i][j] == '>' && split[i][j + 1] == '>' && split[i][j + 2] == '>' && split[i][j + 3] == '>') || (split[i][j] == '<' && split[i][j + 1] == '>') || (split[i][j] == '>' && split[i][j + 1] == '<'))
					return (1);
				j++;
			}
		}
		i++;
	}
	return (0);
}

void	fill_in_out_file(char **split)
{
	int	place_raft;

	place_raft = found_place_raft(split, 0);
	while (place_raft != -1)
	{
		if (split[place_raft][0] == '<')
			open(file
	}
	place_raft = found_place_raft(split);
	if (<)
		open(file, O_RDONLY);
	if (>)
		open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (>>)
		open(file, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (<<)
	{
		if (infile > 0)
			close(infile);
		heredoc();
	}
}

t_lst	*parsing(t_data *d)
{
	t_sep	sep[1];
	char	**split_pipe;
	char	**split;
	int		i;

	i = 0;
	init_sep(sep);
	fill_sep(d, sep);
	if (!check_sep(sep))
	{
		split_pipe = ft_split(d->line, '|');
		while (split_pipe[i])
		{
			split = ft_split_parsing(split_pipe[i]);
			if (!check_chev(split))
			{
				check_infile_outfile(split, sep); //--> detecte la cmd quand il y a chevrons
				fill_in_out_file(split);
			//ouvrir les fichiers avec chevrons
			//detecter la commande
			//detecter les guillemets && les args 
			//free split_pipe
				print_sep(sep, split);
				i++;
			}
			else
				printf("free_split et split_pipe\n");
		}
	}
	return (NULL);
}
