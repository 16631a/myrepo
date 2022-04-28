##include ../dimensions/calculation.h

CPP      =${CXX} -std=c++11
CXXFLAGS = $(CXXINCS) -Wall -O0
OPENCV=$(shell pkg-config --cflags --libs opencv4)
ZBAR=$(shell pkg-config --cflags --libs zbar)
#all: main

#main: barcode.o angle.o decode.o text.o  parsing.o time.o
#	$(CPP) main.o dimensions.o barcode.o -o agv2 ${OPENCV} ${ZBAR}

barcode.o: barcode.cpp
	$(CPP) -c barcode.cpp -o barcode.o $(CXXFLAGS) ${OPENCV} ${ZBAR}
angle.o: angle.cpp
	$(CPP) -c angle.cpp -o angle.o $(CXXFLAGS) ${OPENCV} ${ZBAR}
text.o: text.cpp
	$(CPP) -c text.cpp -o text.o $(CXXFLAGS) ${OPENCV} ${ZBAR}
time.o: time.cpp
	$(CPP) -c time.cpp -o time.o $(CXXFLAGS) ${OPENCV} ${ZBAR}
parsing.o:parsing.cpp
	$(CPP) -c parsing.cpp -o parsing.o $(CXXFLAGS) ${OPENCV} ${ZBAR}
decode.o: decode.cpp
	$(CPP) -c decode.cpp -o decode.o $(CXXFLAGS) ${OPENCV} ${ZBAR}
