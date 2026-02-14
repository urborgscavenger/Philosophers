NAME = philo

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

SRC = main.c utils.c print.c actions.c routines.c monitor.c init.c start.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re