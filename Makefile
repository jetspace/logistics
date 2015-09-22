build:
	gcc -std=gnu99 src/main.c `pkg-config --cflags --libs libalpm gtk+-3.0 webkitgtk-3.0` -o logistics
install:
	cp logistics /usr/bin/
	mkdir -p /usr/share/logistics/ui
	cp src/ui/* /usr/share/logistics/ui/
