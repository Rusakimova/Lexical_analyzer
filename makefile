LIB_SOURCE=lib/source
LIB_OBJ=lib/obj
TASK_SOURCE=task/source
TASK_OBJ=task/obj
CC=g++
LIB_CFLAGS=-c -Wall -I lib/headers -std=c++11
TASK_CFLAGS=-c -Wall -I task/headers -I lib/headers -std=c++11
FLAGS=-I lib/headers -I task/headers
LIB_S=$(wildcard $(LIB_SOURCE)/*.cpp)
LIB_O=$(patsubst $(LIB_SOURCE)/%.cpp,$(LIB_OBJ)/%.o,$(LIB_S))
TASK_S=$(wildcard $(TASK_SOURCE)/*.cpp)
TASK_O=$(patsubst $(TASK_SOURCE)/%.cpp,$(TASK_OBJ)/%.o,$(TASK_S))
EX=a.exe
all: $(TASK_S) $(LIB_S) $(EX)

$(EX): $(LIB_O) $(TASK_O)
	$(CC) $(FLAGS) $(LIB_O) $(TASK_O) -o $@

$(LIB_OBJ)/%.o: $(LIB_SOURCE)/%.cpp
	$(CC) $(LIB_CFLAGS) $< -o $@
$(TASK_OBJ)/%.o: $(TASK_SOURCE)/%.cpp
	$(CC) $(TASK_CFLAGS) $< -o $@

clean:
	rm $(LIB_O)
	rm $(TASK_O)
	rm $(EX)
