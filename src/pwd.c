/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 12:11:52 by mkralik           #+#    #+#             */
/*   Updated: 2021/11/23 13:10:55 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_key(char *arg, t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, arg))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

int	exec_pwd(t_lst *cmd_lst, t_data *data)
{
	char	*pwd;

	(void) data;
	(void) cmd_lst;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_putstr_fd("pwd: error retrieving current directory: ", 2);
		ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
		ft_putstr_fd("No such file or directory", 2);
		g_exit_status = 1;
		return (g_exit_status);
	}
	ft_putstr_fd(pwd, 1);
	ft_putchar_fd('\n', 1);
	return (g_exit_status);
}
