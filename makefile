main.exe: main.c maze.c maze.h node.c node.h stack.c stack.h search.c search.h
	gcc main.c maze.c node.c stack.c search.c -o main.exe
