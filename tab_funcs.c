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

#include "libft/libft.h"
#include "readline.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

static char	*next_path(char **path)
{
	static char	*curr = NULL;
	char		*start;
	char		*end;
	size_t		len;

	if (path != NULL)
		curr = *path;
	if (curr == NULL || *curr == '\0')
		return (NULL);
	start = curr;
	end = ft_strchr(start, ':');
	if (end != NULL)
	{
		len = end - start;
		curr = end + 1;
	}
	else
	{
		len = ft_strlen(start);
		curr = NULL;
	}
	return (ft_substr(start, 0, len));
}

void	complete_command(char **str, t_readline_info *rl_info)
{
	char	*path = getenv("PATH");
	DIR	*dir;
	struct dirent	*entry;
	int	possibilities;
	size_t	len = ft_strlen(*str);

	possibilities = 0;
	char	*curr_path = next_path(&path);
	while (curr_path)
	{
		dir = opendir(curr_path);
		if (dir)
		{
			entry = readdir(dir);
			while (entry)
			{
				if (ft_strncmp(entry->d_name, *str, len) == 0)
					possibilities++;
				entry = readdir(dir);
			}
			closedir(dir);
		}
		free(curr_path);
		curr_path = next_path(NULL);
	}
	if (possibilities == 1)
	{
		curr_path = next_path(&path);
		while (curr_path)
		{
			dir = opendir(curr_path);
			if (dir)
			{
				entry = readdir(dir);
				while (entry)
				{
					if (ft_strncmp(entry->d_name, *str, len) == 0)
					{
						char	*to_write = ft_substr(entry->d_name, len, ft_strlen(entry->d_name) - len);
						for (int i = 0; to_write[i]; i++)
						{
							write(1, &to_write[i], 1);
							*str = append(*str, to_write[i]);
							rl_info->cur_x++;
						}
						write(1, " ", 1);
						*str = append(*str, ' ');
						rl_info->cur_x++;
						free(to_write);
						return ;
					}
					entry = readdir(dir);
				}
				closedir(dir);
			}
			free(curr_path);
			curr_path = next_path(NULL);
		}
	}
	else
	{
		write(1, "\a", 1);
	}
}

void	write_completed_path(char **str, t_readline_info *rl_info, char *path, char *file)
{
	DIR *dir;
	struct dirent *entry;

	dir = opendir(path);
	entry = readdir(dir);
	while (entry)
	{
		if (ft_strncmp(entry->d_name, file, ft_strlen(file)) == 0)
		{
			char	*to_write = ft_substr(entry->d_name, ft_strlen(file), ft_strlen(entry->d_name) - ft_strlen(file));
			for (int i = 0; to_write[i]; i++)
			{
				write(1, &to_write[i], 1);
				*str = append(*str, to_write[i]);
				rl_info->cur_x++;
			}
			return ;
		}
		entry = readdir(dir);
	}
}

void	complete_path(char *str, t_readline_info *rl_info, char **strstr)
{
	int	possibilities = 0;
	DIR *dir;
	struct dirent *entry;
	char	*temp, *path, *file; 
	if (ft_strchr(str, '/'))
	{
		temp = ft_strchr(str, '/');
		path = ft_substr(str, 0, temp - str);
		int	x = temp - str + 1;
		file = ft_substr(str, x, ft_strlen(str) - x);
		dir = opendir(path);
		if (dir)
		{
			entry = readdir(dir);
			while (entry)
			{
				if (ft_strncmp(entry->d_name, file, ft_strlen(file)) == 0)
					possibilities++;
				entry = readdir(dir);
			}
			closedir(dir);
		}
		if (possibilities == 1)
			write_completed_path(strstr, rl_info, path, file);
		else
			write(1, "\a", 1);
		free(path);
		free(file);
	}
	else
	{
		path = ".";
		file = str;
		dir = opendir(path);
		if (dir)
		{
			entry = readdir(dir);
			while (entry)
			{
				if (ft_strncmp(entry->d_name, file, ft_strlen(file)) == 0)
					possibilities++;
				entry = readdir(dir);
			}
			closedir(dir);
		}
		if (possibilities == 1)
			write_completed_path(strstr, rl_info, path, file);
		else
			write(1, "\a", 1);
	}
}

void	complete_line(char **str, t_readline_info *rl_info)
{
	if (ft_strcmp(*str, "") == 0 || (*str)[ft_strlen(*str) - 1] == ' ')
	{
		alert_sound();
		return ;
	}
	char	**full_command = ft_split(*str, ' ');
	int	i = 0;
	for (; full_command[i + 1]; i++)
		;
	if (i == 0)
		complete_command(str, rl_info);
	else
		complete_path(full_command[i], rl_info, str);
}
