CPP      = ${CXX} -std=c++11
#CXXFLAGS = $(CXXINCS) -Wall -O0
OPENCV=$(shell pkg-config --cflags --libs opencv4)
ZBAR=$(shell pkg-config --cflags --libs zbar)
#all: main

#main: calculation.o dimensions.o aruco.o Text.o  parsing.o
#	$(CPP) main.o dimensions.o barcode.o -o agv1 ${OPENCV} ${ZBAR}

calculation.o: calculation.cpp
	$(CPP) -c calculation.cpp -o calculation.o ${OPENCV} ${ZBAR}
dimensions.o: dimensions.cpp
	$(CPP) -c dimensions.cpp -o dimensions.o ${OPENCV} ${ZBAR}
Text.o: Text.cpp
	$(CPP) -c Text.cpp -o Text.o ${OPENCV} ${ZBAR}
aruco.o: aruco.cpp
	$(CPP) -c aruco.cpp -o aruco.o ${OPENCV} ${ZBAR}
parsing.o:parsing.cpp
	$(CPP) -c parsing.cpp -o parsing.o ${OPENCV} ${ZBAR}
