/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 15:03:06 by dfranke           #+#    #+#             */
/*   Updated: 2022/01/28 14:06:34 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H

# include "libft.h"

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <stdlib.h>
# include <stdbool.h>

typedef struct s_cmd
{
	pid_t			child;
	int				s_in;
	int				s_out;
	int				close;
	char			**array;
	char			*av;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	t_cmd	*fir_cmd;	//first command
	t_cmd	*lst_cmd;	//last command
	char	**envp_c;	//envp
	char	**pths;		//possible paths
	int		fdin;		//fd input file
	int		fdout;		//fd output file
	bool	islimiter;  //is "here_doc?"
	char	*limiter;   //value of limiter
	int		ac;			//argc
	char	*shell;		//shell type (bash/zsh/...)
	int		cmd_no;		//number of commands
	int		end[2];	//pipe I/O
}	t_env;

# define ERR -1

t_env	*parse_args(int argc, char **argv, char **envp);
void	create_cmd(t_env *env, char **cmd_a, char *argv);
void	pipex(t_env *env);
void	set_io(t_env *env);
t_cmd	*cmdlast(t_cmd *lst);
void	free_env(t_env *env);


#endif