CC = g++

GFLAG = -O3 -fopenmp -g -Wall -Wextra -std=c++0x -Iheader/ -lboost_system  -lboost_filesystem  -lboost_serialization -L/usr/X11R6/lib  -lm  -lpthread  -lX11 
CLUSTER = -DKNN

EXEC = ocr

SRC= $(wildcard src/*.cpp)
LIB= $(wildcard header/*.h)
OBJ= $(SRC:src/%.cpp=obj/%.o) 
OBJ_WITHOUT_MAIN=$(filter-out obj/main.o,$(OBJ))
TEST= $(wildcard tests/*.cpp)

all:  $(OBJ)
	$(CC) $(OBJ) $(GFLAG) $(CLUSTER) -o $(EXEC)

run: 
	./$(EXEC)

obj/%.o: src/%.cpp
	$(CC) -c $<  $(GFLAG) $(CLUSTER) -o $@

obj/%.o: tests/%.cpp
	$(CC) -c $<  $(GFLAG) $(CLUSTER) -o $@

exec_test/%: $(OBJ_WITHOUT_MAIN) obj/%.o
	$(CC) $? $(GFLAG) $(CLUSTER) -o $@

test: cleantest $(TEST:tests/%.cpp=exec_test/%)
	
clean: cleantest
	rm -f $(OBJ) $(EXEC) $(TEST:tests/%.cpp=exec_test/%) $(TEST:tests/%.cpp=obj/%.o)

cleantest:
	rm -f $(TEST:tests/%.cpp=exec_test/%) $(TEST:tests/%.cpp=obj/%.o)

full: clean all test run