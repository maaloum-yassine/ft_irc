# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/11 15:52:46 by ymaaloum          #+#    #+#              #
#    Updated: 2024/09/12 08:54:51 by ymaaloum         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX			=	g++
CXXFLAGS	=
# -Wall -Wextra -Werror -std=c++98

NAME		= 	ircserv

SRC			=	src/main.cpp \
				src/network/server.cpp \
				src/network/server_cmd.cpp \
				src/network/client.cpp \
				src/network/channels.cpp \
				src/Parse/Parse.cpp \
				src/function_utils/Func_Utils.cpp \
#

RESET		= \033[1;97m
GREEN		= \033[1;32m
RED			= \033[1;31m

OBJ			= $(patsubst src/%.cpp,obj/%.o,$(SRC))
OBJ_DIR 	= obj/
OBJ_SUBDIRS = $(OBJ_DIR) $(OBJ_DIR)network $(OBJ_DIR)function_utils $(OBJ_DIR)Parse

all: $(OBJ_SUBDIRS) $(NAME)

$(OBJ_SUBDIRS):
	@mkdir -p $@
	@echo "$(GREEN)$@ : Created ! [^_^]$(RESET)"

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)$(NAME) : Created ! [^_^]$(RESET)"

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)$(OBJ_DIR) : directory deleted ! [^_^]$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)$(NAME) : executable deleted ! [^_^]$(RESET)"

re: fclean all
