INCLUDE		=	$(addprefix -I,$(shell find src -name "*.hpp" -exec dirname {} + | sort -u))

SRC			=	$(shell find src -name "*.cpp")

BUILD_DIR	=	.build
OBJS		=	$(addprefix $(BUILD_DIR)/,$(SRC:.cpp=.o))
DEPS		=	$(OBJS:.o=.d)

CPP			=	c++

FLAGS		=	-Wall -Wextra -Werror -std=c++98

NAME		=	webserv

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(FLAGS) $(INCLUDE) -o $@ $(OBJS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(FLAGS) $(INCLUDE) -MD -MP -c $< -o $@

clean:
	@rm -rf $(BUILD_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY : all clean fclean re clangd