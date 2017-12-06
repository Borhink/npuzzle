# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/10 16:05:27 by qhonore           #+#    #+#              #
#    Updated: 2017/12/06 20:22:14 by qhonore          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_PATH = ./src/
OBJ_PATH = ./obj/
INC_PATH = ./inc/

SRC_NAME = main.cpp heuristic.cpp board.cpp node.cpp npuzzle.cpp
OBJ_NAME = $(SRC_NAME:.cpp=.o)
NAME = npuzzle

SRC = $(addprefix $(SRC_PATH),$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH),$(OBJ_NAME))
INC = $(addprefix -I,$(INC_PATH))

CC = g++
CFLAGS = -Wall -Wextra -Werror -MMD -pedantic -Wuninitialized -std=c++11

all:
	@echo "\033[37;44m Make $(NAME) \033[0m"
	@make $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(LIB) $(LIB_NAME) $^ -o $@

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp $(DEP)
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

clean:
	rm -rf $(OBJ) $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(OBJ:.o=.d)

.PHONY: lib clean fclean re
