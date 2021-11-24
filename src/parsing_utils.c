/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcavallu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:07:05 by lcavallu          #+#    #+#             */
/*   Updated: 2021/11/24 14:12:44 by lcavallu         ###   ########.fr       */
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
	return (cell);
}

void	add_cell_parsing(t_data *d, t_lst *new)
{
    t_lst	*last;

    if (!(d->cmd_lst))
        d->cmd_lst = new;
    else
    {
		last = d->cmd_lst;
		while (last->next != NULL)
			last = last->next;
		last->next = new;
    }
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
		printf("| pipe = %d                        \n", i);
		printf("| cmd = %s            \n", list->cmd);
//		printf("| arg = %s			  \n", list->arg);
		printf("| path = %s			  \n", list->path);
		printf("| input = %i		  \n", list->input);
		printf("| output = %i		  \n", list->output);
		printf("-----------------------------------\n");
		list = list->next;
		i++;
	}
}

t_lst	*create_new_char(t_lst *cell, char *split, char **arg, char what)
{
	if (what == 'c')	//commande
		cell->cmd = split;	
	if (what == 'p')	//path
		cell->path = split;
	if (what == 'a') // arguments
		cell->arg = arg;
	return (cell);
}

t_lst	*create_new_int(t_lst *cell, char what, int file)
{
	if (what == 'i')
		cell->input = file; //input
	if (what == 'o')
		cell->output = file; // output
	return (cell);
}

int	found_place_raft(char **split, int i)
{
	int	j;

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
