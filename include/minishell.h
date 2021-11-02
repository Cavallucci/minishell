/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 15:14:29 by mkralik           #+#    #+#             */
/*   Updated: 2021/11/02 17:57:57 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_lst t_lst;
typedef struct s_env t_env;

struct s_lst //structure pour chaque commande "ls -la"
{
	char	*cmd;
	char	**arg;
	char	*path;
	int		input;
	int		output;
	t_lst	*next;
};

struct s_env
{
	char	*key;
	char	*value;
	t_env	*next;
};

typedef struct s_data
{
	char	*prompt;
	char	*line;
	t_env	*env;
	t_lst	*cmd_lst;

}		t_data;

typedef struct s_sep
{
    int pipe;
    int double_and;
    int simple_quo;
    int double_quo;
    int simple_raft_left;
    int simple_raft_right;
    int double_raft_left;
    int double_raft_right;
}t_sep;

typedef struct s_sp
{
    int     line;
    int     count;
    int     j;
    int     k;
    char    **new;
}t_sp;

/*---------------------lst.c--------------------------*/

void	add_cell(t_env **env, t_env *new);
t_env	*new_cell(char *key, char *value);
t_env	*ft_lstlast(t_env *lst);

/*---------------------utils.c--------------------------*/

size_t	ft_strlen(const char *str);
void	ft_putstr(const char *s);
void	ft_putchar(char c);

/*---------------------ft_split_parsing.c--------------------------*/

void	free_split(char **args);
char	**ft_split(const char *s, char c);

/*---------------------ft_split_parsing.c--------------------------*/

t_sp	*init_sp();
int		is_charset(char s);
char	**ft_split_parsing(char *s);

/*---------------------parsing_utils.c--------------------*/

t_lst   *create_cell(char *cmd);
t_lst   *add_cell_pos(t_lst *list, char *cmd, int pos);
void    print_list(t_lst *list);
t_lst	*create_new(char *split, char **arg, char what);
int		found_place_raft(char **split);

/*---------------------parsing.c--------------------------*/

int		check_separators(t_data *d, t_sep *sep);
void	init_sep(t_sep *sep);
void	print_sep(t_sep *sep, char **split);
void	check_dash(char **split);
void	check_infile_outfile(char **split, t_sep *sep);
t_lst	*parsing(t_data *d);

/*------------------------mini.c--------------------------*/

void	print_env(t_env *env);
t_env	*get_env(char **envp);
t_data	*init_data(char **envp);
int		main(int argc, char **argv, char **envp);

#endif


