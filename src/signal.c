/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcavallu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 17:01:08 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/13 17:02:11 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signal.h"

void    ft_signal_stdin(void)
{
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    g_exit_status = 130;
}

void    ft_signal_sigint(void)
{
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    g_exit_status = 130;
}

void    handle_sig(int signum)
{
    if (signum == SIGINT) //cas classique
        ft_signal_sigint();
    else if (signum == SIGINT)
        ft_signal_stdin(); //cas du stdin
    else if (signum == SIGINT)//cas du multipipe
        ft_signal_stdin();
    else if (signum == SIGQUIT)
        write(1, "\b\b  \b\b", 6);
}


void    init_signal(t_data *data)
{
    (void) data;
    signal(SIGINT, handle_sig); //ctrl C
    signal(SIGQUIT, handle_sig); //ctrl backslash
    signal(SIGTSTP, handle_sig); //ctrl Z
}
