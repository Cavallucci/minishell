/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 14:48:10 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/06 18:34:34 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute(t_data *data, int exit_code, t_lst *lst, char **ch_env)
{
	if (lst && lst->builtin)
	{
		exit_code = exec_builtin(lst, data);
		exit(exit_code);
	}
	else if (lst && lst->path)
	{
		printf("check66\n");
		exit_code = execve(lst->path, lst->arg, ch_env);
	}
	else
	{
		error_cmd(lst->cmd);
		//ft_free_split(split);
		//exit (ft_free_data(data, 127));
		g_exit_status = 127;
		exit(g_exit_status);
	}
	return (exit_code);
}
