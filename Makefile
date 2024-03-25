SRC		=	./src/*c	\
			./src/lse/*c	\
			./src/queue/*c	\
			./src/utils/*c	\

NAME	=	my-shell

CC		=	gcc

OBJ		=	$(SRC:.c=.o)

all		:	$(NAME)

$(NAME)	:	$(OBJ)
			$(CC) -o $(NAME) $(SRC)



