/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 15:58:26 by mkralik           #+#    #+#             */
/*   Updated: 2021/11/23 17:27:29 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	use_cdpath(t_lst *cmd_lst, char *cdpath)
{
	char	*path;
	int		new;

	path = ft_strjoin(cdpath, cmd_lst->arg[0]);
	new = chdir(path);
	ft_putstr_fd(path, 1);
	ft_putstr_fd("\n", 1);
	free(path);
	return (new);
}

void	update_pwd(t_data *data, char *pwd, char *old_pwd)
{
	if (!get_key("PWD", data->env))
	{
		add_cell(&data->env, new_cell("PWD", pwd, 1));
		add_cell(&data->export, new_cell("PWD", pwd, 1));
	}
	if (!get_key("OLDPWD", data->env))
	{
		add_cell(&data->env, new_cell("OLDPWD", old_pwd, 1));
		add_cell(&data->export, new_cell("OLDPWD", old_pwd, 1));
	}
	if (pwd)
	{
		change_cell_env("PWD", pwd, data->env);
		change_cell_env("PWD", pwd, data->export);
	}
	if (old_pwd)
	{
		change_cell_env("OLDPWD", old_pwd, data->env);
		change_cell_env("OLDPWD", old_pwd, data->export);
	}
	free(pwd);
	free(old_pwd);
}

int	exec_cd(t_lst *cmd_lst, t_data *data)
{
	char	*old_pwd;
	char	*pwd;
	char	*cdpath;
	int		new;

	cdpath = get_key("CDPATH", data->env);
	old_pwd = getcwd(NULL, 0);
	//obtenir le repertoire de travail courant avant deplacement
	if (cmd_lst->arg && cmd_lst->arg[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		g_exit_status = 1;
		return (g_exit_status);
	}
	else if (!cmd_lst->arg || !ft_strcmp(cmd_lst->arg[1], "--"))
	{
		if (get_key("HOME", data->env))
		{
			new = chdir(get_key("HOME", data->env));
		}
		else
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			g_exit_status = 1;
			return (g_exit_status);
		}
	}
	else if (!ft_strcmp(cmd_lst->arg[1], "-"))
	{
		if (get_key("OLDPWD", data->env))
		{
			ft_putstr_fd(get_key("OLDPWD", data->env), 1);
			ft_putstr_fd("\n", 1);
			new = chdir(get_key("OLDPWD", data->env));
		}
		else
		{
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
			g_exit_status = 1;
			return (g_exit_status);
		}
	}
	else
	{
		if (cdpath && cmd_lst->arg[1] && ft_strcmp(cmd_lst->arg[1], ".")
			&& ft_strcmp(cmd_lst->arg[1], ".."))
			new = use_cdpath(cmd_lst, cdpath);
		else
			new = chdir(cmd_lst->arg[0]);
	}
	pwd = getcwd(NULL, 0); //obtenir le repertoire courant
	if (new == 0)
		update_pwd(data, pwd, old_pwd);
	return (g_exit_status);
}

//chdir = changer le repertoire courant,
//remplace le répertoire de travail courant par celui
//indiqué dans le chemin path en arg de chdir.
//mettre le pwd avant chgmt dans le oldpwd
