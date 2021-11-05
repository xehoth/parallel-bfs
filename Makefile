CXX=g++
FLAGS=-g -std=c++17 -Wall -Wextra -fopenmp -O2
INC_FILES = include/bfs.h include/bitmap.h include/frontier.h include/graph.h include/memory_manager.h include/prefix_sum.h include/reader.h include/serial_bfs.h include/timer.h include/topdown_pbfs.h include/writer.h
SRC_FILES = src/main.cpp 

all: ${INC_FILES} ${SRC_FILES}
	cd data && python3 fetch.py && cd ..
	${CXX} ${SRC_FILES} -I include/ -o main ${FLAGS}

clean:
	rm -f main out.txt