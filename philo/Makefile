NAME = philo

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -oz -fsanitize=address -fsanitize=undefined -fstack-protector-all -pg -ftrapv -fno-inline -fno-omit-frame-pointer -g3 -fvar-tracking -fprofile-arcs -ftest-coverage -fPIC -fno-builtin -D_FORTIFY_SOURCE=2 -fanalyzer -mno-sse -mno-avx
RM = rm -f

SRC = main.c
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