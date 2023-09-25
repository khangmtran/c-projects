generator: generator.c 3d.o 3d.h
	gcc -Wall -Werror -std=c11 generator.c 3d.o -o generator
3d.o: 3d.c 
	gcc -Wall -Werror -std=c11 -c 3d.c -o 3d.o
clean:
	rm -rf generator 3d.o
