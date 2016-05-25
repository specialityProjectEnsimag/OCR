CC = g++

GFLAG = -O3 -fopenmp -g -Wall -Wextra -std=c++0x -Iheader/ -lboost_system  -lboost_filesystem  -L/usr/X11R6/lib  -lm  -lpthread  -lX11 
CLUSTER = -DKNN -DSKELETONIZATION -DCHAMFER

EXEC = ocr

SRC= $(wildcard src/*.cpp)
LIB= $(wildcard header/*.h)
OBJ= $(SRC:src/%.cpp=obj/%.o) 
TEST= $(wildcard tests/*.cpp)

all:  $(OBJ)
	$(CC) $(OBJ) $(GFLAG) $(CLUSTER) -o $(EXEC)

run: 
	./$(EXEC)

obj/%.o: src/%.cpp
	$(CC) -c $<  $(GFLAG) $(CLUSTER) -o $@

obj/%.o: tests/%.cpp
	$(CC) -c $<  $(GFLAG) $(CLUSTER) -o $@

exec_test/%: $(OBJ)
	$(CC) $(OBJ) $(GFLAG) $(CLUSTER) -o $@

test: $(TEST:tests/%.cpp=obj/%.o) $(TEST:tests/%.cpp=exec_test/%)
	
clean:
	rm -f $(OBJ) $(EXEC) $(TEST:tests/%.cpp=exec_test/%)

full: clean all test run