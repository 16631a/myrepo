CC=${CXX} -std=c++11
CFLAGS=-I.
FILES=main.cpp
EXE_FILE=agv
# OPENCV=$(shell pkg-config --cflags --libs /usr/local/Cellar/opencv/4.5.3_2/lib/pkgconfig/opencv4.pc)
# ZBAR=$(shell pkg-config --cflags --libs /usr/local/Cellar/zbar/0.23.90_1/lib/pkgconfig/zbar.pc)
# OPENCV=$(shell pkg-config --cflags --libs /opt/opencv/release/unix-install/opencv4.pc )
# ZBAR=$(shell pkg-config --cflags --libs /usr/lib/x86_64-linux-gnu/pkgconfig/zbar.pc )

# Passing opencv and zbar as parameters to the make file
OPENCV=$(shell pkg-config --cflags --libs opencv4)
ZBAR=$(shell pkg-config --cflags --libs zbar)

all:
	$(CC) -o ${EXE_FILE} ${FILES} ${OPENCV} ${ZBAR}

debug: ${FILES}
	$(CC) -g -o ${EXE_FILE} ${FILES} ${OPENCV} ${ZBAR}
clean:
	rm -rf detected/* notdetected/* data.log agv
