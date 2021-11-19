/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcavallu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:07:05 by lcavallu          #+#    #+#             */
/*   Updated: 2021/11/19 15:47:44 by lcavallu         ###   ########.fr       */
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
		printf("-----------------------------------\n");
		list = list->next;
		i++;
	}
}

t_lst	*create_new(char *split, char **arg, char what)
{
	t_lst   *cell;

	cell = malloc(sizeof(t_lst));
	if (!cell)
		return (NULL);
	if (what == 'c')	//commande
		cell->cmd = split;	
	if (what == 'p')	//path
		cell->path = split;
	if (what == 'a') // arguments
		cell->arg = arg;
	if (what == 'i')
		cell->infile = split; //infile
	if (what == 'o')
		cell->outfile = split; // outfile
	cell->next = NULL;
	if (cell->cmd)
		printf("cmd = %s\n", cell->cmd);
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
