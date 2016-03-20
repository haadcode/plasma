all:
	gcc plasma.c -o plasma -I /usr/local/include/SDL2 -D_THREAD_SAFE -L /usr/local/lib -l SDL2
