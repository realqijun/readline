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

static int	find_matches(char *dir_path, char *prefix, char **match)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	dir = opendir(dir_path);
	if (dir == NULL)
		return (0);
	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (ft_strncmp(entry->d_name, prefix, ft_strlen(prefix)) == 0
			&& (*match == NULL || ft_strcmp(entry->d_name, *match) != 0))
		{
			if (*match == NULL)
				*match = ft_strdup(entry->d_name);
			count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

static char	*make_suffix(char *match, size_t len, int count, char *end)
{
	char	*suffix;

	suffix = NULL;
	if (count == 1)
		suffix = ft_strjoin(match + len, end);
	free(match);
	return (suffix);
}

static char	*complete_command(char *prefix)
{
	char	*path;
	char	*dir;
	char	*match;
	int		count;

	match = NULL;
	count = 0;
	path = getenv("PATH");
	dir = next_path(&path);
	while (dir)
	{
		count += find_matches(dir, prefix, &match);
		free(dir);
		dir = next_path(NULL);
	}
	return (make_suffix(match, ft_strlen(prefix), count, " "));
}

static char	*complete_path(char *word)
{
	char	*slash;
	char	*dir;
	char	*prefix;
	char	*match;
	int		count;

	slash = ft_strrchr(word, '/');
	if (slash)
	{
		dir = ft_substr(word, 0, slash - word + 1);
		prefix = slash + 1;
	}
	else
	{
		dir = ft_strdup(".");
		prefix = word;
	}
	match = NULL;
	count = find_matches(dir, prefix, &match);
	free(dir);
	return (make_suffix(match, ft_strlen(prefix), count, ""));
}

char	*complete_line(char *line, int pos)
{
	char	*word;
	char	*suffix;
	int		start;
	int		i;

	start = pos;
	while (start > 0 && line[start - 1] != ' ')
		start--;
	if (start == pos)
		return (NULL);
	i = 0;
	while (i < start && line[i] == ' ')
		i++;
	word = ft_substr(line, start, pos - start);
	if (i == start)
		suffix = complete_command(word);
	else
		suffix = complete_path(word);
	free(word);
	return (suffix);
}
