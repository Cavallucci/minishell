/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 17:34:26 by mkralik           #+#    #+#             */
/*   Updated: 2021/11/23 13:47:13 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_is_digit(char *s)
{
	int	i;

	i = 0;
	if (!ft_isdigit(s[i]) && !(s[i] == '+' || s[i] == '-'))
		return (1);
	while (s[++i])
	{
		if (!ft_isdigit(s[i]))
			return (1);
	}
	return (0);
}

int	get_exit_code(t_data *data, t_lst *cmd_lst)
{
	(void) data;
	if (cmd_lst->arg && cmd_lst->arg[1])
	{
		if (exit_is_digit(cmd_lst->arg[1]))
		{
			ft_putstr_fd("bash: exit: ", 2);
			ft_putstr_fd(cmd_lst->arg[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			g_exit_status = 2;
			exit(g_exit_status);
		}
		else
		{
			if (cmd_lst->arg[2])
			{
				ft_putstr_fd("bash : exit : too many arguments\n", 2); // ici on ne veut pas sortir du pg
				g_exit_status = 1; // juste un return dans bash --posix donc exit_value = 1 pour avoir ce code erreur
			}
			else
			{
				g_exit_status = ft_atoi(cmd_lst->arg[1]);
				exit(g_exit_status);
			}
		}
	}
	return (g_exit_status);
}

int	exec_exit(t_data *data, t_lst *cmd_lst)
{
	(void) data;
	ft_putstr_fd("exit\n", 1);
	g_exit_status = get_exit_code(data, cmd_lst);
	//free
	exit(g_exit_status);
}
// il faut arreter le programme et prevoir de renvoyer un message d'erreur si l'utilisateur envoie echo $? (obtenir le dernier code d'erreur du syst)
