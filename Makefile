INCLUDE_DIR	=	include src/class
INCLUDE		=	$(addprefix -I, $(INCLUDE_DIR))

SRC			=	$(shell find src -name "*.cpp")

BUILD_DIR	=	.build
OBJS		=	$(addprefix $(BUILD_DIR)/, $(SRC:.cpp=.o))
DEPS		=	$(OBJS:.o=.d)

CPP			=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98

NAME		=	webserv

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) $(INCLUDE) -o $@ $(OBJS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $(INCLUDE) -MD -MP -c $< -o $@

clangd:
	@echo "$(CPPFLAGS) $(INCLUDE)" | tr ' ' '\n' > compile_flags.txt

clean:
	@rm -rf $(BUILD_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean
	$(MAKE) all

-include $(DEPS)

.PHONY : all clean fclean re clangd
