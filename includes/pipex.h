/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 15:03:06 by dfranke           #+#    #+#             */
/*   Updated: 2022/02/01 16:11:15 by dfranke          ###   ########.fr       */
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
#include <sys/types.h>
#include <sys/wait.h>

# define ERR -1
# define ERR_MALL 0
# define ERR_PTH 1
# define ERR_PER 2
# define ERR_CMD 3

/* STRUCT FOR LIMITER */

typedef struct s_lmt
{
	pid_t	pid;				//PID AFTER FORK
	bool	islmt;				//IS HERE_DOC REQUESTED
	char	*lmtr;				//LIMITER STRING
}	t_lmt;

/* STRUCT FOR COMMAND LIST */

typedef struct s_cmd
{
	pid_t			pid;		//PID AFTER FORK
	int				s_in;		//STIN
	int				s_out;		//STOUT
	char			*pth;		//PATH TO COMMAND
	char			**array;	//ARRAY OF COMMAND AND ASSOCIATED FLAGS
	int				idx;		//INDEX OF COMMAND
	struct s_cmd	*next;		//LINK TO NEXT COMMAND
}	t_cmd;

/* STRUCT FOR ENVIRONMENT VARIABLES */

typedef struct s_env
{
	t_cmd	*fir_cmd;			//FIRST COMMAND
	t_cmd	*lst_cmd;			//LAST COMMAND
	t_lmt	*lmt;				//LIMITER PROPERTIES
	char	**envp_c;			//ENVP
	int		fdin;				//FD INPUT FILE
	int		fdout;				//FD OUTPUT FILE
	int		ac;					//ARGC
	char	*shell;				//SHELL TYPE
	int		cmd_no;				//NUMBER OF COMMANDS
	int		pipe_no;			//NEEDE PIPES
	int		error;				//ERROR MARKER
	int		pipes[][2];		//PIPES I/O
}	t_env;

/* MAIN FUNCTION */

void	pipex(t_env *env);

/* PARSING FUNCTION */

t_env	*parse_args(int argc, char **argv, char **envp);

/* INIT & CREATE FUNCTIONS */

t_env	*init_env(int argc, char **envp);
void	create_cmd(t_env *env, char **cmd_a);

/* HERE_DOC FORK FUNCTION */

void	fork_lmt(t_env *env);

/* UTILITY FUNCTIONS */

void	access_cmds(t_env *env, t_cmd *node, char *tmp);
void	access_io(t_env *env, char **argv);
void	set_io(t_env *env);
t_cmd	*cmdlast(t_cmd *lst);

/* FREE & TERMINATE FUNCTIONS */

void	wrong_input(int argc);
void	free_env(t_env *env);
void	free_array(char **array);
int		terminate(t_env *env, int error, char *str);

#endif