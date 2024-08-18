NAME = libreadline.a
CC = gcc 
CFLAGS = -Wall -Wextra -Werror
AR = ar -rcs

LIBFT = libft/libft.a
INC = -I libft
LINK = -L libft -lft -lcurses

SRCS = readline.c \
	  push.c \
	  signal_handling.c \
	  standard_utils.c \
	  tab_funcs.c \
	  termcap_utils.c \
	  termios_utils.c

BUILD_DIR = objs

OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C libft

dir:
	@mkdir -p $(sort $(dir $(OBJS)))

$(BUILD_DIR)/%.o: %.c | dir
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJS)
	mv libft/libft.a $(NAME)
	$(AR) $(NAME) $(OBJS)

clean:
	@if [ -d $(BUILD_DIR) ]; then \
		rm -rf $(BUILD_DIR); \
		fi
	@make -C libft clean

fclean: clean
	@rm -f $(NAME)
	@rm -f readline
	@make -C libft fclean

re: fclean all

.PHONY: all clean fclean re
