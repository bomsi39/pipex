/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:27:27 by dfranke           #+#    #+#             */
/*   Updated: 2022/01/28 14:00:24 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	parse_env(t_env *env, char **argv, char **envp)
{
	int i;
	char *t_paths = NULL;

	t_paths = NULL;
	
	if (!strcmp(argv[1], "here_doc"))
	{
		env->islimiter = true;
		env->cmd_no -= env->islimiter;
		env->limiter = argv[2];
	}
	i = -1;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			t_paths = ft_substr(envp[i], 5, (ft_strlen(envp[i]) - 1));
		if (!ft_strncmp(envp[i], "SHELL=/bin/", 11))
			env->shell=ft_substr(envp[i], 11, (ft_strlen(envp[i])));
	}
	env->pths = ft_split(t_paths, ':');
	free(t_paths);
}

void	parse_cmd(t_env *env, char **argv)
{
	int		i;
	
	i = 2 + env->islimiter;
	while (i < env->ac - 1)
	{
		create_cmd(env, ft_split(argv[i], ' '), argv[i]);
		i++;
	}
}

t_env	*init_env(int argc, char **envp)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	//if (!env)
		//terminate
	env->fir_cmd = NULL;
	env->lst_cmd = NULL;
	env->fdin = 0;
	env->fdout = 0;
	env->envp_c = envp;
	env->pths = NULL;
	env->limiter = NULL;
	env->islimiter = false;
	env->ac	= argc;
	env->cmd_no = argc - 3;
	return (env);
}

t_env	*parse_args(int argc, char **argv, char **envp)
{
	t_env	*env;

	env = init_env(argc, envp);
	parse_env(env, argv, envp);
	parse_cmd(env, argv);
	if (!env->limiter)
		env->fdin = open(argv[1], O_RDONLY);
	env->fdout = open(argv[argc - 1], O_WRONLY);
	if (env->fdout < 0 || env->fdin < 0)
		ft_putendl_fd("Error: I/O files not valid", 2);
	return (env);
}