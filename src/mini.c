/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 15:50:42 by mkralik           #+#    #+#             */
/*   Updated: 2021/10/27 14:17:26 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_env *env)
{
	while (env)
	{
		ft_putstr(env->key);
		ft_putchar('=');
		ft_putstr(env->value);
		ft_putchar('\n');
		env = env->next;
	}
}

t_env	*get_env(char **envp)
{
	t_env	*block;
	t_env	*b;
	int		i;
	char	**cell;

	i = 0;
	block = NULL;
	while (envp[i])
	{
		cell = ft_split(envp[i], '=');
		b = new_cell(cell[0], cell[1]);
		add_cell(&block, b);
		free_split(cell);
		i++;
	}
	return (block);
}

t_data	*init_data(char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->prompt = NULL;
	data->line = NULL; //get_line pour l'instant sans split
	data->env = NULL;
	data->cmd_lst = NULL;
	data->env = get_env(envp);
	return (data);
}

int	main(int argc, char **argv, char **envp)
{
	(void)	argv;
	t_data	*d;

	if (argc != 1)
		exit(EXIT_FAILURE);
	d = init_data(envp);
	if (!d)
		return(1);
	d->prompt = "minishell > ";
	while (1)
	{
		d->line = readline(d->prompt);
		d->cmd_lst = parsing(d);
	}
	return (0);
}

/*
faire Makefile \/
recuperer env / a mettre dans la struct en liste chainee
creer une liste chainee pour le parsing
check builtin ou pas
envoyer parsing en double tableau
pipe fork et compagnie
commande a executer
*/
