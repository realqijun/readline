CC = gcc
CFLAGS = -Wall -Wextra -Werror

NAME = libreadline.a

LIBFT = libft/libft.a
INC = -I./includes -I./libft/
LIB = -L./libft -lft

SRC = readline.c \
	  push.c \
		signal_handling.c \
		standard_utils.c \
		tab_funcs.c \
		termcap_utils.c \
		termios_utils.c

OBJ_DIR = obj

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C libft

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJ)
	# $(CC) $(CFLAGS) $(OBJ) $(INC) $(LIB) $(LIBFT) -o readline -lcurses
	$(CC) $(CFLAGS) $(OBJ) -o readline -ltermcap

clean:
	@if [ -d $(OBJ_DIR) ]; then \
		rm -rf $(OBJ_DIR); \
	fi
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	rm -f readline
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re