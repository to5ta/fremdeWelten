# Makefile for fremdeWelten, tostas 3D Spielplatz

WARN 	= -Wall -Wextra -Wstack-protector -W
BIN 	= ../bin/
OBJ 	= ../obj/
TARGET 	= $(BIN)fremdeWelten
LIBS 	= -lglfw -lGL -lGLEW
# LIBS = -lGL -lGLU -lglfw -lGLEW -glew32 -lopengl32
MODULES = $(OBJ)*.o
# GLAD 	= /usr/share/glfw/deps/
INCLUDE = -I/usr/include/ -I. 
#-I$(GLAD)
CC 		= $(DEBUG) $(WARN)
DEBUG 	= -d


# all: clean glad camera fremdeWelten link
all: clean camera fremdeWelten link
	# @echo Building fremdeWelten...

clean:
	@if test -e $(TARGET).o; then rm $(TARGET).o; \
	fi;

link:
	# @echo Linking modules...
	g++ $(CC) -o $(TARGET) $(LIBS) $(MODULES)

# glad:
# 	g++ $(CC) -c $(GLAD)glad.c -o $(OBJ)glad.o $(INCLUDE)


%: 
	# $(info ccache g++ -c $@.cpp -o $(OBJ)$@.o $(INCLUDE) )
	ccache g++ $(CC) -c $@.cpp -o $(OBJ)$@.o $(INCLUDE) 

run:
	cd $(BIN) && ./fremdeWelten