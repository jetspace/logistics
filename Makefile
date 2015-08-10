build:
	gcc -std=c11 src/main.c `pkg-config --cflags --libs libalpm` -o logistics
