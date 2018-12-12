SRCS = svm.cpp dnn.cpp transformer.cpp
INCLUDE_DIRS = -I../../lib/
CFLAGS = $(INCLUDE_DIRS) -Wall -Wextra -Werror -O3 -std=c++17 -ldlib -lopenblas  -g
CC = g++

.PHONY: all clean

all: svm dnn transformer

depend: .depend
.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ > ./.depend;
include .depend

dnn: dnn.cpp .depend
	g++ dnn.cpp -o dnn $(CFLAGS) 
dnn-clean:
	rm -f dnn

svm: svm.cpp .depend
	g++ svm.cpp -o svm $(CFLAGS)
svm-clean:
	rm -f svm

transformer: transformer.cpp .depend
	g++ transformer.cpp -o transformer $(CFLAGS)
transformer-clean:
	rm -f transformer

clean: svm-clean dnn-clean transformer-clean
	rm .depend