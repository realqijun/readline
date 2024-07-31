/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qang <qang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 22:47:30 by qang              #+#    #+#             */
/*   Updated: 2024/07/31 00:07:43 by qang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "readline.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void substr(char *s, int a, int b, char *t) 
{
	strncpy(t, s+a, b);
}

void  complete_cmd(char *str)
{
	char	*cwd = getenv("PWD");
	char	**paths = ft_split(getenv("PATH"), ':');
	char	**possibles = NULL;
	int	count = 0;
	DIR	*dir;
	struct dirent	*entry;

	if (strcmp(str, "") == 0)
		return ;
	dir = opendir(cwd);
	if (dir == NULL)
	{
		dprintf(2, "Error while opening directory\n");
		exit(1);
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (strncmp(entry->d_name, str, strlen(str)) == 0)
		{
			count++;
			possibles = (char **)realloc(possibles, sizeof(char *) * count);
			possibles[count - 1] = strdup(entry->d_name);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	for (int i = 0; paths[i]; i++)
	{
		dir = opendir(paths[i]);
		if (dir == NULL)
		{
			dprintf(2, "Error while opening directory\n");
			exit(1);
		}
		entry = readdir(dir);
		while (entry != NULL)
		{
			if (strncmp(entry->d_name, str, strlen(str)) == 0)
			{
				count++;
				possibles = (char **)realloc(possibles, sizeof(char *) * count);
				possibles[count - 1] = strdup(entry->d_name);
			}
			entry = readdir(dir);
		}
		closedir(dir);
	}
	for (int i = 0; possibles[i]; i++)
	{
		printf("%s\n", possibles[i]);
		free(possibles[i]);
	}
}

void	complete_line(char *str)
{
	DIR	*dir;
	struct dirent	*entry;
	char	*cwd = getenv("PWD");
	char	line[] = "";
	if (strcmp(str, "") == 0)
		return ;
	dir = opendir(cwd);
	if (dir == NULL)
	{
		dprintf(2, "Error while opening directory\n");
		exit(1);
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (strncmp(entry->d_name, str, strlen(str)) == 0)
		{
			substr(entry->d_name, strlen(str), strlen(entry->d_name) - strlen(str), line);
			write(1, line, strlen(line));
			break ;
		}
		entry = readdir(dir);
	}
	closedir(dir);
}