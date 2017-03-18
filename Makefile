CC = g++
CXX_FLAGS = -pthread -std=c++11 -w -O3 

SRC = $(wildcard *.cpp) 

OBJ = $(SRC:.cpp=.o)

all: mkbin igda rmobj
	
.PHONY: mkbin
mkbin:
	mkdir -p bin
	
igda: $(OBJ) 
	$(CC) -o bin/countlines $^ $(CXX_FLAGS)

%.o: %.cpp
	$(CC) $(INCLUDES) -c $< -o $@ $(CXX_FLAGS)

rmobj:
	rm -f $(OBJ)

.PHONY: clean
clean:
	rm -rf bin

