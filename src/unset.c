/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:46:27 by mkralik           #+#    #+#             */
/*   Updated: 2021/11/23 13:08:02 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*get_prev(char *key, t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp->next)
	{
		if (!ft_strcmp(tmp->next->key, key))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	unset(t_lst *cmd_lst, t_env *env) // dans env BIB suivi de BOB et BUB, on veut liberer BOB
{
	int		i;
	t_env	*prev;
	t_env	*tmp;

	i = 0;
	while (cmd_lst->arg[++i])
	{
		if (!get_key(cmd_lst->arg[i], env))
			return ;
		prev = get_prev(cmd_lst->arg[i], env); //get_prev rapporte BIB
		if (!prev)
		{
			tmp = env;
			free(env);
			env = tmp->next;
			return ;
		}
		tmp = prev->next->next; //prochain prochain de BIB est BUB
		free(prev->next); //on libere BOB
		prev->next = tmp; //BIB pointe sur BUB
	}
}

int	exec_unset(t_lst *cmd_lst, t_data *data)
{
	int	i;

	i = 0;
	if (!cmd_lst->arg || !cmd_lst->arg[1])
		return (g_exit_status);
	while (cmd_lst->arg[++i])
	{
		if (ft_strnstr(cmd_lst->arg[i], "=", ft_strlen(cmd_lst->arg[i]))
			|| ft_isdigit(cmd_lst->arg[i][0]))
		{
			ft_putstr_fd("unset: `", 2);
			ft_putstr_fd(cmd_lst->arg[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			g_exit_status = 1;
			return (g_exit_status);
		}
		unset(cmd_lst, data->env);
		unset(cmd_lst, data->export);
	}
	return (g_exit_status);
}
