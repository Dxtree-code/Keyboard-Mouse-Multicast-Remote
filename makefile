app: main.o libhello.o
	g++ libhello.o main.o -o app.exe

main.o: main.cpp head.h
	g++ -c main.cpp

libhello.o: libhello.cpp head.h
	g++ -c libhello.cpp