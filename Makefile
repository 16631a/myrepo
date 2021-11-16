all:
	g++ -std=c++17 hello.cpp -o hello

test:
	./hello
check:
	echo $?==0 

clean:
	$(RM) hello 
