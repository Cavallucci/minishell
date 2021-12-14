/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 15:50:42 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/14 16:12:07 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

char	*design_prompt(t_data *data)
{
	char	*user;

	user = get_key("USER", data->env);
	user = ft_strjoin("\e[34;1m", user);
	user = ft_free_strjoin(user, "\e[0m: ❤️ ");
	user = ft_free_strjoin(user, "\e[92;7m");
	user = ft_free_strjoin(user, "minishell");
	user = ft_free_strjoin(user, "\e[0m > ");
	return (user);
}

int	exec_builtin(t_lst *cmd_lst, t_data *data)
{
	if (!ft_strcmp(cmd_lst->cmd, "echo"))
		return (exec_echo(cmd_lst, data));
	else if (!ft_strcmp(cmd_lst->cmd, "cd"))
		return (exec_cd(cmd_lst, data));
	else if (!ft_strcmp(cmd_lst->cmd, "pwd"))
		return (exec_pwd(cmd_lst, data));
	else if (!ft_strcmp(cmd_lst->cmd, "export"))
		return (exec_export(cmd_lst, data));
	else if (!ft_strcmp(cmd_lst->cmd, "unset"))
		return (exec_unset(cmd_lst, data));
	else if (!ft_strcmp(cmd_lst->cmd, "env"))
		return (exec_env(cmd_lst, data));
	else if (!ft_strcmp(cmd_lst->cmd, "exit"))
		exec_exit(data, cmd_lst);
	return (EXIT_FAILURE);
}

void	print_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		ft_putstr_fd(tmp->key, 1);
		ft_putchar_fd('=', 1);
		ft_putstr_fd(tmp->value, 1);
		ft_putchar_fd('\n', 1);
		tmp = tmp->next;
	}
}

void	print_export(t_env *export)
{
	t_env	*tmp;

	tmp = export;
	while (tmp)
	{
		ft_putstr_fd("export ", 1);
		ft_putstr_fd(tmp->key, 1);
		if (!tmp->with_value)
		{
			ft_putstr_fd("=", 1);
			ft_putchar_fd('"', 1);
			ft_putstr_fd(tmp->value, 1);
			ft_putchar_fd('"', 1);
		}
		ft_putstr_fd("\n", 1);
		tmp = tmp->next;
	}
}

t_env	*get_env_export(char **envp)
{
	t_env	*block;
	int		i;
	char	**cell;

	i = 0;
	block = NULL;
	while (envp[i])
	{
		cell = ft_split_env(envp[i], '=');
		add_cell(&block, new_cell(cell[0], cell[1], 0));
		free(cell);
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
	data->line = NULL;
	data->env = get_env_export(envp);
	data->export = get_env_export(envp);
	data->sp = NULL;
	data->cmd_lst = NULL;
	data->split = NULL;
	data->argo = NULL;
	init_signal(data);
	return (data);
}

int    main(int argc, char **argv, char **envp)
{
    (void)    argv;
    t_data    *d;

    if (*envp == NULL)
    {
        ft_putstr_fd("Basic environment variables are missing\n", 2);
        exit (1);
    }
    if (argc != 1)
        exit(EXIT_FAILURE);
    d = init_data(envp);
    if (!d)
        return(1);
    d->prompt = design_prompt(d);
    while (1)
    {
        d->line = readline(d->prompt);
        if (!d->line)
		{
			printf("exit\n");
			break;
		}
		if (d->line[0])
        {
			add_history(d->line);
    	    d->cmd_lst = parsing(d);
    	    if (d->cmd_lst->cmd)
    	    {
    	        ft_pipe(d, d->cmd_lst, -1, 1);
    	    }
    	    if (d->cmd_lst)
    	        free_cmd_lst(d, &d->cmd_lst);
   			if (d->split)
				ft_free_str(d->split);
			if (d->argo)
				ft_free_str(d->argo);
		}
   }
    //free(add_history)
    ft_free_all(d);
    return (0);
}

/*
faire Makefile \/
recuperer env / a mettre dans la struct en liste chainee
creer une liste chainee pour le parsing
check builtin ou pas
envoyer parsing en double tableau
pipe fork et compagnie
cmd a executer
*/
