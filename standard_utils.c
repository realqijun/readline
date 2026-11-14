/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   standard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qang <qang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 22:42:28 by qang              #+#    #+#             */
/*   Updated: 2024/07/15 15:27:00 by qang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int ft_putchar(int c)
{
	write(1, &c, 1);
	return (0);
}

char	*insert(char *str, char c, size_t pos)
{
	char	*new;
	size_t	len;

	len = strlen(str);
	new = (char *)malloc(sizeof(char) * (len + 2));
	if (new == NULL)
	{
		printf("Error while allocating memory\n");
		exit(1);
	}
	memcpy(new, str, pos);
	new[pos] = c;
	strcpy(new + pos + 1, str + pos);
	free(str);
	return (new);
}

char	*append(char *str, char c)
{
	return (insert(str, c, strlen(str)));
}

int ft_iscntrl(int c)
{
  return (c < 32 || c == 127);
}
