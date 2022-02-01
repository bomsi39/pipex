/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:27:27 by dfranke           #+#    #+#             */
/*   Updated: 2022/02/01 15:20:25 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	access_cmds(t_env *env, t_cmd *node, char *tmp)
{
	if (!access(tmp, F_OK))
	{
		if (access(tmp, X_OK))
			env->error = terminate(env, ERR_PER, node->array[0]);
		free(node->pth);
		node->pth = ft_strdup(tmp);
	}
}

/*
access_cmds checks the node at first for the existence of the command in the
cartain path because all possible paths are being checked. If a command extists,
execution permissions are being checked. And the path is stored in node->pth.
If permission is not given, error is set to env->error.
*/

void	check_cmds(char **pths, t_env *env)
{
	t_cmd	*node;
	int		i;
	char	*tmp;

	node = env->fir_cmd;
	while (node)
	{
		i = 0;
		while (pths[i])
		{
			tmp = ft_strinsjoin(pths[i++], node->array[0], '/');
			access_cmds(env, node, tmp);
			free(tmp);
		}
		if (!node->pth)
			env->error = terminate(env, ERR_CMD, node->array[0]);
		node = node->next;
	}
	free_array(pths);
}

/*
check_cmds walks though the linked list of commands and creates every command
with every previous extracted path in a temporary string. 
The availablitiy and accessibility check happens in access_cmds (utils.c).
*/

void	parse_cmd(t_env *env, char **argv, char **envp)
{
	int		i;
	char	*t_paths;

	t_paths = NULL;
	i = 2 + env->lmt->islmt;
	while (i < env->ac - 1)
	{
		create_cmd(env, ft_split(argv[i], ' '));
		i++;
	}
	i = -1;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			t_paths = ft_substr(envp[i], 5, (ft_strlen(envp[i]) - 1));
		if (!ft_strncmp(envp[i], "SHELL=/bin/", 11))
			env->shell = ft_substr(envp[i], 11, (ft_strlen(envp[i])));
	}
	check_cmds(ft_split(t_paths, ':'), env);
	free(t_paths);
}

/*
Execve is being used later. For eceve its necessary to save the commands with 
its flags/options in a 2D array. I do this here with ft_split and save the 
output in my cmd linked list.
The possible command paths are extracted with ft_strncmp and ft_split from the
envp variable. And tested for availablity and accesibility in check_cmds.
Further i save the shell of the environment to create matching error messages
for different environments.
*/

void	lmt_info(t_env *env, char **argv)
{
	if (!ft_strncmp(argv[1], "here_doc", 8))
	{
		env->lmt->islmt = true;
		env->cmd_no -= 1;
		env->lmt->lmtr = argv[2];
	}
}

/*
If here_doc is in our input string, the flag is lmt is set to identificate that
we need to look for a limiter.
The limiter is saved to the limiter struct.
*/

t_env	*parse_args(int argc, char **argv, char **envp)
{
	t_env	*env;

	env = init_env(argc, envp);
	if (!env)
		env->error = terminate(env, ERR_MALL, "ERROR");
	lmt_info(env, argv);
	parse_cmd(env, argv, envp);
	access_io(env, argv);
	if (!env->lmt->islmt)
	{
		env->fdin = open(argv[1], O_RDONLY);
		env->fdout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	}
	else
		env->fdout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0666);
	return (env);
}

/*
To parse the arguments i need to initialise my environemnt struct at first in
intit_env (create_env.c).
The next step is to check he if here_doc is in our input string.
In parse_cmd, the commands are being checked for their existence and 
accessibiliy and the put in a linked list.
In access_io the input / ouput files existence and access permissions are being
checked.
Finally the I/O fds are opened and saved in the env struct.
*/