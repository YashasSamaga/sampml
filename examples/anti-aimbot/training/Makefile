SRCS = svm.cpp dnn.cpp transformer.cpp analyzer.cpp
INCLUDE_DIRS = -I../../../sampml/
CXXFLAGS = $(INCLUDE_DIRS) -Wall -Wextra -Werror -O3 -ldlib -lopenblas -lstdc++fs -std=c++17

.PHONY: all clean dnn-clean svm-clean transformer-clean analyzer-clean combined-clean standalone_tester-clean

all: dnn svm rf transformer analyzer combined standalone_tester

dnn: dnn.cpp
	$(CXX) dnn.cpp -o dnn $(CXXFLAGS) 
dnn-clean:
	rm -f dnn

svm: svm.cpp
	$(CXX) svm.cpp -o svm $(CXXFLAGS)
svm-clean:
	rm -f svm

rf: rf.cpp
	$(CXX) rf.cpp -o rf $(CXXFLAGS)
rf-clean:
	rm -f rf

transformer: transformer.cpp
	$(CXX) transformer.cpp -o transformer $(CXXFLAGS)
transformer-clean:
	rm -f transformer

analyzer: analyzer.cpp
	$(CXX) analyzer.cpp -o analyzer $(CXXFLAGS)
analyzer-clean:
	rm -f analyzer

combined: combined.cpp
	$(CXX) combined.cpp -o combined $(CXXFLAGS)
combined-clean:
	rm -f combined

standalone_tester: standalone_tester.cpp
	$(CXX) standalone_tester.cpp -o standalone_tester $(CXXFLAGS)
standalone_tester-clean:
	rm -f standalone_tester

clean: dnn-clean svm-clean rf-clean transformer-clean analyzer-clean combined-clean standalone_tester-clean
