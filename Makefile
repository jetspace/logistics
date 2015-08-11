build:
	gcc -std=c11 src/main.c `pkg-config --cflags --libs libalpm gtk+-3.0 webkit2gtk-4.0` -o logistics
