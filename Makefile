PREFIX?=/usr/X11R6
CFLAGS?=-Os -pedantic -Wall

all:
	mkdir -p build/
	cc $(CFLAGS) -I$(PREFIX)/include src/main.c -L$(PREFIX)/lib -lX11 -o build/enforcer

init:
	Xephyr -br -ac -noreset -screen 800x600 :1 &

run:
	make
	DISPLAY=:1 ./build/enforcer

clean:
	rm -f enforcer
