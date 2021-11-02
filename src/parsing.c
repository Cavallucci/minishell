/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcavallu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:20:28 by lcavallu          #+#    #+#             */
/*   Updated: 2021/11/02 16:28:20 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_separators(t_data *d, t_sep *sep)
{
	int	i;

	i = 0;
	while (d->line[i])
	{
		if (d->line[i] == '|')
			sep->pipe++;
		if (d->line[i] == '&' && d->line[i + 1] == '&')
			sep->double_and++;
		if (d->line[i] == '\'')
			sep->simple_quo++;
		if (d->line[i] == '"')
			sep->double_quo++;
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
	if (sep->simple_quo % 2 == 1 || sep->double_quo % 2 == 1)
	{
		ft_putstr("Syntax error\n");
		return (1);
	}
	return (0);
}

void	init_sep(t_sep *sep)
{
//malloc sep ??
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

void	check_dash(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		if (split[i][0] == '-')
			create_new(split[i], split, 'o');
		i++;
	}
}

void	check_infile_outfile(char **split, t_sep *sep)
{
	int	place_raft;

	if (sep->simple_raft_left > 0 || sep->simple_raft_right > 0)
	{
		place_raft = found_place_raft(split);
		if (place_raft != -1)
		{
			if (split[place_raft][0] == '<')
			{
				create_new(split[place_raft + 1], NULL, 'i');
				if (place_raft != 0)
					create_new(split[place_raft - 1], NULL, 'c');
				else
					create_new(split[place_raft + 2], NULL, 'c');
			}
			else if (split[place_raft][0] == '>')
			{
				create_new(split[place_raft + 1], NULL, 'o');
				create_new(split[place_raft - 1], NULL, 'c');
			}
		}
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
	if (!check_separators(d, sep))
	{
		split_pipe = ft_split(d->line, '|');
	//	while (split_pipe[i])
	//	{
			split = ft_split_parsing(split_pipe[i]);
	//		i++;
	//	}
		//detacher les chevrons colles 
		//check_infile_outfile(split, sep);
		//detecter les chevrons && infiles outfiles
		//detecter la commande
		//detecter les guillemets && les args 
		//free split_pipe
		print_sep(sep, split);
	}
	return (NULL);
}
