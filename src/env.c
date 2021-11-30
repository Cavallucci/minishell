/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 12:19:41 by mkralik           #+#    #+#             */
/*   Updated: 2021/11/26 16:08:13 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	size_env(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	return (i);
}

char	**get_env_to_char(t_env *env)
{
	char	**ch_env;
	t_env	*tmp;
	int		i;

	ch_env = malloc(sizeof(char *) * (size_env(env) + 1));
	if (!ch_env)
		return (NULL);
	tmp = env;
	i = 0;
	while (tmp)
	{
		ch_env[i] = ft_strjoin(ft_strdup(""), tmp->key);
		ch_env[i] = ft_strjoin(ch_env[i], "=");
		ch_env[i] = ft_strjoin(ch_env[i], tmp->value);
		i++;
		tmp = tmp->next;
	}
	ch_env[i] = 0;
	return (ch_env);
}

int	exec_env(t_lst *cmd_lst, t_data *data)
{
	(void) cmd_lst;
	if (cmd_lst->arg[1])
	{
		ft_putstr_fd(cmd_lst->arg[0], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(cmd_lst->arg[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		g_exit_status = 1;
		return (g_exit_status);
	}
	print_env(data->env);
	return (g_exit_status);
}
