/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qang <qang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 23:39:11 by qang              #+#    #+#             */
/*   Updated: 2024/07/29 17:07:57 by qang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READLINE_H
# define READLINE_H

# include <termios.h>
# include <stdbool.h>

# define SHELL "zish> "

typedef struct s_history
{
	char	*line;
	char  *tmp;
	bool  final;
	struct s_history	*next;
}	t_history;

typedef struct s_readline_info
{
	char  *prompt;
	int		cur_x;
	int		cur_y;
	int   row;
	int   col;
	int   pad;
	struct termios orig_termios;
}	t_readline_info;

char	*ft_readline(char *prompt);

void	push(t_history **a, t_history **b);
void	append_history(char *str, t_history **lst);

/*termios utils*/
void	set_rl_term(struct termios *orig_termios);
void	reset_terminal_mode(struct termios *orig_termios);
void	get_term_size(int *row, int *col);

/*standard utils*/
int ft_putchar(int c);
char	*append(char *str, char c);
int ft_iscntrl(int c);

/*termcap utils*/
void	move_cursor(int row, int col);
void	clear_scr(void);
void	ft_init_term(void);
void	clear_line(int row, int col);
void	alert_sound(void);

/*tab to complete*/
void	complete_line(char **str, t_readline_info *rl_info);
#endif
