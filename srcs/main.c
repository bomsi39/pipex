/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 15:08:20 by dfranke           #+#    #+#             */
/*   Updated: 2022/01/28 14:00:16 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int main	(int argc, char **argv, char **envp)
{
	t_env	*env;
	
	if (argc < 5)
		ft_putendl_fd("Error: Pipex needs 4 arguments", 2);
	else
	{
		env = parse_args(argc, argv, envp);
		pipex(env);
		free_env(env);
	} 
	return (0);
}	