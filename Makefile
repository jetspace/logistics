build:
	gcc -std=c11 src/main.c `pkg-config --cflags --libs libalpm gtk+-3.0 webkitgtk-3.0 vte-2.91` -o logistics
