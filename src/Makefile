# Makefile for fremdeWelten, tostas 3D Spielplatz

# WARN 	= -Wall -Wextra -Wstack-protector -W
WARN 	= 
BIN 	= ../bin/
OBJ 	= ../obj/
TARGET 	= $(BIN)fremdeWelten
MODULES = $(OBJ)*.o
INCLUDE = -I/usr/include/ -I. -Ideps
CC 		= $(DEBUG) $(WARN)
DEBUG 	= 
LIBS 	= -lglfw -lGL -lSOIL -lassimp
# LIBS = -lGL -lGLU -lglfw -lGLEW -glew32 -lopengl32


# all: clean glad camera fremdeWelten link
# all: clean glad camera Shader fremdeWelten link 
all: clean glad Shader Camera fremdeWelten link 
	@echo Building fremdeWelten done!

clean:
	@if test -e $(TARGET).o; then rm $(TARGET).o; \
	fi;

link:
	@echo Linking modules...
	g++ $(CC) -o $(TARGET) $(LIBS) $(MODULES)

glad:
	g++ $(CC) -c deps/glad.c $(INCLUDE) -o $(OBJ)glad.o


run:
	cd $(BIN) && ./fremdeWelten


debug:
	cd $(BIN) && gdb ./fremdeWelten


%: 
	# $(info ccache g++ -c $@.cpp -o $(OBJ)$@.o $(INCLUDE) )
	ccache g++ $(CC) -c $@.cpp -o $(OBJ)$@.o $(INCLUDE) 