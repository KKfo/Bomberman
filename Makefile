##
## Makefile for bomberman
##
## Made by Adolfo Flores
## Login   <flores_a@epitech.eu>
##
##
## 
##

BMBR_SRC =		src/tmp/test.cpp

BMBR_OBJ =		$(BMBR_SRC:.c=.o)

DEPS =		

BMBR =		bin/bomberman

CC =		g++

CFLAGS =	-Wall -Werror -Wextra -g

RM =		rm -f

all:		$(BMBR)

$(BMBR):	$(BMBR_OBJ)
			$(CC) $(BMBR_SRC) -Ilib/LibBomberman_linux_x64/includes/ -Llib/LibBomberman_linux_x64/libs/ \
			-lgdl_gl -lGL -lGLEW -ldl -lrt -lfbxsdk -lSDL2 -lpthread -o $(BMBR)

clean:
			$(RM) $(BMBR_OBJ)

fclean:		clean
			$(RM) $(BMBR)

re:			fclean $(BMBR)

.PHONY:			all clean fclean re
