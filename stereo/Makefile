CC=g++ -std=c++11
CFLAGS=-I.
FILES=StereoVision.cpp main.cpp
EXE_FILE=agv
# OPENCV=$(shell pkg-config --cflags --libs /usr/local/Cellar/opencv/4.5.3_2/lib/pkgconfig/opencv4.pc)
# ZBAR=$(shell pkg-config --cflags --libs /usr/local/Cellar/zbar/0.23.90_1/lib/pkgconfig/zbar.pc)
# OPENCV=$(shell pkg-config --cflags --libs /opt/opencv/release/unix-install/opencv4.pc )
# ZBAR=$(shell pkg-config --cflags --libs /usr/lib/x86_64-linux-gnu/pkgconfig/zbar.pc )

# Passing opencv and zbar as parameters to the make file
OPENCV=$(shell pkg-config --cflags --libs opencv4)
ZBAR=$(shell pkg-config --cflags --libs zbar)

build: StereoVision.cpp main.cpp
	$(CC) -o ${EXE_FILE} ${FILES} ${OPENCV} ${ZBAR}

debug: main.cpp StereoVision.cpp
	$(CC) -g -o ${EXE_FILE} ${FILES} ${OPENCV} ${ZBAR}

time.o: time.cpp
	$(CC) time.cpp

decode.o: decode.cpp
	$(CC) decode.cpp

text.o: text.cpp
	$(CC) text.cpp

angle.o: angle.cpp
	$(CC) angle.cpp

# parsing.o: parsing.cpp
#   $(CC) parsing.cppi

clean: 
	#rm *.o agv data.log
	rm data.log
	rm -rf detected/* not_detected/*
rebuild: clean build
