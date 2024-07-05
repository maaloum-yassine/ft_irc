# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/11 15:52:46 by ymaaloum          #+#    #+#              #
#    Updated: 2024/07/05 04:44:55 by ymaaloum         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX			= g++
# CXXFLAGS  = -Wall -Wextra -Werror -std=c++98
CXXFLAGS	=
NAME		= ircserv

SRC			= 	main.cpp \
				server.cpp \
				Parse/Parse.cpp \
				function_utils/Func_Utils.cpp

RESET			= \033[1;97m
GREEN			= \033[1;32m
RED  			= \033[1;31m

OBJ				= 	$(SRC:.cpp=.o)
OBJ 			:=	$(addprefix obj/, $(OBJ))
OBJ_DIR			= 	obj/
OBJ_SUBDIRS		=	$(OBJ_DIR) $(OBJ_DIR)function_utils $(OBJ_DIR)Parse

all: $(OBJ_SUBDIRS) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR) $(OBJ_DIR)function_utils $(OBJ_DIR)Parse
	@echo "$(GREEN)$(OBJ_DIR) : Created ! [^_^]$(RESET)"

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)$(NAME) : Created ! [^_^]$(RESET)"

obj/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/function_utils/%.o: function_utils/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/Parse/%.o: Parse/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)$(OBJ_DIR) : directory deleted ! [^_^]$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)$(NAME) : executable deleted ! [^_^]$(RESET)"

re: fclean all
