NAME = philo

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -oz -fsanitize=address -fsanitize=undefined -fstack-protector-all -pg -ftrapv -fno-inline -fno-omit-frame-pointer -g3 -fvar-tracking -fprofile-arcs -ftest-coverage -fPIC -fno-builtin -D_FORTIFY_SOURCE=2 -fanalyzer -mno-sse -mno-avx
RM = rm -f

SRC = main.c utils.c print.c actions.c routines.c monitor.c init.c start.c
OBJ = $(SRC:.c=.o)

all: $(NAME) clean-coverage

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean-coverage:
	$(RM) $(SRC:.c=.gcno)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re clean-coverage