/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcavallu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:20:28 by lcavallu          #+#    #+#             */
/*   Updated: 2021/10/27 18:11:15 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_separators(t_data *d, t_sep *sep)
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
	int	j;

	j = 0;
	printf("| = %i\n&& = %i\n' = %i\n\" = %i\n< = %i\n> = %i\n<< = %i\n>> = %i\n", sep->pipe, sep->double_and, sep->simple_quo, sep->double_quo, sep->simple_raft_left, sep->simple_raft_right, sep->double_raft_left, sep->double_raft_right);
	while (split[j])
		printf("%s\n", split[j]);
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
	char	**split;

	init_sep(sep);
	check_separators(d, sep);
	split = ft_split_parsing(d->line); // diviser la fonction split
	check_infile_outfile(split, sep); // detecter les chevrons && infiles outfiles
	// detecter la commande
	check_dash(split);	// detecter l'option
	// detecter les guillemets && les args 
	print_sep(sep, split);
	return (NULL);
}
