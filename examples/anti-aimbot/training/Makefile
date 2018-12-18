SRCS = svm.cpp dnn.cpp transformer.cpp analyzer.cpp
INCLUDE_DIRS = -I../../lib/
CFLAGS = $(INCLUDE_DIRS) -Wall -Wextra -Werror -march=native -O3 -std=c++17 -ldlib -lopenblas
CC = g++

.PHONY: all clean dnn-clean svm-clean transformer-clean analyzer-clean combined-clean standalone_tester-clean

all: dnn svm rf transformer analyzer combined standalone_tester

dnn: dnn.cpp
	g++ dnn.cpp -o dnn $(CFLAGS) 
dnn-clean:
	rm -f dnn

svm: svm.cpp
	g++ svm.cpp -o svm $(CFLAGS)
svm-clean:
	rm -f svm

rf: rf.cpp
	g++ rf.cpp -o rf $(CFLAGS)
rf-clean:
	rm -f rf

transformer: transformer.cpp
	g++ transformer.cpp -o transformer $(CFLAGS)
transformer-clean:
	rm -f transformer

analyzer: analyzer.cpp
	g++ analyzer.cpp -o analyzer $(CFLAGS)
analyzer-clean:
	rm -f analyzer

combined: combined.cpp
	g++ combined.cpp -o combined $(CFLAGS)
combined-clean:
	rm -f combined

standalone_tester: standalone_tester.cpp
	g++ standalone_tester.cpp -o standalone_tester $(CFLAGS)
standalone_tester-clean:
	rm -f standalone_tester

clean: dnn-clean svm-clean rf-clean transformer-clean analyzer-clean combined-clean standalone_tester-clean