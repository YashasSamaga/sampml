SRCS = svm.cpp dnn.cpp transformer.cpp analyzer.cpp
INCLUDE_DIRS = -I../../lib/
CFLAGS = $(INCLUDE_DIRS) -Wall -Wextra -Werror -march=native -O3 -std=c++17 -ldlib -lopenblas
CC = g++

.PHONY: all clean dnn-clean svm-clean transformer-clean analyzer-clean

all: svm dnn transformer analyzer

dnn: dnn.cpp
	g++ dnn.cpp -o dnn $(CFLAGS) 
dnn-clean:
	rm -f dnn

svm: svm.cpp
	g++ svm.cpp -o svm $(CFLAGS)
svm-clean:
	rm -f svm

transformer: transformer.cpp
	g++ transformer.cpp -o transformer $(CFLAGS)
transformer-clean:
	rm -f transformer

analyzer: analyzer.cpp
	g++ analyzer.cpp -o analyzer $(CFLAGS)
analyzer-clean:
	rm -f analyzer

clean: svm-clean dnn-clean transformer-clean analyzer-clean