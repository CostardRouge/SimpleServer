NAME_EXE	=	Server

SRC_EXE		=	Server.cpp		\
				Main.cpp		\

OBJ_EXE		=	$(SRC_EXE:.cpp=.o)

LDFLAGS		+=	-lpthread
CXXFLAGS	+=	-std=gnu++0x

CXX		=	g++

all:		$(NAME_EXE)

$(NAME_EXE):	$(OBJ_EXE)
		$(CXX) -o $(NAME_EXE) $(OBJ_EXE) $(LDFLAGS)

clean:
		rm -f  $(OBJ_EXE)

fclean:		clean
		rm -f $(NAME_EXE)

re:		fclean all
