/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:20:28 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/12 15:50:26 by lcavallu         ###   ########.fr       */
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
		ft_putstr("Syntax error : unmatched quote\n");
		g_exit_status = 2;
		return (g_exit_status);
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
	sep->infile = NULL;
	sep->outfile = NULL;
}

t_lst	*init_cell()
{
	t_lst	*cell;

	cell = malloc(sizeof(t_lst));
	if (!cell)
		return (NULL);
	cell->cmd = NULL;
	cell->arg = NULL;
	cell->path = NULL;
	cell->input = 0;
	cell->output = 0;
	cell->builtin = 0;
	return (cell);
}

void	print_sep(t_sep *sep, char **split)
{
	printf("| = %i\n&& = %i\n' = %i\n\" = %i\n< = %i\n> = %i\n<< = %i\n>> = %i\ninfile = %s\noutfile = %s\n", sep->pipe, sep->double_and, sep->simple_quo, sep->double_quo, sep->simple_raft_left, sep->simple_raft_right, sep->double_raft_left, sep->double_raft_right, sep->infile, sep->outfile);
	for(int q=0; split[q]; q++)
		printf("split[%i] = %s\n", q, split[q]);
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


t_lst	*check_infile_outfile(char **split, t_sep *sep, t_lst *cell)
{
	int		place_raft;

	place_raft = found_place_raft(split, 0);
	if (place_raft != -1)
	{
		while (place_raft != -1)
		{
			if (split[place_raft][0] == '<')
			{
				sep->infile = split[place_raft + 1];
				if (place_raft != 0)
				{
					while (split[place_raft - 1][0] == '-')
						place_raft -= 1;
					cell = create_new_char(cell, split[place_raft - 1], NULL, 'c');
				}
				else
				{
					cell = create_new_char(cell, split[place_raft + 2], NULL, 'c');
					ft_swap(&split[place_raft + 1], &split[place_raft + 2]);
				}
			}
			else if (split[place_raft][0] == '>')
			{
				sep->outfile = split[place_raft + 1];
				if (!cell->cmd)
					cell = create_new_char(cell, split[0], NULL, 'c');
			}
			place_raft = found_place_raft(split, place_raft + 1);
		}
	}
	else
		cell = create_new_char(cell, split[0], NULL, 'c');
	return (cell);
}

t_lst	*fill_in_out_file(char **split, t_sep *sep, t_lst *cell)
{
	int	place_raft;

	place_raft = found_place_raft(split, 0);
	while (place_raft != -1)
	{
	//	if (cell->input > 0)
	//		close(cell->input);
	//	if (cell->output > 0)
	//		close(cell->output);
		if (split[place_raft][0] == '<' && split[place_raft][1] != '<')
		{
			printf("infile: |%s|\n", sep->infile);
			cell = create_new_int(cell, 'i', open(sep->infile, O_RDONLY));
		}
		else if (split[place_raft][0] == '>' && split[place_raft][1] != '>')
		{
			printf("outfile: |%s|\n", sep->outfile);
			cell = create_new_int(cell, 'o', open(sep->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644));
		}
		else if (split[place_raft][0] == '>' && split[place_raft][1] == '>')
			cell = create_new_int(cell, 'i', open(sep->infile, O_CREAT | O_WRONLY | O_APPEND, 0644));
		else if (split[place_raft][0] == '<' && split[place_raft][1] == '<')
		{
			if (cell->input > 0)
				close(cell->input);
			//heredoc;
		}
//		add_cell_parsing(d, list);
		place_raft = found_place_raft(split, place_raft + 1);
	}
	return (cell);
}

int	found_cmd(char **split, t_lst *cell)
{
	int	i;

	i = 0;
	while (split[i])
	{
		if (ft_strcmp_parsing(cell->cmd, split[i]) != 1)
			return (i);
		i++;
	}
	return (-1);
}

t_lst	*fill_arg(char **split, t_lst *cell)
{
	int		place_cmd;
	char	**arg;
	int		i;
	int		tmp;

	i = 0;
	place_cmd = found_cmd(split, cell);
	if (place_cmd != -1)
	{
		tmp = place_cmd;
		while (split[place_cmd])
			place_cmd++;
		arg = malloc(sizeof(char*) * (place_cmd + 1));
		if (!arg)
			return (NULL);
		place_cmd = tmp;
		while (split[place_cmd] && split[place_cmd][0] != '|')
		{
			if (split[place_cmd][0] == '<' || split[place_cmd][0] == '>')
				break;
			arg[i] = split[place_cmd];
			i++;
			place_cmd++;
		}
		arg[i] = NULL;
		cell = create_new_char(cell, NULL, arg, 'a');
		//free arg ??
	}
	return (cell);
}

t_lst	*fill_builtin(t_lst *cell)
{

	if (cmp_str(cell->cmd, "echo") || cmp_str(cell->cmd, "cd") || cmp_str(cell->cmd, "pwd") || cmp_str(cell->cmd, "export") || cmp_str(cell->cmd, "unset") || cmp_str(cell->cmd, "env") || cmp_str(cell->cmd, "exit"))
		cell = create_new_int(cell, 'b', 1);
	else
		cell = create_new_int(cell, 'b', 0);
	return (cell);
}

int	check_if_path(char *argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (argv[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

t_lst	*ft_free_double(char **path, char *cmd, t_lst *cell)
{
	int	i;

	i = 0;
	while (path[i])
	{
		free(path[i]);
		i++;
	}
	free(path);
	if (cmd)
		cell = create_new_char(cell, cmd, NULL, 'p');
	else
		cell = create_new_char(cell, cell->cmd, NULL, 'p');
	return (cell);
}

t_lst	*found_path(t_lst *cell, t_data *d)
{
	char	**path;
	t_env	*tmp;
	int		i;
	char	*c;
	char	*cmd;

	tmp = d->env;
	while (tmp && ft_strcmp_parsing(tmp->key, "PATH"))
		tmp = tmp->next;
	path = ft_split(tmp->value, ':');
	if (!path)
		return (NULL);
	i = 0;
	if (check_if_path(cell->cmd))
		return (ft_free_double(path, NULL, cell));
	while (path[i])
	{
		c = ft_strjoin(path[i], "/");
		cmd = ft_strjoin(c, cell->cmd);
		free(c);
		if (access(cmd, F_OK) != -1)
			return (ft_free_double(path, cmd, cell));
		free(cmd);
		i++;
	}
	cell = create_new_char(cell, NULL, NULL, 'p');
	return (cell);
}

t_lst	*parsing(t_data *d)
{
	t_sep	sep[1];
	char	**split_pipe;
	char	**split;
	int		i;
	t_lst	*cell;

	i = 0;
	init_sep(sep);
	fill_sep(d, sep);
//	cell = init_cell();
//	cell->next = NULL;
	if (!check_sep(sep))
	{
		split_pipe = ft_split(d->line, '|');
		if (!split_pipe[i])
		{
			cell = init_cell();
			cell->next = NULL;
			ft_free_str(split_pipe);
			add_cell_parsing(d, cell);
			return (d->cmd_lst);
		}
		while (split_pipe[i])
		{
			split = ft_split_parsing(split_pipe[i], d);
			if (!check_chev(split))
			{
//				free(cell);
//				cell = NULL;
				cell = init_cell();
				cell = check_infile_outfile(split, sep, cell); //--> detecte la cmd quand il y a chevrons

				cell = fill_in_out_file(split, sep, cell);	//ouvrir et detecte les fichiers avec chevrons
				cell = fill_builtin(cell);
				if (cell->builtin == 0)
					cell = found_path(cell, d);
				cell = fill_arg(split, cell); // remplir les arguments
				cell->next = NULL;
				add_cell_parsing(d, cell);
			//free split_pipe
//			print_sep(sep, split);
			}
			else
			{
				cell = init_cell();
				ft_free_str(split_pipe);
				add_cell_parsing(d, cell);
				return (d->cmd_lst);
			}
				//printf("free_split et split_pipe\n");
			i++;
			free(d->sp);
			d->sp = NULL;
		}
	ft_free_str(split_pipe);
	print_list(d->cmd_lst);
	}
	else
	{
		cell = init_cell();
		add_cell_parsing(d, cell);		
		return (d->cmd_lst);
	}
	return (d->cmd_lst);
}
