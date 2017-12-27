SRC_PATH = ./src/
OBJ_PATH = ./obj/
INC_PATH = ./inc/

SRC_NAME = main.cpp board.cpp node.cpp npuzzle.cpp\
	renderer.cpp mesh.cpp shaders.cpp texture.cpp\
	astar.cpp
OBJ_NAME = $(SRC_NAME:.cpp=.o)
NAME = npuzzle

SRC = $(addprefix $(SRC_PATH),$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH),$(OBJ_NAME))
INC = $(addprefix -I,$(INC_PATH))

CC = g++
CFLAGS = -MMD -pedantic -Wuninitialized -std=c++11 -I ~/.brew/include
LIBS = `pkg-config --static --libs glm` \
	-lm -framework OPENGL `pkg-config --static --libs glfw3` \
	`pkg-config --static --libs glew`

all:
	@echo "\033[36;44m Make $(NAME) \033[0m"
	@make $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp $(DEP)
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

install:
	brew install pkg-config
	brew install glm
	brew install glfw
	brew install glew

clean:
	@echo "\033[32;44m Make clean $(NAME) \033[0m"
	rm -rf $(OBJ) $(OBJ_PATH)

fclean: clean
	@echo "\033[32;44m Make fclean $(NAME) \033[0m"
	rm -f $(NAME)

re: fclean all

-include $(OBJ:.o=.d)

.PHONY: lib clean fclean re
