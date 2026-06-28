INCLUDE_DIR	=	include src/class
INCLUDE		=	$(addprefix -I, $(INCLUDE_DIR))

SRC			=	$(shell find src -name "*.cpp")

BUILD_DIR	=	.build
OBJS		=	$(addprefix $(BUILD_DIR)/, $(SRC:.cpp=.o))
DEPS		=	$(OBJS:.o=.d)

# CPP is already set by defult by make, we can't juste use 'CPP ?= c++'
EXTCPP		?=	c++
CPP			=	$(EXTCPP)

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

compile_commands.json: fclean
	@bear -- make re -k > /dev/null 2>&1

something_looking_like_compile_command.json:
	@echo '[' > 'compile_commands.json'
	@first=true; for src in $(SRC); do \
[ "$$first" = true ] && first=false || echo ',' >> compile_commands.json; \
echo '  {' >> compile_commands.json; \
echo '    "directory": "'$(shell pwd)'",' >> compile_commands.json; \
obj_path=$$(echo "$$src" | sed 's|src/|$(BUILD_DIR)/|' | sed 's|\.cpp$$|.o|'); \
echo "    \"command\": \"$(CPP) $(CPPFLAGS) $(INCLUDE) -c $$src -o $$obj_path\"," >> compile_commands.json; \
echo '    "file": "'$$src'"' >> compile_commands.json; \
echo '  }' >> compile_commands.json; \
done
	@echo ']' >> compile_commands.json

clean:
	@rm -rf $(BUILD_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean
	$(MAKE) all

-include $(DEPS)

.PHONY : all clean fclean re clangd
